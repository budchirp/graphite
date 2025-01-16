#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>

#include "cli/commands/compile.hpp"
#include "codegen/codegen.hpp"
#include "compiler/compiler.hpp"

void CompileCommand::execute() {
  auto file_option = get_option<string>("file");
  auto ldflags_option = get_option<string>("ldflags");

  filesystem::path path(file_option->value);

  Compiler().compile(path);
}
