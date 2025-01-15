#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/statement/function.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *FunctionStatement::codegen() { return codegen_function(); }
Function *FunctionStatement::codegen_function() {
  Function *function = module->getFunction(proto->name->get_value());
  if (function) {
    Logger::warn("Function `" + proto->name->get_value() + "` exists", LogTypes::Warn::SUGGESTION, &position);
  } else {
    function = proto->codegen_function();
    if (!function) {
      Logger::error("Failed to generate function " + proto->name->get_value(), LogTypes::Error::INTERNAL, &position);
      return nullptr;
    }
  }

  named_values.clear();
  for (auto &arg : function->args()) {
    named_values[string(arg.getName())] = &arg;
  }

  BasicBlock *body_block = body->codegen_block(function, "entry");
  if (!body_block) {
    Logger::error("Failed to generate function body", LogTypes::Error::INTERNAL, &position);
    function->eraseFromParent();
    return nullptr;
  }

  if (!body_block->getTerminator()) {
    if (function->getReturnType()->isVoidTy()) {
      builder->CreateRetVoid();
    } else {
      Logger::error("Non-void function missing return statement", LogTypes::Error::UNKNOWN, &position);
      function->eraseFromParent();
      return nullptr;
    }
  }

  verifyFunction(*function);

  return function;
}
