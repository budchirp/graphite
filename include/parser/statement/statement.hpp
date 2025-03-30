#pragma once

#include <memory>

#include "ast/statement.hpp"
#include "parser/parser.hpp"

using namespace std;

class StatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit StatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<Statement> parse();
};
