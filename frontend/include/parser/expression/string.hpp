#pragma once

#include <memory>

#include "ast/expression/string.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class StringExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit StringExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<Expression> parse() override;
  shared_ptr<StringExpression> parse_string() const;
};
