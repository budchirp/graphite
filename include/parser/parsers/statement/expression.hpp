#pragma once

#include <memory>

#include "ast/expression.hpp"
#include "ast/statement/expression.hpp"
#include "parser/parser.hpp"
#include "parser/precedence.hpp"

using namespace std;

class ExpressionStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ExpressionStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<ExpressionStatement> parse();
  unique_ptr<Expression> parse_expression(Precedence precedence);
};
