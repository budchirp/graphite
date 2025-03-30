#include "parser/parsers/statement/include.hpp"

#include <memory>

#include "ast/statement/include.hpp"
#include "lexer/token/token_type.hpp"
#include "parser/parsers/expression/string.hpp"

unique_ptr<IncludeStatement> IncludeStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat use

  if (parser->current_token.type != TokenType::TOKEN_STRING) {
    parser->get_logger()->error("Expected string after use",
                                LogTypes::Error::SYNTAX);
  }

  auto module_expression = StringExpressionParser(parser).parse_string();
  if (!module_expression) {
    parser->get_logger()->error("Failed to parse the module string",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  return make_unique<IncludeStatement>(position, std::move(module_expression));
}
