#pragma once

#include <memory>

#include "parser/parser.hpp"
#include "parser/parsers/expression/parser.hpp"

using namespace std;

class UnaryExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

  unique_ptr<Expression> left;

 public:
  explicit UnaryExpressionParser(const shared_ptr<Parser> &parser,
                                 unique_ptr<Expression> left)
      : parser(parser), left(std::move(left)) {};

  unique_ptr<Expression> parse() override;
};
