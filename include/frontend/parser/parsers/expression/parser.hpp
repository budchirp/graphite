#pragma once

#include <memory>

#include "frontend/ast/expression.hpp"

using namespace std;

class ExpressionParser {
public:
  virtual ~ExpressionParser() = default;

  virtual unique_ptr<Expression> parse() = 0;
};
