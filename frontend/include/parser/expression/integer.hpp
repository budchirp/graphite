#pragma once

#include <memory>

#include "ast/expression/integer.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class IntegerExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit IntegerExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<Expression> parse() override;
  shared_ptr<IntegerExpression> parse_integer() const;
};
