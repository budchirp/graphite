#pragma once

#include <memory>

#include "frontend/ast/statements/function.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class FunctionStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit FunctionStatementParser(shared_ptr<Parser> parser);

  unique_ptr<FunctionStatement> parse();
};
