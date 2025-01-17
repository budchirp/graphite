#include "ast/statement/block.hpp"

#include <llvm/IR/Value.h>

#include <sstream>

#include "codegen/codegen.hpp"

using namespace llvm;

Value *BlockStatement::codegen(
    const shared_ptr<CodegenContext> &context) const {
  Value *last = nullptr;
  for (const auto &statement : statements) {
    last = statement->codegen(context);
    if (context->builder->GetInsertBlock()->getTerminator()) {
      break;
    }
  }

  return last;
}

BasicBlock *BlockStatement::codegen_block(
    const shared_ptr<CodegenContext> &context, Function *parent,
    const string &name) const {
  auto block = BasicBlock::Create(*context->llvm_context, name, parent);
  context->builder->SetInsertPoint(block);

  codegen(context);

  return block;
}

void BlockStatement::analyze(const shared_ptr<ProgramContext> &context) {
  for (const auto &statement : statements) {
    statement->analyze(context);
  }
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
