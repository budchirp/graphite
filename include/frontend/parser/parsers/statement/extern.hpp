#pragma once

#include <memory>

#include "frontend/ast/statement/extern.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ExternStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ExternStatementParser(const shared_ptr<Parser> &parser);

  unique_ptr<ExternStatement> parse();
};
