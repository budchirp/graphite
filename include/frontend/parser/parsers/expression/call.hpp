#pragma once

#include <memory>

#include "frontend/parser/parser.hpp"
#include "parser.hpp"

using namespace std;

class CallExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;
  
  unique_ptr<Expression> left;

public:
  explicit CallExpressionParser(const shared_ptr<Parser> &parser, unique_ptr<Expression> left);

  unique_ptr<Expression> parse() override;
};
