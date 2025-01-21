#pragma once

#include <memory>

#include "ast/statement/while.hpp"
#include "parser/parser.hpp"

using namespace std;

class WhileStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit WhileStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<WhileStatement> parse();
};
