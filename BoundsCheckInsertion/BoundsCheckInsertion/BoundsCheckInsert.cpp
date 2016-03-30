//DIBAKAR BARUA-- COMPILERS HW 3.3

#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/Twine.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include <map>
#include <string>
#include <time.h>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "newpass"

namespace {
   
  // Newpass - The first implementation, without getAnalysisUsage.
  struct BoundsCheckInsert : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    BoundsCheckInsert() : FunctionPass(ID) {}

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesCFG();
      AU.addRequired<LoopInfoWrapperPass>();
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<TargetLibraryInfoWrapperPass>();
    }

    //Copied from LLVM Header File
    //Meant to Reduce all GEP to not more than two operands
    /*
    bool splitAllGEPs( Function &F ) {
      	bool Changed = false;
	
	// Visit each GEP instruction.
	for (Function::iterator I = F.begin(), E = F.end(); I != E; ++I)
	  for (BasicBlock::iterator II = I->begin(), IE = I->end(); II != IE; )
	    if (GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(II++)) {
	      unsigned NumOps = GEP->getNumOperands();
	      // Ignore GEPs which are already simple.
	      if (NumOps <= 2)
	        continue;
	      bool FirstIndexIsZero = isa<ConstantInt>(GEP->getOperand(1)) &&
	                              cast<ConstantInt>(GEP->getOperand(1))->isZero();
	      if (NumOps == 3 && FirstIndexIsZero)
	        continue;
	      // The first index is special and gets expanded with a 2-operand GEP
	      // (unless it's zero, in which case we can skip this).
	      Value *NewGEP = FirstIndexIsZero ? GEP->getOperand(0) : GetElementPtrInst::Create(GEP->getOperand(0), GEP->getOperand(1), "tmp", GEP);
	      // All remaining indices get expanded with a 3-operand GEP with zero
	      // as the second operand.
	      Value *Idxs[2];
	      Idxs[0] = ConstantInt::get(Type::getInt64Ty(F.getContext()), 0);
	      for (unsigned i = 2; i != NumOps; ++i) {
	        Idxs[1] = GEP->getOperand(i);
	        NewGEP = GetElementPtrInst::Create(NewGEP, Idxs, Idxs+2, "tmp", GEP);
	      }
	      GEP->replaceAllUsesWith(NewGEP);
	      GEP->eraseFromParent();
	      Changed = true;
	    }
	return Changed;
    }
    */

    bool DynArraySize( GetElementPtrInst* gep) {
    Value *gepOperand = gep->getOperand(0);
    if (BitCastInst *bitcast = dyn_cast<BitCastInst>(gepOperand)) {
      if (bitcast->getNumOperands() == 0) return 0;
      Value *bitcastOperand = bitcast->getOperand(0);
      if (AllocaInst *alloca = dyn_cast<AllocaInst>(bitcastOperand)) {
        if (alloca->getNumOperands() == 0) return 0;
        Value *allocaOperand = alloca->getOperand(0);
        if (BinaryOperator *bin = dyn_cast<BinaryOperator>(allocaOperand)) {
          if (bin->getNumOperands() < 2) return 0;
          return dyn_cast<Instruction>(bin->getOperand(1));
         }
       }
     }
    return 0;
    }

    bool doInitialization(Module &M) override {
      return false;
    }

    bool runOnFunction(Function &F) override {
	
     //GEP Reduction First
     //splitAllGEPs(F);
  
     set<GetElementPtrInst*> allIndices;
     for(inst_iterator i = inst_begin(F); i != inst_end(F); i++)
     {
	//if a GEP instruction is found
	if(GetElementPtrInst* index = dyn_cast<GetElementPtrInst>(&*i))
		allIndices.insert(index);
     }

     //Try to print out Index Value, Array Variable, High Bound and Low Bound for each GEP
     for(set<GetElementPtrInst*>::iterator it = allIndices.begin(); it != allIndices.end(); it++)
     {
	GetElementPtrInst* gepInst = *it;
	//Getting Iterator for this particular GEP.
	gep_type_iterator gepIter = ++gep_type_begin(gepInst);

	//Found GEP FUNCTION, PRINT OPERANDS
  	for(unsigned i = 0; i < gepInst->getNumOperands(); i++)
	{
		errs() << *gepInst->getOperand(i) << "\t";
	}
	errs() << "\n";

	//Check if Constant Sized Array
	if(const ArrayType* at = dyn_cast<ArrayType>(*gepIter))
	{
		Value* indexVal = gepInst->getOperand(gepInst->getNumOperands() - 1);
		//This is a little weird though
		IntegerType* t = cast<IntegerType>(indexVal->getType());
		Value* lowerBound = ConstantInt::get(t,0);
		Value* upperBound = ConstantInt::get(t, at->getNumElements());
		//errs() << "AT GEP " << *gepInst << " with value " << *indexVal << " with bounds " << *lowerBound << " and " << *upperBound << "\n";
		//errs() << "AT GEP " << *gepInst << " with upper bound " << " " << at->getNumElements() << " and index value " << *gepInst->getOperand(gepInst->getNumOperands() - 1) << "\n";
	}
     }
     
    }


  };
}

char BoundsCheckInsert::ID = 0;
static RegisterPass<BoundsCheckInsert> X("BoundsCheckInsertion", "Insertion of Naive Bounds checking at every GetElementPtr Instruction found");
