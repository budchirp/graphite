#include <cctype>
#include <string>
#include <unordered_map>

#include "frontend/lexer/lexer.hpp"
#include "frontend/lexer/position.hpp"
#include "frontend/token/grammar.hpp"
#include "frontend/token/token_type.hpp"

using namespace std;

Lexer::Lexer(const string &source) : source(source), position(0, 0) {
  eat_char();
}

i8 Lexer::get_current_char() {
  return (read_position < source.size()) ? source[read_position] : '\0';
}

void Lexer::eat_char() {
  current_char = get_current_char();

  read_position++;
  position.column++;

  next_char = get_current_char();
}

void Lexer::eat_whitespace() {
  while ((current_char == ' ' || current_char == '\t') &&
         current_char != '\0') {
    eat_char();
  }
}

Token Lexer::tokenize(i8 character) {
  Token token;

  switch (character) {
  case '!':
    if (next_char == '=') {
      eat_char();
      token = Token(TokenType::TOKEN_NOT_EQUAL, "!=");
    } else {
      token = Token(TokenType::TOKEN_BANG, "!");
    }
    break;

  case '=':
    if (next_char == '=') {
      eat_char();
      token = Token(TokenType::TOKEN_EQUAL, "==");
    } else {
      token = Token(TokenType::TOKEN_ASSIGN, "=");
    }
    break;

  case '<':
    token = Token(TokenType::TOKEN_LESS_THAN, "<");
    break;

  case '>':
    token = Token(TokenType::TOKEN_GREATER_THAN, ">");
    break;

  case '+':
    token = Token(TokenType::TOKEN_PLUS, "+");
    break;

  case '-':
    token = Token(TokenType::TOKEN_MINUS, "-");
    break;

  case '*':
    token = Token(TokenType::TOKEN_ASTERISK, "*");
    break;

  case '/':
    token = Token(TokenType::TOKEN_SLASH, "/");
    break;

  case ';':
    token = Token(TokenType::TOKEN_SEMICOLON, ";");
    break;

  case ',':
    token = Token(TokenType::TOKEN_COMMA, ",");
    break;

  case '(':
    token = Token(TokenType::TOKEN_LEFT_PARENTHESES, "(");
    break;

  case ')':
    token = Token(TokenType::TOKEN_RIGHT_PARENTHESES, ")");
    break;

  case '{':
    token = Token(TokenType::TOKEN_LEFT_BRACE, "{");
    break;

  case '}':
    token = Token(TokenType::TOKEN_RIGHT_BRACE, "}");
    break;

  case '\0':
    token = Token(TokenType::TOKEN_EOF, "");
    break;

  default:
    if (isalpha(character)) {
      string identifier;
      do {
        identifier += current_char;
        if (!(isalnum(next_char) || next_char == '_'))
          break;

        eat_char();
      } while (isalnum(current_char) || current_char == '_');

      if (keywords.find(identifier) != keywords.end()) {
        token = Token(keywords.at(identifier), identifier);
      } else {
        token = Token(TokenType::TOKEN_IDENTIFIER, identifier);
      }
    } else if (isdigit(character)) {
      string number;
      do {
        number += current_char;
        if (!isdigit(next_char))
          break;

        eat_char();
      } while (isdigit(current_char));

      token = Token(TokenType::TOKEN_INT, number);
    } else {
      token = Token(TokenType::TOKEN_EOF, "");
    }
  }

  eat_char();

  return token;
}

Token Lexer::next_token() {
  eat_whitespace();
  return tokenize(current_char);
}
