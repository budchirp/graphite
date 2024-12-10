#pragma once

#include <memory>

#include "frontend/ast/statement/function.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class FunctionStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit FunctionStatementParser(const shared_ptr<Parser> &parser);

  unique_ptr<FunctionStatement> parse();
};
