#pragma once

#include <memory>

#include "frontend/lexer/lexer.hpp"
#include "frontend/token/token.hpp"

using namespace std;

class Parser {
private:
  shared_ptr<Lexer> lexer;

public:
  Token current_token;
  Token next_token;

  explicit Parser(shared_ptr<Lexer> lexer);

  void eat_token();
};
