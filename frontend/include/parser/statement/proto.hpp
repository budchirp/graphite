#pragma once

#include <memory>

#include "ast/statement/proto.hpp"
#include "parser/parser.hpp"

using namespace std;

class ProtoStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit ProtoStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<ProtoStatement> parse();
};
