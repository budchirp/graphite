#pragma once

#include <memory>

#include "frontend/ast/statement/block.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class BlockStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit BlockStatementParser(const shared_ptr<Parser> &parser);

  unique_ptr<BlockStatement> parse();
};
