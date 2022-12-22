#pragma once
#include <llvm/IR/PassManager.h>

using namespace llvm;
struct ShowLoopPass: public PassInfoMixin<ShowLoopPass> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM);
    static bool isRequired() { return true; }
};
