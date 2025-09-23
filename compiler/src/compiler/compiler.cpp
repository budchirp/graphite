#include "compiler/compiler.hpp"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <filesystem>
#include <memory>
#include <numeric>
#include <string>

#include "codegen/codegen.hpp"
#include "codegen_llvm/codegen.hpp"
#include "logger/logger.hpp"
#include "program/parser.hpp"

string join_strings(const vector<string> &strings, const string &delimiter) {
  return accumulate(strings.begin(), strings.end(), std::string(),
                    [&](const string &a, const string &b) {
                      return a.empty() ? b : a + delimiter + b;
                    });
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
  create_directory(root / "build");

  auto program = compile_gph(root, filename);

  vector<string> modules;
  for (const auto &module : program->get_context()->get_env()->get_includes()) {
    compile_gph(root, filesystem::path(module + ".gph"));
    modules.push_back((root / "build" / module).string() + ".o");
  }

  modules.push_back((root / "build" / filename.stem()).string() + ".o");

  return modules;
}

shared_ptr<Program> Compiler::compile_gph(
    const filesystem::path &root, const filesystem::path &filename) const {
  Logger::log("Parsing `" + filename.string() + "`");

  auto program = ProgramParser::parse_program(root, filename);

  LLVMCodegen::init();

  auto codegen_backend = make_shared<LLVMCodegen>(
      make_shared<LLVMCodegenContext>(program->get_context()));
  auto codegen = Codegen(program, codegen_backend);

  Logger::log("Compiling `" + filename.string() + "`");

  codegen.codegen();

  codegen_backend->get_context()->module->print(llvm::outs(), nullptr);

  codegen_backend->optimize();

  error_code err_code;
  llvm::raw_fd_ostream output_file(
      (root / "build" / filename.stem()).string() + ".o", err_code,
      llvm::sys::fs::OF_None);
  if (err_code) {
    Logger::error("Failed to open file `" +
                  (root / "build" / filename.stem()).string() + ".o`");
    return nullptr;
  }

  llvm::legacy::PassManager pass;
  if (codegen_backend->get_context()->target_machine->addPassesToEmitFile(
          pass, output_file, nullptr, llvm::CodeGenFileType::ObjectFile)) {
    Logger::error("Failed to generate object code");
    return nullptr;
  }

  pass.run(*codegen_backend->get_context()->module);
  output_file.flush();

  return program;
}

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
