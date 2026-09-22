// Write a C++ function `bool isGuaranteedToExecute(const std::vector<std::vector<int>>& adj, const std::vector<int>& exitBlocks, int header, int target)` that determines whether a given instruction at block `target` in a control-flow graph (CFG) representing a loop is guaranteed to execute at least once when the loop is entered, assuming the loop is single-header and has no nested loops. The CFG is represented as an adjacency list where each block has a list of successors. The function should return `true` if and only if every path from the loop header (block `header`) to any exit block (blocks listed in `exitBlocks`) or to the loop latch (the block that has a back edge to the header) that can be taken on the very first iteration must pass through block `target` (or `target` is the header itself). For simplicity, treat the loop as having no throws, and assume that any conditional branch may be taken on the first iteration, except those where the condition is a comparison of a loop-invariant PHI node's incoming value from the preheader against a constant and can be statically proven to be taken or not taken on the first iteration (you may ignore this optimization and conservatively assume all edges may be taken). Also, assume the header is block index 0 and that the first predecessor of each non-header block is its unique preheader predecessor outside the loop, but for this task you can ignore that detail and simply treat all predecessors as potentially reachable. The function must work for any loop where the CFG has a single entry (header) and at least one exit block.

#include <cassert>
#include <vector>

int main() {
    // Simple loop: header=0 -> block1 -> exit (2)
    // 0 -> {1}, 1 -> {0 (latch?), 2 (exit)} Actually let's design: header 0 -> 1, 1 -> 2 (exit) and 1 -> 0 (back edge)
    std::vector<std::vector<int>> adj1 = {{1}, {0, 2}, {}}; // block2 is exit, block1 is latch
    std::vector<int> exits1 = {2};
    assert(isGuaranteedToExecute(adj1, exits1, 0, 1) == true); // target=1: header->1, then must go to 1 before exit
    assert(isGuaranteedToExecute(adj1, exits1, 0, 0) == true); // header always executes
    assert(isGuaranteedToExecute(adj1, exits1, 0, 2) == false); // block2 is exit itself, can reach without going through target (since target=2, path header->1->2 avoids target? Actually target=2, we cannot go through target, but we start at header, and to reach 2 we must go through 1, not 2, so it's avoidable; but 2 is an exit, so we reach it without going through 2? That's silly. Better test: target in the middle.)

    // More realistic: header=0 -> 1 -> 2 (exit) and 1 -> 0 (back edge), target=1 => guaranteed.
    std::vector<std::vector<int>> adj2 = {{1}, {0, 2}, {}};
    std::vector<int> exits2 = {2};
    assert(isGuaranteedToExecute(adj2, exits2, 0, 1) == true);
    // target=2: then to exit at 2 itself, path header->1->2 avoids target=2 because we reach exit, but that's the target block itself; we can't avoid it because you have to be in it to be at it. Actually, if target=2, then we want to know if 2 always executes; but 2 is an exit, it might not execute if loop never exits. So false.
    assert(isGuaranteedToExecute(adj2, exits2, 0, 2) == false);

    // Loop with branch: header=0 -> 1 and 0 -> 2 (exit), where 1 -> 0 (back edge). target=1 is not guaranteed because exit can be taken directly.
    std::vector<std::vector<int>> adj3 = {{1, 2}, {0}, {}};
    std::vector<int> exits3 = {2};
    assert(isGuaranteedToExecute(adj3, exits3, 0, 1) == false); // path 0->2 avoids 1
    assert(isGuaranteedToExecute(adj3, exits3, 0, 2) == false); // exit block itself not guaranteed? Actually, to exit, you must go to 2, so every path to exit goes through 2; but what about latch? Latch is 1, and to iterate you go 0->1->0, avoiding 2. So 2 not guaranteed.
    // Check target=0: true.

    // Multiple exits: header=0 -> 1, 1 -> 3(exit1) and 1 -> 4(exit2), and 1->0 (back). target=1 guaranteed.
    std::vector<std::vector<int>> adj4 = {{1}, {0,3,4}, {}, {}, {}};
    std::vector<int> exits4 = {3,4};
    assert(isGuaranteedToExecute(adj4, exits4, 0, 1) == true);

    // Nested path: header=0 -> 1 -> 2 -> 3 (exit), with back edge from 1 to 0? Actually need latch. Let's do 0->1, 1->2, 2->3(exit) and 2->1? That's not a valid loop (two backedges). Simpler: 0->1, 1->2, 2->0 (back) and 2->3(exit). target=1: header->1 must pass through 1 before anything else? Actually to reach latch (2) or exit (3), you go 0->1->..., so yes.
    std::vector<std::vector<int>> adj5 = {{1}, {2}, {0,3}, {}};
    std::vector<int> exits5 = {3};
    assert(isGuaranteedToExecute(adj5, exits5, 0, 1) == true);
    assert(isGuaranteedToExecute(adj5, exits5, 0, 2) == true); // all paths to exit/latch go through 2? Actually path 0->1->2->0 (latch) must go through 2, and to exit also through 2. Yes.
    // target=3 (exit block): path 0->1->2->3 goes through 3 to exit, but latch path avoids 3. So not guaranteed.

    // All tests pass if no assertion fails.
    return 0;
}

#include <vector>
#include <queue>
#include <unordered_set>

// adj: adjacency list of the CFG (indices are block IDs).
// exitBlocks: list of block IDs that are loop exit blocks (not contained in loop).
// header: the loop header block ID (assumed to be reachable from itself).
// target: the block ID of the instruction we are checking.
// Returns true if every path from header to any exit block or to the latch (block that has an edge back to header) must pass through target.
bool isGuaranteedToExecute(const std::vector<std::vector<int>>& adj,
                           const std::vector<int>& exitBlocks,
                           int header,
                           int target) {
    // If target is the header, it always executes first.
    if (target == header) return true;

    // Build a set of exit blocks for quick lookup.
    std::unordered_set<int> exitSet(exitBlocks.begin(), exitBlocks.end());

    // Determine the latch: a block that has a successor equal to header.
    // For simplicity, assume there is at most one such block in the loop.
    int latch = -1;
    for (size_t i = 0; i < adj.size(); ++i) {
        for (int succ : adj[i]) {
            if (succ == header) {
                latch = static_cast<int>(i);
                break;
            }
        }
        if (latch != -1) break;
    }

    // If no latch found (malformed loop), conservatively return false.
    if (latch == -1) return false;

    // Perform BFS from header, avoiding the target block.
    // We want to see if we can reach any exit block or the latch without hitting target.
    std::vector<bool> visited(adj.size(), false);
    std::queue<int> q;
    q.push(header);
    visited[header] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        // If we reached an exit block or the latch via a path that avoided target, then not guaranteed.
        if (exitSet.count(cur) || cur == latch) {
            return false;
        }

        for (int next : adj[cur]) {
            if (next == target) continue; // do not go through target
            if (!visited[next]) {
                visited[next] = true;
                q.push(next);
            }
        }
    }

    // If we exhaust all reachable blocks without hitting any exit or latch (avoiding target),
    // then all paths must go through target. But note: there might be paths that go through target
    // and then to exits/latch; those are fine. Since we avoided target, if we cannot reach exit/latch,
    // then any path to exit/latch must go through target.
    return true;
}

// The solution requires determining whether every path from the header to any exit or to the latch (the block that has an edge back to the header) must pass through the target block. Since we ignore convergence on first-iteration conditions (conservatively assume all branches may be taken), we can simplify: the target is guaranteed to execute if and only if the target dominates all exit blocks and the latch. In a CFG with a single entry, dominance can be computed by a simple reachability analysis: a block `D` dominates a block `B` if every path from the header to `B` goes through `D`. For a loop with one header, we can compute the set of blocks that are reachable from the header without passing through the target. If any exit block or the latch is reachable without passing through the target, then there exists a path that avoids the target, so it is not guaranteed. Otherwise, every path to any exit or latch must pass through the target, so it is guaranteed. Additionally, if the target is the header itself, it is trivially guaranteed. Edge cases: if there are no exit blocks (infinite loop), then the only way to leave the loop is via the latch's back edge? Actually, in a loop, the latch is the block that jumps back to the header; if there are no exits, then the loop never terminates, but the instruction in any block could be skipped only if there is a path that avoids it and still reaches the latch? If the loop is infinite, "guaranteed to execute" makes sense only if every path from header either reaches the target or stays in the loop forever without executing the target? Since the CFG is finite, every infinite path must eventually repeat a block, but it might avoid the target forever. However, for simplicity, we can still use the same reachability approach: if there is any path from header to an exit or to the latch that avoids the target, then not guaranteed. If there is no such path (i.e., all paths must go through target to reach any exit or the latch), then it is guaranteed, because to exit the loop or to iterate, the target must be visited. Complexity: O(V+E) time for a BFS/DFS, and O(V) space for visited set.
