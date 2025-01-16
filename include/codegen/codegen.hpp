#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>

#include <map>
#include <memory>
#include <string>

#include "codegen/context.hpp"
#include "llvm/IR/DerivedTypes.h"

using namespace std;

extern shared_ptr<CodegenContext> context;

class Codegen {
 private:
  map<string, llvm::Value *> named_values;

 public:
  explicit Codegen(const shared_ptr<CodegenContext> &context);

  string generate_ir() const;

  static llvm::Value *cast_type(llvm::Value *value, llvm::Type *expectedType);
};
