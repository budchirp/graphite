#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>

#include "frontend/ast/expression.hpp"
#include "frontend/ast/expression/identifier.hpp"
#include "frontend/ast/statement.hpp"
#include "frontend/lexer/position.hpp"

using namespace std;

class VarStatement : public Statement {
private:
  Position position;

  unique_ptr<IdentifierExpression> name;
  unique_ptr<IdentifierExpression> type;
  unique_ptr<Expression> expression;

public:
  explicit VarStatement(const Position &position,
                        unique_ptr<IdentifierExpression> name,
                        unique_ptr<IdentifierExpression> type,
                        unique_ptr<Expression> expression);

  llvm::Value *codegen() override;

  Position *get_position() override { return &position; };

  string to_string() const override;
  string to_string_tree() const override;
};
