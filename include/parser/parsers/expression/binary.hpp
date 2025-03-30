#pragma once

#include <memory>

#include "ast/expression.hpp"
#include "parser/parser.hpp"
#include "parser/parsers/expression/parser.hpp"

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
