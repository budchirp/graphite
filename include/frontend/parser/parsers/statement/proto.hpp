#pragma once

#include <memory>

#include "frontend/ast/statement/proto.hpp"
#include "frontend/parser/parser.hpp"

using namespace std;

class ProtoStatementParser {
private:
  shared_ptr<Parser> parser;

public:
  explicit ProtoStatementParser(const shared_ptr<Parser> &parser);

  unique_ptr<ProtoStatement> parse();
};
