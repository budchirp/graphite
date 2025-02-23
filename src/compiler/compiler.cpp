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

void Compiler::compile(const filesystem::path &source_file_path,
                       const vector<string> &libs, const vector<string> &objs) {
  auto filename = source_file_path.filename().stem().string();

  vector<string> initial_objs = {source_file_path.stem().string() + ".o"};
  for (const auto &lib_file : libs) {
    filesystem::path lib_file_path(lib_file);

    compile_gph(lib_file_path);

    initial_objs.push_back(lib_file_path.filename().stem().string() + ".o");
  }

  compile_gph(source_file_path);

  initial_objs.insert(initial_objs.end(), objs.begin(), objs.end());
  compile_objects(initial_objs, filename);
}

void Compiler::compile_gph(const filesystem::path &source_file_path) {
  auto filename = source_file_path.filename().stem().string();
  Logger::log("Compiling `" + filename + ".gph`");

  ifstream source_file(source_file_path);
  if (!source_file.is_open()) {
    Logger::error("Failed to open file `" + source_file_path.string() + "`");
    return;
  }

  string content((istreambuf_iterator<char>(source_file)),
                 istreambuf_iterator<char>());
  source_file.close();

  auto lexer = make_shared<Lexer>(content);

  auto env = make_shared<Env>();

  auto program_context = make_shared<ProgramContext>(filename, env);
  auto program = make_shared<Program>(program_context);
  auto parser = make_shared<Parser>(lexer, program);

  auto program_parser = make_shared<ProgramParser>(parser);
  program_parser->parse();

  auto type_resolver = make_shared<TypeResolver>(program);
  type_resolver->resolve();

  auto validator = make_shared<Validator>(program);
  validator->validate();

  Codegen::init();

  auto codegen_context = make_shared<CodegenContext>(program_context);
  auto codegen = make_shared<Codegen>(codegen_context);
  codegen->codegen(program);
  codegen_context->module->print(llvm::outs(), nullptr);
  codegen->optimize();
  codegen_context->module->print(llvm::outs(), nullptr);

  error_code err_code;
  llvm::raw_fd_ostream output_file(source_file_path.stem().string() + ".o",
                                   err_code, llvm::sys::fs::OF_None);
  if (err_code) {
    Logger::error("Failed to open file `" + source_file_path.string() + "`");
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

void Compiler::compile_objects(const vector<string> &objs,
                               const filesystem::path &output_file_path) {
  auto filename = output_file_path.filename().stem().string();
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
