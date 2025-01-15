#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/call.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *CallExpression::codegen() {
  Function *function = module->getFunction(name->get_value());
  if (!function) {
    Logger::error("Unknown function " + name->get_value() + " called", LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  if (function->arg_size() != arguments.size()) {
    Logger::error("Incorrect number of arguments passed to function " +
                  name->get_value(), LogTypes::Error::SYNTAX, &position);
    return nullptr;
  }

  vector<Value *> llvm_arguments;
  int idx = 0;

  for (const auto &argument : arguments) {
    Value *argument_value = argument->codegen();
    if (!argument_value) {
      Logger::error("Failed to generate argument for function " +
                    name->get_value(), LogTypes::Error::INTERNAL, &position);
      return nullptr;
    }

    Type *parameter_type = function->getFunctionType()->getParamType(idx);
    argument_value = Codegen::convert_type(argument_value, parameter_type);
    if (!argument_value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    } else {
      if (parameter_type->isPointerTy() &&
          !argument_value->getType()->isPointerTy()) {
        Logger::error("Expected pointer but received value", LogTypes::Error::TYPE_MISMATCH, &position);
        return nullptr;
      } else if (!parameter_type->isPointerTy() &&
                 argument_value->getType()->isPointerTy()) {
        Logger::error("Expected value but received pointer", LogTypes::Error::TYPE_MISMATCH, &position);
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
    return builder->CreateCall(function, llvm_arguments, "calltmp");
  }
}
