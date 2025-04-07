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

  shared_ptr<Expression> parse() override;
  shared_ptr<IdentifierExpression> parse_identifier() const;
};
