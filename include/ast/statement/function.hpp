#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/statement.hpp"
#include "ast/statement/proto.hpp"
#include "block.hpp"
#include "env/env.hpp"
#include "lexer/position.hpp"
#include "types/void.hpp"

using namespace std;

class FunctionStatement : public Statement {
 private:
  Position position;

  shared_ptr<Env> env;

  unique_ptr<ProtoStatement> proto;
  unique_ptr<BlockStatement> body;

 public:
  explicit FunctionStatement(const Position &position,
                             const shared_ptr<Env> &env,
                             unique_ptr<ProtoStatement> proto,
                             unique_ptr<BlockStatement> body)
      : position(position),
        env(env),
        proto(std::move(proto)),
        body(std::move(body)) {};

  llvm::Value *codegen(
      const shared_ptr<CodegenContext> &context) const override;
  llvm::Function *codegen_function(
      const shared_ptr<CodegenContext> &context) const;

  void analyze(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override {
    return make_shared<VoidType>();
  };

  shared_ptr<Env> get_env() { return env; }

  string to_string() const override;
  string to_string_tree() const override;
};
