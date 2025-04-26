#pragma once

#include <memory>

#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class FieldExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

  shared_ptr<Expression> left;

 public:
  explicit FieldExpressionParser(const shared_ptr<Parser> &parser,
                                 const shared_ptr<Expression> &left)
      : parser(parser), left(left) {};

  shared_ptr<Expression> parse() override;
};
