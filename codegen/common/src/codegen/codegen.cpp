#include "codegen/codegen.hpp"

#include <memory>

#include "codegen_llvm/codegen.hpp"
#include "compiler/compiler_backend.hpp"
#include "logger/logger.hpp"

void Codegen::init() const {
  switch (backend) {
    case CompilerBackend::LLVM: {
      LLVMCodegen::init();

      break;
    }

    default: {
      Logger::error("Invalid compiler backend!");
      break;
    }
  }
}

shared_ptr<CodegenBackend> Codegen::get() const {
  switch (backend) {
    case CompilerBackend::LLVM: {
      auto context = make_shared<LLVMCodegenContext>(program->get_context());
      return make_shared<LLVMCodegen>(context);
    }

    default: {
      Logger::error("Invalid compiler backend!");
      return nullptr;
    }
  }

  return nullptr;
}
