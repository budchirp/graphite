#include "compiler/compiler.hpp"

#include <filesystem>
#include <fstream>
#include <numeric>

#include "ast/program.hpp"
#include "codegen/codegen.hpp"
#include "env/env.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/parsers/statement/program.hpp"

string join_strings(const vector<string>& strings, const string& delimiter) {
    return accumulate(
        strings.begin(), strings.end(), std::string(),
        [&](const string& a, const string& b) {
            return a.empty() ? b : a + delimiter + b;
        });
}

void Compiler::compile(const filesystem::path& source_file_path) {
  ifstream source_file(source_file_path);
  if (!source_file.is_open()) {
    Logger::error("Failed to open file `" + source_file_path.string() + "`");
    return;
  }

  string content((istreambuf_iterator<char>(source_file)),
                 istreambuf_iterator<char>());
  source_file.close();

  auto lexer = make_shared<Lexer>(content);

  auto env = make_shared<Env>(nullptr);
  env->init();

  auto filename = source_file_path.filename().stem().string();

  auto program = make_shared<Program>(filename, env);
  auto parser = make_shared<Parser>(lexer, program);

  ProgramParser(parser).parse();

  auto codegen = Codegen(program);
  auto ir = codegen.generate_ir();

  auto ir_file_path = filesystem::path(source_file_path.stem().string() + ".ll");
  ofstream ir_file(ir_file_path);
  if (!ir_file.is_open()) {
    Logger::error("Failed to open file `" + source_file_path.string() + "`");
    return;
  }

  ir_file << ir;
  ir_file.close();

  compile_ir(ir_file_path.stem());
  compile_objects({
    filename + ".o",
    "print.o"
  }, filename);
};

void Compiler::compile_ir(const filesystem::path& ir_file_path) {
  auto filename = ir_file_path.filename().stem().string();

  vector<string> command{"clang", "-c", "-o", filename + ".o",
                               filename + ".ll"};

  auto result = system(join_strings(command, " ").c_str());
  if (result > 0) {
    Logger::error("Failed to generate object code for llvm ir");
    return;
  }
}

void Compiler::compile_objects(const vector<string> &objs, const filesystem::path &output_file_path) {
  auto filename = output_file_path.filename().stem().string();

  vector<string> command {"clang", "-fPIE", "-o", filename, join_strings(objs, " ")};

  auto result = system(join_strings(command, " ").c_str());
  if (result > 0) {
    Logger::error("Failed to generate executable");
    return;
  }
}
