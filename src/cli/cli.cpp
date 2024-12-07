#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

#include "backend/codegen/codegen.hpp"
#include "cli/cli.hpp"
#include "frontend/lexer/lexer.hpp"
#include "frontend/parser/parser.hpp"
#include "frontend/parser/parsers/statement/program.hpp"
#include "std/types.hpp"

class CompileCommand : public Command {
public:
  CompileCommand(const string &name, const string &description)
      : Command(name, description) {}

  void execute() override {
    auto file_option = get_option<string>("file");
    auto ldflags_option = get_option<string>("ldflags");

    filesystem::path path(file_option->value);
    ifstream file(path);
    if (file) {
      std::stringstream buffer;
      buffer << file.rdbuf();
      auto content = buffer.str();

      auto lexer = make_shared<Lexer>(content);
      auto parser = make_shared<Parser>(lexer);

      auto program_parser = ProgramParser(parser);
      auto program = program_parser.parse();

      auto codegen = Codegen(program);
      auto ir = codegen.generate();

      if (!filesystem::is_directory("build")) {
        if (!filesystem::create_directory("build")) {
          throw invalid_argument("could not open file " + path.string());
        }
      }

      ofstream file("build/" + path.stem().string() + ".ll");
      if (file.is_open()) {
        file << ir;
        file.close();

        i32 result = system(("clang " + ldflags_option->value + " build/" +
                             path.stem().string() + ".ll -o build/" +
                             path.stem().string())
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
};

CLI::CLI() {
  auto command_line_parser = make_shared<CommandLineParser>("graphite");

  auto build_command =
      make_unique<CompileCommand>("compile", "Compile specified file");
  build_command->add_option(
      make_unique<Option<string>>("file", "File path", OptionType::STRING, ""));
  build_command->add_option(make_unique<Option<string>>(
      "ldflags", "Linker flags", OptionType::STRING, ""));

  command_line_parser->add_command(std::move(build_command));

  this->parser = std::move(command_line_parser);
}
