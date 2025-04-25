
#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "position.hpp"
#include "types/struct.hpp"
#include "types/type.hpp"

using namespace std;

class StructExpression : public Expression {
 private:
  Position position;

  shared_ptr<StructType> type;

 public:
  shared_ptr<IdentifierExpression> name;

  vector<pair<shared_ptr<IdentifierExpression>, shared_ptr<Expression>>> fields;

  explicit StructExpression(const Position &position,
                            const shared_ptr<IdentifierExpression> &name,
                           const vector<pair<shared_ptr<IdentifierExpression>, shared_ptr<Expression>>> &fields)
      : position(position), name(name), fields(fields) {};

  void resolve_types(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context,
                     const shared_ptr<Type> &destination_type);
  void validate(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override {
    this->type = dynamic_pointer_cast<StructType>(type);
  }
  shared_ptr<Type> get_type() const override { return type; }

  string to_string() const override;
  string to_string_tree() const override;
};
