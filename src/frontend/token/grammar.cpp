#include "frontend/token/grammar.hpp"

const unordered_map<string, TokenType> keywords = {
    {"fn", TokenType::TOKEN_FUN},
    {"extern", TokenType::TOKEN_EXTERN},
    {"var", TokenType::TOKEN_VAR},
    {"return", TokenType::TOKEN_RETURN}};
