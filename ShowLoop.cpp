#include "./ShowLoop.h"
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Passes/PassBuilder.h>

using namespace llvm;

PreservedAnalyses ShowLoopPass::run(Function &F, FunctionAnalysisManager &FAM) {
    errs() << "Visit function " << F.getName() << "\n";

    LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
    for (auto *const L: LI) {
        errs() << "-----------------\n";
        for (auto *const B: L->getBlocks()) {
            B->print(errs(), nullptr, false, true);
        }
    }
    return PreservedAnalyses::all();
}

extern "C" LLVM_ATTRIBUTE_WEAK
::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "ShowLoop", "v0.1",
        [](PassBuilder &PB) {
            PB.registerScalarOptimizerLateEPCallback(
                [](
                    FunctionPassManager &FAM,
                    PassBuilder::OptimizationLevel OL
                ) {
                    FAM.addPass(ShowLoopPass());
                }
            );
        }
    };
};

