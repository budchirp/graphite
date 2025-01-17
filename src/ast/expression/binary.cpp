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
  auto right_value = right->codegen(context);

  if (!left_value || !right_value) {
    Logger::error("Failed to generate low level code for left or right operand",
                  LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  if (!(left_value->getType()->isPointerTy() &&
        left_value->getType()->getContainedType(0)->isIntegerTy(8)) &&
      (right_value->getType()->isPointerTy() ||
       left_value->getType()->isPointerTy())) {
    Logger::error("Cannot do binary arithmetics on pointers",
                  LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  right_value = Codegen::cast_type(context, right_value, left_value->getType());
  if (!right_value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  if (auto type = left_value->getType(); type->isIntegerTy(1)) {
    switch (op.type) {
      case TokenType::TOKEN_AND:
        return context->builder->CreateAnd(left_value, right_value, "and");
      case TokenType::TOKEN_OR:
        return context->builder->CreateOr(left_value, right_value, "or");
      case TokenType::TOKEN_EQUAL:
        return context->builder->CreateICmpEQ(left_value, right_value, "eq");
      case TokenType::TOKEN_NOT_EQUAL:
        return context->builder->CreateICmpNE(left_value, right_value, "ne");
      default:
        Logger::error("Unsupported operator for boolean type",
                      LogTypes::Error::SYNTAX, &position);
        return nullptr;
    }
  } else if (type->isIntegerTy()) {
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
        return context->builder->CreateICmpEQ(left_value, right_value, "eq");
      case TokenType::TOKEN_NOT_EQUAL:
        return context->builder->CreateICmpNE(left_value, right_value, "ne");
      default:
        Logger::error("Unsupported operator for integer type",
                      LogTypes::Error::SYNTAX, &position);
        return nullptr;
    }
  } else if (type->isFloatingPointTy()) {
    switch (op.type) {
      case TokenType::TOKEN_PLUS:
        return context->builder->CreateFAdd(left_value, right_value, "fadd");
      case TokenType::TOKEN_MINUS:
        return context->builder->CreateFSub(left_value, right_value, "fsub");
      case TokenType::TOKEN_ASTERISK:
        return context->builder->CreateFMul(left_value, right_value, "fmul");
      case TokenType::TOKEN_SLASH:
        return context->builder->CreateFDiv(left_value, right_value, "fdiv");
      case TokenType::TOKEN_EQUAL:
        return context->builder->CreateFCmpUEQ(left_value, right_value, "eq");
      case TokenType::TOKEN_NOT_EQUAL:
        return context->builder->CreateFCmpUNE(left_value, right_value, "ne");
      default:
        Logger::error("Unsupported operator for floating-point type",
                      LogTypes::Error::SYNTAX, &position);
        return nullptr;
    }
  } else if (type->isPointerTy() && type->getContainedType(0)->isIntegerTy(8)) {
    switch (op.type) {
      case TokenType::TOKEN_EQUAL:
        return context->builder->CreateICmpEQ(left_value, right_value, "streq");
      case TokenType::TOKEN_NOT_EQUAL:
        return context->builder->CreateICmpNE(left_value, right_value, "strne");
      default:
        Logger::error("Unsupported operator for string type",
                      LogTypes::Error::SYNTAX, &position);
        return nullptr;
    }
  }

  Logger::error("Unsupported operand type in binary expression",
                LogTypes::Error::SYNTAX, &position);
  return nullptr;
}

void BinaryExpression::analyze(const shared_ptr<ProgramContext> &context) {
  left->analyze(context);
  right->analyze(context);

  auto left_type = left->get_type();
  auto right_type = right->get_type();

  if (!Analyzer::is_string(left_type).first &&
      !Analyzer::is_string(right_type).first) {
    if (Analyzer::is_pointer(left_type).first &&
        Analyzer::is_pointer(right_type).first) {
      Logger::error("Can't do binary arithmetics on pointers",
                    LogTypes::Error::TYPE_MISMATCH, &position);
      return;
    }
  }

  if (!Analyzer::compare(left_type, right_type)) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return;
  }

  static const unordered_map<string, vector<TokenType>> operations = {
      {typeid(IntType).name(),
       {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH, TOKEN_EQUAL,
        TOKEN_NOT_EQUAL}},
      {typeid(FloatType).name(),
       {TOKEN_PLUS, TOKEN_MINUS, TOKEN_ASTERISK, TOKEN_SLASH, TOKEN_EQUAL,
        TOKEN_NOT_EQUAL}},
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
    Logger::error("Unsupported operation for type `" + left_type->to_string() + "`",
                  LogTypes::Error::SYNTAX, &position);
  }
}

string BinaryExpression::to_string() const {
  return "(" + left->to_string() + " " + op.to_string() + " " +
         right->to_string() + ")";
}

string BinaryExpression::to_string_tree() const {
  return "BinaryExpression(type: " + type->to_string_tree() + ", op: '" + op.to_string() +
         "', left: " + left->to_string_tree() +
         ", right: " + right->to_string_tree() + ")";
}
