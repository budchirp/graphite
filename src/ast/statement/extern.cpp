#include "ast/statement/extern.hpp"

#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include <memory>

#include "ast/statement/proto.hpp"
#include "codegen/codegen.hpp"
#include "semantic/linkage_type.hpp"

llvm::Value *ExternStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto function = proto->codegen_function(context);

  context->get_env()
      ->get_function(proto->name->get_identifier())
      ->add_llvm_value(function);

  return function;
}

void ExternStatement::validate(const shared_ptr<ProgramContext> &context) {
  proto->validate(context);
}

void ExternStatement::resolve_types(const shared_ptr<ProgramContext> &context) {
  proto->resolve_types(context);

  vector<pair<string, shared_ptr<Type>>> parameters;
  for (const auto &[parameter_name_expression, parameter_type_expression] :
       proto->parameters) {
    parameters.emplace_back(parameter_name_expression->get_identifier(),
                            parameter_type_expression->get_type());
  }

  auto name = proto->name->get_identifier();
  context->get_env()->add_function(
      name, make_shared<FunctionSymbol>(
                name, SymbolLinkageType::Internal, visibility,
                make_shared<FunctionType>(parameters,
                                          proto->return_type->get_type())));
}

string ExternStatement::to_string() const {
  return "extern " + proto->to_string();
}

string ExternStatement::to_string_tree() const {
  return "ExternStatement(proto: " + proto->to_string_tree() + ")";
}
