#pragma once

#include <memory>

#include "frontend/parser/parser.hpp"
#include "parser.hpp"

using namespace std;

class IdentifierExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit IdentifierExpressionParser(const shared_ptr<Parser> &parser);

  unique_ptr<Expression> parse() override;
};
