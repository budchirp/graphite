#pragma once

#include <memory>

#include "frontend/ast/program.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ProgramParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ProgramParser(const shared_ptr<Parser> &parser);

  shared_ptr<Program> parse();
};
