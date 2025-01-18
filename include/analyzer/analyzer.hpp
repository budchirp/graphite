#pragma once

#include <memory>

#include "ast/program.hpp"
#include "types/pointer.hpp"
#include "types/string.hpp"
#include "types/type.hpp"

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

  static bool compare(const shared_ptr<Type> &a, const shared_ptr<Type> &b) {
    return a->get_type_info() == b->get_type_info();
  }
};
