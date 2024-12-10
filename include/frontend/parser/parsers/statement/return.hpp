#pragma once

#include <memory>

#include "frontend/ast/statement/return.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ReturnStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ReturnStatementParser(const shared_ptr<Parser> &parser);

  unique_ptr<ReturnStatement> parse();
};
