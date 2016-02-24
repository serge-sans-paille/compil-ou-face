// compile with clang++-3.7 `llvm-config-3.7 --cxxflags --ldflags` -lLLVM -rdynamic interp.cpp -o interp
// FIXME: update gist

#include <iostream>
#include <string>

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Support/MemoryBuffer.h"
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include "llvm/IR/Verifier.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

extern "C" int64_t Acc = 0;

void interp(char op, int64_t val) {
  llvm::LLVMContext& C = llvm::getGlobalContext();
  std::unique_ptr<llvm::Module> M{new llvm::Module {"", C}};
  auto* LT =  llvm::IntegerType::get(C, 64);
  auto* GV = new llvm::GlobalVariable(*M, LT, false, llvm::GlobalValue::ExternalLinkage, nullptr, "Acc");

  auto* FT = llvm::FunctionType::get(LT, false);
  auto* F = llvm::Function::Create(FT, llvm::GlobalValue::InternalLinkage, "runme", M.get());
  auto* BB = llvm::BasicBlock::Create(C, "", F, nullptr);
  llvm::IRBuilder<> Builder{BB};
  llvm::Value* Acc = Builder.CreateLoad(GV);
  llvm::Value* Arg = llvm::ConstantInt::get(LT, val);
  llvm::Value* Res = nullptr;
  switch(op) {
    case '+':
      Res = Builder.CreateAdd(Acc, Arg); break;
    case '-':
      Res = Builder.CreateSub(Acc, Arg); break;
  }
  Builder.CreateStore(Res, GV, true);
  Builder.CreateRetVoid();
  llvm::verifyModule(*M);


  std::string err;
  llvm::EngineBuilder EB(std::move(M));
  EB.setEngineKind(llvm::EngineKind::JIT).setErrorStr(&err);
  llvm::ExecutionEngine *EE = EB.create();
  if (!EE) {
    std::cerr << "unable to create a jitter: " << err << "\n";
    return ;
  }
  EE->finalizeObject();

  llvm::SmallVector<llvm::GenericValue, 1> noargs;
  EE->runFunction(F, noargs);
}

void init_llvm() {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
}

int main() {
  init_llvm();
  std::string wipe;
  std::cout << "Hit Ctrl-D to stop" << std::endl;
  while(std::cin) {
    char op;
    if(! (std::cin >> op)) return 1;
    switch(op) {
      case '+':
      case '-':
        break;
      default:
        return 2;
    }
    std::getline(std::cin, wipe);

    int64_t val;
    if(!(std::cin >> val)) return 2;
    std::getline(std::cin, wipe);
    interp(op, val);
    std::cout << "Acc = " << Acc << std::endl;
  }
  return 0;
}
