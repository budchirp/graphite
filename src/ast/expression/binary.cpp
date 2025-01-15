#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "ast/expression/binary.hpp"
#include "codegen/codegen.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *BinaryExpression::codegen() {
  Value *left_value = left->codegen();
  Value *right_value = right->codegen();

  if (!left_value || !right_value) {
    Logger::error("Failed to generate code for left or right operand",
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

  right_value = Codegen::cast_type(right_value, left_value->getType());
  if (!right_value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  llvm::Type *type = left_value->getType();
  if (type->isIntegerTy(1)) {
    switch (op.type) {
    case TokenType::TOKEN_AND:
      return builder->CreateAnd(left_value, right_value, "and");
    case TokenType::TOKEN_OR:
      return builder->CreateOr(left_value, right_value, "or");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "eq");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "ne");
    default:
      Logger::error("Unsupported operator for boolean type",
                    LogTypes::Error::SYNTAX, &position);
      return nullptr;
    }
  } else if (type->isIntegerTy()) {
    switch (op.type) {
    case TokenType::TOKEN_PLUS:
      return builder->CreateAdd(left_value, right_value, "add");
    case TokenType::TOKEN_MINUS:
      return builder->CreateSub(left_value, right_value, "sub");
    case TokenType::TOKEN_ASTERISK:
      return builder->CreateMul(left_value, right_value, "mul");
    case TokenType::TOKEN_SLASH:
      return builder->CreateSDiv(left_value, right_value, "div");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "eq");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "ne");
    default:
      Logger::error("Unsupported operator for integer type",
                    LogTypes::Error::SYNTAX, &position);
      return nullptr;
    }
  } else if (type->isFloatingPointTy()) {
    switch (op.type) {
    case TokenType::TOKEN_PLUS:
      return builder->CreateFAdd(left_value, right_value, "fadd");
    case TokenType::TOKEN_MINUS:
      return builder->CreateFSub(left_value, right_value, "fsub");
    case TokenType::TOKEN_ASTERISK:
      return builder->CreateFMul(left_value, right_value, "fmul");
    case TokenType::TOKEN_SLASH:
      return builder->CreateFDiv(left_value, right_value, "fdiv");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateFCmpUEQ(left_value, right_value, "eq");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateFCmpUNE(left_value, right_value, "ne");
    default:
      Logger::error("Unsupported operator for floating-point type",
                    LogTypes::Error::SYNTAX, &position);
      return nullptr;
    }
  } else if (type->isPointerTy() && type->getContainedType(0)->isIntegerTy(8)) {
    switch (op.type) {
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "streq");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "strne");
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

string BinaryExpression::to_string() const {
  return "(" + left->to_string() + " " + op.to_string() + " " +
         right->to_string() + ")";
}

string BinaryExpression::to_string_tree() const {
  return "BinaryExpression(op: '" + op.to_string() +
         "', left: " + left->to_string_tree() +
         ", right: " + right->to_string_tree() + ")";
}
