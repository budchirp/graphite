#include "cli/commands/compile.hpp"

#include <string>
#include <string_view>
#include <vector>

#include "codegen/codegen.hpp"
#include "compiler/compiler.hpp"

auto split_string(string_view text, char delimiter) -> vector<string> {
  vector<string> result;
  size_t start = 0;

  while (true) {
    auto pos = text.find(delimiter, start);
    if (pos == string_view::npos) {
      break;
    }

    if (pos > start) {
      result.emplace_back(text.substr(start, pos - start));
    }
    start = pos + 1;
  }

  if (start < text.length()) {
    result.emplace_back(text.substr(start));
  }

  return result;
}

void CompileCommand::execute() {
  auto file = get_option_value<string>("main");
  auto objs = get_option_value<string>("objs");
  auto ldflags = get_option_value<string>("ldflags");

  filesystem::path path(file);

  Compiler(CodegenBackendType::Value::LLVM, ldflags)
      .compile(path, split_string(objs, ','));
}
