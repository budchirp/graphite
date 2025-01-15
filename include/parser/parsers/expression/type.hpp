#pragma once

#include <memory>

#include "parser.hpp"
#include "parser/parser.hpp"
#include "types/type.hpp"

using namespace std;

class TypeExpressionParser : public ExpressionParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit TypeExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<Expression> parse() override;
  shared_ptr<Type> parse_type(const shared_ptr<Parser> &parser);
};
