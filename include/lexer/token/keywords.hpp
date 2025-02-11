#pragma once

#include <string>
#include <unordered_map>

#include "lexer/token/token_type.hpp"

using namespace std;

extern const unordered_map<string, TokenType> keywords;
