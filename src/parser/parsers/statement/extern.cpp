#include "parser/parsers/statement/extern.hpp"

#include <memory>

#include "logger/log_types.hpp"
#include "parser/parsers/statement/proto.hpp"
#include "types/function.hpp"

unique_ptr<ExternStatement> ExternStatementParser::parse() {
  const auto position = *parser->get_lexer()->position;

  parser->eat_token();  // eat extern

  if (parser->current_token.type != TokenType::TOKEN_IDENTIFIER) {
    parser->get_logger()->error("Expected identifier after extern",
                                LogTypes::Error::SYNTAX);
    return nullptr;
  }

  auto proto_statement = ProtoStatementParser(parser).parse();
  if (!proto_statement) {
    parser->get_logger()->error("Failed to parse the proto of the function",
                                LogTypes::Error::INTERNAL);
    return nullptr;
  }

  vector<pair<string, shared_ptr<Type>>> parameters;
  for (const auto &[parameter_name, parameter_type_expression] : proto_statement->parameters) {
    parameters.emplace_back(parameter_name->get_identifier(), parameter_type_expression->get_type());
  }

  parser->get_program()->get_env()->add_function(
      proto_statement->name->get_identifier(),
      make_shared<EnvFunction>(
          make_shared<FunctionType>(parameters,
                                    proto_statement->return_type->get_type())));

  return make_unique<ExternStatement>(position, std::move(proto_statement));
}
