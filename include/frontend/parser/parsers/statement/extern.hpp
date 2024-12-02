#pragma once

#include <memory>

#include "frontend/ast/statements/extern.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ExternStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ExternStatementParser(shared_ptr<Parser> parser);

  unique_ptr<ExternStatement> parse();
};
