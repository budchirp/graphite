#pragma once

#include <memory>

#include "ast/statement/block.hpp"
#include "parser/parser.hpp"

using namespace std;

class BlockStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit BlockStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<BlockStatement> parse();
};
