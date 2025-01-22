#pragma once

enum TokenType {
  TOKEN_EOF,

  TOKEN_FUN,
  TOKEN_RETURN,

  TOKEN_EXTERN,

  TOKEN_IF,
  TOKEN_ELSE,

  TOKEN_VAR,

  TOKEN_FOR,
  TOKEN_WHILE,

  TOKEN_IDENTIFIER,
  TOKEN_INT,
  TOKEN_STRING,

  TOKEN_TRUE,
  TOKEN_FALSE,

  TOKEN_BANG,
  TOKEN_ASSIGN,
  TOKEN_EQUAL,
  TOKEN_NOT_EQUAL,

  TOKEN_AS,

  TOKEN_AND,
  TOKEN_OR,

  TOKEN_LESS_THAN,
  TOKEN_GREATER_THAN,

  TOKEN_AMPERSAND,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_ASTERISK,
  TOKEN_SLASH,

  TOKEN_HASHTAG,

  TOKEN_PLUSPLUS,
  TOKEN_MINUSMINUS,

  TOKEN_COLON,
  TOKEN_SEMICOLON,
  TOKEN_COMMA,

  TOKEN_ARROW,

  TOKEN_QUOTE,

  TOKEN_LEFT_PARENTHESES,
  TOKEN_RIGHT_PARENTHESES,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
};
