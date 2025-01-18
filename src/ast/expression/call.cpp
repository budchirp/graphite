#include "ast/expression/call.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "analyzer/analyzer.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "types/function.hpp"

using namespace std;

llvm::Value *CallExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
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

  vector<llvm::Value *> llvm_arguments;
  int idx = 0;

  for (const auto &argument : arguments) {
    llvm::Value *argument_value = argument->codegen(context);
    if (!argument_value) {
      Logger::error(
          "Failed to generate low level code for argument in function `" +
              name->get_value() + "`",
          LogTypes::Error::INTERNAL, argument->get_position());
      return nullptr;
    }

    auto parameter_type = function->getFunctionType()->getParamType(idx);
    argument_value =
        Codegen::cast_type(context, argument_value, parameter_type);
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

void CallExpression::analyze(const shared_ptr<ProgramContext> &context) {
  if (!context->get_env()->get_symbol(name->get_value())) {
    Logger::error("Undefined function `" + name->get_value() + "` called",
                  LogTypes::Error::UNDEFINED, name->get_position());
    return;
  }

  auto function_type = dynamic_pointer_cast<FunctionType>(
      context->get_env()->get_type(name->get_value()));
  if (!function_type) {
    Logger::error("Undefined function `" + name->get_value() + "` called");
    return;
  }

  if (function_type->parameters.size() != arguments.size()) {
    Logger::error("Incorrect number of arguments passed to function `" +
                      name->get_value() + "`",
                  LogTypes::Error::SYNTAX, name->get_position());
    return;
  }

  int idx = 0;
  for (const auto &argument : arguments) {
    argument->analyze(context);

    auto parameter_type = function_type->parameters[idx];
    if (!Analyzer::compare(argument->get_type(), parameter_type)) {
      Logger::error("Type mismatch on callee `" + name->get_value() +
                        "`\nExpected `" + parameter_type->to_string() +
                        "` Received `" + argument->get_type()->to_string() +
                        "`",
                    LogTypes::Error::TYPE_MISMATCH, argument->get_position());
      return;
    }

    ++idx;
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
  result << "CallExpression(type: " << type->to_string_tree()
         << ", name: " << name->to_string_tree() << ", arguments: [";

  for (auto i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string_tree();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << "])";

  return result.str();
}
