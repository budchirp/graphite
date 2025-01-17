#include "codegen/context.hpp"

CodegenContext::CodegenContext(
    const shared_ptr<ProgramContext> &program_context)
    : program_context(program_context) {
  llvm_context = make_shared<llvm::LLVMContext>();
  module =
      make_shared<llvm::Module>(program_context->get_name(), *llvm_context);
  builder = make_shared<llvm::IRBuilder<>>(*llvm_context);
}
