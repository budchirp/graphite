#pragma once

#include <memory>

#include "ast/statement/proto.hpp"
#include "parser/parser.hpp"

using namespace std;

class FunctionProtoStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit FunctionProtoStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<FunctionProtoStatement> parse();
};

class VarProtoStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit VarProtoStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  shared_ptr<VarProtoStatement> parse();
};