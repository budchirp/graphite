#include "cli/cli.hpp"

#include <memory>

#include "cli/commands/compile.hpp"

CLI::CLI() {
  auto parser =
      make_shared<CommandLineParser>("graphitec", "Graphite Compiler");

  parser->add_command(make_shared<CompileCommand>());

  this->parser = std::move(parser);
}

void CLI::parse(int argc, char *argv[]) { parser->parse(argc, argv); }
