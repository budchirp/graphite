#pragma once

#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/statements/expression.hpp"
#include "frontend/parser/parser.hpp"
#include "frontend/parser/precedence.hpp"

using namespace std;

class ExpressionStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ExpressionStatementParser(shared_ptr<Parser> parser);

  unique_ptr<ExpressionStatement> parse();
  unique_ptr<Expression> parse_expression(Precedence precedence);
};
