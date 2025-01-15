#include <llvm/IR/Value.h>
#include <sstream>

#include "ast/statement/block.hpp"
#include "codegen/codegen.hpp"

using namespace llvm;

Value *BlockStatement::codegen() {
  Value *last = nullptr;
  for (const auto &statement : statements) {
    last = statement->codegen();
    if (builder->GetInsertBlock()->getTerminator()) {
      break;
    }
  }

  return last;
}

BasicBlock *BlockStatement::codegen_block(Function *parent,
                                          const string &name) {
  auto block = BasicBlock::Create(*context, name, parent);
  builder->SetInsertPoint(block);

  codegen();

  return block;
}

string BlockStatement::to_string() const {
  ostringstream oss;
  oss << "{\n";

  for (const auto &statement : statements) {
    oss << "  " << statement->to_string() << ";\n";
  }

  oss << "}";
  return oss.str();
}

string BlockStatement::to_string_tree() const {
  ostringstream oss;
  oss << "BlockStatement(statement: [";

  for (auto i = 0; i < statements.size(); ++i) {
    oss << statements[i]->to_string_tree();
    if (i < statements.size() - 1) {
      oss << ", ";
    }
  }

  oss << "]";

  return oss.str();
}
