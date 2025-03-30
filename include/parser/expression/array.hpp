#pragma once

#include <memory>

#include "ast/expression/array.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class ArrayExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit ArrayExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<Expression> parse() override;
  unique_ptr<ArrayExpression> parse_array() const;
};
