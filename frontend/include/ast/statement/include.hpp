#pragma once

#include <memory>
#include <string>

#include "ast/expression/string.hpp"
#include "ast/statement.hpp"
#include "position.hpp"
#include "program/program.hpp"
#include "types/void.hpp"

using namespace std;

class IncludeStatement : public Statement {
 private:
  Position position;

 public:
  shared_ptr<StringExpression> module;

  shared_ptr<Program> program;

  explicit IncludeStatement(const Position &position,
                            const shared_ptr<StringExpression> &module)
      : position(position), module(module), program(nullptr) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override {
    return make_shared<VoidType>();
  };

  string to_string() const override;
  string to_string_tree() const override;
};
