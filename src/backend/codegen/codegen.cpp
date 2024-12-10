#include <llvm/ADT/StringRef.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/binary.hpp"
#include "frontend/ast/expression/boolean.hpp"
#include "frontend/ast/expression/call.hpp"
#include "frontend/ast/expression/identifier.hpp"
#include "frontend/ast/expression/if.hpp"
#include "frontend/ast/expression/integer.hpp"
#include "frontend/ast/expression/prefix.hpp"
#include "frontend/ast/expression/string.hpp"
#include "frontend/ast/statement/expression.hpp"
#include "frontend/ast/statement/extern.hpp"
#include "frontend/ast/statement/function.hpp"
#include "frontend/ast/statement/return.hpp"
#include "frontend/ast/statement/var.hpp"
#include "frontend/token/token_type.hpp"
#include "logger/logger.hpp"

using namespace std;

unique_ptr<llvm::LLVMContext> context;
unique_ptr<llvm::Module> module;
unique_ptr<llvm::IRBuilder<>> builder;
unordered_map<string, llvm::Value *> named_values;
unordered_map<const llvm::Value *, llvm::Type *> pointer_type_map;

Codegen::Codegen(const shared_ptr<Program> &program) {
  this->program = program;

  context = make_unique<llvm::LLVMContext>();
  module = make_unique<llvm::Module>("graphite", *context);
  builder = make_unique<llvm::IRBuilder<>>(*context);
}

string Codegen::generate_ir() const {
  program->codegen();

  string ir_string;
  llvm::raw_string_ostream ir_stream(ir_string);
  module->print(ir_stream, nullptr);

  return ir_stream.str();
}

llvm::Type *Codegen::resolve_type(const string &type_name) {
  if (type_name[0] == '*') {
    return Codegen::resolve_type(type_name.substr(1));
  }

  if (type_name == "i8")
    return llvm::Type::getInt8Ty(*context);
  if (type_name == "i16")
    return llvm::Type::getInt16Ty(*context);
  if (type_name == "i32")
    return llvm::Type::getInt32Ty(*context);
  if (type_name == "i64")
    return llvm::Type::getInt64Ty(*context);
  if (type_name == "u8")
    return llvm::Type::getInt8Ty(*context);
  if (type_name == "u16")
    return llvm::Type::getInt16Ty(*context);
  if (type_name == "u32")
    return llvm::Type::getInt32Ty(*context);
  if (type_name == "u64")
    return llvm::Type::getInt64Ty(*context);
  if (type_name == "f16")
    return llvm::Type::getHalfTy(*context);
  if (type_name == "f32")
    return llvm::Type::getFloatTy(*context);
  if (type_name == "f80")
    return llvm::Type::getX86_FP80Ty(*context);
  if (type_name == "void")
    return llvm::Type::getVoidTy(*context);
  if (type_name == "boolean")
    return llvm::Type::getInt1Ty(*context);
  if (type_name == "string")
    return llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0);

  Logger::error("Unknown type: " + type_name);
  return nullptr;
}

llvm::Value *Codegen::convert_type(llvm::Value *value,
                                   llvm::Type *expectedType) {
  if (value->getType()->isPointerTy()) {
    return value;
  };

  if (value->getType() == expectedType) {
    return value;
  }

  if (value->getType()->isIntegerTy(1) && expectedType->isIntegerTy(1)) {
    return value;
  }

  if (value->getType()->isIntegerTy() && expectedType->isIntegerTy()) {
    if (value->getType()->getIntegerBitWidth() >
        expectedType->getIntegerBitWidth()) {
      return builder->CreateTrunc(value, expectedType);
    } else {
      return builder->CreateSExt(value, expectedType);
    }
  }

  if (value->getType()->isFloatingPointTy() &&
      expectedType->isFloatingPointTy()) {
    if (value->getType()->getPrimitiveSizeInBits() >
        expectedType->getPrimitiveSizeInBits()) {
      return builder->CreateFPTrunc(value, expectedType);
    } else {
      return builder->CreateFPExt(value, expectedType);
    }
  }

  if (value->getType()->isIntegerTy() && expectedType->isFloatingPointTy()) {
    return builder->CreateSIToFP(value, expectedType);
  }

  if (value->getType()->isFloatingPointTy() && expectedType->isIntegerTy()) {
    return builder->CreateFPToSI(value, expectedType);
  }

  return nullptr;
}

llvm::Value *Program::codegen() {
  for (const auto &statement : statements) {
    statement->codegen();
  }

  return nullptr;
}

llvm::Value *BinaryExpression::codegen() {
  auto *left_value = left->codegen();
  auto *right_value = right->codegen();

  if (!left_value || !right_value) {
    Logger::error("Failed to generate code for left or right operand");
    return nullptr;
  }

  if (right_value->getType()->isPointerTy() ||
      left_value->getType()->isPointerTy()) {
    Logger::error("Cannot do binary arithmetics on pointers");
    return nullptr;
  }

  right_value = Codegen::convert_type(right_value, left_value->getType());
  if (!right_value || left_value->getType() != right_value->getType()) {
    Logger::error("Type mismatch in binary expression");
    return nullptr;
  }

  auto type = left_value->getType();

  if (type->isIntegerTy(1)) {
    switch (op.type) {
    case TokenType::TOKEN_AND:
      return builder->CreateAnd(left_value, right_value, "andtmp");
    case TokenType::TOKEN_OR:
      return builder->CreateOr(left_value, right_value, "ortmp");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "eqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "netmp");
    default:
      Logger::error("Unsupported operator for boolean type");
      return nullptr;
    }
  } else if (type->isIntegerTy()) {
    switch (op.type) {
    case TokenType::TOKEN_PLUS:
      return builder->CreateAdd(left_value, right_value, "addtmp");
    case TokenType::TOKEN_MINUS:
      return builder->CreateSub(left_value, right_value, "subtmp");
    case TokenType::TOKEN_ASTERISK:
      return builder->CreateMul(left_value, right_value, "multmp");
    case TokenType::TOKEN_SLASH:
      return builder->CreateSDiv(left_value, right_value, "divtmp");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "eqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "netmp");
    default:
      Logger::error("Unsupported operator for integer type");
      return nullptr;
    }
  } else if (type->isFloatingPointTy()) {
    switch (op.type) {
    case TokenType::TOKEN_PLUS:
      return builder->CreateFAdd(left_value, right_value, "faddtmp");
    case TokenType::TOKEN_MINUS:
      return builder->CreateFSub(left_value, right_value, "fsubtmp");
    case TokenType::TOKEN_ASTERISK:
      return builder->CreateFMul(left_value, right_value, "fmultmp");
    case TokenType::TOKEN_SLASH:
      return builder->CreateFDiv(left_value, right_value, "fdivtmp");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateFCmpUEQ(left_value, right_value, "eqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateFCmpUNE(left_value, right_value, "netmp");
    default:
      Logger::error("Unsupported operator for floating-point type");
      return nullptr;
    }
  } else if (type->isPointerTy() && type->getContainedType(0)->isIntegerTy(8)) {
    switch (op.type) {
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "streqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "strnetmp");
    default:
      Logger::error("Unsupported operator for string type");
      return nullptr;
    }
  }

  Logger::error("Unsupported operand type in binary expression");
  return nullptr;
}

llvm::Value *BooleanExpression::codegen() { return builder->getInt1(value); }

llvm::Value *CallExpression::codegen() {
  auto function = module->getFunction(name->get_value());
  if (!function) {
    Logger::error("Unknown function " + name->get_value() + " called");
    return nullptr;
  }

  if (function->arg_size() != arguments.size()) {
    Logger::error("Incorrect number of arguments passed to function " +
                  name->get_value());
    return nullptr;
  }

  vector<llvm::Value *> llvm_arguments;
  auto idx = 0;

  for (const auto &argument : arguments) {
    auto argument_value = argument->codegen();
    if (!argument_value) {
      Logger::error("Failed to generate argument for function " +
                    name->get_value());
      return nullptr;
    }

    auto parameter_type = function->getFunctionType()->getParamType(idx);
    argument_value = Codegen::convert_type(argument_value, parameter_type);
    if (!argument_value) {
      Logger::error("Type mismatch");
      return nullptr;
    } else {
      if (parameter_type->isPointerTy() &&
          !argument_value->getType()->isPointerTy()) {
        Logger::error("Expected pointer but received value");
        return nullptr;
      } else if (!parameter_type->isPointerTy() &&
                 argument_value->getType()->isPointerTy()) {
        Logger::error("Expected value but received pointer");
        return nullptr;
      } else

        llvm_arguments.push_back(argument_value);
      ++idx;
    }
  }

  if (function->getReturnType()->isVoidTy()) {
    builder->CreateCall(function, llvm_arguments);
    return nullptr;
  } else {
    auto value = builder->CreateCall(function, llvm_arguments, "calltmp");
    if (function->getReturnType()->isPointerTy())
      pointer_type_map[value] = pointer_type_map[function];

    return value;
  }
}

llvm::Value *IdentifierExpression::codegen() {
  auto it = named_values.find(value);
  if (it == named_values.end()) {
    Logger::error("Undefined variable " + value);
    return nullptr;
  }

  return it->second;
}

llvm::Value *IfExpression::codegen() {
  auto condition_value = condition->codegen();
  if (!condition_value) {
    Logger::error("Failed to generate condition for if expression");
    return nullptr;
  }

  if (!condition_value->getType()->isIntegerTy(1)) {
    Logger::error("Condition must be a boolean");
    return nullptr;
  }

  auto parent_function = builder->GetInsertBlock()->getParent();

  auto then_block = llvm::BasicBlock::Create(*context, "then", parent_function);
  auto else_block = llvm::BasicBlock::Create(*context, "else");
  auto merge_block = llvm::BasicBlock::Create(*context, "ifcontent");

  builder->CreateCondBr(condition_value, then_block, else_block);

  builder->SetInsertPoint(then_block);
  auto then_value = consequence->codegen();
  builder->CreateBr(merge_block);
  then_block = builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), else_block);
  builder->SetInsertPoint(else_block);
  auto else_value = alternative ? alternative->codegen() : nullptr;
  builder->CreateBr(merge_block);
  else_block = builder->GetInsertBlock();

  parent_function->insert(parent_function->end(), merge_block);
  builder->SetInsertPoint(merge_block);

  if (then_value && else_value &&
      then_value->getType() == else_value->getType()) {
    auto phi = builder->CreatePHI(then_value->getType(), 2, "iftmp");
    phi->addIncoming(then_value, then_block);
    phi->addIncoming(else_value, else_block);

    return phi;
  } else {
    return then_value;
  }
}

llvm::Value *IntegerExpression::codegen() {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), value, true);
}

llvm::Value *PrefixExpression::codegen() {
  auto value = right->codegen();

  switch (prefix.type) {
  case TokenType::TOKEN_ASTERISK: {
    if (!value->getType()->isPointerTy()) {
      Logger::error("Cannot dereference non-pointer type");
      return nullptr;
    }

    if (!pointer_type_map[value]) {
      Logger::error("Failed to get pointer type");
      return nullptr;
    }

    auto loaded_value =
        builder->CreateLoad(pointer_type_map[value], value, "deref");
    return loaded_value;
  }

  case TokenType::TOKEN_AMPERSAND: {
    if (value->getType()->isPointerTy()) {
      return value;
    } else {
      auto alloca =
          builder->CreateAlloca(value->getType(), nullptr, "constaddr");
      builder->CreateStore(value, alloca);

      pointer_type_map[alloca] = value->getType();

      return alloca;
    }
  }

  default:
    Logger::error("Unsupported operator type in prefix expression");
    return nullptr;
  }
}

llvm::Value *StringExpression::codegen() {
  return builder->CreateGlobalStringPtr(llvm::StringRef(value), "str", 0,
                                        module.get());
}

llvm::Value *BlockStatement::codegen() {
  llvm::Value *last = nullptr;
  for (const auto &statement : statements) {
    last = statement->codegen();
    if (builder->GetInsertBlock()->getTerminator()) {
      break;
    }
  }

  return last;
}
llvm::BasicBlock *BlockStatement::codegen_block(llvm::Function *parent,
                                                const string &name) {
  auto block = llvm::BasicBlock::Create(*context, name, parent);
  builder->SetInsertPoint(block);

  codegen();

  return block;
}

llvm::Value *ExpressionStatement::codegen() { return expression->codegen(); }

llvm::Value *ExternStatement::codegen() { return proto->codegen_function(); }

llvm::Value *FunctionStatement::codegen() { return codegen_function(); }
llvm::Function *FunctionStatement::codegen_function() {
  auto function = module->getFunction(proto->name->get_value());
  if (function) {
    Logger::warn("Function " + proto->name->get_value() + " exists");
  } else {
    function = proto->codegen_function();
    if (!function) {
      Logger::error("Failed to generate function " + proto->name->get_value());
      return nullptr;
    }
  }

  named_values.clear();
  for (auto &arg : function->args()) {
    named_values[string(arg.getName())] = &arg;
  }

  auto body_block = body->codegen_block(function, "entry");
  if (!body_block) {
    Logger::error("Failed to generate function body");
    function->eraseFromParent();
    return nullptr;
  }

  if (!body_block->getTerminator()) {
    if (function->getReturnType()->isVoidTy()) {
      builder->CreateRetVoid();
    } else {
      Logger::error("Non-void function missing return statement");
      function->eraseFromParent();
      return nullptr;
    }
  }

  llvm::verifyFunction(*function);

  return function;
}

llvm::Value *ProtoStatement::codegen() { return codegen_function(); }
llvm::Function *ProtoStatement::codegen_function() {
  vector<llvm::Type *> parameter_types;
  for (const auto &parameter : parameters) {
    auto is_pointer = false;
    if (parameter.second->get_value()[0] == '*') {
      is_pointer = true;
    }

    auto resolved_type = Codegen::resolve_type(parameter.second->get_value());
    if (!resolved_type) {
      Logger::error("Failed to resolve type for argument " +
                    parameter.first->get_value());
      return nullptr;
    }

    if (is_pointer) {
      resolved_type = llvm::PointerType::get(resolved_type, 0);
    }

    parameter_types.push_back(resolved_type);
  }

  bool is_pointer = false;
  if (return_type->get_value()[0] == '*') {
    is_pointer = true;
  }

  auto return_type = Codegen::resolve_type(this->return_type->get_value());
  if (!return_type) {
    Logger::error("Failed to resolve return type for function " +
                  name->get_value());
    return nullptr;
  }

  auto function_type = llvm::FunctionType::get(
      is_pointer ? llvm::PointerType::get(return_type, 0) : return_type,
      parameter_types, false);
  auto function =
      llvm::Function::Create(function_type, llvm::Function::ExternalLinkage,
                             name->get_value(), module.get());

  auto idx = 0;
  for (auto &argument : function->args())
    argument.setName(parameters[idx++].first->get_value());

  pointer_type_map[function] = return_type;

  return function;
}

llvm::Value *ReturnStatement::codegen() {
  if (auto value = expression->codegen()) {
    const auto function = builder->GetInsertBlock()->getParent();
    const auto return_type = function->getReturnType();

    value = Codegen::convert_type(value, return_type);
    if (!value) {
      Logger::error("Return type does not match function's return type");
      return nullptr;
    } else {
      if (return_type->isPointerTy() && !value->getType()->isPointerTy()) {
        Logger::error("Return expected pointer but received value");
        return nullptr;
      } else if (!return_type->isPointerTy() &&
                 value->getType()->isPointerTy()) {
        Logger::error("Return expected value but received pointer");
        return nullptr;
      } else if (return_type->isPointerTy() &&
                 value->getType()->isPointerTy()) {
        if (pointer_type_map[value] != pointer_type_map[function]) {
          Logger::error("Return type does not match function's return type");
          return nullptr;
        }
      }
    }

    builder->CreateRet(value);

    return value;
  } else {
    Logger::error("Failed to generate return value");
    return nullptr;
  }
}

llvm::Value *VarStatement::codegen() {
  bool is_pointer = false;
  if (type->get_value()[0] == '*') {
    is_pointer = true;
  }

  auto llvm_type = Codegen::resolve_type(type->get_value());
  if (!llvm_type) {
    Logger::error("Failed to resolve type for variable " + name->get_value());
    return nullptr;
  }

  auto value = expression->codegen();
  if (!value) {
    Logger::error("Failed to generate initializer for variable " +
                  name->get_value());
    return nullptr;
  }

  if (llvm::isa<llvm::PHINode>(value)) {
    auto phi = llvm::cast<llvm::PHINode>(value);
    for (auto i = 0; i < phi->getNumIncomingValues(); ++i) {
      auto incoming_value =
          Codegen::convert_type(phi->getIncomingValue(i), llvm_type);
      if (!incoming_value) {
        Logger::error("Type mismatch");
        return nullptr;
      } else {
        if (is_pointer && !incoming_value->getType()->isPointerTy()) {
          Logger::error("Expected pointer but received incoming_value");
          return nullptr;
        } else if (!is_pointer && incoming_value->getType()->isPointerTy()) {
          Logger::error("Expected value but received pointer");
          return nullptr;
        } else if (is_pointer && incoming_value->getType()->isPointerTy()) {
          if (llvm_type != pointer_type_map[incoming_value]) {
            Logger::error("Type mismatch");
            return nullptr;
          }
        }
      }
    }
  }

  value = Codegen::convert_type(value, llvm_type);
  if (!value) {
    Logger::error("Type mismatch");
    return nullptr;
  } else {
    if (is_pointer && !value->getType()->isPointerTy()) {
      Logger::error("Expected pointer but received value");
      return nullptr;
    } else if (!is_pointer && value->getType()->isPointerTy()) {
      Logger::error("Expected value but received pointer");
      return nullptr;
    } else if (is_pointer && value->getType()->isPointerTy()) {
      if (llvm_type != pointer_type_map[value]) {
        Logger::error("Type mismatch");
        return nullptr;
      }
    }
  }

  named_values[name->get_value()] = value;

  return value;
}
