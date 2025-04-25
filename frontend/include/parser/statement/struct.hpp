#pragma once

#include <memory>

#include "ast/statement/struct.hpp"
#include "lexer/token/token_type.hpp"
#include "parser/parser.hpp"

using namespace std;

class StructStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit StructStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<StructStatement> parse();
  shared_ptr<StructStatement> parse(const TokenType &visibility);
};
