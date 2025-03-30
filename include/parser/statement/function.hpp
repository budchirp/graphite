#pragma once

#include <memory>

#include "ast/statement/function.hpp"
#include "lexer/token/token.hpp"
#include "lexer/token/token_type.hpp"
#include "parser/parser.hpp"

using namespace std;

class FunctionStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit FunctionStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<FunctionStatement> parse();
  unique_ptr<FunctionStatement> parse(const TokenType &visibility);
};
