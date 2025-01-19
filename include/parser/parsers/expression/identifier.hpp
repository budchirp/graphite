#pragma once

#include <memory>

#include "ast/expression/identifier.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class IdentifierExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit IdentifierExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<Expression> parse() override;
  unique_ptr<IdentifierExpression> parse_identifier();
};
