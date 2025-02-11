#include "lexer/lexer.hpp"

#include <cctype>
#include <string>

#include "lexer/token/keywords.hpp"
#include "logger/logger.hpp"

using namespace std;

char Lexer::get_current_char() const {
  return (read_position < source.size()) ? source[read_position] : '\0';
}

void Lexer::eat_char() {
  current_char = get_current_char();

  read_position++;
  position->column++;

  next_char = get_current_char();
}

void Lexer::eat_whitespace() {
  if (current_char == '#') {
    while (current_char != '\n') {
      eat_char();
    }
  }

  while (current_char == '\n') {
    position->column = 1;
    position->line++;
    eat_char();
    eat_whitespace();
  }

  while (current_char == ' ' || current_char == '\t') {
    eat_char();
  }
}

Token Lexer::next_token() {
  Token token;

  eat_whitespace();

  switch (current_char) {
    case '!':
      if (next_char == '=') {
        eat_char();
        token = Token(TOKEN_NOT_EQUAL, "!=");
      } else if (next_char == '!') {
        eat_char();
        token = Token(TOKEN_BANG_BANG, "!!");
      } else {
        token = Token(TOKEN_BANG, "!");
      }
      break;

    case '=':
      if (next_char == '=') {
        eat_char();
        token = Token(TOKEN_EQUAL, "==");
      } else {
        token = Token(TOKEN_ASSIGN, "=");
      }
      break;

    case '&':
      if (next_char == '&') {
        eat_char();
        token = Token(TOKEN_AND, "&&");
      } else {
        token = Token(TOKEN_AMPERSAND, "&");
      }
      break;

    case '|':
      if (next_char == '|') {
        eat_char();
        token = Token(TOKEN_OR, "||");
      } else {
        Logger::error("Unknown char " + ::to_string(current_char));
      }
      break;

    case '<':
      token = Token(TOKEN_LESS_THAN, "<");
      break;

    case '>':
      token = Token(TOKEN_GREATER_THAN, ">");
      break;

    case '+':
      if (next_char == '+') {
        eat_char();
        token = Token(TOKEN_PLUSPLUS, "++");
      } else {
        token = Token(TOKEN_PLUS, "+");
      }
      break;

    case '-':
      if (next_char == '>') {
        eat_char();
        token = Token(TOKEN_ARROW, "->");
      } else if (next_char == '-') {
        eat_char();
        token = Token(TOKEN_MINUSMINUS, "--");
      } else {
        token = Token(TOKEN_MINUS, "-");
      }
      break;

    case '*':
      token = Token(TOKEN_ASTERISK, "*");
      break;

    case '/':
      token = Token(TOKEN_SLASH, "/");
      break;

    case '?':
      token = Token(TOKEN_QUESTION_MARK, "?");
      break;

    case ':':
      token = Token(TOKEN_COLON, ":");

      break;

    case ';':
      token = Token(TOKEN_SEMICOLON, ";");
      break;

    case ',':
      token = Token(TOKEN_COMMA, ",");
      break;

    case '(':
      token = Token(TOKEN_LEFT_PARENTHESES, "(");
      break;

    case ')':
      token = Token(TOKEN_RIGHT_PARENTHESES, ")");
      break;

    case '{':
      token = Token(TOKEN_LEFT_BRACE, "{");
      break;

    case '}':
      token = Token(TOKEN_RIGHT_BRACE, "}");
      break;

    case '[':
      token = Token(TOKEN_LEFT_BRACKET, "[");
      break;

    case ']':
      token = Token(TOKEN_RIGHT_BRACKET, "]");
      break;

    case '"': {
      eat_char();  // eat "

      string str;
      do {
        if (current_char == '\\') {
          eat_char();
          if (current_char == 'n') {
            str += '\n';
          } else if (current_char == 't') {
            str += '\t';
          } else if (current_char == '\\') {
            str += '\\';
          } else if (current_char == '"') {
            str += '"';
          } else {
            str += '\\';
            str += current_char;
          }
        } else {
          str += current_char;
        }

        eat_char();
      } while (current_char != '"');

      token = Token(TOKEN_STRING, str);
      break;
    }

    case '\0':
      token = Token(TOKEN_EOF, "");
      break;

    default:
      if (isalpha(current_char)) {
        string identifier;
        do {
          identifier += current_char;
          if (!(isalnum(next_char) || next_char == '_')) break;

          eat_char();
        } while (isalnum(current_char) || current_char == '_');

        if (keywords.find(identifier) != keywords.end()) {
          token = Token(keywords.at(identifier), identifier);
        } else {
          token = Token(TOKEN_IDENTIFIER, identifier);
        }
      } else if (isdigit(current_char)) {
        string number;
        do {
          number += current_char;
          if (!isdigit(next_char)) break;

          eat_char();
        } while (isdigit(current_char));

        token = Token(TOKEN_INT, number);
      }
  }

  eat_char();
  eat_whitespace();

  return token;
}
