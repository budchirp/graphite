#pragma once

#include <memory>

#include "ast/program.hpp"
#include "parser/parser.hpp"

using namespace std;

class ProgramParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ProgramParser(const shared_ptr<Parser> &parser) : parser(parser) {};

  shared_ptr<Program> parse();
};
