#pragma once

#include <memory>

#include "ast/expression/var_ref.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class VarRefExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit VarRefExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<Expression> parse() override;
  unique_ptr<VarRefExpression> parse_var_ref() const;
};
