#pragma once

#include <memory>

#include "frontend/lexer/lexer.hpp"
#include "frontend/token/token.hpp"
#include "logger/logger.hpp"

using namespace std;

class Parser {
private:
  shared_ptr<Lexer> lexer;

public:
  shared_ptr<Logger> logger;

  Token current_token;
  Token next_token;

  explicit Parser(shared_ptr<Lexer> lexer);

  void eat_token();

  shared_ptr<Lexer> get_lexer();
};
