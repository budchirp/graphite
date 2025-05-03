#pragma once

#include <memory>

#include "parser/expression/parser.hpp"
#include "parser/parser.hpp"

using namespace std;

class AssemblyExpressionParser : public ExpressionParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit AssemblyExpressionParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<Expression> parse();
};
