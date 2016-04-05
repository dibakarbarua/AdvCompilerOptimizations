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
  struct BoundsCheckRemove : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    map<BasicBlock*, bool> exitBlocks;
    BoundsCheckRemove() : FunctionPass(ID) {}

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesCFG();
      AU.addRequired<LoopInfoWrapperPass>();
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<TargetLibraryInfoWrapperPass>();
    }

    set<Instruction*> CalcBackwardSet(set<Instruction*> input)
    {
        //Dummy
        return input;
    }

    set<Instruction*> CalcForwardSet(set<Instruction*> input)
    {
        return input;
    }

    set<Instruction*> Intersection(vector< set<Instruction*> > input)
    {
        set<Instruction*> output;
        if(input.size() == 1)
        {
            return (*input.begin());
        }
        for(vector<set<Instruction*>>::iterator set1 = input.begin(); set1 != input.end(); set1++)
        {
            for(set<Instruction*>::iterator inst1 = set1->begin(); inst1 != set1->end(); inst1++)
            {
                bool flag1 = 0;
                for(vector<set<Instruction*>>::iterator set2 = input.begin(); set2 != input.end(); set2++)
                {
                    bool flag2 = 0;
                    if(set1 == set2) { flag2 = 1; continue; }
                    for(set<Instruction*>::iterator inst2 = set2->begin(); inst2 != set2->end(); inst2++)
                    {
                        if( subsumesOrIdentical( *inst1, *inst2 ) )
                        {
                            flag2 = 1; break;
                        }
                    }
                    if(flag2 == 0) { flag1 = 0; break;}
                    else flag1 = 1;
                }
                if(flag1 == 1)
                    output.insert(*inst1);
            }
            /*errs() << "\nPerforming Intersection Now\n";
            for(set<Instruction*>::iterator inst = output.begin(); inst != output.end(); inst++)
            {
                errs() << **inst << "\n";
            }
            errs() << "\n";*/
        }
        return output;
    }

    set<Instruction*> Union(Instruction* cmp, set<Instruction*> input)
    {
        set<Instruction*> output;
        bool flag = 1;
        for(set<Instruction*>::iterator inst = input.begin(); inst!= input.end(); inst++)
        {
            if(identical(*inst,cmp))
            {
                output.insert(*inst);
                continue;
            }
            if(subsumesOrIdentical(*inst,cmp))
            {
                flag = 0;
                continue;

            }
            if(!subsumesOrIdentical(cmp,*inst))
            {
                output.insert(*inst);
            }
        }
        if(flag == 1)
            output.insert(cmp);
        return output;
    }

    void removeBoundCheck(Instruction* cmp)
    {
    	BasicBlock* currBB = cmp->getParent();
    	TerminatorInst* termInst = currBB->getTerminator();
        BasicBlock* pointTo;
        BasicBlock* toDelete;
    	for(unsigned i =0; i< termInst->getNumSuccessors(); i++)
    	{
            BasicBlock* succBB = termInst->getSuccessor(i);
            errs() <<"Exploring " << *succBB << "\n";
    		if( exitBlocks.find(succBB) != exitBlocks.end())
    			//Basic Block to be removed
            {
    			toDelete = succBB;
                errs() <<"Erased Exit Block\n";
    		}
            else
    	       pointTo = succBB;
    	}
        BranchInst* newBr = BranchInst::Create(pointTo);
        ReplaceInstWithInst(termInst, newBr);
        toDelete->eraseFromParent();
        //cmp->removeFromParent();
    	return;

    }

    bool sameIndex(Value* v1, Value* v2)
    {
    	LoadInst* l1 = dyn_cast<LoadInst>(v1);
    	LoadInst* l2 = dyn_cast<LoadInst>(v2);
    	//errs() << "Values " << *v1 << " " << *v2 << "\n";
    	//errs() << "Index of v1 " << l1->getPointerOperand()->getName() << "\n";
    	//errs() << "Index of v2 " << l2->getPointerOperand()->getName() << "\n"; 
    	return (l1->getPointerOperand()->getName() == l2->getPointerOperand()->getName());
     }

     bool isLowerBound(Instruction* cmp)
     {
     	Value* val = cmp->getOperand(1);
     	if(isa<ConstantInt>(val))
     		return 1;
     	else return 0;
     }

     bool isUpperBound(Instruction* cmp)
     {
     	Value* val = cmp->getOperand(0);
     	if(isa<ConstantInt>(val))
     		return 1;
     	else return 0;
     }

    bool subsumesOrIdentical(Instruction* cmp1, Instruction* cmp2)
    {
    	if(isUpperBound(cmp1) && isLowerBound(cmp2)) return 0;
    	if(isLowerBound(cmp1) && isUpperBound(cmp2)) return 0;
    	if (isLowerBound(cmp1))
    	{
    		if(sameIndex(cmp1->getOperand(0), cmp2->getOperand(0)))
    		{
    			if(cmp1->getOperand(1) >= cmp2->getOperand(1))
    				return 1;
    			else
    				return 0;
    		}
    		else
    			return 0;
    	}
    	else
    	{
    		if(sameIndex(cmp1->getOperand(1), cmp2->getOperand(1)))
    		{
    			if(cmp2->getOperand(0) >= cmp1->getOperand(0))
    				return 1;
    			else
    				return 0;
    		}
    		else
    			return 0;
    	}

    }

    bool identical(Instruction* cmp1, Instruction* cmp2)
    {
        if(isUpperBound(cmp1) && isLowerBound(cmp2)) return 0;
        if(isLowerBound(cmp1) && isUpperBound(cmp2)) return 0;
        if (isLowerBound(cmp1))
        {
            if(sameIndex(cmp1->getOperand(0), cmp2->getOperand(0)))
            {
                if(cmp1->getOperand(1) == cmp2->getOperand(1))
                    return 1;
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            if(sameIndex(cmp1->getOperand(1), cmp2->getOperand(1)))
            {
                if(cmp2->getOperand(0) == cmp1->getOperand(0))
                    return 1;
                else
                    return 0;
            }
            else
                return 0;
        }

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

     //Collect Exit Blocks
     for(Function::iterator myFuncIter = F.begin(); myFuncIter != F.end(); myFuncIter++)
     {
        Instruction* exitInst = myFuncIter->begin();
        if(CallInst* callExitInst = dyn_cast<CallInst>(exitInst))
        {
            //Call Function blindly assumed to be Exit. Better Code may be possible
            exitBlocks[myFuncIter] = 1;
        }
     }

     errs() <<"Ant Start\n";

     //------------------Loop1: Anticipatability Analysis---------------------------------:

     //basic block map for storing IN and OUT sets 
     map<BasicBlock*, pair<set<Instruction*>, set<Instruction*>>> antSetsMap;
     //to keep track of with loop and without loop cases
     map<BasicBlock*, bool > antVisitedMap;

     Function::iterator it = F.begin();

     while(it!=F.end())
     { 
         antVisitedMap[it] = false;
         set<Instruction*> antInSet;
         set<Instruction*> antOutSet;
         antSetsMap[it] = make_pair(antInSet, antOutSet);
         it++;
     }

     errs() <<"Init done\n";
    
    //reinitialize 
    int counter = 0;
    while(counter < 2)
    {
        it = --F.end();
        while(1)
        {
                vector<set<Instruction*>> antOutVector;
                TerminatorInst* termInst = it->getTerminator();

                //errs() << "--------------WORKING ON -------------\n" << *it << "\n";
                //Collect successor antIn sets to get antOut
                for(unsigned i = 0; i< termInst->getNumSuccessors(); i++)
                {
                        BasicBlock* currSucc = termInst->getSuccessor(i);
                        //skip an exit Block successor.
                        if( exitBlocks.find(currSucc) != exitBlocks.end() ) continue;
                        antOutVector.push_back(antSetsMap[currSucc].first);
                }

                /*errs() << "Successor Sets Relevant " << antOutVector.size() << "\n";
                for(vector<set<Instruction*>>::iterator vecIter = antOutVector.begin(); vecIter != antOutVector.end(); vecIter++)
                {
                    set<Instruction*> mySet = *vecIter;
                    for(set<Instruction*>::iterator setIter = mySet.begin(); setIter != mySet.end(); setIter++)
                    {
                        errs() << **setIter << "\n";
                    }
                }
                errs() << "\n";*/


                set<Instruction*> antOut = Intersection(antOutVector);
                set<Instruction*> backwardSet = CalcBackwardSet(antOut);
                set<Instruction*> antIn = backwardSet;
                for(BasicBlock::iterator myBBIter = it->begin(); myBBIter!= it->end(); myBBIter++)
                {
                   
                    const char* opCode = myBBIter->getOpcodeName();
                    if( !strcmp(opCode, "icmp") )
                    {
                            antIn = Union(myBBIter, backwardSet);
                    }
                }
                antSetsMap[it].first = antIn;
                antSetsMap[it].second = antOut;
                if(it == F.begin()) break;
                it--;

        }
        counter++;
    }
    errs() <<"Ant Map Finalized\n";


    /*for(map<BasicBlock*, pair<set<Instruction*>, set<Instruction*>>>::iterator myMapIter = antSetsMap.begin(); myMapIter != antSetsMap.end(); myMapIter++)
    {
        errs() << "-------------------BASIC BLOCK -----------------\n";
        errs() << *(myMapIter->first) << "\n";
        errs() << "ANT OUT SET\n";
        set<Instruction*> antOut = (myMapIter->second).second;
        set<Instruction*> antIn = (myMapIter->second).first;
        for(set<Instruction*>::iterator set1 = antOut.begin(); set1!= antOut.end(); set1++)
            errs() << **set1 << "\n";
        errs() << "ANT IN SET\n";
        for(set<Instruction*>::iterator set1 = antIn.begin(); set1!= antIn.end(); set1++)
            errs() << **set1 << "\n";
    }*/

    //----------------------------LOOP2: Modification of Bound Checks----------------------------------------

    errs() <<"Beginnning Modification\n";

    it = F.begin();
    while(it!= F.end())
    {
        for(BasicBlock::iterator myBBIter = it->begin(); myBBIter!= it->end(); myBBIter++)
        {
            if( !strcmp(myBBIter->getOpcodeName(), "icmp"))
            {
                set<Instruction*> antOut = antSetsMap[it].second;
                for(set<Instruction*>::iterator inst = antOut.begin(); inst != antOut.end(); inst++)
                {
                    if(subsumesOrIdentical(*inst, myBBIter))
                    {
                        //Need to replace inst by myBBIter by changing Bound Value
                        errs() << "Found future subsumption! for " << *myBBIter << " before " << **inst << "\n";
                        if(isLowerBound(myBBIter))
                        {
                            myBBIter->setOperand(1, (*inst)->getOperand(1));
                        }
                        else
                        {
                            myBBIter->setOperand(0, (*inst)->getOperand(0));
                        }
                        break;
                    }
                }
            }
        }
        it++;
    }

    errs() <<"Done with Modification\n";

    //-------------------------AVAILABILITY ANALYSIS-----------------------------------
    //basic block map for storing IN and OUT sets 
     map<BasicBlock*, pair<set<Instruction*>, set<Instruction*>>> availSetsMap;
     //to keep track of with loop and without loop cases
     map<BasicBlock*, bool > availVisitedMap;

     it = F.begin();

     while(it!=F.end())
     { 
         availVisitedMap[it] = false;
         set<Instruction*> availInSet;
         set<Instruction*> availOutSet;
         availSetsMap[it] = make_pair(availInSet, availOutSet);
         it++;
     }

     errs() <<"Avail Init done\n";
    
    //reinitialize 
    counter = 0;
    while(counter < 2)
    {
        it = F.begin();
        while(it!=F.end())
        {
                vector<set<Instruction*>> availInVector;

                auto predIt =  pred_begin(it);
                while(predIt!= pred_end(it))
                {
                        BasicBlock* currPred = *predIt;
                        availInVector.push_back(availSetsMap[currPred].second);
                        predIt++;
                }

                /*errs() << *it <<"\n";
                //errs() << "Predecessor Sets Relevant " << availInVector.size() << "\n";
                errs() << "AVAILABILITY IN ANALYSIS\n";
                for(vector<set<Instruction*>>::iterator vecIter = availInVector.begin(); vecIter != availInVector.end(); vecIter++)
                {
                    set<Instruction*> mySet = *vecIter;
                    for(set<Instruction*>::iterator setIter = mySet.begin(); setIter != mySet.end(); setIter++)
                    {
                        errs() << **setIter << "\n";
                    }
                }
                errs() << "\n";*/


                set<Instruction*> availIn = Intersection(availInVector);
                set<Instruction*> forwardSet = CalcForwardSet(availIn);
                set<Instruction*> availOut = forwardSet;

                for(BasicBlock::iterator myBBIter = it->begin(); myBBIter!= it->end(); myBBIter++)
                {
                   
                    const char* opCode = myBBIter->getOpcodeName();
                    if( !strcmp(opCode, "icmp") )
                    {
                            availOut = Union(myBBIter, forwardSet);
                    }
                }

                /*errs() << "AVAILABILITY OUT ANALYSIS \n";
                for(set<Instruction*>::iterator setIter = availOut.begin(); setIter != availOut.end(); setIter++)
                {
                       errs() << **setIter << "\n";
                }
                
                errs() << "\n";*/

                availSetsMap[it].first = availIn;
                availSetsMap[it].second = availOut;
                it++;

        }
        counter++;
    }
    errs() <<"Avail Map Finalized\n";     

    errs() << "Beginnning Redundancy Elimination\n";

    it = F.begin();
    while(it!= F.end())
    {
        for(BasicBlock::iterator myBBIter = it->begin(); myBBIter!= it->end(); myBBIter++)
        {
            if( !strcmp(myBBIter->getOpcodeName(), "icmp"))
            {
                set<Instruction*> availIn = availSetsMap[it].first;
                for(set<Instruction*>::iterator inst = availIn.begin(); inst != availIn.end(); inst++)
                {
                    if(subsumesOrIdentical(*inst, myBBIter))
                    {
                        //Need to replace inst by myBBIter by changing Bound Value
                        errs() << "Found past subsumption, potential elimination! for " << *myBBIter << " before " << **inst << "\n";
                        removeBoundCheck(myBBIter);
                        break;
                    }
                }
            }
        }
        it++;
    }

    errs() <<"Done with Redundancy Elimination\n"; 
}

  };
    
}

char BoundsCheckRemove::ID = 0;
static RegisterPass<BoundsCheckRemove> X("BoundsCheckRemoval", "Optimization of Bound Checks, Paper II");
