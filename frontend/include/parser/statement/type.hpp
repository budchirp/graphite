#pragma once

#include <memory>

#include "ast/statement/type.hpp"
#include "parser/parser.hpp"

using namespace std;

class TypeStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit TypeStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<TypeStatement> parse();
  shared_ptr<TypeStatement> parse(const TokenType &visibility) const;
};
