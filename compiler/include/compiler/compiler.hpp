#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "codegen_backend_type.hpp"
#include "program/program.hpp"

using namespace std;

class Compiler {
 private:
  CodegenBackendType::Value backend_type;

  string ld_flags;

 public:
  explicit Compiler(const CodegenBackendType::Value &backend_type,
                    const string &ld_flags)
      : backend_type(backend_type), ld_flags(ld_flags) {};

  void compile(const filesystem::path &main, const vector<string> &objs) const;

  vector<string> compile_project(const filesystem::path &root,
                                 const filesystem::path &filename) const;

  shared_ptr<Program> compile_gph(const filesystem::path &root,
                                  const filesystem::path &filename) const;

  void link(const vector<string> &objs, const filesystem::path &output) const;
};
