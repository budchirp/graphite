#pragma once

#include <memory>

#include "frontend/ast/statements/block.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class BlockStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit BlockStatementParser(shared_ptr<Parser> parser);

  unique_ptr<BlockStatement> parse();
};
