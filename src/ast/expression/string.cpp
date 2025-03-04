#include "ast/expression/string.hpp"

#include <llvm/IR/Value.h>

#include <regex>
#include <string>

#include "codegen/codegen.hpp"

using namespace llvm;

Value *StringExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  return context->builder->CreateGlobalStringPtr(StringRef(value), "str", 0,
                                                 context->module.get());
}

void StringExpression::validate(const shared_ptr<ProgramContext> &context) {}
void StringExpression::resolve_types(
    const shared_ptr<ProgramContext> &context) {
  set_type(make_shared<StringType>());
}

string StringExpression::to_string() const {
  return "\"" + regex_replace(value, regex("\n"), "\\n") + "\"";
}

string StringExpression::to_string_tree() const {
  return "StringExpression(type: " + type->to_string_tree() + ", value: '" +
         value + "')";
}
