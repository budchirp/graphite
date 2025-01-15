#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/statement.hpp"

using namespace std;

class Program : public Statement {
private:
  vector<unique_ptr<Statement>> statements;

public:
  explicit Program(vector<unique_ptr<Statement>> statements);

  llvm::Value *codegen() override;
  
  Position *get_position() override {
    return nullptr;
  };

  string to_string() const override;
  string to_string_tree() const override;
};
