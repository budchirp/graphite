#pragma once

#include <memory>

#include "env/env.hpp"
#include "lexer/lexer.hpp"
#include "logger/logger.hpp"
#include "token/token.hpp"

using namespace std;

class Parser {
private:
  shared_ptr<Lexer> lexer;
  shared_ptr<Logger> logger;
  shared_ptr<Env> env;

public:
  Token current_token;
  Token next_token;

  explicit Parser(shared_ptr<Lexer> lexer, shared_ptr<Env> env);

  void eat_token();

  shared_ptr<Lexer> get_lexer() { return lexer; };
  shared_ptr<Logger> get_logger() { return logger; };
  shared_ptr<Env> get_env() { return env; };

  void set_env(shared_ptr<Env> env) { this->env = env; }
};
