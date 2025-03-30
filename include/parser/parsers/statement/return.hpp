#pragma once

#include <memory>

#include "ast/statement/return.hpp"
#include "parser/parser.hpp"

using namespace std;

class ReturnStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit ReturnStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<ReturnStatement> parse();
};
