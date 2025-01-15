#pragma once

#include <memory>

#include "ast/statement/function.hpp"
#include "parser/parser.hpp"

using namespace std;

class FunctionStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit FunctionStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<FunctionStatement> parse();
};
