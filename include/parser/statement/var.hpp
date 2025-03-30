#pragma once

#include <memory>

#include "ast/statement/var.hpp"
#include "parser/parser.hpp"
#include "semantic/visibilty.hpp"

using namespace std;

class VarStatementParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit VarStatementParser(const shared_ptr<Parser> &parser)
      : parser(parser) {};

  unique_ptr<VarStatement> parse();
  unique_ptr<VarStatement> parse(const SymbolVisibility::Value &visibility) const;
};
