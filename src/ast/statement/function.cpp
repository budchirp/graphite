#include "ast/statement/function.hpp"

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include <memory>
#include <sstream>

#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "semantic/type_helper.hpp"

llvm::Value *FunctionStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return codegen_function(context);
}
llvm::Function *FunctionStatement::codegen_function(
    const shared_ptr<CodegenContext> &context) const {
  auto function = context->module->getFunction(proto->name->get_identifier());
  if (function) {
    Logger::warn("Function `" + proto->name->get_identifier() + "` exists",
                 LogTypes::Warn::SUGGESTION, proto->name->get_position());
  } else {
    function = proto->codegen_function(context);
    if (!function) {
      Logger::error("Failed to generate low level code for function `" +
                        proto->name->get_identifier() + "`",
                    LogTypes::Error::INTERNAL, proto->get_position());
      return nullptr;
    }
  }

  context->get_env()->set_current_scope(scope->get_name());

  for (auto &argument : function->args()) {
    scope->get_variable(argument.getName().str())->add_llvm_value(&argument);
  }

  auto body_block = body->codegen_block(context, function, "entry");
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

  context->get_env()->set_current_scope(scope->get_parent()->get_name());

  context->get_env()
      ->get_function(proto->name->get_identifier())
      ->add_llvm_value(function);

  return function;
}

void FunctionStatement::validate(const shared_ptr<ProgramContext> &context) {
  context->get_env()->set_current_scope(scope->get_name());

  proto->validate(context);
  body->validate(context);

  auto return_type = proto->return_type->get_type();
  if (!TypeHelper::compare(return_type, make_shared<VoidType>()) &&
      !TypeHelper::compare(return_type, body->get_type())) {
    Logger::error(
        "Function `" + proto->name->get_identifier() + "` \nexpected `" +
            return_type->to_string() + "` as return type but received `" +
            body->get_type()->to_string() + "`",
        LogTypes::Error::TYPE_MISMATCH, proto->return_type->get_position());
  }

  context->get_env()->set_current_scope(scope->get_parent()->get_name());
}

void FunctionStatement::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  proto->resolve_types(context);

  context->get_env()->set_current_scope(scope->get_name());

  vector<pair<string, shared_ptr<Type>>> parameters;
  for (const auto &[parameter_name_expression, parameter_type_expression] :
       proto->parameters) {
    auto name = parameter_name_expression->get_identifier();
    auto type = parameter_type_expression->get_type();
    scope->add_variable(
        name, make_shared<VariableSymbol>(name, type, false, true));

    parameters.emplace_back(name, type);
  }

  body->resolve_types(context);

  context->get_env()->set_current_scope(scope->get_parent()->get_name());

  auto name = proto->name->get_identifier();
  context->get_env()->add_function(
      name, make_shared<FunctionSymbol>(
                name, make_shared<FunctionType>(
                          parameters, proto->return_type->get_type())));
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
