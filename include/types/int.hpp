#pragma once

#include <llvm/IR/DerivedTypes.h>

#include "logger/logger.hpp"
#include "types/type.hpp"

class IntType : public Type {
 public:
  int size;
  bool is_unsigned;

  IntType(char size, bool is_unsigned)
      : size(size), is_unsigned(is_unsigned) {};

  llvm::IntegerType *to_llvm(shared_ptr<llvm::LLVMContext> context) override {
    switch (size) {
      case 8:
        return llvm::Type::getInt8Ty(*context);
      case 16:
        return llvm::Type::getInt16Ty(*context);
      case 32:
        return llvm::Type::getInt32Ty(*context);
      case 64:
        return llvm::Type::getInt64Ty(*context);

      default:
        Logger::error("Unsupported int size: " + std::to_string(size));
    }
  };

  string to_string() const override { return "i" + ::to_string(size); };
  string to_string_tree() const override {
    return "IntType(size: '" + ::to_string(size) + "', is_unsigned: '" +
           ::to_string(is_unsigned) + "')";
  };
};
