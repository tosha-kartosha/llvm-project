#ifndef LLVM_TRANSFORMS_UTILS_LOOPWRAP_H
#define LLVM_TRANSFORMS_UTILS_LOOPWRAP_H

#include "llvm/IR/PassManager.h"

namespace llvm {

class LoopWrapPass : public PassInfoMixin<LoopWrapPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_LOOPWRAP_H