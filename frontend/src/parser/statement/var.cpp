#include "parser/statement/var.hpp"

#include <memory>

#include "ast/expression/type.hpp"
#include "ast/statement/var.hpp"
#include "logger/log_types.hpp"
#include "parser/expression/identifier.hpp"
#include "parser/expression/type.hpp"
#include "parser/precedence.hpp"
#include "parser/statement/expression.hpp"
#include "semantic/visibilty.hpp"

shared_ptr<VarStatement> VarStatementParser::parse() {
  return parse(SymbolVisibility::Value::SCOPE);
}

shared_ptr<VarStatement> VarStatementParser::parse(
    const SymbolVisibility::Value &visibility) const {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat var

  bool is_mutable = false;
  if (parser->current_token.type == TOKEN_MUT) {
    parser->eat_token();  // eat mut

    is_mutable = true;
  }

  if (parser->current_token.type != TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after var",
                                LogTypes::Error::SYNTAX);
  }

  auto name_expression = IdentifierExpressionParser(parser).parse_identifier();
  if (!name_expression) {
    parser->get_logger()->error(
        "Failed to parse the name of the variable statement",
        LogTypes::Error::INTERNAL);
    return nullptr;
  }

  shared_ptr<TypeExpression> type_expression;
  shared_ptr<Expression> expression;

  auto expression_statement_parser = ExpressionStatementParser(parser);

  if (parser->current_token.type != TOKEN_COLON) {
    if (parser->current_token.type != TOKEN_ASSIGN) {
      parser->get_logger()->error("Expected : or = after identifier",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    } else {
      parser->eat_token();  // eat =

      expression =
          expression_statement_parser.parse_expression(Precedence::LOWEST);
      if (!expression) {
        parser->get_logger()->error(
            "Failed to parse the expression of the variable statement",
            LogTypes::Error::INTERNAL);
        return nullptr;
      }
    }
  } else {
    parser->eat_token();  // eat :

    shared_ptr<TypeExpression> _type_expression =
        TypeExpressionParser(parser).parse_type();
    if (!_type_expression) {
      parser->get_logger()->error(
          "Failed to parse the type of the variable statement",
          LogTypes::Error::INTERNAL);
      return nullptr;
    }

    type_expression = std::move(_type_expression);
  }

  if (parser->current_token.type == TOKEN_ASSIGN) {
    parser->eat_token();  // eat =

    expression =
        expression_statement_parser.parse_expression(Precedence::LOWEST);
    if (!expression) {
      parser->get_logger()->error(
          "Failed to parse the expression of the variable statement",
          LogTypes::Error::INTERNAL);
      return nullptr;
    }
  }

  return make_shared<VarStatement>(
      position, visibility, is_mutable, expression != nullptr,
      name_expression, type_expression,
      expression);
}
