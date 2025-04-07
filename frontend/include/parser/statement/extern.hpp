#pragma once

#include <memory>

#include "ast/statement/extern.hpp"
#include "parser/parser.hpp"

using namespace std;

class ExternStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit ExternStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<ExternStatement> parse();
  shared_ptr<ExternStatement> parse(const TokenType &visibility) const;
};
