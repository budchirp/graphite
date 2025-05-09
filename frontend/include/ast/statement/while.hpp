#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/statement.hpp"
#include "ast/statement/block.hpp"
#include "position.hpp"
#include "types/void.hpp"

using namespace std;

class WhileStatement : public Statement {
 private:
  Position position;

 public:
  shared_ptr<Expression> condition;

  shared_ptr<BlockStatement> body;

  explicit WhileStatement(const Position &position,
                          const shared_ptr<Expression> &condition,
                          const shared_ptr<BlockStatement> &body)
      : position(position), condition(condition), body(body) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override {
    return make_shared<VoidType>();
  };

  string to_string() const override;
  string to_string_tree() const override;
};
