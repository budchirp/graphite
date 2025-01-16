#include "codegen/context.hpp"

CodegenContext::CodegenContext(const shared_ptr<Program> &program)
    : program(program) {
  llvm_context = make_shared<llvm::LLVMContext>();
  module = make_shared<llvm::Module>(program->get_name(), *llvm_context);
  builder = make_shared<llvm::IRBuilder<>>(*llvm_context);
}
