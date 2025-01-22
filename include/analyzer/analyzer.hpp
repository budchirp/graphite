#pragma once

#include <memory>

#include "ast/program.hpp"
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

class Analyzer {
 private:
  shared_ptr<Program> program;

 public:
  explicit Analyzer(const shared_ptr<Program> &program) : program(program) {};

  void analyze() { program->analyze(); };

  static pair<bool, shared_ptr<PointerType>> is_pointer(
      const shared_ptr<Type> &type) {
    auto pointer = dynamic_pointer_cast<PointerType>(type);

    return {pointer != nullptr, pointer};
  }

  static pair<bool, shared_ptr<StringType>> is_string(
      const shared_ptr<Type> &type) {
    if (auto [is_ptr, ptr] = is_pointer(type); is_ptr) {
      auto pointer = dynamic_pointer_cast<StringType>(ptr->type);

      return {pointer != nullptr, pointer};
    }

    return {false, nullptr};
  }

  static pair<bool, shared_ptr<IntType>> is_int(const shared_ptr<Type> &type) {
    auto int_type = dynamic_pointer_cast<IntType>(type);

    return {int_type != nullptr, int_type};
  }

  static pair<bool, shared_ptr<BooleanType>> is_boolean(
      const shared_ptr<Type> &type) {
    auto boolean_type = dynamic_pointer_cast<BooleanType>(type);

    return {boolean_type != nullptr, boolean_type};
  }

  static pair<bool, shared_ptr<FloatType>> is_float(
      const shared_ptr<Type> &type) {
    auto float_type = dynamic_pointer_cast<FloatType>(type);

    return {float_type != nullptr, float_type};
  }

  static pair<bool, shared_ptr<VoidType>> is_void(
      const shared_ptr<Type> &type) {
    auto void_type = dynamic_pointer_cast<VoidType>(type);

    return {void_type != nullptr, void_type};
  }

  static pair<bool, shared_ptr<FunctionType>> is_function(
      const shared_ptr<Type> &type) {
    auto function_type = dynamic_pointer_cast<FunctionType>(type);

    return {function_type != nullptr, function_type};
  }

  static pair<bool, shared_ptr<NullType>> is_null(
      const shared_ptr<Type> &type) {
    auto null_type = dynamic_pointer_cast<NullType>(type);

    return {null_type != nullptr, null_type};
  }

  static bool compare(const shared_ptr<Type> &a, const shared_ptr<Type> &b,
                      bool allow_null = false) {
    if (allow_null && (is_null(a).first)) {
      return true;
    }

    auto a_pointer_type = is_pointer(a);
    auto b_pointer_type = is_pointer(b);
    if (a_pointer_type.first && b_pointer_type.first) {
      return compare(a_pointer_type.second->type, b_pointer_type.second->type);
    }

    return a->get_type_info() == b->get_type_info();
  }
};
