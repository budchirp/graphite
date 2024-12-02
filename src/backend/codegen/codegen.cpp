#include <iostream>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>

#include "backend/codegen/codegen.hpp"
#include "frontend/ast/expressions/binary.hpp"
#include "frontend/ast/expressions/call.hpp"
#include "frontend/ast/expressions/identifier.hpp"
#include "frontend/ast/expressions/integer.hpp"
#include "frontend/ast/expressions/prefix.hpp"
#include "frontend/ast/statements/expression.hpp"
#include "frontend/ast/statements/extern.hpp"
#include "frontend/ast/statements/function.hpp"
#include "frontend/ast/statements/return.hpp"
#include "frontend/ast/statements/var.hpp"
#include "frontend/token/token_type.hpp"
#include "utils/logger/logger.hpp"

using namespace std;

unique_ptr<llvm::LLVMContext> context;
unique_ptr<llvm::Module> module;
unique_ptr<llvm::IRBuilder<>> builder;
map<string, llvm::Value *> named_values;

Codegen::Codegen(unique_ptr<Program> program) {
  this->program = std::move(program);

  context = make_unique<llvm::LLVMContext>();
  module = make_unique<llvm::Module>("graphite", *context);
  builder = make_unique<llvm::IRBuilder<>>(*context);
}

void Codegen::generate() {
  program->codegen();

  cout << endl;
  module->print(llvm::errs(), nullptr);
}

llvm::Value *Program::codegen() {
  for (auto &statement : statements) {
    statement->codegen();
  }

  return nullptr;
}

llvm::Value *BinaryExpression::codegen() {
  auto *left_value = left->codegen();
  auto *right_value = right->codegen();

  if (!left_value || !right_value) {
    Logger::error(
        "Failed to parse left or right expressions of binary expression");
    return nullptr;
  }

  if (left_value->getType() != right_value->getType()) {
    Logger::error("Type mismatch in binary expression");
    return nullptr;
  }

  switch (op.type) {
  case TokenType::TOKEN_PLUS: {
    return builder->CreateAdd(left_value, right_value, "addtmp");
  }

  case TokenType::TOKEN_MINUS: {
    return builder->CreateSub(left_value, right_value, "subtmp");
  }

  case TokenType::TOKEN_ASTERISK: {
    return builder->CreateMul(left_value, right_value, "multmp");
  }

  case TokenType::TOKEN_SLASH: {
    return builder->CreateSDiv(left_value, right_value, "divtmp");
  }

  case TokenType::TOKEN_EQUAL: {
    return builder->CreateICmpEQ(left_value, right_value, "eqtmp");
  }

  case TokenType::TOKEN_NOT_EQUAL: { // !=
    return builder->CreateICmpNE(left_value, right_value, "netmp");
  }

  case TokenType::TOKEN_LESS_THAN: { // <
    return builder->CreateICmpSLT(left_value, right_value, "lttmp");
  }

  case TokenType::TOKEN_GREATER_THAN: { // >
    return builder->CreateICmpSGT(left_value, right_value, "gttmp");
  }

  default: {
    Logger::error("Unknown operator at binary expression: " + op.literal);
    return nullptr;
  }
  }
}

llvm::Value *CallExpression::codegen() {
  auto *function = module->getFunction(name->get_value());
  if (!function) {
    Logger::error("Unkown function called: " + name->get_value());
  }

  if (function->arg_size() != arguments.size()) {
    Logger::error("Incorrect number of arguments passed to " +
                  name->get_value());
  }

  std::vector<llvm::Value *> llvm_arguments;
  for (u32 i = 0, e = arguments.size(); i != e; ++i) {
    auto *arg_value = arguments[i]->codegen();
    if (!arg_value)
      return nullptr;

    if (arg_value->getType()->isPointerTy()) {
      auto *element_type = llvm::Type::getInt32Ty(*context);
      arg_value = builder->CreateLoad(element_type, arg_value, "deref_arg");
    }

    llvm_arguments.push_back(arg_value);
  }

  return builder->CreateCall(function, llvm_arguments, "calltmp");
}

llvm::Value *IdentifierExpression::codegen() {
  auto it = named_values.find(value);
  if (it == named_values.end()) {
    Logger::error("Undefined variable: " + value);
    return nullptr;
  }

  return it->second;
}

llvm::Value *IntegerExpression::codegen() {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*context), value, true);
}

llvm::Value *PrefixExpression::codegen() { return nullptr; }

llvm::Value *BlockStatement::codegen() { return nullptr; }
llvm::BasicBlock *BlockStatement::codegen_block(llvm::Function *parent,
                                                string name) {
  auto block = llvm::BasicBlock::Create(*context, name, parent);
  builder->SetInsertPoint(block);

  for (auto &statement : statements) {
    statement->codegen();
  }

  return block;
}

llvm::Value *ExpressionStatement::codegen() { return expression->codegen(); }

llvm::Value *ExternStatement::codegen() { return proto->codegen_function(); }

llvm::Value *FunctionStatement::codegen() { return codegen_function(); }
llvm::Function *FunctionStatement::codegen_function() {
  auto function = module->getFunction(proto->name->get_value());
  if (function) {
    Logger::warn("Function " + proto->name->get_value() + " exists");
    return nullptr;
  }

  function = proto->codegen_function();
  if (!function) {
    Logger::error("Failed to generate function " + proto->name->get_value());
    return nullptr;
  }

  named_values.clear();
  for (auto &arg : function->args()) {
    named_values[string(arg.getName())] = &arg;
  }

  auto body_block = body->codegen_block(function, "entry");
  if (!body_block) {
    Logger::error("Failed to generate function body");
    function->eraseFromParent();
    return nullptr;
  }

  if (!llvm::isa<llvm::ReturnInst>(body_block->getTerminator())) {
    if (function->getReturnType()->isVoidTy()) {
      builder->CreateRetVoid();
    } else {
      Logger::error("Non-void function missing return statement");
      function->eraseFromParent();
      return nullptr;
    }
  }

  llvm::verifyFunction(*function);
  return function;
}

llvm::Value *ProtoStatement::codegen() { return codegen_function(); }
llvm::Function *ProtoStatement::codegen_function() {
  vector<llvm::Type *> types(arguments.size(),
                             llvm::Type::getInt32Ty(*context));

  auto function_type =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), types, false);
  auto function =
      llvm::Function::Create(function_type, llvm::Function::ExternalLinkage,
                             name->get_value(), module.get());

  u32 idx = 0;
  for (auto &argument : function->args())
    argument.setName(arguments[idx++]->get_value());

  return function;
}

llvm::Value *ReturnStatement::codegen() {
  auto value = expression->codegen();
  if (value) {
    if (value->getType() !=
        builder->GetInsertBlock()->getParent()->getReturnType()) {
      Logger::error("Return value type does not match function return type");
      return nullptr;
    }

    builder->CreateRet(value);
  } else {
    return nullptr;
  }

  return value;
}

llvm::Value *VarStatement::codegen() {
  auto value = expression->codegen();
  auto function = builder->GetInsertBlock()->getParent();
  llvm::IRBuilder<> temp_builder(&function->getEntryBlock(),
                                 function->getEntryBlock().begin());
  auto alloca = temp_builder.CreateAlloca(llvm::Type::getInt32Ty(*context),
                                          nullptr, name->get_value());

  builder->CreateStore(value, alloca);
  named_values[name->get_value()] = alloca;

  return alloca;
}
