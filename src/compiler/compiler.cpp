#include "compiler/compiler.hpp"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>

#include "ast/program.hpp"
#include "ast/program_context.hpp"
#include "codegen/codegen.hpp"
#include "lexer/lexer.hpp"
#include "logger/logger.hpp"
#include "parser/parser.hpp"
#include "parser/parsers/statement/program.hpp"
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

  return program;
}

void Compiler::compile(const filesystem::path &main,
                       const vector<string> &objs) {
  auto root = main.parent_path();
  auto filename = main.filename().stem().string();

  auto modules = compile_project(root, main);
  modules.insert(modules.end(), objs.begin(), objs.end());

  link(modules, filename);
}

vector<string> Compiler::compile_project(const filesystem::path &root,
                                         const filesystem::path &filename) {
  auto program = parse_program(root, filename);

  vector<string> modules;

  filesystem::create_directory(root / "build");

  compile_gph(root, filename);
  modules.push_back((root / "build" / filename.stem()).string() + ".o");

  for (const auto &module : program->get_context()->get_env()->get_includes()) {
    compile_gph(root, filesystem::path(module + ".gph"));
    modules.push_back((root / "build" / module).string() + ".o");
  }

  return modules;
}

void Compiler::compile_gph(const filesystem::path &root,
                           const filesystem::path &filename) {
  Logger::log("Compiling `" + filename.string() + "`");

  auto program = parse_program(root, filename);

  Codegen::init();

  auto codegen_context = make_shared<CodegenContext>(program->get_context());
  auto codegen = Codegen(codegen_context);

  codegen.codegen(program);
  codegen_context->module->print(llvm::outs(), nullptr);

  codegen.optimize();
  codegen_context->module->print(llvm::outs(), nullptr);

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
  if (codegen_context->target_machine->addPassesToEmitFile(
          pass, output_file, nullptr, llvm::CodeGenFileType::ObjectFile)) {
    Logger::error("Failed to generate object code");
    return;
  }

  pass.run(*codegen_context->module);
  output_file.flush();

  cout << endl;
};

void Compiler::link(const vector<string> &objs,
                    const filesystem::path &output) {
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
