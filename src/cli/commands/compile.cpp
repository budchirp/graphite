#include "cli/commands/compile.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "codegen/codegen.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/parsers/statement/program.hpp"

shared_ptr<Env> env;

void CompileCommand::execute() {
  auto file_option = get_option<string>("file");
  auto ldflags_option = get_option<string>("ldflags");

  filesystem::path path(file_option->value);
  if (ifstream file(path); file) {
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    auto content = buffer.str();

    auto lexer = make_shared<Lexer>(content);

    env = make_shared<Env>(nullptr);
    env->init();

    auto parser = make_shared<Parser>(lexer, env);

    auto program_parser = ProgramParser(parser);
    auto program = program_parser.parse();

    cout << program->to_string_tree() << endl;

    auto codegen = Codegen(program);
    auto ir = codegen.generate_ir();

    if (!filesystem::is_directory("build")) {
      if (!filesystem::create_directory("build")) {
        throw invalid_argument("could not open file " + path.string());
      }
    }

    if (ofstream ll_file("build/" + path.stem().string() + ".ll");
        ll_file.is_open()) {
      ll_file << ir;
      ll_file.close();

      auto result =
          system(("clang " + ldflags_option->value + " build/" +
                  path.stem().string() + ".ll -o build/" + path.stem().string())
                     .c_str());
      if (result != 0) {
        throw invalid_argument("failed to compile " + path.string() + "ll");
      }
    } else {
      throw invalid_argument("failed to open file");
    }
  } else {
    throw invalid_argument("failed to create build directory");
  }
}
