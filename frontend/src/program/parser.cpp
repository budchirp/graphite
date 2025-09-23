#include "program/parser.hpp"

#include <fstream>
#include <memory>

#include "parser/statement/statement.hpp"
#include "semantic/type_resolver/type_resolver.hpp"
#include "semantic/validator/validator.hpp"

void ProgramParser::parse() const {
  auto statement_parser = StatementParser(parser);

  auto statements = vector<shared_ptr<Statement>>();
  while (parser->current_token.type != TokenType::TOKEN_EOF) {
    if (auto statement = statement_parser.parse()) {
      parser->get_program()->statements.push_back(statement);
    }
  }
}

shared_ptr<Program> ProgramParser::parse_program(
    const filesystem::path &root, const filesystem::path &filename) {
  ifstream source_file(root / filename);
  if (!source_file.is_open()) {
    Logger::error("Failed to open file `" + filename.string() + "`");
    return nullptr;
  }

  string content((istreambuf_iterator<char>(source_file)),
                 istreambuf_iterator<char>());
  source_file.close();

  auto lexer = make_shared<Lexer>(content);

  auto module = filename.filename().stem().string();

  auto program_context = make_shared<ProgramContext>(
      module, (root / filename).string(), make_shared<Env>());
  auto program = make_shared<Program>(program_context);

  auto program_parser = ProgramParser(make_shared<Parser>(lexer, program));
  program_parser.parse();

  auto type_resolver = TypeResolver(program);
  type_resolver.resolve();

  auto validator = Validator(program);
  validator.validate();

  return program;
}