#include "token/grammar.hpp"

#include "token/token_type.hpp"

const unordered_map<string, TokenType> keywords = {
    {"fn", TOKEN_FUN},      {"extern", TOKEN_EXTERN}, {"if", TOKEN_IF},
    {"else", TOKEN_ELSE},   {"var", TOKEN_VAR},       {"return", TOKEN_RETURN},
    {"true", TOKEN_TRUE},   {"false", TOKEN_FALSE},   {"for", TOKEN_FOR},
    {"while", TOKEN_WHILE}, {"as", TOKEN_AS},         {"mut", TOKEN_MUT}};
