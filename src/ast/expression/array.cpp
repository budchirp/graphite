#include "ast/expression/array.hpp"

#include <llvm/IR/Constants.h>
#include <llvm/IR/Value.h>

#include <cstddef>
#include <memory>
#include <string>

#include "codegen/codegen.hpp"
#include "logger/logger.hpp"
#include "types/null.hpp"

llvm::Value *ArrayExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto llvm_array_type = type->to_llvm(context->llvm_context);
  auto array =
      context->builder->CreateAlloca(llvm_array_type, nullptr, "array");

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
        llvm_array_type, array, {context->builder->getInt32(0), index},
        "array_index");
    context->builder->CreateStore(llvm_value, index_ptr);
  }

  return array;
}

void ArrayExpression::validate(const shared_ptr<ProgramContext> &context) {}
void ArrayExpression::resolve_types(const shared_ptr<ProgramContext> &context) {
  return resolve_types(context, nullptr);
}
void ArrayExpression::resolve_types(const shared_ptr<ProgramContext> &context,
                                    const shared_ptr<Type> &destination_type) {
  for (const auto &value : values) {
    value->resolve_types(context);
  }

  set_type(destination_type ? dynamic_pointer_cast<ArrayType>(destination_type)
                            : make_shared<ArrayType>(
                                  values[0] ? values[0]->get_type()
                                            : make_shared<NullType>(nullptr),
                                  values.size()));
}

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

  result << "])";
  return result.str();
}
