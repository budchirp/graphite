#include "parser/parsers/expression/unary.hpp"

#include <memory>

#include "analyzer/analyzer.hpp"
#include "ast/expression.hpp"
#include "ast/expression/unary.hpp"
#include "ast/expression/var_ref.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/statement/expression.hpp"
#include "parser/precedence.hpp"
#include "token/token.hpp"
#include "token/token_type.hpp"
#include "types/pointer.hpp"
#include "types/type.hpp"

unique_ptr<Expression> UnaryExpressionParser::parse() {
  Token op_token;

  unique_ptr<VarRefExpression> expression;
  shared_ptr<Type> type;

  if (left) {
    unique_ptr<VarRefExpression> _expression(
        dynamic_cast<VarRefExpression *>(left.release()));
    if (!_expression) {
      parser->get_logger()->error("Expected variable reference expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    expression = std::move(_expression);
    type = expression->get_type();

    op_token = parser->current_token;
    parser->eat_token();  // eat postfix

    switch (op_token.type) {
      case TOKEN_BANG_BANG: {
        if (auto null_type = Analyzer::is_null(type);
            null_type.first && null_type.second->child_type) {
          type = null_type.second->child_type;
        } else {
          parser->get_logger()->error(
              "Trying to make a non-null value non-null",
              LogTypes::Error::TYPE_MISMATCH);
          return nullptr;
        }

        return make_unique<VarRefExpression>(*expression->get_position(), type,
                                             expression->get_name());

        break;
      }

      default:
        break;
    }
  } else {
    op_token = parser->current_token;
    parser->eat_token();  // eat prefix

    unique_ptr<VarRefExpression> _expression(dynamic_cast<VarRefExpression *>(
        ExpressionStatementParser(parser)
            .parse_expression(Precedence::LOWEST)
            .release()));
    if (!_expression) {
      parser->get_logger()->error("Expected variable reference expression",
                                  LogTypes::Error::SYNTAX);
      return nullptr;
    }

    expression = std::move(_expression);
    type = expression->get_type();

    switch (op_token.type) {
      case TOKEN_ASTERISK: {
        if (auto pointer_type = Analyzer::is_pointer(type);
            pointer_type.first) {
          type = pointer_type.second->pointee_type;
        } else {
          parser->get_logger()->error("Cannot dereference non-pointer type",
                                      LogTypes::Error::TYPE_MISMATCH);
          return nullptr;
        }

        break;
      }

      case TOKEN_AMPERSAND: {
        type = make_shared<PointerType>(type);
        break;
      }

      default:
        break;
    }
  }

  return make_unique<UnaryExpression>(*expression->get_position(), type,
                                      op_token, std::move(expression));
}
