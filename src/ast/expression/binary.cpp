#include "ast/expression/binary.hpp"

#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Value.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "analyzer/analyzer.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"
#include "token/token_type.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/int.hpp"
#include "types/string.hpp"

llvm::Value *BinaryExpression::codegen(
    const shared_ptr<CodegenContext> &context) const {
  auto left_value = left->codegen(context);
  if (!left_value) {
    Logger::error(
        "Failed to generate low level code for left hand side expression",
        LogTypes::Error::INTERNAL, left->get_position());
    return nullptr;
  }

  if (op.type == TOKEN_AS) return left_value;

  auto right_value = right->codegen(context);
  if (!right_value) {
    Logger::error(
        "Failed to generate low level code for right hand side expression",
        LogTypes::Error::INTERNAL, right->get_position());
    return nullptr;
  }

  auto type = left->get_type();
  auto llvm_type = type->to_llvm(context->llvm_context);
  right_value = Codegen::cast_type(context, right_value, llvm_type);
  if (!right_value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH,
                  right->get_position());
    return nullptr;
  }

  switch (op.type) {
    default: {
      if (Analyzer::is_boolean(type).first) {
        switch (op.type) {
          case TokenType::TOKEN_AND:
            return context->builder->CreateAnd(left_value, right_value, "and");
          case TokenType::TOKEN_OR:
            return context->builder->CreateOr(left_value, right_value, "or");
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "ne");
          default:
            return nullptr;
        }
      } else if (Analyzer::is_int(type).first) {
        switch (op.type) {
          case TokenType::TOKEN_PLUS:
            return context->builder->CreateAdd(left_value, right_value, "add");
          case TokenType::TOKEN_MINUS:
            return context->builder->CreateSub(left_value, right_value, "sub");
          case TokenType::TOKEN_ASTERISK:
            return context->builder->CreateMul(left_value, right_value, "mul");
          case TokenType::TOKEN_SLASH:
            return context->builder->CreateSDiv(left_value, right_value, "div");
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "eq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "ne");
          case TokenType::TOKEN_LESS_THAN:
            return context->builder->CreateICmpSLT(left_value, right_value,
                                                   "lt");
          case TokenType::TOKEN_GREATER_THAN:
            return context->builder->CreateICmpSGT(left_value, right_value,
                                                   "gt");
          default:
            return nullptr;
        }
      } else if (Analyzer::is_float(type).first) {
        switch (op.type) {
          case TokenType::TOKEN_PLUS:
            return context->builder->CreateFAdd(left_value, right_value,
                                                "fadd");
          case TokenType::TOKEN_MINUS:
            return context->builder->CreateFSub(left_value, right_value,
                                                "fsub");
          case TokenType::TOKEN_ASTERISK:
            return context->builder->CreateFMul(left_value, right_value,
                                                "fmul");
          case TokenType::TOKEN_SLASH:
            return context->builder->CreateFDiv(left_value, right_value,
                                                "fdiv");
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateFCmpUEQ(left_value, right_value,
                                                   "eq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateFCmpUNE(left_value, right_value,
                                                   "ne");
          case TokenType::TOKEN_LESS_THAN:
            return context->builder->CreateFCmpULT(left_value, right_value,
                                                   "lt");
          case TokenType::TOKEN_GREATER_THAN:
            return context->builder->CreateFCmpUGT(left_value, right_value,
                                                   "gt");
          default:
            return nullptr;
        }
      } else if (Analyzer::is_string(type).first) {
        switch (op.type) {
          case TokenType::TOKEN_EQUAL:
            return context->builder->CreateICmpEQ(left_value, right_value,
                                                  "streq");
          case TokenType::TOKEN_NOT_EQUAL:
            return context->builder->CreateICmpNE(left_value, right_value,
                                                  "strne");
          default:
            return nullptr;
        }
      }
    }
  }

  return nullptr;
}

void BinaryExpression::analyze(const shared_ptr<ProgramContext> &context) {
  left->analyze(context);
  
  if (op.type == TOKEN_AS) return;
  
  right->analyze(context);

  auto left_type = left->get_type();
  auto right_type = right->get_type();

  if (Analyzer::is_pointer(left_type).first ||
      Analyzer::is_pointer(right_type).first) {
    Logger::error("Can't do binary arithmetics on pointers",
                  LogTypes::Error::TYPE_MISMATCH, &position);
    return;
  }

  if (!Analyzer::compare(left_type, right_type)) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return;
  }

  static const unordered_map<string, vector<TokenType>> operations = {
      {typeid(IntType).name(),
       {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH, TOKEN_EQUAL,
        TOKEN_NOT_EQUAL, TOKEN_LESS_THAN, TOKEN_GREATER_THAN}},
      {typeid(FloatType).name(),
       {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH, TOKEN_EQUAL,
        TOKEN_NOT_EQUAL, TOKEN_LESS_THAN, TOKEN_GREATER_THAN}},
      {typeid(BooleanType).name(),
       {TOKEN_AND, TOKEN_OR, TOKEN_EQUAL, TOKEN_NOT_EQUAL}},
      {typeid(StringType).name(), {TOKEN_EQUAL, TOKEN_NOT_EQUAL}}};

  auto it = operations.find(left_type->get_type_info().name());
  if (it == operations.end()) {
    cout << left_type->to_string() << endl;
    cout << right_type->to_string() << endl;

    Logger::error("Unsupported type for binary expression",
                  LogTypes::Error::TYPE_MISMATCH, &position);
    return;
  }

  auto type_operations = operations.at(left_type->get_type_info().name());
  auto _it = find(type_operations.begin(), type_operations.end(), op.type);
  if (_it == type_operations.end()) {
    Logger::error(
        "Unsupported operation for type `" + left_type->to_string() + "`",
        LogTypes::Error::SYNTAX, &position);
  }
}

string BinaryExpression::to_string() const {
  return "(" + left->to_string() + " " + op.to_string() + " " +
         right->to_string() + ")";
}

string BinaryExpression::to_string_tree() const {
  return "BinaryExpression(type: " + type->to_string_tree() + ", op: '" +
         op.to_string() + "', left: " + left->to_string_tree() +
         ", right: " + right->to_string_tree() + ")";
}
