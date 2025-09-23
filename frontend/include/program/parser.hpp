#pragma once

#include <memory>

#include "../parser/parser.hpp"

using namespace std;

class ProgramParser {
 private:
  shared_ptr<Parser> parser;

 public:
  explicit ProgramParser(const shared_ptr<Parser> &parser) : parser(parser) {};

  void parse() const;
  static shared_ptr<Program> parse_program(const filesystem::path &root,
                                           const filesystem::path &filename);
};
