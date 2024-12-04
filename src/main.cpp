#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "backend/codegen/codegen.hpp"
#include "frontend/lexer/lexer.hpp"
#include "frontend/parser/parser.hpp"
#include "frontend/parser/parsers/statement/program.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <path_to_file>" << std::endl;
    return 1;
  }

  std::string path = argv[1];
  std::ifstream file(path);
  if (file) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto content = buffer.str();

    cout << "Source:" << endl << content << endl << endl;

    auto lexer = make_shared<Lexer>(content);
    auto parser = make_shared<Parser>(lexer);

    auto program_parser = ProgramParser(parser);
    auto program = program_parser.parse();

    cout << "Program generated AST:" << endl << program->to_string() << endl << endl;
    cout << "Program AST tree:" << endl << program->to_string_tree() << endl << endl;

    auto codegen = Codegen(program);
    codegen.generate();
  } else {
    std::cerr << "Could not open the file at path: " << path << std::endl;
    return 1;
  }

  return 0;
}
