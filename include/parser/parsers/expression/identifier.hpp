#pragma once

#include <memory>

#include "parser/parser.hpp"
#include "parser.hpp"

using namespace std;

class IdentifierExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit IdentifierExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<Expression> parse() override;
};
