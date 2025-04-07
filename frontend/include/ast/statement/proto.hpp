#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/expression/identifier.hpp"
#include "ast/expression/type.hpp"
#include "ast/statement.hpp"
#include "position.hpp"
#include "types/void.hpp"

using namespace std;

class ProtoStatement : public Statement {
 private:
  Position position;

 public:
  shared_ptr<IdentifierExpression> name;
  vector<pair<shared_ptr<IdentifierExpression>, shared_ptr<TypeExpression>>>
      parameters;
  shared_ptr<TypeExpression> return_type;

  explicit ProtoStatement(
      const Position &position, const shared_ptr<IdentifierExpression> &name,
      const vector<pair<shared_ptr<IdentifierExpression>,
                        shared_ptr<TypeExpression>>> &parameters,
      const shared_ptr<TypeExpression> &return_type)
      : position(position),
        name(name),
        parameters(parameters),
        return_type(return_type) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  shared_ptr<Type> get_type() const override {
    return make_shared<VoidType>();
  };

  string to_string() const override;
  string to_string_tree() const override;
};
