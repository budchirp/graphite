#pragma once

#include <memory>

#include "parser.hpp"
#include "ast/expression.hpp"
#include "parser/parser.hpp"

using namespace std;

class BinaryExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

  unique_ptr<Expression> left;

 public:
  explicit BinaryExpressionParser(const shared_ptr<Parser> &parser,
                                  unique_ptr<Expression> left)
      : parser(parser), left(std::move(left)) {};

  unique_ptr<Expression> parse() override;
};
