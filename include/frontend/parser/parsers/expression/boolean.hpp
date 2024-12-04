#pragma once

#include <memory>

#include "frontend/parser/parser.hpp"
#include "parser.hpp"

using namespace std;

class BooleanExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit BooleanExpressionParser(shared_ptr<Parser> parser);

  unique_ptr<Expression> parse();
};
