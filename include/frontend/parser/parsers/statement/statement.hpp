#pragma once

#include <memory>

#include "frontend/ast/statement.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class StatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit StatementParser(const shared_ptr<Parser> &parser);

  unique_ptr<Statement> parse();
};
