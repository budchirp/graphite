#include "codegen/codegen.hpp"

void Codegen::codegen() const { backend->codegen(program); }