#include "frontend/ast/program.hpp"
#include "llvm/IR/DerivedTypes.h"
#include <map>
#include <memory>
#include <string>

using namespace std;

class Codegen {
private:
  unique_ptr<Program> program;

  map<string, llvm::Value *> named_values;

public:
  Codegen(unique_ptr<Program> program);

  void generate();
};
