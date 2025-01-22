#include "ast/statement/extern.hpp"

#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>

#include <memory>

#include "ast/statement/proto.hpp"
#include "codegen/codegen.hpp"

using namespace llvm;

Value *ExternStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto function = proto->codegen_function(context);

  context->get_env()
      ->get_function(proto->name->get_identifier())
      ->add_llvm_value(function);

  return function;
}

void ExternStatement::analyze(const shared_ptr<ProgramContext> &context) {
  proto->analyze(context);
}

string ExternStatement::to_string() const {
  return "extern " + proto->to_string();
}

string ExternStatement::to_string_tree() const {
  return "ExternStatement(proto: " + proto->to_string_tree() + ")";
}
