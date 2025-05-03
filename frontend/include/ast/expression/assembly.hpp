#pragma once

#include <memory>
#include <string>

#include "ast/expression.hpp"
#include "ast/expression/identifier.hpp"
#include "ast/expression/string.hpp"
#include "position.hpp"
#include "types/function.hpp"

using namespace std;

class AssemblyExpression : public Expression {
 private:
  Position position;

  shared_ptr<Type> type;
  shared_ptr<FunctionType> function_type;

 public:
  shared_ptr<StringExpression> assembly;
  vector<pair<shared_ptr<StringExpression>, shared_ptr<IdentifierExpression>>>
      output;
  vector<pair<shared_ptr<StringExpression>, shared_ptr<IdentifierExpression>>>
      input;
  vector<shared_ptr<StringExpression>> clobbered;

  explicit AssemblyExpression(
      const Position &position, const shared_ptr<StringExpression> &assembly,
      const vector<pair<shared_ptr<StringExpression>,
                        shared_ptr<IdentifierExpression>>> &output,
      const vector<pair<shared_ptr<StringExpression>,
                        shared_ptr<IdentifierExpression>>> &input,
      const vector<shared_ptr<StringExpression>> &clobbered)
      : position(position),
        assembly(assembly),
        output(output),
        input(input),
        clobbered(clobbered) {};

  void validate(const shared_ptr<ProgramContext> &context) override;
  void resolve_types(const shared_ptr<ProgramContext> &context) override;

  Position *get_position() override { return &position; };

  void set_type(const shared_ptr<Type> &type) override { this->type = type; }
  void set_function_type(const shared_ptr<FunctionType> &type) { this->function_type = type; }
  shared_ptr<Type> get_type() const override { return type; };
  shared_ptr<FunctionType> get_function_type() const { return function_type; };

  string to_string() const override;
  string to_string_tree() const override;
};
