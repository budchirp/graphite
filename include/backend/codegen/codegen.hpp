#include <llvm/IR/IRBuilder.h>
#include <map>
#include <memory>
#include <string>

#include "frontend/ast/program.hpp"
#include "llvm/IR/DerivedTypes.h"

using namespace std;
using namespace llvm;

extern unique_ptr<LLVMContext> context;
extern unique_ptr<Module> module;
extern unique_ptr<IRBuilder<>> builder;
extern unordered_map<string, Value *> named_values;

class Codegen {
private:
  shared_ptr<Program> program;

  map<string, llvm::Value *> named_values;

public:
  explicit Codegen(const shared_ptr<Program> &program);

  string generate_ir() const;

  static Type *resolve_type_identifier(const string &type_name);
  static Value *convert_type(Value *value,
                                   Type *expectedType);
  static bool type_check(Value *value, Type *expectedType);
};
