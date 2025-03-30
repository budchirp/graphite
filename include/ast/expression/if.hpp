#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/statement/block.hpp"
#include "lexer/position.hpp"
#include "types/type.hpp"

using namespace std;

class IfExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<Expression> condition;
  unique_ptr<BlockStatement> consequence;
  unique_ptr<BlockStatement> alternative;

 public:
  explicit IfExpression(const Position &position,
                        unique_ptr<Expression> condition,
                        unique_ptr<BlockStatement> consequence,
                        unique_ptr<BlockStatement> alternative)
      : position(position),
        condition(std::move(condition)),
        consequence(std::move(consequence)),
        alternative(std::move(alternative)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
