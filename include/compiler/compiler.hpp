#pragma once

#include <filesystem>
#include <string>
#include <vector>
using namespace std;

class Compiler {
 public:
  explicit Compiler() {};

  void compile(const filesystem::path &filename);
  void compile_ir(const filesystem::path &filename);
  void compile_objects(const vector<string> &objs, const filesystem::path &filename);
};
