#pragma once

#include <filesystem>
#include <string>
#include <vector>
using namespace std;

class Compiler {
 private:
  string ld_flags;

 public:
  explicit Compiler(const string &ld_flags) : ld_flags(ld_flags) {};

  void compile(const filesystem::path &source_file_path,
               const vector<string> &libs, const vector<string> &objs);
  void compile_gph(const filesystem::path &source_file_path);
  void compile_ir(const filesystem::path &ir_file_path);
  void compile_objects(const vector<string> &objs,
                       const filesystem::path &output_file_path);
};
