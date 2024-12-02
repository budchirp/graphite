#pragma once

#include <memory>

#include "frontend/ast/statements/return.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ReturnStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ReturnStatementParser(shared_ptr<Parser> parser);

  unique_ptr<ReturnStatement> parse();
};
