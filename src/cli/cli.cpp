#include <memory>
#include <string>

#include "backend/codegen/codegen.hpp"
#include "cli/commands/compile.hpp"
#include "cli/cli.hpp"

CLI::CLI() {
  auto command_line_parser = make_shared<CommandLineParser>("graphite");

  auto build_command =
      make_unique<CompileCommand>("compile", "Compile specified file");
  build_command->add_option(
      make_unique<Option<string>>("file", "File path", ""));
  build_command->add_option(make_unique<Option<string>>(
      "ldflags", "Linker flags", ""));

  command_line_parser->add_command(std::move(build_command));

  this->parser = std::move(command_line_parser);
}

void CLI::parse(int argc, char *argv[]) const {
  parser->parse(argc, argv);
}
