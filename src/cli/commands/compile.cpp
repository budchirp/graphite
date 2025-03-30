#include "cli/commands/compile.hpp"

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <ranges>
#include <string>

#include "codegen/codegen.hpp"
#include "compiler/compiler.hpp"

vector<string> split_string(const string& text, char delimiter) {
  vector<string> result;

  for (auto part : text | views::split(delimiter)) {
    result.emplace_back(part.begin(), part.end());
  }

  return result;
}

void CompileCommand::execute() {
  auto file_option = get_option<string>("main")->value;
  auto objs_option = get_option<string>("objs")->value;
  auto ldflags_option = get_option<string>("ldflags")->value;

  filesystem::path path(file_option);

  Compiler(ldflags_option).compile(path, split_string(objs_option, ','));
}
