#pragma once

#include <memory>

#include "frontend/ast/statements/proto.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ProtoStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ProtoStatementParser(shared_ptr<Parser> parser);

  unique_ptr<ProtoStatement> parse();
};
