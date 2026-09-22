/*
Write a C++ function that performs a backward data-flow analysis to compute liveness information for local variables in a method represented as a sequence of bytecode-like instructions. The function should take a simplified method description (number of local variables, a list of basic blocks defined by start and end bytecode indices, and for each block a list of its successors) and return, for every block, the set of local variables that are live on entry to that block. A local variable is live at a point if there exists a path from that point to a use of the variable along which the variable is not redefined. The bytecode is simplified to only two instruction types affecting liveness: `LOAD` (uses a local variable) and `STORE` (defines a local variable). The input method is a vector of instructions, each either a load or store of a specific local index. Basic blocks are contiguous ranges of instructions, and control flow is represented by a successor list for each block (implicitly, blocks may have multiple successors). The function should compute entry-liveness sets for all blocks using iterative work-list propagation. Because exceptional control flow is ignored in this simplified model, the only sets needed are the normal entry and normal exit sets per block. The gen set for a block is the set of locals loaded before any store to that local within the block; the kill set is the set of locals stored before any load of that local within the block. The analysis terminates when all entry sets reach a fixed point.
*/

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <cstdint>

// A simplified bytecode instruction: stores or loads a local variable.
enum class Op { LOAD, STORE };

struct Instruction {
    Op op;
    int local; // index of the local variable
};

struct BasicBlock {
    int start;              // index of first instruction in the block
    int end;                // index one past the last instruction (exclusive)
    std::vector<int> successors; // indices of successor blocks
};

// Result: for each block index, a set of live-on-entry local indices.
using LivenessResult = std::vector<std::unordered_set<int>>;

// Performs backward liveness analysis on the given method.
// instructions: the full list of instructions.
// blocks: list of basic blocks, each defined by [start, end) interval.
// num_locals: total number of local variables (0 .. num_locals-1).
// Returns a vector with one unordered_set per block.
LivenessResult compute_liveness(
    const std::vector<Instruction>& instructions,
    const std::vector<BasicBlock>& blocks,
    int num_locals);

#include <algorithm>
#include <cassert>

LivenessResult compute_liveness(
    const std::vector<Instruction>& instructions,
    const std::vector<BasicBlock>& blocks,
    int num_locals)
{
    int num_blocks = static_cast<int>(blocks.size());
    if (num_blocks == 0) return {};

    // 1. Compute predecessor lists for each block.
    std::vector<std::vector<int>> predecessors(num_blocks);
    for (int b = 0; b < num_blocks; ++b) {
        for (int succ : blocks[b].successors) {
            assert(succ >= 0 && succ < num_blocks);
            predecessors[succ].push_back(b);
        }
    }

    // 2. Compute gen and kill sets for each block.
    std::vector<std::unordered_set<int>> gen(num_blocks);
    std::vector<std::unordered_set<int>> kill(num_blocks);

    for (int b = 0; b < num_blocks; ++b) {
        auto& g = gen[b];
        auto& k = kill[b];
        for (int idx = blocks[b].start; idx < blocks[b].end; ++idx) {
            const Instruction& ins = instructions[idx];
            if (ins.op == Op::LOAD) {
                // Add to gen only if not in kill.
                if (k.find(ins.local) == k.end()) {
                    g.insert(ins.local);
                }
            } else { // STORE
                // Add to kill only if not in gen.
                if (g.find(ins.local) == g.end()) {
                    k.insert(ins.local);
                }
            }
        }
    }

    // 3. Initialize entry sets to empty.
    LivenessResult entry(num_blocks);
    // Work list: all blocks initially.
    std::queue<int> work;
    for (int b = 0; b < num_blocks; ++b) work.push(b);

    // Helper to check if the entry set changed and update it.
    auto update_entry = [&](int b, std::unordered_set<int>&& new_entry) -> bool {
        if (entry[b] == new_entry) return false;
        entry[b] = std::move(new_entry);
        return true;
    };

    while (!work.empty()) {
        int b = work.front();
        work.pop();

        // Compute exit set: union of entry sets of all successors.
        std::unordered_set<int> exit_set;
        for (int succ : blocks[b].successors) {
            for (int v : entry[succ]) exit_set.insert(v);
        }

        // new_entry = (exit_set - kill) union gen
        std::unordered_set<int> new_entry;
        for (int v : exit_set) {
            if (kill[b].find(v) == kill[b].end()) {
                new_entry.insert(v);
            }
        }
        for (int v : gen[b]) {
            new_entry.insert(v);
        }

        if (update_entry(b, std::move(new_entry))) {
            // Entry changed, push all predecessors.
            for (int pred : predecessors[b]) {
                work.push(pred);
            }
        }
    }

    return entry;
}

#include <cassert>
#include <vector>
#include <unordered_set>

// The solution function is defined above; include the implementation here.

int main() {
    // Simple straight-line block: load 0, store 1, load 0.
    {
        std::vector<Instruction> instructions = {
            {Op::LOAD, 0}, {Op::STORE, 1}, {Op::LOAD, 0}
        };
        std::vector<BasicBlock> blocks = {{0, 3, {}}}; // one block, no successors
        auto res = compute_liveness(instructions, blocks, 2);
        assert(res.size() == 1);
        std::unordered_set<int> expected = {0};
        assert(res[0] == expected);
    }

    // Two blocks: first loads a, second uses a after store of a.
    // Block0: LOAD 0, STORE 0. Block1: LOAD 0. Block1 is successor of block0.
    {
        std::vector<Instruction> instructions = {
            {Op::LOAD, 0}, {Op::STORE, 0}, {Op::LOAD, 0}
        };
        std::vector<BasicBlock> blocks = {{0, 2, {1}}, {2, 3, {}}};
        auto res = compute_liveness(instructions, blocks, 1);
        assert(res.size() == 2);
        // Block1: gen {0}, so entry = {0}.
        assert(res[1] == std::unordered_set<int>({0}));
        // Block0: exit = entry of block1 = {0}; then (exit - kill) = {0} - {0} = empty; plus gen = {0} (because LOAD before STORE) → {0}.
        assert(res[0] == std::unordered_set<int>({0}));
    }

    // Store before load in same block: STORE 0, LOAD 0. No exit.
    {
        std::vector<Instruction> instructions = {
            {Op::STORE, 0}, {Op::LOAD, 0}
        };
        std::vector<BasicBlock> blocks = {{0, 2, {}}};
        auto res = compute_liveness(instructions, blocks, 1);
        assert(res[0] == std::unordered_set<int>({0}));
    }

    // Kill prevents liveness from successor: block0: STORE 0; block1: LOAD 0.
    {
        std::vector<Instruction> instructions = {
            {Op::STORE, 0}, {Op::LOAD, 0}
        };
        std::vector<BasicBlock> blocks = {{0, 1, {1}}, {1, 2, {}}};
        auto res = compute_liveness(instructions, blocks, 1);
        // Block1 entry = {0}
        assert(res[1] == std::unordered_set<int>({0}));
        // Block0: exit = {0}; kill = {0}; gen = empty → new entry = empty.
        assert(res[0].empty());
    }

    // Loop: block0: LOAD 0; block1: STORE 0, then loop back to block0.
    {
        // Block0: index0: LOAD 0; block1: index1: STORE 0; successors: block0 for block1, block1 for block0.
        std::vector<Instruction> instructions = {
            {Op::LOAD, 0}, {Op::STORE, 0}
        };
        std::vector<BasicBlock> blocks = {{0, 1, {1}}, {1, 2, {0}}};
        auto res = compute_liveness(instructions, blocks, 1);
        // Block1: gen empty, kill {0}, exit = entry of block0.
        // Block0: gen {0}, kill empty, exit = entry of block1.
        // Solve: entry0 = {0} ∪ (entry1 - empty) = {0} ∪ entry1
        //        entry1 = (entry0 - {0}) ∪ empty = entry0 without 0
        // The only solution is entry0 = {0}, entry1 = empty.
        assert(res[0] == std::unordered_set<int>({0}));
        assert(res[1].empty());
    }

    // Multiple successors: block0 loads 0 and branches to block1 and block2.
    // block1 loads 1, block2 loads 2.
    {
        std::vector<Instruction> instructions = {
            {Op::LOAD, 0}, {Op::LOAD, 1}, {Op::LOAD, 2}
        };
        std::vector<BasicBlock> blocks = {
            {0, 1, {1,2}}, // block0: LOAD 0, successors block1 and block2
            {1, 2, {}},    // block1: LOAD 1
            {2, 3, {}}     // block2: LOAD 2
        };
        auto res = compute_liveness(instructions, blocks, 3);
        // Block1 entry = {1}; block2 entry = {2}.
        // Block0 exit = union = {1,2}; then (exit - kill) = {1,2} (kill empty) ∪ gen = {0} → {0,1,2}.
        assert(res[0] == std::unordered_set<int>({0,1,2}));
        assert(res[1] == std::unordered_set<int>({1}));
        assert(res[2] == std::unordered_set<int>({2}));
    }

    // No instructions at all (empty method)
    {
        std::vector<Instruction> instructions = {};
        std::vector<BasicBlock> blocks = {{0, 0, {}}}; // empty block
        auto res = compute_liveness(instructions, blocks, 2);
        assert(res.size() == 1);
        assert(res[0].empty());
    }

    return 0;
}

// The approach is a classic backward data-flow analysis. For each basic block, we first compute two bit sets: `gen` (variables that are read before being written inside the block) and `kill` (variables that are written before being read inside the block). To compute these, we scan the instructions of the block in order. For each instruction, if it is a `LOAD` of local `i`, we add `i` to `gen` only if `i` is not already in `kill`; if it is a `STORE` of local `i`, we add `i` to `kill` only if `i` is not already in `gen`. This ensures `gen` and `kill` are disjoint.
//
// Then we initialize every block’s entry set to the empty set. We put all blocks on a work list. For each block, we compute its exit set as the union of the entry sets of all its successors. The new entry set is then `(exit − kill) ∪ gen` (using set difference and union). If the new entry set differs from the old, we update it and push all predecessors of the block onto the work list (since their exit sets may change). The process repeats until no changes occur.
//
// Time complexity: Each block is processed at most `O(iterations)` times, where the number of iterations is bounded by the number of blocks times the number of local variables in the worst case (since each iteration can add at least one variable to some entry set). In practice it converges much faster. Space complexity is `O(blocks × locals)` for storing per-block sets.
//
// Edge cases: a block with no successors has an exit set of empty; a block that is its own predecessor (a loop) will converge because the union of its own entry with other successors is monotonic. Variables that are only stored and never loaded produce no liveness. Disjointness of gen and kill must be preserved during computation.
