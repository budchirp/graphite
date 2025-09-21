#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "compiler/compiler_backend.hpp"
#include "program/program.hpp"

using namespace std;

class Compiler {
 private:
  CompilerBackend::Value backend;

  string ld_flags;

 public:
  explicit Compiler(const CompilerBackend::Value &backend,
                    const string &ld_flags)
      : backend(backend), ld_flags(ld_flags) {};

  static shared_ptr<Program> parse_program(const filesystem::path &root,
                                           const filesystem::path &filename);

  void compile(const filesystem::path &main, const vector<string> &objs) const;

  vector<string> compile_project(const filesystem::path &root,
                                 const filesystem::path &filename) const;

  shared_ptr<Program> compile_gph(const filesystem::path &root,
                                  const filesystem::path &filename) const;

  void link(const vector<string> &objs, const filesystem::path &output) const;
};
