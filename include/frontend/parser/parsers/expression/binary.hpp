#pragma once

#include <memory>

#include "frontend/ast/expression.hpp"
#include "frontend/parser/parser.hpp"
#include "frontend/parser/parsers/expression/parser.hpp"

using namespace std;

class BinaryExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;

  unique_ptr<Expression> left;

public:
  explicit BinaryExpressionParser(const shared_ptr<Parser> &parser, unique_ptr<Expression> left);

  unique_ptr<Expression> parse() override;
};
