#pragma once

#include <memory>

#include "ast/expression/type.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class TypeExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit TypeExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<Expression> parse() override;
  shared_ptr<TypeExpression> parse_type();

  shared_ptr<Type> get_type();
};
