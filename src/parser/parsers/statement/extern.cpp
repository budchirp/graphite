#include "parser/parsers/statement/extern.hpp"

#include <memory>

#include "logger/log_types.hpp"
#include "parser/parsers/statement/proto.hpp"
#include "types/function.hpp"
#include "types/void.hpp"

unique_ptr<ExternStatement> ExternStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat extern

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after extern",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto proto = ProtoStatementParser(parser).parse();
  if (!proto) {
    parser->get_logger()->error("Failed to parse the proto of the function", LogTypes::Error::INTERNAL);
    return nullptr;
  }

  vector<shared_ptr<Type>> parameter_types;
  for (const auto &[_, type] : proto->parameters) {
    parameter_types.push_back(type->get_type());
  }

  auto env = parser->get_program()->get_env();

  env->set_type(proto->name->get_value(),
                make_shared<FunctionType>(parameter_types,
                                          proto->return_type->get_type()));
  env->set_symbol(proto->name->get_value(), proto->return_type->get_type());

  return make_unique<ExternStatement>(position, make_shared<VoidType>(),
                                      std::move(proto));
}
