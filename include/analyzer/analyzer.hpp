#pragma once

#include <memory>

#include "ast/program.hpp"
#include "types/boolean.hpp"
#include "types/float.hpp"
#include "types/function.hpp"
#include "types/int.hpp"
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

    return pair(pointer != nullptr, pointer);
  }

  static pair<bool, shared_ptr<StringType>> is_string(
      const shared_ptr<Type> &type) {
    if (auto [is_ptr, ptr] = is_pointer(type); is_ptr) {
      auto pointer = dynamic_pointer_cast<StringType>(ptr->type);

      return pair(pointer != nullptr, pointer);
    }

    return pair(false, nullptr);
  }

  static pair<bool, shared_ptr<IntType>> is_int(const shared_ptr<Type> &type) {
    auto int_type = dynamic_pointer_cast<IntType>(type);

    return pair(int_type != nullptr, int_type);
  }

  static pair<bool, shared_ptr<BooleanType>> is_boolean(
      const shared_ptr<Type> &type) {
    auto boolean_type = dynamic_pointer_cast<BooleanType>(type);

    return pair(boolean_type != nullptr, boolean_type);
  }

  static pair<bool, shared_ptr<FloatType>> is_float(
      const shared_ptr<Type> &type) {
    auto float_type = dynamic_pointer_cast<FloatType>(type);

    return pair(float_type != nullptr, float_type);
  }

  static pair<bool, shared_ptr<VoidType>> is_void(
      const shared_ptr<Type> &type) {
    auto void_type = dynamic_pointer_cast<VoidType>(type);

    return pair(void_type != nullptr, void_type);
  }

  static pair<bool, shared_ptr<FunctionType>> is_function(
      const shared_ptr<Type> &type) {
    auto function_type = dynamic_pointer_cast<FunctionType>(type);

    return pair(function_type != nullptr, function_type);
  }

  static bool compare(const shared_ptr<Type> &a, const shared_ptr<Type> &b) {
    return a->get_type_info() == b->get_type_info();
  }
};
