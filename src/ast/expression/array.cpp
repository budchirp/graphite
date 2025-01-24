#include "ast/expression/array.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>

#include <cstddef>
#include <memory>
#include <string>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *ArrayExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto array_type = type->to_llvm(context->llvm_context);
  auto array = context->builder->CreateAlloca(array_type, nullptr, "array");

  size_t idx = 0;
  for (const auto &value : values) {
    auto llvm_value = value->codegen(context);
    if (!llvm_value) {
      Logger::error(
          "Failed to generate low level code for array value at index `" +
              ::to_string(idx) + "`",
          LogTypes::Error::INTERNAL, value->get_position());
      return nullptr;
    }

    llvm_value = Codegen::cast_type(
        context, llvm_value, type->child_type->to_llvm(context->llvm_context));
    if (!llvm_value) {
      Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                    value->get_position());
      return nullptr;
    }

    auto index = context->builder->getInt32(idx++);
    auto index_ptr = context->builder->CreateGEP(
        array_type, array, {context->builder->getInt32(0), index},
        "array_index");
    context->builder->CreateStore(llvm_value, index_ptr);
  }

  return array;
}

void ArrayExpression::analyze(const shared_ptr<ProgramContext> &context) {}

string ArrayExpression::to_string() const {
  ostringstream result;
  result << "[";

  for (size_t i = 0; i < values.size(); ++i) {
    result << values[i]->to_string();
    if (i < values.size() - 1) {
      result << ", ";
    }
  }

  result << "]";
  return result.str();
}

string ArrayExpression::to_string_tree() const {
  ostringstream result;
  result << "ArrayExpression(type: " << type->to_string_tree() << ", values: [";

  for (size_t i = 0; i < values.size(); ++i) {
    result << values[i]->to_string_tree();
    if (i < values.size() - 1) {
      result << ", ";
    }
  }

  result <<"])";
  return result.str();
}
