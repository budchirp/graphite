#pragma once

#include <memory>

#include "parser/parser.hpp"
#include "parser/parsers/expression/parser.hpp"

using namespace std;

class PrefixExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit PrefixExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<Expression> parse() override;
};
