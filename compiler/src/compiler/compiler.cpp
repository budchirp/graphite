#include "compiler/compiler.hpp"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>

#include "codegen/codegen.hpp"
#include "codegen_llvm/codegen.hpp"
#include "codegen_llvm/context.hpp"
#include "compiler/compiler_backend.hpp"
#include "lexer/lexer.hpp"
#include "logger/logger.hpp"
#include "parser/parser.hpp"
#include "program/parser.hpp"
#include "semantic/type_resolver/type_resolver.hpp"
#include "semantic/validator/validator.hpp"

string join_strings(const vector<string> &strings, const string &delimiter) {
  return accumulate(strings.begin(), strings.end(), std::string(),
                    [&](const string &a, const string &b) {
                      return a.empty() ? b : a + delimiter + b;
                    });
}

shared_ptr<Program> Compiler::parse_program(const filesystem::path &root,
                                            const filesystem::path &filename) {
  ifstream source_file(root / filename);
  if (!source_file.is_open()) {
    Logger::error("Failed to open file `" + filename.string() + "`");
    return nullptr;
  }

  string content((istreambuf_iterator<char>(source_file)),
                 istreambuf_iterator<char>());
  source_file.close();

  auto lexer = make_shared<Lexer>(content);

  auto module = filename.filename().stem().string();

  auto program_context = make_shared<ProgramContext>(
      module, (root / filename).string(), make_shared<Env>());
  auto program = make_shared<Program>(program_context);

  auto program_parser = ProgramParser(make_shared<Parser>(lexer, program));
  program_parser.parse();

  auto type_resolver = TypeResolver(program);
  type_resolver.resolve();

  auto validator = Validator(program);
  validator.validate();

  cout << program->to_string_tree() << endl;

  return program;
}

void Compiler::compile(const filesystem::path &main,
                       const vector<string> &objs) const {
  auto root = main.parent_path();
  auto filename = main.filename().stem().string();

  auto modules = compile_project(root, main);
  modules.insert(modules.end(), objs.begin(), objs.end());

  link(modules, filename);
}

vector<string> Compiler::compile_project(
    const filesystem::path &root, const filesystem::path &filename) const {
  auto program = parse_program(root, filename);

  vector<string> modules;

  create_directory(root / "build");

  compile_gph(root, filename);
  modules.push_back((root / "build" / filename.stem()).string() + ".o");

  for (const auto &module : program->get_context()->get_env()->get_includes()) {
    compile_gph(root, filesystem::path(module + ".gph"));
    modules.push_back((root / "build" / module).string() + ".o");
  }

  return modules;
}

void Compiler::compile_gph(const filesystem::path &root,
                           const filesystem::path &filename) const {
  Logger::log("Compiling `" + filename.string() + "`");

  auto program = parse_program(root, filename);

  auto codegen = Codegen(program, CompilerBackend::Value::LLVM);
  codegen.init();

  auto backend = static_pointer_cast<LLVMCodegen>(codegen.get());
  backend->codegen(program);
  backend->optimize();

  error_code err_code;
  llvm::raw_fd_ostream output_file(
      (root / "build" / filename.stem()).string() + ".o", err_code,
      llvm::sys::fs::OF_None);
  if (err_code) {
    Logger::error("Failed to open file `" +
                  (root / "build" / filename.stem()).string() + ".o`");
    return;
  }

  llvm::legacy::PassManager pass;
  if (backend->get_context()->target_machine->addPassesToEmitFile(
          pass, output_file, nullptr, llvm::CodeGenFileType::ObjectFile)) {
    Logger::error("Failed to generate object code");
    return;
  }

  pass.run(*backend->get_context()->module);
  output_file.flush();

  cout << endl;
};

void Compiler::link(const vector<string> &objs,
                    const filesystem::path &output) const {
  auto filename = output.filename().stem().string();
  Logger::log("Creating executable `" + filename + "`");

  vector<string> command{"clang", ld_flags, "-o", filename,
                         join_strings(objs, " ")};
  auto command_str = join_strings(command, " ");

  Logger::log("Executing `" + command_str + "`");

  auto result = system(command_str.c_str());
  if (result > 0) {
    Logger::error("Failed to generate executable");
    return;
  }
}
