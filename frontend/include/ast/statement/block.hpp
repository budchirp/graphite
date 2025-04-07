#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/statement.hpp"
#include "position.hpp"

using namespace std;

class BlockStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

 public:
  vector<shared_ptr<Statement>> statements;

  explicit BlockStatement(const Position &position,
                          const vector<shared_ptr<Statement>> &statements)
      : position(position), statements(statements) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
