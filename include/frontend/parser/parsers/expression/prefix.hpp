#pragma once

#include <memory>

#include "frontend/parser/parser.hpp"
#include "frontend/parser/parsers/expression/parser.hpp"

using namespace std;

class PrefixExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit PrefixExpressionParser(shared_ptr<Parser> parser);

  unique_ptr<Expression> parse();
};
