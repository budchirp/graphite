#include "ast/expression/call.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;
using namespace std;

Value *CallExpression::codegen() const {
  auto function = context->module->getFunction(name->get_value());
  if (!function) {
    Logger::error("Undefined function `" + name->get_value() + "` called",
                  LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  if (function->arg_size() != arguments.size()) {
    Logger::error("Incorrect number of arguments passed to function `" +
                      name->get_value() + "`",
                  LogTypes::Error::SYNTAX, &position);
    return nullptr;
  }

  vector<Value *> llvm_arguments;
  int idx = 0;

  for (const auto &argument : arguments) {
    Value *argument_value = argument->codegen();
    if (!argument_value) {
      Logger::error(
          "Failed to generate low level code for argument in function `" +
              name->get_value() + "`",
          LogTypes::Error::INTERNAL, argument->get_position());
      return nullptr;
    }

    auto parameter_type = function->getFunctionType()->getParamType(idx);
    argument_value = Codegen::cast_type(argument_value, parameter_type);
    if (!argument_value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
      return nullptr;
    }

    llvm_arguments.push_back(argument_value);
    ++idx;
  }

  if (function->getReturnType()->isVoidTy()) {
    context->builder->CreateCall(function, llvm_arguments);
    return nullptr;
  } else {
    return context->builder->CreateCall(function, llvm_arguments, "call");
  }
}

string CallExpression::to_string() const {
  ostringstream result;
  result << name->to_string() << "(";

  for (auto i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << ")";

  return result.str();
}

string CallExpression::to_string_tree() const {
  ostringstream result;
  result << "CallExpression(name: " << name->to_string_tree()
         << ", arguments: [";

  for (auto i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string_tree();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << "])";

  return result.str();
}
