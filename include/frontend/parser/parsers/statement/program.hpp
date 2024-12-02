#pragma once

#include <memory>

#include "frontend/ast/program.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ProgramParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ProgramParser(shared_ptr<Parser> parser);

  unique_ptr<Program> parse();
};
