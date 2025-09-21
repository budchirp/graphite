#pragma once

struct CodegenOptions {
  explicit CodegenOptions(const bool &load_variable, const bool &load_pointer)
      : load_variable(load_variable), load_pointer(load_pointer) {}

  bool load_variable;
  bool load_pointer;
};
