#include "ast/statement/function.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include <memory>
#include <sstream>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *FunctionStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return codegen_function(context);
}
Function *FunctionStatement::codegen_function(
    const shared_ptr<CodegenContext> &context) const {
  auto function = context->module->getFunction(proto->name->get_value());
  if (function) {
    Logger::warn("Function `" + proto->name->get_value() + "` exists",
                 LogTypes::Warn::SUGGESTION, proto->name->get_position());
  } else {
    function = proto->codegen_function(context);
    if (!function) {
      Logger::error("Failed to generate low level code for function `" +
                        proto->name->get_value() + "`",
                    LogTypes::Error::INTERNAL, proto->get_position());
      return nullptr;
    }
  }

  context->value_map.clear();
  for (auto &arg : function->args()) {
    context->value_map[string(arg.getName())] = &arg;
  }

  BasicBlock *body_block = body->codegen_block(context, function, "entry");
  if (!body_block) {
    Logger::error("Failed to generate low level code for function body",
                  LogTypes::Error::INTERNAL, body->get_position());
    function->eraseFromParent();
    return nullptr;
  }

  if (!body_block->getTerminator()) {
    if (function->getReturnType()->isVoidTy()) {
      context->builder->CreateRetVoid();
    } else {
      Logger::error("Non-void function missing return statement",
                    LogTypes::Error::UNKNOWN,
                    proto->return_type->get_position());
      function->eraseFromParent();
      return nullptr;
    }
  }

  verifyFunction(*function);

  // context->fpm->run(*function, *context->fam);

  return function;
}

void FunctionStatement::analyze(const shared_ptr<ProgramContext> &context) {
  context->set_env(env);

  proto->analyze(context);
  body->analyze(context);

  context->set_env(env->get_parent());
}

string FunctionStatement::to_string() const {
  ostringstream oss;
  oss << "fn " << proto->to_string() << " " << body->to_string();
  return oss.str();
}

string FunctionStatement::to_string_tree() const {
  ostringstream oss;
  oss << "FunctionStatement(proto: " << proto->to_string_tree()
      << ", body: " << body->to_string_tree() << ")";
  return oss.str();
}
