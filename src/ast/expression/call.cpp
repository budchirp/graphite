#include "ast/expression/call.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

using namespace std;

llvm::Value *CallExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto function = context->module->getFunction(name->get_identifier());
  if (!function) {
    Logger::error("Undefined function `" + name->get_identifier() + "` called",
                  LogTypes::Error::UNDEFINED, &position);
    return nullptr;
  }

  vector<llvm::Value *> llvm_arguments;
  size_t idx = 0;

  for (const auto &argument : arguments) {
    auto argument_value = argument->codegen(context);
    if (!argument_value) {
      Logger::error("Failed to generate low level code for argument `" +
                        argument->to_string() + "` in function `" +
                        name->get_identifier() + "`",
                    LogTypes::Error::INTERNAL, argument->get_position());
      return nullptr;
    }

    argument_value =
        Codegen::cast_type(context, argument_value,
                           context->get_env()
                               ->get_function(name->get_identifier())
                               ->type->parameters[idx++]
                               .second->to_llvm(context->llvm_context));
    if (!argument_value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    argument->get_position());
      return nullptr;
    }

    llvm_arguments.push_back(argument_value);
  }

  if (function->getReturnType()->isVoidTy()) {
    context->builder->CreateCall(function, llvm_arguments);
    return llvm::Constant::getNullValue(
        llvm::Type::getVoidTy(*context->llvm_context));
  } else {
    return context->builder->CreateCall(function, llvm_arguments, "call");
  }
}

void CallExpression::validate(const shared_ptr<ProgramContext> &context) {
  auto function =
      context->get_env()->get_function(name->get_identifier())->type;
  if (!function) {
    Logger::error("Undefined function `" + name->get_identifier() + "` called",
                  LogTypes::Error::UNDEFINED, name->get_position());
    return;
  }

  if (function->parameters.size() != arguments.size()) {
    Logger::error("Incorrect number of arguments passed to function `" +
                      name->get_identifier() + "`",
                  LogTypes::Error::SYNTAX, name->get_position());
    return;
  }

  size_t idx = 0;
  for (const auto &argument : arguments) {
    argument->validate(context);

    auto parameter = function->parameters[idx++];
    if (!TypeHelper::compare(parameter.second, argument->get_type(), true)) {
      Logger::error("Type mismatch in argument `" + parameter.first +
                        "`\nExpected `" + parameter.second->to_string() +
                        "` Received `" + argument->get_type()->to_string() +
                        "`",
                    LogTypes::Error::TYPE_MISMATCH, argument->get_position());
      return;
    }
  }
}

void CallExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  for (const auto &argument : arguments) {
    argument->resolve_types(context);
  }

  type = context->get_env()
             ->get_function(name->get_identifier())
             ->type->return_type;
  if (!type) {
    Logger::error("Undefined function `" + name->get_identifier() + "` called",
                  LogTypes::Error::UNDEFINED, name->get_position());
    return;
  }
}

string CallExpression::to_string() const {
  ostringstream result;
  result << name->to_string() << "(";

  for (size_t i = 0; i < arguments.size(); ++i) {
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

  for (size_t i = 0; i < arguments.size(); ++i) {
    result << arguments[i]->to_string_tree();
    if (i < arguments.size() - 1) {
      result << ", ";
    }
  }

  result << "])";
  return result.str();
}
