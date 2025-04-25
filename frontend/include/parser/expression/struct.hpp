#pragma once

#include <memory>

#include "ast/expression/struct.hpp"
#include "parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class StructExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

  shared_ptr<Expression> left;

 public:
  explicit StructExpressionParser(const shared_ptr<Parser> &parser,
                                  const shared_ptr<Expression> &left)
      : parser(parser), left(left) {};

  shared_ptr<Expression> parse() override;
  shared_ptr<StructExpression> parse_struct() const;
};
