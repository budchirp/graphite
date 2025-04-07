#include "cli/cli.hpp"

#include <memory>
#include <string>

#include "cli/commands/compile.hpp"

CLI::CLI() {
  auto command_line_parser = make_shared<CommandLineParser>("graphite");

  auto compile_command =
      make_unique<CompileCommand>("compile", "Compile specified file");
  compile_command->add_option(
      make_unique<Option<string>>("main", "Main file path", "src/main.gph"));
  compile_command->add_option(
      make_unique<Option<string>>("objs", "Object files to link", ""));
  compile_command->add_option(
      make_unique<Option<string>>("ldflags", "Linker flags", ""));

  command_line_parser->add_command(std::move(compile_command));

  this->parser = std::move(command_line_parser);
}

void CLI::parse(int argc, char *argv[]) { parser->parse(argc, argv); }
