#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/ManagedStatic.h>

#include <memory>
#include <string>

#include "ast/program.hpp"
#include "codegen/context.hpp"

using namespace std;

class Codegen {
public:
  explicit Codegen() = default;
  ~Codegen() { llvm::llvm_shutdown(); }

  string generate_ir(const shared_ptr<CodegenContext> &context,
                     const shared_ptr<Program> &program) const;

  static llvm::Value *cast_type(const shared_ptr<CodegenContext> &context,
                                llvm::Value *value, llvm::Type *expectedType);
};
