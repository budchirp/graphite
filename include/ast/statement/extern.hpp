#pragma once

#include <llvm/IR/Value.h>

#include <memory>
#include <string>

#include "ast/statement.hpp"
#include "ast/statement/proto.hpp"
#include "lexer/position.hpp"
#include "proto.hpp"

using namespace std;

class ExternStatement : public Statement {
 private:
  Position position;

  shared_ptr<Type> type;

  unique_ptr<ProtoStatement> proto;

 public:
  explicit ExternStatement(const Position &position,const shared_ptr<Type> &type,
                           unique_ptr<ProtoStatement> proto)
      : position(position),type(type), proto(std::move(proto)) {};

  llvm::Value *codegen(const shared_ptr<CodegenContext> &context) const override;
  void analyze(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override { return type; };

  string to_string() const override;
  string to_string_tree() const override;
};
