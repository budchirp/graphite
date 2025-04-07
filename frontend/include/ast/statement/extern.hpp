#pragma once

#include <memory>
#include <string>

#include "ast/statement.hpp"
#include "ast/statement/proto.hpp"
#include "position.hpp"
#include "types/void.hpp"

using namespace std;

class ExternStatement : public Statement {
 private:
  Position position;

 public:
  SymbolVisibility::Value visibility;

  shared_ptr<ProtoStatement> proto;

  explicit ExternStatement(const Position &position,
                           const SymbolVisibility::Value &visibility,
                           const shared_ptr<ProtoStatement> &proto)
      : position(position), visibility(visibility), proto(proto) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override {
    return make_shared<VoidType>();
  };

  string to_string() const override;
  string to_string_tree() const override;
};
