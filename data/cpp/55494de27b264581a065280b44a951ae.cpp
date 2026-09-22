// Write a C++ function `splitIfThenElse` that takes a reference to an `llvm::Instruction` (the "anchor" instruction), an `llvm::BasicBlock*` pointer to the "then" block's terminator, and an `llvm::BasicBlock*` pointer to the "else" block's terminator, along with a boolean flag `useORD` indicating whether to use an ordered comparison. The function must transform the control flow at the anchor as follows: given that the anchor instruction is in some block `B` and its terminator is `T`, the function should split `B` into two blocks at the instruction following the anchor. Then, it should insert a conditional branch at `T` that branches to the "then" block (which contains a cloned copy of the anchor) when the condition is true, and to the "else" block (which contains the original anchor) otherwise. The condition for the branch must be: if `useORD` is `true`, use `fcmp ord` on the anchor's result with itself; otherwise, use `fcmp oge` on the anchor's first operand with a constant `0.0` of the anchor's type. The function must return the newly created "join" block (the block after splitting) and ensure the original anchor is moved to the "else" block (i.e., the "else" block's terminator is the original terminator `T`). The "then" block must contain a clone of the anchor (using `Instruction::clone()`) inserted before its terminator, and the "else" block must contain the original anchor inserted before its terminator. If the anchor is a `PHINode` or has no result, return `nullptr`. The function must also set both successors of the conditional branch correctly: successor 0 is the "then" block, successor 1 is the "else" block. The function must not modify the anchor's operands except possibly moving it; also, it must use `IRBuilder<>` minimally to create the branch and compare.
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include <cassert>

int main() {
  using namespace llvm;
  LLVMContext Ctx;
  Module M("test", Ctx);
  FunctionType* FT = FunctionType::get(Type::getDoubleTy(Ctx), {Type::getDoubleTy(Ctx)}, false);
  Function* F = Function::Create(FT, GlobalValue::ExternalLinkage, "f", &M);
  BasicBlock* Entry = BasicBlock::Create(Ctx, "entry", F);
  BasicBlock* Exit = BasicBlock::Create(Ctx, "exit", F);
  IRBuilder<> Builder(Entry);
  Function* sqrtFunc = Function::Create(FT, GlobalValue::ExternalLinkage, "sqrt", &M);
  Value* Arg = &*F->arg_begin();
  CallInst* call = Builder.CreateCall(sqrtFunc, Arg, "sqrt_call");
  Builder.CreateBr(Exit);
  Builder.SetInsertPoint(Exit);
  Builder.CreateRet(call);

  // Test 1: Basic split with ORD.
  BasicBlock* JoinBB = splitIfThenElse(call, true);
  assert(JoinBB != nullptr);
  assert(call->getParent()->getName() == "else");
  // The clone should be in the 'then' block.
  bool foundThen = false;
  for (auto& I : *JoinBB->getSinglePredecessor()) { (void)I; } // dummy
  // Check that the conditional branch in Entry has two successors.
  auto* Br = dyn_cast<BranchInst>(Entry->getTerminator());
  assert(Br && Br->isConditional());
  assert(Br->getNumSuccessors() == 2);
  assert(Br->getSuccessor(0)->getName() == "then");
  assert(Br->getSuccessor(1)->getName() == "else");
  // Check the PHI in Join.
  auto* Phi = dyn_cast<PHINode>(&*JoinBB->begin());
  assert(Phi && Phi->getNumIncomingValues() == 2);

  // Test 2: Invalid case – void return type.
  FunctionType* VoidFT = FunctionType::get(Type::getVoidTy(Ctx), false);
  Function* F2 = Function::Create(VoidFT, GlobalValue::ExternalLinkage, "f2", &M);
  BasicBlock* B2 = BasicBlock::Create(Ctx, "b2", F2);
  Builder.SetInsertPoint(B2);
  CallInst* voidCall = Builder.CreateCall(sqrtFunc);
  Builder.CreateRetVoid();
  assert(splitIfThenElse(voidCall, true) == nullptr);

  // Test 3: Invalid case – PHINode.
  FunctionType* IntFT = FunctionType::get(Type::getInt32Ty(Ctx), false);
  Function* F3 = Function::Create(IntFT, GlobalValue::ExternalLinkage, "f3", &M);
  BasicBlock* B3e = BasicBlock::Create(Ctx, "entry", F3);
  BasicBlock* B3l = BasicBlock::Create(Ctx, "loop", F3);
  BasicBlock* B3x = BasicBlock::Create(Ctx, "exit", F3);
  Builder.SetInsertPoint(B3l);
  PHINode* phi = Builder.CreatePHI(Type::getInt32Ty(Ctx), 2);
  Builder.CreateBr(B3x);
  Builder.SetInsertPoint(B3x);
  Builder.CreateRet(phi);
  assert(splitIfThenElse(phi, true) == nullptr);

  // Test 4: Non-FP type.
  FunctionType* IntFT2 = FunctionType::get(Type::getInt32Ty(Ctx), {Type::getInt32Ty(Ctx)}, false);
  Function* F4 = Function::Create(IntFT2, GlobalValue::ExternalLinkage, "f4", &M);
  BasicBlock* B4 = BasicBlock::Create(Ctx, "b4", F4);
  Builder.SetInsertPoint(B4);
  Function* intFunc = Function::Create(IntFT2, GlobalValue::ExternalLinkage, "add", &M);
  Value* A4 = &*F4->arg_begin();
  CallInst* intCall = Builder.CreateCall(intFunc, A4);
  Builder.CreateRet(intCall);
  assert(splitIfThenElse(intCall, true) == nullptr);

  return 0;
}
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

llvm::BasicBlock* splitIfThenElse(llvm::Instruction* Anchor, bool useORD) {
  using namespace llvm;
  if (!Anchor || Anchor->getType()->isVoidTy() || isa<PHINode>(Anchor))
    return nullptr;
  Type* Ty = Anchor->getType();
  if (!Ty->isFPOrFPVectorTy())
    return nullptr;
  if (!useORD && Anchor->getNumOperands() < 1)
    return nullptr;

  BasicBlock* B = Anchor->getParent();
  Function* F = B->getParent();

  // Split B at the instruction after Anchor. The continuation block is Join.
  BasicBlock* Join = SplitBlock(B, Anchor->getNextNode());

  // Create 'then' and 'else' blocks.
  BasicBlock* ThenBB = BasicBlock::Create(F->getContext(), "then", F, Join);
  BasicBlock* ElseBB = BasicBlock::Create(F->getContext(), "else", F, Join);

  // Move original anchor to ElseBB.
  Anchor->removeFromParent();
  ElseBB->getInstList().push_back(Anchor);

  // Clone anchor into ThenBB.
  Instruction* Clone = Anchor->clone();
  ThenBB->getInstList().push_back(Clone);

  // Create terminator branches for ThenBB and ElseBB.
  BranchInst::Create(Join, ThenBB);
  BranchInst::Create(Join, ElseBB);

  // Create condition for the branch in B.
  IRBuilder<> Builder(B, B->getFirstInsertionPt());
  Value* Cond = nullptr;
  if (useORD) {
    Cond = Builder.CreateFCmpORD(Anchor, Anchor);
  } else {
    Value* Zero = ConstantFP::get(Ty, 0.0);
    Cond = Builder.CreateFCmpOGE(Anchor->getOperand(0), Zero);
  }

  // Replace B's terminator (which is unconditional to Join) with a conditional branch.
  Builder.SetInsertPoint(B->getTerminator());
  Builder.CreateCondBr(Cond, ThenBB, ElseBB);
  B->getTerminator()->eraseFromParent();

  // Create PHI in Join to merge results.
  Builder.SetInsertPoint(Join, Join->begin());
  PHINode* Phi = Builder.CreatePHI(Ty, 2);
  Phi->addIncoming(Clone, ThenBB);
  Phi->addIncoming(Anchor, ElseBB);

  // Replace all uses of Anchor with Phi.
  Anchor->replaceAllUsesWith(Phi);

  // Fix the PHI's incoming value: after replaceAllUsesWith, the Phi's incoming
  // for Anchor still references Anchor but that's intended.
  return Join;
}
// The solution involves the following steps: first, validate that the anchor instruction has a result type (i.e., not a `void` type) and is not a `PHINode`, since we need to create a PHI node later. Then, split the original block `B` at the instruction immediately after the anchor. This splitting is done using `SplitBlockAndInsertIfThen` from LLVM, but we need to customize it because we want an "else" block. We can achieve this by first creating a "then" block using `SplitBlockAndInsertIfThen` with a false condition (here we use a constant `true` condition as a placeholder, but we will later replace it). The function `SplitBlockAndInsertIfThen` creates a new block that branches back to the continuation (the join block) and inserts a conditional branch at the original block's terminator. However, we need to swap the successors to get an "else" block. Specifically, after calling `SplitBlockAndInsertIfThen(Builder.getTrue(), Call->getNextNode(), ...)`, we get a terminator that branches to the new "then" block and the join block (the split-off tail). We swap the successors so that the original block branches to the join block on true and to the "then" block (which we rename as "else") on false. Then, we create a PHI node in the join block that merges the original anchor (which stays in the original block) and the cloned anchor (which goes into the "else" block). We then replace all uses of the original anchor with the PHI node. The original anchor is moved to the "else" block (which is the block that was originally created as "then" but now serves as the "else" because we swapped successors). Finally, we insert the clone into the "else" block before its terminator. The condition for the branch is created using `IRBuilder` at the original terminator: if `useORD` is true, create `fcmp ord` on the anchor's result with itself; otherwise, create `fcmp oge` on the anchor's first operand with a constant zero of the anchor's type. Then set that as the condition of the original terminator. The function returns the join block. Edge cases: If the anchor has no result (void), we cannot create a PHI or compare, so return `nullptr`. If the anchor is a `PHINode` itself, it might have uses that are difficult to replace safely, so we skip. Also, the anchor must be in a block with a terminator that is a `BranchInst`; otherwise, we cannot swap successors. We also need to ensure that the anchor is the only instruction we move, and that no other code depends on the original block's layout. The time complexity is O(1) in terms of the number of instructions in the block (we only clone one instruction and insert it), and O(1) space. A subtle point: When we clone the anchor, we must keep the same operands; the clone is a new instruction with the same operands, and we insert it before the terminator of the "else" block. Also, we must ensure that the original anchor is moved to the "else" block, which means we need to remove it from its original position and insert it before the terminator of the "else" block. But in the given code snippet, the original call remains in the original block and the clone goes into the "else" block; in our task, the original anchor goes to the "else" block and the clone goes to the "then" block? Actually, read the task carefully: "the function should transform the control flow at the anchor as follows: ... The condition for the branch must be: if useORD is true, use fcmp ord on the anchor's result with itself; otherwise, use fcmp oge on the anchor's first operand with a constant 0.0 of the anchor's type. The function must return the newly created 'join' block (the block after splitting) and ensure the original anchor is moved to the 'else' block (i.e., the 'else' block's terminator is the original terminator T). The 'then' block must contain a clone of the anchor (using Instruction::clone()) inserted before its terminator, and the 'else' block must contain the original anchor inserted before its terminator." So the original block `B` becomes the "then" block? Actually, after splitting, we have the original block that now has the conditional branch terminator, and the join block. The "then" block is a new block created by `SplitBlockAndInsertIfThen` that contains the clone. The "else" block is the original block `B` itself (because we swap successors, so the original block becomes the "else" target). Wait, the snippet's logic: `SplitBlockAndInsertIfThen` creates a new block between the original block and the join block, and the original block's terminator becomes a conditional branch with the new block as the "then" successor. Then they swap successors to make the original block branch to the join block on true and to the new block on false. So the original block is the "then" target? No, after swapping, the original block's terminator has successor 0 = join, successor 1 = new block. So if condition is true, branch to join (skip the library call), if false, branch to new block (the library call). So the original block is not the "then" block; it's the block where the condition is evaluated, and the new block is the "else" block. But the task says "the original anchor is moved to the 'else' block". That matches: the original anchor remains in the original block (which is the "then" path? Actually, in the snippet, the original call stays in the original block, and that block is the "if" block. The "else" block is the new block containing the library call. In our task, we are asked to put the original anchor in the "else" block and the clone in the "then" block. So we need to move the original anchor to the new block (which we call "else") and put the clone in the original block? No, re-read: "The 'then' block must contain a clone of the anchor ... and the 'else' block must contain the original anchor". In the snippet, the original call is in the original block, and the clone is in the new block. The original block is the "if" block (the condition is evaluated there). The new block is the "else" block. So the original anchor is in the original block, which is not called "else". The task says "the original anchor is moved to the 'else' block". That implies that the original block becomes the "else" block. But that contradicts the snippet. Let's clarify: The task is inspired by the snippet but is a simplified standalone task. We can define the "else" block as the new block created by splitting, and the "then" block as the original block. Or we can define it as in our own way. To be consistent, we'll follow the task description precisely: We will create a "then" block and an "else" block. The "then" block will contain the clone, and the "else" block will contain the original. We'll use `SplitBlockAndInsertIfThen` to create a block that we will treat as the "else" block (by swapping successors). Our plan: Given anchor instruction `A` in block `B` with terminator `T` (a branch). We split `B` after `A` (i.e., at `A->getNextNode()`) using `SplitBlockAndInsertIfThen` with a placeholder condition. The function will create a new block `NewBB` and insert a conditional branch at `T` that branches to `NewBB` on true and to the continuation (join) on false. We then swap successors so that `T` branches to join on true and to `NewBB` on false. Now `NewBB` is the "else" block. We move the original anchor `A` from `B` to `NewBB` (insert before its terminator). We clone `A` and insert the clone into `B`? But `B` is the block that evaluates the condition; it does not contain the anchor anymore, so we cannot put the clone there. Instead, the "then" block should be a new block that contains the clone and branches to join. But we already have the join block as the continuation. To create a "then" block, we can split `B` at the anchor, but that is not what `SplitBlockAndInsertIfThen` does. Alternatively, we can think differently: The task says "transform the control flow at the anchor as follows: given that the anchor instruction is in some block B and its terminator is T, the function should split B into two blocks at the instruction following the anchor." That means we split `B` into `B1` (containing the anchor and possibly preceding instructions) and `B2` (the rest, the join). Then we insert a conditional branch at the new terminator of `B1` that branches to a "then" block and an "else" block. The "then" block contains a clone of the anchor, and the "else" block contains the original anchor. So after splitting, `B1`'s terminator is a conditional branch. We need to create two new blocks: `ThenBB` and `ElseBB`. The original anchor is in `B1`. We move the original anchor to `ElseBB`. We clone it into `ThenBB`. Both `ThenBB` and `ElseBB` branch to `B2` (the join). This is different from the snippet, which uses `SplitBlockAndInsertIfThen` to create only one new block. For our task, we can implement it manually: Use `SplitBlockAndInsertIfThen` twice? Or use `SplitBlock` and then create blocks manually. To keep it simple, we can follow a simpler approach: Use `SplitBlockAndInsertIfThen` to create a block that will be the "else" block (since in the snippet, that new block is where the libcall goes). Then we create another block for the "then" block by splitting the original block at the anchor? But the original block still contains the anchor and the terminator; we need to remove the anchor from it and put it elsewhere. Actually, the snippet does not move the original call; it stays in the original block. The condition is in the original block, and if the condition is true (meaning no library call needed), it branches directly to the join, skipping the new block. If false, it goes to the new block which contains the library call. So the original anchor is not on any branch; it is in the same block as the condition, and it is always executed (the result of the original call is used in the phi). In our task, we are asked to have the original anchor in the "else" block, which is a branch target. This is a different structure. We'll implement it from scratch using basic block utilities.
//
// Let's design: Given anchor `A` in block `B` with terminator `T`. We split `B` at `A->getNextNode()` using `SplitBlock` (from `llvm/Transforms/Utils/BasicBlockUtils.h`), which gives us a new block `Join` (the continuation) and modifies `B` so that its terminator becomes an unconditional branch to `Join`. But we need a conditional branch, so we'll then remove the unconditional branch and create a conditional branch. Steps:
//
// 1. Check that `A` has a result type (not void) and is not a `PHINode`. Also check that `B`'s terminator is a `BranchInst` (though we will replace it).
// 2. `BasicBlock *Join = SplitBlock(B, A->getNextNode(), DTU)` – this splits `B` at the instruction after `A`, so `B` ends with an unconditional branch to `Join`. Note: `A` remains in `B` (the first part).
// 3. Create two new blocks: `ThenBB` and `ElseBB`. We can use `BasicBlock::Create(F.getContext(), "then", &F, Join)` and similarly for "else". Insert them after `B` in the function's block list.
// 4. Move `A` from `B` to `ElseBB`: `A->removeFromParent(); ElseBB->getInstList().push_back(A);` (or insert before the terminator of `ElseBB`).
// 5. Clone `A`: `Instruction *Clone = A->clone();` and insert into `ThenBB` (e.g., `ThenBB->getInstList().push_back(Clone);`).
// 6. Create branch instructions: In `B`, replace its terminator with a conditional branch. We need to create a condition using `IRBuilder`. The condition is `fcmp ord` on `A`'s result with itself if `useORD` true, else `fcmp oge` on `A->getOperand(0)` with `ConstantFP::get(A->getType(), 0.0)`. Use `IRBuilder<> Builder(B, B->getFirstInsertionPt())`? Actually we will set the condition at the end of `B` (where the old terminator was). We'll create a new `BranchInst` with the condition, `ThenBB`, `ElseBB`, and replace the old terminator.
// 7. Make `ThenBB` and `ElseBB` branch unconditionally to `Join`: `BranchInst::Create(Join, ThenBB);` and similarly for `ElseBB`.
// 8. Create a PHI node in `Join` at the beginning: `PHINode *Phi = Builder.CreatePHI(A->getType(), 2);` (need to set insert point to `Join->begin()`). Add incoming values: one from `A` (which is now in `ElseBB`) and one from `Clone` (in `ThenBB`). Specifically, `Phi->addIncoming(A, ElseBB);` and `Phi->addIncoming(Clone, ThenBB);`.
// 9. Replace all uses of `A` with `Phi` before moving `A`? Actually, we should replace uses before moving, but after moving we can still do it because `A` is still a valid `Value*`. We can do: `A->replaceAllUsesWith(Phi);` after creating the PHI and adding incoming values, but careful: the incoming value `A` is also a use? No, `Phi->addIncoming(A, ElseBB)` adds `A` as an operand of the PHI, but that is fine since we are just setting the operands directly via the method, not using `replaceAllUsesWith` on that. So we can call `A->replaceAllUsesWith(Phi)` to redirect all other uses of `A` (outside the PHI) to `Phi`. But then the PHI already has `A` as an incoming value, which is fine; it refers to the original `A` instruction itself.
// 10. Return `Join`.
//
// Edge cases: If `A` has no uses except the PHI, that's fine. If `A` is a terminator itself? We assume it's not a terminator; it must be a regular instruction. We also need to ensure that the anchor's type is floating-point (since we use `fcmp` and `ConstantFP`). We can check `A->getType()->isFPOrFPVectorTy()`? The task says "anchor's result" and "first operand", so we assume it's a floating-point operation. But we should check that `A->getType()->isFPOrFPVectorTy()` and `A->getNumOperands() > 0` for the `oge` case. If not, return `nullptr`. Also, if the block `B` has a terminator that is not a `BranchInst` (e.g., `ReturnInst`), we cannot split after the anchor properly, but `SplitBlock` works fine because it creates an unconditional branch. However, after splitting, the original terminator `T` is removed and replaced by an unconditional branch to `Join`, and the original `T` (if it was a return) would be at the end of `Join`? No, `SplitBlock` moves everything after the split point (including the original terminator) to the new block, and creates a new unconditional terminator in the original block. So `Join`'s terminator is the original `T`. That's fine. We don't need to check that `T` is branch. So the only checks are: `A` is not void, not a PHI, and has FP type, and has at least one operand if `useORD` is false.
//
// Time complexity: O(1) (constant number of new instructions). Space: O(1) (we create two new blocks and a few instructions).
//
// We'll implement as a free function `BasicBlock* splitIfThenElse(Instruction* Anchor, bool useORD)`.
