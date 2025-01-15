#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include <map>
#include <memory>
#include <string>

#include "ast/program.hpp"
#include "llvm/IR/DerivedTypes.h"

using namespace std;

extern shared_ptr<llvm::LLVMContext> context;
extern shared_ptr<llvm::Module> module;
extern shared_ptr<llvm::IRBuilder<>> builder;
extern unordered_map<string, llvm::Value *> named_values;

class Codegen {
 private:
  shared_ptr<Program> program;

  map<string, llvm::Value *> named_values;

 public:
  explicit Codegen(const shared_ptr<Program> &program);

  string generate_ir() const;

  static llvm::Value *cast_type(llvm::Value *value, llvm::Type *expectedType);
};
