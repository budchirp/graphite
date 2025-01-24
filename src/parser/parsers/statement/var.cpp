#include "parser/parsers/statement/var.hpp"

#include <iostream>
#include <memory>

#include "analyzer/analyzer.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "env/env.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/expression/identifier.hpp"
#include "parser/parsers/expression/type.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"
#include "token/token_type.hpp"
#include "types/null.hpp"

unique_ptr<VarStatement> VarStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat var

  bool is_mutable = false;
  if (parser->current_token.type == TOKEN_MUT) {
    parser->eat_token();  // eat mut

    is_mutable = true;
  }

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
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

  shared_ptr<Type> type;
  unique_ptr<Expression> expression;

  auto expression_statement_parser = ExpressionStatementParser(parser);

  if (parser->current_token.type != TokenType::TOKEN_COLON) {
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

      type = expression->get_type();
    }
  } else {
    parser->eat_token();  // eat :

    unique_ptr<TypeExpression> type_expression(dynamic_cast<TypeExpression *>(
        TypeExpressionParser(parser).parse().release()));
    if (!type_expression) {
      parser->get_logger()->error(
          "Failed to parse the type of the variable statement",
          LogTypes::Error::INTERNAL);
      return nullptr;
    }

    type = type_expression->get_type();
  }

  if (parser->current_token.type == TokenType::TOKEN_ASSIGN) {
    parser->eat_token();  // eat =

    expression =
        expression_statement_parser.parse_expression(type, Precedence::LOWEST);
    if (!expression) {
      parser->get_logger()->error(
          "Failed to parse the expression of the variable statement",
          LogTypes::Error::INTERNAL);
      return nullptr;
    }
  }

  shared_ptr<Type> new_type = type;
  if (!expression) {
    if (!Analyzer::is_null(type).first ||
        !Analyzer::is_null(Analyzer::is_pointer(type).second).first) {
      if (auto pointer_type = Analyzer::is_pointer(type); pointer_type.first) {
        new_type = make_shared<PointerType>(
            make_shared<NullType>(pointer_type.second->pointee_type));
      }

      new_type = make_shared<NullType>(type);
    }
  }

  parser->get_program()->get_env()->add_variable(
      name_expression->get_identifier(),
      make_shared<EnvVariable>(new_type, type, is_mutable,
                               expression != nullptr));

  return make_unique<VarStatement>(
      position, is_mutable, expression != nullptr, std::move(name_expression),
      make_unique<TypeExpression>(position, std::move(type)),
      std::move(expression));
}
