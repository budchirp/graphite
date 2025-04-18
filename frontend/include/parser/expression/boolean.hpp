#pragma once

#include <memory>

#include "ast/expression/boolean.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class BooleanExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit BooleanExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<Expression> parse() override;
  shared_ptr<BooleanExpression> parse_boolean() const;
};
