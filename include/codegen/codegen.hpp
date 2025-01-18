#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/ManagedStatic.h>

#include <memory>

#include "ast/program.hpp"
#include "codegen/context.hpp"

using namespace std;

class Codegen {
 private:
  shared_ptr<CodegenContext> context;

 public:
  explicit Codegen(const shared_ptr<CodegenContext> &context)
      : context(context) {};
  ~Codegen() { llvm::llvm_shutdown(); }

  static void init();
  void codegen(const shared_ptr<Program> &program) const;

  void optimize();

  static llvm::Value *cast_type(const shared_ptr<CodegenContext> &context,
                                llvm::Value *value, llvm::Type *expectedType);
};
