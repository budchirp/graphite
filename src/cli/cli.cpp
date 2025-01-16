#include <memory>
#include <string>

#include "codegen/codegen.hpp"
#include "cli/commands/compile.hpp"
#include "cli/cli.hpp"

CLI::CLI() {
  auto command_line_parser = make_shared<CommandLineParser>("graphite");

  auto compile_command =
      make_unique<CompileCommand>("compile", "Compile specified file");
  compile_command->add_option(
      make_unique<Option<string>>("main", "Main file path", "src/main.gph"));
  compile_command->add_option(
      make_unique<Option<string>>("libs", "Optional graphite libs", ""));
  compile_command->add_option(
      make_unique<Option<string>>("objs", "Optional C and other libs", ""));
  compile_command->add_option(make_unique<Option<string>>(
      "ldflags", "Linker flags", ""));

  command_line_parser->add_command(std::move(compile_command));

  this->parser = std::move(command_line_parser);
}

void CLI::parse(int argc, char *argv[]) {
  parser->parse(argc, argv);
}
