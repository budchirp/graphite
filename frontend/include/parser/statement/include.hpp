#pragma once

#include <memory>

#include "ast/statement/include.hpp"
#include "parser/parser.hpp"

using namespace std;

class IncludeStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit IncludeStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<IncludeStatement> parse();
};
