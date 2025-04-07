#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Type.h>
#include <llvm/Support/ManagedStatic.h>

#include <memory>

#include "ast/expression.hpp"
#include "ast/statement.hpp"
#include "codegen/codegen_backend.hpp"
#include "codegen_llvm/context.hpp"
#include "context.hpp"
#include "program/program.hpp"

using namespace std;

class LLVMCodegen : public CodegenBackend {
 private:
  shared_ptr<LLVMCodegenContext> context;

 public:
  explicit LLVMCodegen(const shared_ptr<LLVMCodegenContext> &context)
      : context(context) {};
  ~LLVMCodegen() override { llvm::llvm_shutdown(); }

  static void init();

  static llvm::Value *codegen(const shared_ptr<LLVMCodegenContext> &context,
                              const shared_ptr<Expression> &expression);
  static llvm::Value *codegen(const shared_ptr<LLVMCodegenContext> &context,
                              const shared_ptr<Statement> &statement);

  void codegen(const shared_ptr<Program> &program) const override;
  void optimize() const override;


  shared_ptr<LLVMCodegenContext> get_context() const { return context; }
};
