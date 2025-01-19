#pragma once

#include <memory>

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
};
