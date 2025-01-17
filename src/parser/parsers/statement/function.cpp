#include "parser/parsers/statement/function.hpp"

#include <memory>

#include "logger/log_types.hpp"
#include "parser/parsers/statement/block.hpp"
#include "parser/parsers/statement/proto.hpp"
#include "types/function.hpp"
#include "types/void.hpp"

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

  auto proto = ProtoStatementParser(parser).parse();
  if (!proto) {
    parser->get_logger()->error("Failed to parse the proto of the function", LogTypes::Error::INTERNAL);
    return nullptr;
  }

  if (parser->current_token.type != TokenType::TOKEN_LEFT_BRACE) {
    parser->get_logger()->error("Expected left brace after function prototype",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  vector<shared_ptr<Type>> parameter_types;
  for (const auto &[name, type] : proto->parameters) {
    env->set_symbol(name->get_value(), type->get_type());

    parameter_types.push_back(type->get_type());
  }

  auto body = BlockStatementParser(parser).parse();
  if (!body) {
    parser->get_logger()->error("Failed to parse the body of the function", LogTypes::Error::INTERNAL);
    return nullptr;
  }

  auto parent_env = env->get_parent();
  parent_env->set_type(proto->name->get_value(),
                       make_shared<FunctionType>(
                           parameter_types, proto->return_type->get_type()));
  parent_env->set_symbol(proto->name->get_value(),
                         proto->return_type->get_type());

  parser->get_program()->set_env(parent_env);

  return make_unique<FunctionStatement>(position, make_shared<VoidType>(), env, std::move(proto),
                                        std::move(body));
}
