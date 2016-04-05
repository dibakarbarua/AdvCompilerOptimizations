//DIBAKAR BARUA, CHANDAN SHARMA-- COMPILERS PROJECT PHASE 1

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
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
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
  
    //Insert Exit Call
    CallInst* insertExitCall(Instruction *i) {
    	Function *f = i->getParent()->getParent();
    	Module *m = f->getParent();
    	Type *voidType = Type::getVoidTy(m->getContext());
    	Type *intType = IntegerType::get(m->getContext(), 32);
    	Constant *exitCall = m->getOrInsertFunction("exit", voidType, intType, NULL);
    	Value *exitCode = ConstantInt::get(Type::getInt32Ty(f->getContext()), 1);
    	return CallInst::Create(exitCall, exitCode, "", i);
    }

    void DynArraySize( GetElementPtrInst* gep) {
    Value *gepOperand = gep->getOperand(0);
    errs() << "1\n";
    if (BitCastInst *bitcast = dyn_cast<BitCastInst>(gepOperand)) {
	errs() << "1a\n";
      if (bitcast->getNumOperands() == 0) return;
      errs() << "2\n";
      Value *bitcastOperand = bitcast->getOperand(0);
      if (AllocaInst *alloca = dyn_cast<AllocaInst>(bitcastOperand)) {
        if (alloca->getNumOperands() == 0) return;
	errs() << "3\n";
        Value *allocaOperand = alloca->getOperand(0);
        if (BinaryOperator *bin = dyn_cast<BinaryOperator>(allocaOperand)) {
          if (bin->getNumOperands() < 2) return;
	  errs() << "4\n";
          errs()<< dyn_cast<Instruction>(bin->getOperand(1));
         }
       }
     }
	
    return;
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
	
		//Check if Constant Sized Array
		gep_type_iterator gepIter = ++gep_type_begin(gepInst);
		if(const ArrayType* at = dyn_cast<ArrayType>(*gepIter))
		{
			Value* indexVal = gepInst->getOperand(gepInst->getNumOperands() - 1);
			IntegerType* t = cast<IntegerType>(indexVal->getType());
			Value* lowerBound = ConstantInt::get(t,0);
			Value* upperBound = ConstantInt::get(t,9999);
			Value* tempUb = ConstantInt::get(t,9999);
		
			if( !isa<ConstantInt>(*indexVal) )
			{
				errs() << "Expression Index Value Detected " << *indexVal << "\n";
				//Need to change Bounds and the index variable if it is of form Ai + B
				int32_t B = 0;
				int32_t A = 1;
				if( isa<SExtInst>(indexVal) )
				{
					SExtInst* sInst = cast<SExtInst>(indexVal);
					Instruction* aluInst = cast<Instruction>(sInst->getOperand(0));
					if(!strcmp( aluInst->getOpcodeName(), "add" ) )
					{
						Instruction* mul_ld_inst = cast<Instruction>(aluInst->getOperand(0));
						B = (dyn_cast<ConstantInt>(aluInst->getOperand(1)))->getSExtValue();
						if(!strcmp( mul_ld_inst->getOpcodeName(), "mul" ))
						{
							A = (dyn_cast<ConstantInt>(mul_ld_inst->getOperand(0)))->getSExtValue();
							indexVal = mul_ld_inst->getOperand(1);
						}
					else
					{
						indexVal = aluInst->getOperand(0);
					}
				}
				else if(!strcmp( aluInst->getOpcodeName(), "mul" ) )
				{
					A = (dyn_cast<ConstantInt>(aluInst->getOperand(0)))->getSExtValue();
					indexVal = aluInst->getOperand(1); 

				}
				//simple a[i] form:
				else
				{
					indexVal = sInst->getOperand(0);
				}
				errs() << "A " << A << "\n";
				errs() << "B " << B << "\n";
				errs() << "indexVal " << *indexVal << "\n";
				
			}
			t = cast<IntegerType>(indexVal->getType());
			lowerBound = ConstantInt::get(t,(0-B)/A);
			upperBound = ConstantInt::get(t,(at->getNumElements()-B)/A );
			tempUb = ConstantInt::get(t, (at->getNumElements()-B)/A + 1 );
			//errs() << "YAHAAN";
			
	 	} 
	 	else
	 	{
			//IntegerType* t = cast<IntegerType>(indexVal->getType());
			lowerBound = ConstantInt::get(t,0);
			upperBound = ConstantInt::get(t, at->getNumElements());
		}
		
		//errs() << "AT GEP " << *gepInst << " with upper bound " << " " << at->getNumElements() << " and index value " << *gepInst->getOperand(gepInst->getNumOperands() - 1) << "\n";
		//Now we create the 2 compare functions to be inserted before the GEP before the GEP
		ICmpInst* cmp1 = new ICmpInst(ICmpInst::ICMP_SLT, indexVal, upperBound);
		//ICmpInst* tempCmp1 = new ICmpInst(ICmpInst::ICMP_SLT, indexVal, tempUb);
		ICmpInst* cmp2 = new ICmpInst(ICmpInst::ICMP_SLE, lowerBound, indexVal);

		//Now insert the 2 compare Functions before the instruction
		cmp1->insertBefore(gepInst);
		cmp2->insertBefore(cmp1);
		
		//Now insert two exit functions after each compare
		CallInst *exit1 = insertExitCall(gepInst);
		CallInst *exit2 = insertExitCall(cmp1);
		
		//--------------INSERT FIRST EXIT-----------------------------------
		//Now splitcurrent BasicBlock at exit1 and resulting BasicBlock at GEP
		BasicBlock* currBB = gepInst->getParent();
		BasicBlock* secondBlock = currBB->splitBasicBlock(exit1);	
		BasicBlock* finalBlock = secondBlock->splitBasicBlock(gepInst);

		//Remove link from exit1 to GEP and remove branch insert at end of SecondBlock
		finalBlock->removePredecessor(secondBlock);
		TerminatorInst* secondBBTerm = secondBlock->getTerminator();
		new UnreachableInst(secondBBTerm->getContext(), secondBBTerm);
		secondBBTerm->eraseFromParent();
		
		//Insert a branch after cmp1 that would take you to secondBlock or finalBlock as per predicate
		BranchInst *br1 = BranchInst::Create(finalBlock, secondBlock, cmp1);
		
		//Replace branch at end of original BB
		ReplaceInstWithInst(currBB->getTerminator(), br1);
		
		//--------------INSERT SECOND EXIT-----------------------------------
		//Now splitcurrent BasicBlock at exit1 and resulting BasicBlock at GEP
		currBB = cmp1->getParent();
		secondBlock = currBB->splitBasicBlock(exit2);	
		finalBlock = secondBlock->splitBasicBlock(cmp1);

		//Remove link from exit1 to GEP and remove branch insert at end of SecondBlock
		finalBlock->removePredecessor(secondBlock);
		secondBBTerm = secondBlock->getTerminator();
		new UnreachableInst(secondBBTerm->getContext(), secondBBTerm);
		secondBBTerm->eraseFromParent();
		
		//Insert a branch after cmp1 that would take you to secondBlock or finalBlock as per predicate
		BranchInst *br2 = BranchInst::Create(finalBlock, secondBlock, cmp2);
		
		//Replace branch at end of original BB
		ReplaceInstWithInst(currBB->getTerminator(), br2);

	}

			
   }
     
  }


  };
}

char BoundsCheckInsert::ID = 0;
static RegisterPass<BoundsCheckInsert> X("BoundsCheckInsertion", "Insertion of Naive Bounds checking at every GetElementPtr Instruction found");
