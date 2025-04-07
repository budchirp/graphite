#pragma once

#include <memory>

#include "ast/statement.hpp"
#include "parser/parser.hpp"

using namespace std;

class VisibilityStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit VisibilityStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<Statement> parse();
};
