#include "token/grammar.hpp"

const unordered_map<string, TokenType> keywords = {
    {"fn", TokenType::TOKEN_FUN},    {"extern", TokenType::TOKEN_EXTERN},
    {"if", TokenType::TOKEN_IF},     {"else", TokenType::TOKEN_ELSE},
    {"var", TokenType::TOKEN_VAR},   {"return", TokenType::TOKEN_RETURN},
    {"true", TokenType::TOKEN_TRUE}, {"false", TokenType::TOKEN_FALSE}};
