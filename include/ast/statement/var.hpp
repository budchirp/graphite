#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "ast/statement.hpp"
#include "lexer/position.hpp"

using namespace std;

class VarStatement : public Statement {
 private:
  Position position;

  unique_ptr<IdentifierExpression> name;
  unique_ptr<TypeExpression> type;
  unique_ptr<Expression> expression;

 public:
  explicit VarStatement(const Position &position,
                        unique_ptr<IdentifierExpression> name,
                        unique_ptr<TypeExpression> type,
                        unique_ptr<Expression> expression)
      : position(position),
        name(std::move(name)),
        type(std::move(type)),
        expression(std::move(expression)) {};

  llvm::Value *codegen() const override;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
