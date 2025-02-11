#pragma once

#include <memory>

#include "ast/program.hpp"
#include "lexer/lexer.hpp"
#include "logger/logger.hpp"

using namespace std;

class Parser {
 private:
  shared_ptr<Lexer> lexer;
  shared_ptr<Logger> logger;
  shared_ptr<Program> program;

 public:
  Token current_token;
  Token next_token;

  explicit Parser(const shared_ptr<Lexer> &lexer, const shared_ptr<Program> &program);

  void eat_token();

  shared_ptr<Lexer> get_lexer() const { return lexer; };
  shared_ptr<Logger> get_logger() const { return logger; };
  shared_ptr<Program> get_program() const { return program; }
};
