#pragma once

#include <memory>

#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class IndexExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

  unique_ptr<Expression> left;

 public:
  explicit IndexExpressionParser(const shared_ptr<Parser> &parser,
                                 unique_ptr<Expression> left)
      : parser(parser), left(std::move(left)) {};

  unique_ptr<Expression> parse() override;
};
