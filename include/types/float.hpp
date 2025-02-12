#pragma once

#include <string>

#include "logger/logger.hpp"
#include "types/int.hpp"
#include "types/type.hpp"

class FloatType : public Type {
 public:
  short size;

  explicit FloatType(const short size) : size(size) {};

  llvm::Type* to_llvm(
      const shared_ptr<llvm::LLVMContext> context) const override {
    switch (size) {
      case 16:
        return llvm::Type::getHalfTy(*context);
      case 32:
        return llvm::Type::getFloatTy(*context);
      case 64:
        return llvm::Type::getDoubleTy(*context);
      case 128:
        return llvm::Type::getFP128Ty(*context);

      default:
        Logger::error("Unsupported floating-point size: " +
                      std::to_string(size));
        return nullptr;
    }
  };

  string get_name() const override { return "f" + ::to_string(size); }
  const type_info& get_type_info() const override { return typeid(IntType); }

  string to_string() const override { return get_name(); };
  string to_string_tree() const override {
    return "FloatType(size: '" + ::to_string(size) + "')";
  };
};
