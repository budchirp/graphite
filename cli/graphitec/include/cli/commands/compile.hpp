#pragma once

#include "argman.hpp"

using namespace std;
using namespace ArgMan;

class CompileCommand : public Command {
 public:
  string name() const override { return "compile"; }
  string description() const override { return "Compile specified file"; }

  vector<Option> options() const override {
    return {
        Option("main", "Main file path", "string", false, ""),
        Option("objs", "Object files to link", "string", false, ""),
        Option("ldflags", "Linker flags", "string", false, ""),
    };
  }

  void execute() override;
};
