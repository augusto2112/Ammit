

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include <vector>

using namespace llvm;

namespace {
struct SwitchInfo {
  SwitchInfo(unsigned int cases, StringRef functionName,
             bool isReferenceDependant, unsigned int loopDepth)
      : cases{cases}, functionName{functionName},
        isReferenceDependant{isReferenceDependant}, loopDepth{loopDepth} {}

  const unsigned int cases;
  const StringRef functionName;
  const bool isReferenceDependant;
  const unsigned int loopDepth;
};

raw_ostream &operator<<(raw_ostream &os, const SwitchInfo &switchInfo) {
  return os << "{ Function: " << switchInfo.functionName
            << ", cases: " << switchInfo.cases
            << ", is reference dependant: " << switchInfo.isReferenceDependant
            << ", loop depth: " << switchInfo.loopDepth << " }";
}

struct SwitchFinder : public FunctionPass {
  static char ID;
  SwitchFinder() : FunctionPass(ID) {}

  std::vector<SwitchInfo> collectSwitches(const Function &F,
                                          const LoopInfo &LI) {
    std::vector<SwitchInfo> switchInfo;
    for (auto &B : F) {
      for (auto &I : B) {
        if (auto switchInst = dyn_cast<SwitchInst>(&I)) {
          switchInfo.push_back(gatherData(*switchInst, LI));
        }
      }
    }
    return switchInfo;
  }

  bool isReferenceDependant(const Value &value) {
    if (auto gep = dyn_cast<GetElementPtrInst>(&value)) {
      Value* firstOperand = gep->getOperand(0);
      Type* type = firstOperand->getType();
      if (type->isPointerTy() || type->isArrayTy()) {
        return true;
      }
    }

    if (auto instruction = dyn_cast<Instruction>(&value)) {
      for (auto *operand : instruction->operand_values()) {
        if (isReferenceDependant(*operand)) {
          return true;
        }
      }
    }
    return false;
  }

  SwitchInfo gatherData(const SwitchInst &switchInst, const LoopInfo &LI) {
    unsigned int cases = switchInst.getNumCases();
    StringRef name = switchInst.getFunction()->getName();
    bool isLoaded = isReferenceDependant(switchInst);
    unsigned int loopDepth = LI.getLoopDepth(switchInst.getParent());
    return {cases, name, isLoaded, loopDepth};
  }

  std::vector<SwitchInfo>
  filterRelevantSwitches(std::vector<SwitchInfo> switchInfos) {
    std::vector<SwitchInfo> filtered;
    for (auto switchInfo : switchInfos) {
      if (switchInfo.loopDepth > 0 && switchInfo.isReferenceDependant &&
          (switchInfo.cases == 2 || switchInfo.cases == 3)) {
        filtered.push_back(switchInfo);
      }
    }
    return filtered;
  }

  bool runOnFunction(Function &F) override {
    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
    for (auto switchInfo : filterRelevantSwitches(collectSwitches(F, LI))) {
      errs() << switchInfo << "\n";
    }
    return false;
  }
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
    getLoopAnalysisUsage(AU);
  }
};
} // namespace

char SwitchFinder::ID = 0;

static RegisterPass<SwitchFinder> X("switch-finder", "SwitchFinder");
