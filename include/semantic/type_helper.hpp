#pragma once

#include <memory>

#include "types/array.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/function.hpp"
#include "types/int.hpp"
#include "types/null.hpp"
#include "types/pointer.hpp"
#include "types/string.hpp"
#include "types/type.hpp"
#include "types/void.hpp"

using namespace std;

class TypeHelper {
 public:
  static shared_ptr<PointerType> is_pointer(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<PointerType>(type);
  }

  static shared_ptr<StringType> is_string(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<StringType>(type);
  }

  static shared_ptr<IntType> is_int(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<IntType>(type);
  }

  static shared_ptr<ArrayType> is_array(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<ArrayType>(type);
  }

  static shared_ptr<BooleanType> is_boolean(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<BooleanType>(type);
  }

  static shared_ptr<FloatType> is_float(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<FloatType>(type);
  }

  static shared_ptr<VoidType> is_void(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<VoidType>(type);
  }

  static shared_ptr<FunctionType> is_function(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<FunctionType>(type);
  }

  static shared_ptr<NullType> is_null(const shared_ptr<Type>& type) {
    return dynamic_pointer_cast<NullType>(type);
  }

  static bool compare(const shared_ptr<Type>& a, const shared_ptr<Type>& b,
                      bool allow_null = false) {
    if (allow_null && (is_null(a) || is_null(b))) {
      return true;
    }

    auto a_ptr = is_pointer(a);
    auto b_ptr = is_pointer(b);
    if (a_ptr && b_ptr) {
      return compare(a_ptr->pointee_type, b_ptr->pointee_type);
    }

    return a->get_type_info() == b->get_type_info();
  }
};
