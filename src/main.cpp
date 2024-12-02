#include <iostream>
#include <memory>
#include <string>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/program.hpp"
#include "frontend/lexer/lexer.hpp"
#include "frontend/parser/parser.hpp"
#include "frontend/parser/parsers/statement/program.hpp"

using namespace std;

int main() {
  // link this code to the generated llvm ir code.
  // int print(int str) { printf("%d\n", str); }
  std::string source = "extern print(number)"
                       ""
                       "fn add(a, b) {"
                       "  return a + b"
                       "}"
                       "fn main() {"
                       "  var number = add(5 + 10, 5)"
                       "  print(number)"
                       "  return 0"
                       "}";
  cout << "Source: " << source << endl << endl;

  auto lexer = make_shared<Lexer>(source);
  auto parser = make_shared<Parser>(lexer);

  auto program_parser = ProgramParser(parser);
  auto program = program_parser.parse();
  cout << "Program generated AST:\n" << program->to_string() << endl << endl;
  cout << "Program AST tree:\n" << program->to_string_tree() << endl << endl;

  auto codegen = Codegen(std::move(program));
  codegen.generate();

  return 0;
}
