#pragma once

#include <memory>

#include "frontend/ast/statement/var.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class VarStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit VarStatementParser(const shared_ptr<Parser> &parser);

  unique_ptr<VarStatement> parse();
};
