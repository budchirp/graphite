#include "parser/parsers/statement/function.hpp"

#include <memory>

#include "env/env.hpp"
#include "logger/log_types.hpp"
#include "parser/parsers/statement/block.hpp"
#include "parser/parsers/statement/proto.hpp"
#include "types/function.hpp"

unique_ptr<FunctionStatement> FunctionStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat fn

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after fn",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto env = make_shared<Env>(parser->get_program()->get_env());
  parser->get_program()->set_env(env);

  auto proto_statement = ProtoStatementParser(parser).parse();
  if (!proto_statement) {
    parser->get_logger()->error("Failed to parse the proto of the function",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after function prototype",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  vector<pair<string, shared_ptr<Type>>> parameters;
  for (const auto &[parameter_name, parameter_type_expression] : proto_statement->parameters) {
    env->add_variable(parameter_name->get_identifier(),
                      make_shared<EnvVariable>(parameter_type_expression->get_type(), false));

    parameters.emplace_back(parameter_name->get_identifier(), parameter_type_expression->get_type());
  }

  auto body_statement = BlockStatementParser(parser).parse();
  if (!body_statement) {
    parser->get_logger()->error("Failed to parse the body of the function",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  auto parent_env = env->get_parent();
  parent_env->add_function(
      proto_statement->name->get_identifier(),
      make_shared<EnvFunction>(
          make_shared<FunctionType>(parameters, proto_statement->return_type->get_type())));

  parser->get_program()->set_env(parent_env);

  return make_unique<FunctionStatement>(position, env, std::move(proto_statement),
                                        std::move(body_statement));
}
