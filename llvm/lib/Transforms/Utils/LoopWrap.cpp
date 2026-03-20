#include "llvm/Transforms/Utils/LoopWrap.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

void wrapLoops(Loop *L, Module *Mod) {
  // nested loops too!!!
  for (Loop *SubLoop : L->getSubLoops()) {
    wrapLoops(SubLoop, Mod);
  }

  // initialize function_calls
  FunctionCallee LoopStart = Mod->getOrInsertFunction("loop_start", Type::getVoidTy(Mod->getContext()));
  FunctionCallee LoopEnd = Mod->getOrInsertFunction("loop_end", Type::getVoidTy(Mod->getContext()));
  
  // insert loop_start()
  BasicBlock *LoopPreheader = L->getLoopPreheader();
  IRBuilder<> BuilderForStart(LoopPreheader->getTerminator());
  BuilderForStart.CreateCall(LoopStart);

  // insert loop_end()
  SmallVector<BasicBlock*, 8> ExitBlocks;
  L->getExitBlocks(ExitBlocks);
  for (BasicBlock *Exit : ExitBlocks) {
    IRBuilder<> BuilderForEnd(Exit, Exit->getFirstInsertionPt());
    BuilderForEnd.CreateCall(LoopEnd);
  }
}

PreservedAnalyses LoopWrapPass::run(Function &F, FunctionAnalysisManager &AM) {
  auto &LI = AM.getResult<LoopAnalysis>(F); //LoopInfo
  if (LI.empty()) {
    errs() << "Function " << F.getName() << " has no loops!\n";
    return PreservedAnalyses::all();
  }
  errs() << "Function: " << F.getName() << "\n";
  for (Loop *L : LI) {
    wrapLoops(L, F.getParent());
  }
  return PreservedAnalyses::none(); // IR has been changed
}