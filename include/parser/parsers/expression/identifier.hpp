#pragma once

#include <memory>

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
  unique_ptr<Expression> parse(bool is_variable);
};
