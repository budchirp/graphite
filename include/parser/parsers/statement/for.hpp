#pragma once

#include <memory>

#include "ast/statement/for.hpp"
#include "parser/parser.hpp"

using namespace std;

class ForStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit ForStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<ForStatement> parse();
};
