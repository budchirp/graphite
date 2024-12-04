#include <llvm/IR/IRBuilder.h>
#include <map>
#include <memory>
#include <string>

#include "frontend/ast/program.hpp"
#include "llvm/IR/DerivedTypes.h"

using namespace std;

class Codegen {
private:
  shared_ptr<Program> program;

  map<string, llvm::Value *> named_values;

public:
  Codegen(shared_ptr<Program> program);

  void generate();
  static llvm::Type *resolve_type(const string type_name);
  static llvm::Value *convert_type(llvm::Value *value,
                                   llvm::Type *expectedType);
};
