#include "codegen_llvm/codegen.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Pass.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

#include <climits>
#include <cstdio>
#include <iostream>
#include <memory>

#include "ast/expression/array.hpp"
#include "ast/expression/binary.hpp"
#include "ast/expression/boolean.hpp"
#include "ast/expression/call.hpp"
#include "ast/expression/if.hpp"
#include "ast/expression/index.hpp"
#include "ast/expression/integer.hpp"
#include "ast/expression/string.hpp"
#include "ast/expression/struct.hpp"
#include "ast/expression/unary.hpp"
#include "ast/statement/block.hpp"
#include "ast/statement/expression.hpp"
#include "ast/statement/extern.hpp"
#include "ast/statement/for.hpp"
#include "ast/statement/function.hpp"
#include "ast/statement/include.hpp"
#include "ast/statement/return.hpp"
#include "ast/statement/struct.hpp"
#include "ast/statement/while.hpp"
#include "codegen_llvm/expression/array.hpp"
#include "codegen_llvm/expression/binary.hpp"
#include "codegen_llvm/expression/call.hpp"
#include "codegen_llvm/expression/if.hpp"
#include "codegen_llvm/expression/index.hpp"
#include "codegen_llvm/expression/struct.hpp"
#include "codegen_llvm/expression/unary.hpp"
#include "codegen_llvm/expression/var_ref.hpp"
#include "codegen_llvm/program.hpp"
#include "codegen_llvm/statement/block.hpp"
#include "codegen_llvm/statement/extern.hpp"
#include "codegen_llvm/statement/for.hpp"
#include "codegen_llvm/statement/function.hpp"
#include "codegen_llvm/statement/include.hpp"
#include "codegen_llvm/statement/proto.hpp"
#include "codegen_llvm/statement/return.hpp"
#include "codegen_llvm/statement/var.hpp"
#include "codegen_llvm/statement/while.hpp"
#include "codegen_llvm/utils.hpp"
#include "logger/logger.hpp"

using namespace std;

void LLVMCodegen::init() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
}

void LLVMCodegen::codegen(const shared_ptr<Program> &program) const {
  context->add_variable("null",
                        llvm::Constant::getNullValue(
                            llvm::Type::getVoidTy(*context->llvm_context)));

  ProgramCodegen(context, program).codegen();

  llvm::verifyModule(*context->module);
}

llvm::Value *LLVMCodegen::codegen(const shared_ptr<LLVMCodegenContext> &context,
                                  const shared_ptr<Expression> &expression) {
  if (auto array_expression =
          dynamic_pointer_cast<ArrayExpression>(expression)) {
    return ArrayExpressionCodegen(context, array_expression).codegen();
  }
  if (auto binary_expression =
          dynamic_pointer_cast<BinaryExpression>(expression)) {
    return BinaryExpressionCodegen(context, binary_expression).codegen();
  }
  if (auto boolean_expression =
          dynamic_pointer_cast<BooleanExpression>(expression)) {
    return context->builder->getInt1(boolean_expression->value);
  }
  if (auto call_expression = dynamic_pointer_cast<CallExpression>(expression)) {
    return CallExpressionCodegen(context, call_expression).codegen();
  }
  if (auto identifier_expression =
          dynamic_pointer_cast<IdentifierExpression>(expression)) {
    return nullptr;
  }
  if (auto if_expression = dynamic_pointer_cast<IfExpression>(expression)) {
    return IfExpressionCodegen(context, if_expression).codegen();
  }
  if (auto index_expression =
          dynamic_pointer_cast<IndexExpression>(expression)) {
    return IndexExpressionCodegen(context, index_expression).codegen();
  }
  if (auto integer_expression =
          dynamic_pointer_cast<IntegerExpression>(expression)) {
    return llvm::ConstantInt::get(LLVMCodegenUtils::type_to_llvm_type(
                                      context, integer_expression->get_type()),
                                  stoi(integer_expression->value));
  }
  if (auto string_expression =
          dynamic_pointer_cast<StringExpression>(expression)) {
    return context->builder->CreateGlobalStringPtr(
        llvm::StringRef(string_expression->value), "str", 0,
        context->module.get());
  }
  if (auto struct_expression = dynamic_pointer_cast<StructExpression>(expression)) {
    return StructExpressionCodegen(context, struct_expression).codegen();
  }
  if (auto type_expression = dynamic_pointer_cast<TypeExpression>(expression)) {
    return nullptr;
  }
  if (auto unary_expression =
          dynamic_pointer_cast<UnaryExpression>(expression)) {
    return UnaryExpressionCodegen(context, unary_expression).codegen();
  }
  if (auto var_ref_expression =
          dynamic_pointer_cast<VarRefExpression>(expression)) {
    return VarRefExpressionCodegen(context, var_ref_expression).codegen();
  }

  Logger::error("Unknown expression");
  return nullptr;
}

llvm::Value *LLVMCodegen::codegen(const shared_ptr<LLVMCodegenContext> &context,
                                  const shared_ptr<Statement> &statement) {
  if (auto block_statement = dynamic_pointer_cast<BlockStatement>(statement)) {
    return BlockStatementCodegen(context, block_statement).codegen();
  }
  if (auto expression_statement =
          dynamic_pointer_cast<ExpressionStatement>(statement)) {
    return LLVMCodegen::codegen(context, expression_statement->expression);
  }
  if (auto extern_statement =
          dynamic_pointer_cast<ExternStatement>(statement)) {
    return ExternStatementCodegen(context, extern_statement).codegen();
  }
  if (auto for_statement = dynamic_pointer_cast<ForStatement>(statement)) {
    return ForStatementCodegen(context, for_statement).codegen();
  }
  if (auto function_statement =
          dynamic_pointer_cast<FunctionStatement>(statement)) {
    return FunctionStatementCodegen(context, function_statement).codegen();
  }
  if (auto include_statement =
          dynamic_pointer_cast<IncludeStatement>(statement)) {
    return IncludeStatementCodegen(context, include_statement).codegen();
  }
  if (auto proto_statement = dynamic_pointer_cast<ProtoStatement>(statement)) {
    return ProtoStatementCodegen(context, proto_statement).codegen();
  }
  if (auto return_statement =
          dynamic_pointer_cast<ReturnStatement>(statement)) {
    return ReturnStatementCodegen(context, return_statement).codegen();
  }
  if (auto struct_statement =
          dynamic_pointer_cast<StructStatement>(statement)) {
    return nullptr;
  }
  if (auto var_statement = dynamic_pointer_cast<VarStatement>(statement)) {
    return VarStatementCodegen(context, var_statement).codegen();
  }
  if (auto while_statement = dynamic_pointer_cast<WhileStatement>(statement)) {
    return WhileStatementCodegen(context, while_statement).codegen();
  }

  Logger::error("Unknown statement");
  return nullptr;
}

void LLVMCodegen::optimize() const {
  llvm::PassBuilder pb;

  pb.registerModuleAnalyses(*context->mam);
  pb.registerFunctionAnalyses(*context->fam);
  pb.registerLoopAnalyses(*context->lam);
  pb.registerCGSCCAnalyses(*context->cgam);

  pb.crossRegisterProxies(*context->lam, *context->fam, *context->cgam,
                          *context->mam);

  llvm::ModulePassManager mpm = pb.buildModuleOptimizationPipeline(
      llvm::OptimizationLevel::O3, llvm::ThinOrFullLTOPhase::None);

  Logger::log("Optimizing your garbage");
  mpm.run(*context->module, *context->mam);
}
