#include <llvm/IR/Value.h>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expression/binary.hpp"
#include "logger/log_types.hpp"
#include "logger/logger.hpp"

using namespace llvm;

Value *BinaryExpression::codegen() {
  Value *left_value = left->codegen();
  Value *right_value = right->codegen();

  if (!left_value || !right_value) {
    Logger::error("Failed to generate code for left or right operand", LogTypes::Error::INTERNAL, &position);
    return nullptr;
  }

  if (!(left_value->getType()->isPointerTy() && left_value->getType()->getContainedType(0)->isIntegerTy(8)) && (right_value->getType()->isPointerTy() ||
      left_value->getType()->isPointerTy())) {
    Logger::error("Cannot do binary arithmetics on pointers", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  right_value = Codegen::convert_type(right_value, left_value->getType());
  if (!right_value) {
    Logger::error("Type mismatch", LogTypes::Error::TYPE_MISMATCH, &position);
    return nullptr;
  }

  Type *type = left_value->getType();
  if (type->isIntegerTy(1)) {
    switch (op.type) {
    case TokenType::TOKEN_AND:
      return builder->CreateAnd(left_value, right_value, "andtmp");
    case TokenType::TOKEN_OR:
      return builder->CreateOr(left_value, right_value, "ortmp");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "eqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "netmp");
    default:
      Logger::error("Unsupported operator for boolean type", LogTypes::Error::SYNTAX, &position);
      return nullptr;
    }
  } else if (type->isIntegerTy()) {
    switch (op.type) {
    case TokenType::TOKEN_PLUS:
      return builder->CreateAdd(left_value, right_value, "addtmp");
    case TokenType::TOKEN_MINUS:
      return builder->CreateSub(left_value, right_value, "subtmp");
    case TokenType::TOKEN_ASTERISK:
      return builder->CreateMul(left_value, right_value, "multmp");
    case TokenType::TOKEN_SLASH:
      return builder->CreateSDiv(left_value, right_value, "divtmp");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "eqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "netmp");
    default:
      Logger::error("Unsupported operator for integer type", LogTypes::Error::SYNTAX, &position);
      return nullptr;
    }
  } else if (type->isFloatingPointTy()) {
    switch (op.type) {
    case TokenType::TOKEN_PLUS:
      return builder->CreateFAdd(left_value, right_value, "faddtmp");
    case TokenType::TOKEN_MINUS:
      return builder->CreateFSub(left_value, right_value, "fsubtmp");
    case TokenType::TOKEN_ASTERISK:
      return builder->CreateFMul(left_value, right_value, "fmultmp");
    case TokenType::TOKEN_SLASH:
      return builder->CreateFDiv(left_value, right_value, "fdivtmp");
    case TokenType::TOKEN_EQUAL:
      return builder->CreateFCmpUEQ(left_value, right_value, "eqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateFCmpUNE(left_value, right_value, "netmp");
    default:
      Logger::error("Unsupported operator for floating-point type", LogTypes::Error::SYNTAX, &position);
      return nullptr;
    }
  } else if (type->isPointerTy() && type->getContainedType(0)->isIntegerTy(8)) {
    switch (op.type) {
    case TokenType::TOKEN_EQUAL:
      return builder->CreateICmpEQ(left_value, right_value, "streqtmp");
    case TokenType::TOKEN_NOT_EQUAL:
      return builder->CreateICmpNE(left_value, right_value, "strnetmp");
    default:
      Logger::error("Unsupported operator for string type", LogTypes::Error::SYNTAX, &position);
      return nullptr;
    }
  }

  Logger::error("Unsupported operand type in binary expression", LogTypes::Error::SYNTAX, &position);
  return nullptr;
}
