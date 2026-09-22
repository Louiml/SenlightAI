/*
Given a control-flow graph (CFG) represented as a set of basic blocks with predecessor/successor relationships, write a C++ function that implements the core "marker" algorithm from the provided MemorySSA snippet. Specifically, the function must compute, for every basic block, its "previous memory definition" by recursively walking the CFG: if a block has a unique predecessor, the result comes directly from that predecessor; if a block is revisited during the recursion (indicating a cycle), a "phi" placeholder is inserted; if a block has multiple reachable predecessors, the function collects all incoming definitions from those predecessors and checks whether they are all identical—if so, the single value is returned without creating a phi, otherwise a phi node is created to merge them. Your implementation should mimic the behavior of `getPreviousDefRecursive` using a visited set and a cache, and should handle unreachable blocks by returning a sentinel value. You may represent blocks as integer IDs and definitions as integer IDs where `-1` represents the "live-on-entry" sentinel.
*/
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cassert>

// Simplified MemorySSA previous-definition computation.
// Blocks are integers, definitions are integers, -1 is live-on-entry sentinel.
// Returns a map from each block to its previous definition ID.
std::unordered_map<int, int> computePreviousDefs(
    int entryBlock,
    const std::unordered_map<int, std::vector<int>>& predecessors,
    const std::unordered_set<int>& reachableBlocks) {

    // Cache for memoization.
    std::unordered_map<int, int> cache;

    // Visited set for cycle detection during recursion.
    std::unordered_set<int> visited;

    // Counter for generating phi placeholder IDs (negative for clarity).
    int phiCounter = -2;

    // Recursive helper function (captures by reference).
    std::function<int(int)> getPrevRec = [&](int block) -> int {
        // Cache lookup.
        auto it = cache.find(block);
        if (it != cache.end()) return it->second;

        // Unreachable block => sentinel.
        if (reachableBlocks.find(block) == reachableBlocks.end()) {
            cache[block] = -1;
            return -1;
        }

        // Find unique predecessor case.
        auto predIt = predecessors.find(block);
        if (predIt != predecessors.end() && predIt->second.size() == 1) {
            int pred = predIt->second[0];
            // Recurse on the single predecessor.
            int result = getPrevRec(pred);
            cache[block] = result;
            return result;
        }

        // Check for cycle: if we are already on the recursion stack.
        if (visited.find(block) != visited.end()) {
            // Create a phi placeholder to break the cycle.
            int phi = phiCounter--;
            cache[block] = phi;
            return phi;
        }

        // Mark visited, recurse on all predecessors.
        visited.insert(block);
        std::vector<int> incoming;
        bool uniqueIncoming = true;
        int singleAccess = -1;
        if (predIt != predecessors.end()) {
            for (int pred : predIt->second) {
                int val;
                if (reachableBlocks.find(pred) != reachableBlocks.end()) {
                    val = getPrevRec(pred);
                } else {
                    val = -1; // unreachable pred => sentinel
                }
                if (singleAccess == -1) {
                    singleAccess = val;
                } else if (val != singleAccess) {
                    uniqueIncoming = false;
                }
                incoming.push_back(val);
            }
        }
        // If no predecessors (shouldn't happen for reachable non-entry), use sentinel.
        if (incoming.empty()) {
            singleAccess = -1;
            uniqueIncoming = true;
        }

        int result;
        if (uniqueIncoming && singleAccess != -1) {
            // All incoming values identical, no phi needed.
            result = singleAccess;
        } else {
            // Need a phi node.
            result = phiCounter--;
        }

        visited.erase(block);
        cache[block] = result;
        return result;
    };

    // Compute for all reachable blocks (so cache is fully populated).
    std::unordered_map<int, int> resultMap;
    for (int b : reachableBlocks) {
        // Call for each block to fill cache, but we can just call once per block.
        // However, to keep it simple, we call for each reachable block, but the
        // recursion will fill the cache for all predecessors anyway.
        // For correctness, we call for the entry and propagate.
        getPrevRec(b);
    }

    // Extract results from cache.
    for (auto& kv : cache) {
        resultMap[kv.first] = kv.second;
    }
    return resultMap;
}
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

// (Assume computePreviousDefs is defined above.)

int main() {
    // Test 1: Simple chain: 0 -> 1 -> 2. All reachable.
    {
        std::unordered_map<int, std::vector<int>> preds = {
            {0, {}}, {1, {0}}, {2, {1}}
        };
        std::unordered_set<int> reachable = {0, 1, 2};
        auto result = computePreviousDefs(0, preds, reachable);
        // Block 0 has no preds -> sentinel -1.
        assert(result[0] == -1);
        // Block 1 single pred 0 -> result[0] = -1.
        assert(result[1] == -1);
        // Block 2 single pred 1 -> result[1] = -1.
        assert(result[2] == -1);
    }

    // Test 2: Diamond: entry 0 branches to 1 and 2, both join at 3.
    {
        std::unordered_map<int, std::vector<int>> preds = {
            {0, {}}, {1, {0}}, {2, {0}}, {3, {1, 2}}
        };
        std::unordered_set<int> reachable = {0, 1, 2, 3};
        auto result = computePreviousDefs(0, preds, reachable);
        assert(result[0] == -1);
        assert(result[1] == -1);
        assert(result[2] == -1);
        // Block 3 has two identical -1 values => no phi, result = -1.
        assert(result[3] == -1);
    }

    // Test 3: Diamond with different definitions: 0 -> 1 and 0 -> 2, but we
    // simulate that block 1 has a "store" by adding a custom definition. To do
    // this in our simplified model, we need to trick the algorithm. We can add
    // a new block 4 between 0 and 1 that forces a phi. Actually, easier: make
    // the predecessors of 3 be 1 and 2, where 1 and 2 have different previous
    // defs due to different structure. Since both 1 and 2 get -1, they are same.
    // To get different values, we can create a self-loop on block 1, so it gets
    // a phi placeholder. Then 3 will see a phi value and -1, forcing a phi.
    {
        std::unordered_map<int, std::vector<int>> preds = {
            {0, {}}, {1, {0, 1}}, {2, {0}}, {3, {1, 2}}
        };
        std::unordered_set<int> reachable = {0, 1, 2, 3};
        auto result = computePreviousDefs(0, preds, reachable);
        assert(result[0] == -1);
        // Block 1 has preds 0 and 1, 1 is cycle => result[1] is a phi placeholder (< -1).
        assert(result[1] < -1);
        assert(result[2] == -1);
        // Block 3 has different values (phi and -1), so it must be a phi.
        assert(result[3] < -1);
        // The two phis must be different placeholders.
        assert(result[1] != result[3]);
    }

    // Test 4: Unreachable block.
    {
        std::unordered_map<int, std::vector<int>> preds = {
            {0, {}}, {1, {0}}, {2, {1}} // block 2 is unreachable? Actually all reachable if 0 entry.
        };
        std::unordered_set<int> reachable = {0, 1}; // 2 not reachable.
        auto result = computePreviousDefs(0, preds, reachable);
        // Only reachable blocks computed; 2 not in result map because we only call for reachable.
        // But our function computes for reachable only, so 2 won't appear.
        assert(result.find(2) == result.end());
        assert(result[0] == -1);
        assert(result[1] == -1);
    }

    // Test 5: Cycle not reachable from entry? Only compute reachable.
    {
        std::unordered_map<int, std::vector<int>> preds = {
            {0, {}}, {1, {1}} // self-loop, but 1 unreachable from 0.
        };
        std::unordered_set<int> reachable = {0};
        auto result = computePreviousDefs(0, preds, reachable);
        assert(result.size() == 1);
        assert(result[0] == -1);
    }

    // Test 6: More complex: 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3, 3 -> 4 (unique pred).
    {
        std::unordered_map<int, std::vector<int>> preds = {
            {0, {}}, {1, {0}}, {2, {0}}, {3, {1, 2}}, {4, {3}}
        };
        std::unordered_set<int> reachable = {0, 1, 2, 3, 4};
        auto result = computePreviousDefs(0, preds, reachable);
        assert(result[0] == -1);
        assert(result[1] == -1);
        assert(result[2] == -1);
        // 3 has two identical -1 => no phi.
        assert(result[3] == -1);
        // 4 has unique pred 3 => result[3] = -1.
        assert(result[4] == -1);
    }

    return 0;
}
// The problem is a simplified version of the SSA construction algorithm for memory accesses. The main approach is a recursive traversal of the CFG with memoization. For each block:
// - First, check a cache (`std::unordered_map`) to avoid recomputation and prevent exponential blowup on DAG-like structures (e.g., nested if-statements).
// - If the block is unreachable from the entry (we maintain a reachability set), return the sentinel `-1`.
// - If the block has exactly one predecessor, recursively compute the definition from that predecessor and cache the result (this matches the single-predecessor fast path).
// - If the block is already in the current recursion's visited set, we encountered a cycle: return a new "phi" marker (we can use a unique negative integer like `-2` for simplicity, or a counter) to break the cycle.
// - Otherwise, mark the block as visited, collect definitions from all reachable predecessors (unreachable ones contribute the sentinel `-1`), and attempt simplification: if all collected definitions are identical (and non-sentinel), use that single value; otherwise create a phi node (represented by a unique integer) and return it. After processing, unmark the block from the visited set and cache the result.
//
// Edge cases: blocks with no predecessors (entry) should have the sentinel; blocks with only unreachable predecessors should return sentinel; cycles of arbitrary length should be handled; multiple edges between same block pair should not affect correctness if we iterate over a deduplicated predecessor list (we can assume the input provides unique predecessor lists). Time complexity: with memoization, each block is processed at most once per recursion path, but in the worst case (irreducible CFG) it could be exponential without the cache; with cache, it is linear in the number of blocks plus the number of edges, because each block's result is computed once. Space complexity is O(B) for cache and visited sets, plus recursion depth O(B).
//
// We'll implement a self-contained function `computePreviousDefs` that takes an entry block ID, a map from block ID to a list of predecessor IDs, and a set of reachable block IDs, and returns a map from block ID to the computed definition ID. We'll use a mutable counter for phi creation. The solution will be a free function with `const` correctness where appropriate.
