Write a C++ function that, given a control-flow graph represented by a list of basic blocks (each with a unique integer ID, a list of successor IDs, and a list of predcessor IDs), determines for a specified source block ID whether it is possible to sink a particular instruction (represented abstractly by a boolean flag indicating whether it may write to memory, and a set of block IDs where its uses reside) into at least one of its immediate successors without violating safety rules modeled after the LLVM sinking pass. Specifically, the function must return `true` if there exists an immediate successor `S` of the source block such that all of the following hold: (1) `S` is not the source block itself; (2) `S` does not end in an exceptional terminator (assume no block ends exceptionally, so this always passes); (3) either `S` has the source block as its unique predecessor, or (if not) the source block dominates `S` (in the dominator tree of the graph) and the instruction is safe to speculatively execute (we assume it is safe if it does not may-write); (4) every block that contains a use of the instruction is dominated by `S`. The dominator tree must be computed from the provided graph (with block 0 as the entry). If no such successor exists, return `false`.
#include <cassert>
#include <vector>

// Include the solution code here or link to it.
// For brevity, we assume the function is defined above.

int main() {
    // Graph: 0 -> 1, 2 ; 1 -> 3 ; 2 -> 3 ; 3 -> nothing
    // Entry = 0. Dominators: 0 dominates all; 1 dominates 3; 2 dominates 3.
    std::vector<BasicBlockInfo> blocks(4);
    blocks[0] = {0, {1,2}, {}};
    blocks[1] = {1, {3}, {0}};
    blocks[2] = {2, {3}, {0}};
    blocks[3] = {3, {}, {1,2}};

    // Sink from 0 to 1: uses in 3, 1 dominates 3? Yes. unique pred? 1 has pred 0 only -> yes.
    assert(canSinkInstruction(blocks, 0, false, {3}) == true);
    // Sink from 0 to 2: similarly true.
    assert(canSinkInstruction(blocks, 0, false, {3}) == true);
    // If uses in 1, sinking to 2: 2 does not dominate 1 -> false.
    assert(canSinkInstruction(blocks, 0, false, {1}) == false);
    // If mayWrite and it's a critical edge (1 and 2 both have pred 0? 1 has unique pred 0, but 2 also unique pred 0. So they are not critical edges.)
    // Here every successor of 0 has unique predecessor 0, so mayWrite is fine.
    assert(canSinkInstruction(blocks, 0, true, {3}) == true);
    // If uses in both 1 and 3, sinking to 2 fails because 2 does not dominate 1.
    assert(canSinkInstruction(blocks, 0, false, {1,3}) == false);

    // More complex: add a critical edge. Graph: 0 -> 1, 2 ; 1 -> 3 ; 2 -> 3 ; 3 -> 4 ; 1 -> 4 ; 4 -> nothing
    // Now 4 has two preds: 1 and 3. Sinking from 3 to 4 is a critical edge from 3 (3->4) because 4 has two preds.
    std::vector<BasicBlockInfo> blocks2(5);
    blocks2[0] = {0, {1,2}, {}};
    blocks2[1] = {1, {3,4}, {0}};
    blocks2[2] = {2, {3}, {0}};
    blocks2[3] = {3, {4}, {1,2}};
    blocks2[4] = {4, {}, {1,3}};
    // Sink from 3 to 4: mayWrite=false, 3 dominates 4? 3 does not dominate 4 (4's idom is 1). So cannot sink.
    assert(canSinkInstruction(blocks2, 3, false, {4}) == false);
    // Sink from 1 to 4: 1 has 4 as successor, 4 has preds 1 and 3, so critical edge. 1 dominates 4? yes. mayWrite=false -> true.
    assert(canSinkInstruction(blocks2, 1, false, {4}) == true);
    // If mayWrite=true, cannot sink across critical edge from 1 to 4.
    assert(canSinkInstruction(blocks2, 1, true, {4}) == false);

    // Self-loop check: graph 0 -> 0,1.
    std::vector<BasicBlockInfo> blocks3(2);
    blocks3[0] = {0, {0,1}, {0}};
    blocks3[1] = {1, {}, {0}};
    // Sink from 0 to 0 is disallowed.
    assert(canSinkInstruction(blocks3, 0, false, {1}) == true); // sink to 1 works.
    assert(canSinkInstruction(blocks3, 0, false, {0}) == false); // cannot sink to itself.
}
#include <vector>
#include <unordered_set>
#include <queue>
#include <algorithm>

struct BasicBlockInfo {
    int id;
    std::vector<int> successors;
    std::vector<int> predecessors;
};

// Compute immediate dominators for a graph with n blocks, assuming block 0 is entry.
// Returns a vector `idom` where idom[i] is the immediate dominator of block i,
// and idom[0] = 0 (self).
std::vector<int> computeImmediateDominators(
        const std::vector<BasicBlockInfo>& blocks, int n) {
    // Initialize: entry dominates itself; others undefined (use n as placeholder).
    std::vector<int> idom(n, -1);
    idom[0] = 0;

    bool changed = true;
    while (changed) {
        changed = false;
        // Process blocks in BFS order from entry.
        std::vector<int> order;
        std::queue<int> q;
        std::vector<bool> visited(n, false);
        visited[0] = true;
        q.push(0);
        while (!q.empty()) {
            int v = q.front(); q.pop();
            order.push_back(v);
            for (int s : blocks[v].successors) {
                if (!visited[s]) {
                    visited[s] = true;
                    q.push(s);
                }
            }
        }
        // For each block except entry, recompute idom as intersection of idoms of its predecessors.
        for (int v : order) {
            if (v == 0) continue;
            int new_idom = -1;
            for (int p : blocks[v].predecessors) {
                if (idom[p] == -1) continue; // predecessor not yet processed
                if (new_idom == -1) {
                    new_idom = p;
                } else {
                    // Intersect p and new_idom by walking up dominator chains.
                    int a = p, b = new_idom;
                    while (a != b) {
                        while (a != 0 && a != b && !isAncestor(a, b, idom)) a = idom[a];
                        while (b != 0 && b != a && !isAncestor(b, a, idom)) b = idom[b];
                    }
                    new_idom = a;
                }
            }
            // For simplicity in a small graph, we use a simpler intersection algorithm:
            // Walk up from p until we hit a node dominated by new_idom.
            // To avoid complexity, we use a simpler approach:
            // Recompute using a known simple technique: intersect by walking up from both.
            // Because the graph is small, we can do a simpler O(n^2) method:
            // Compute the set of dominators for each block using bitmasks.
            // We'll override with a robust method below.
        }
    }

    // Simpler and correct for small graphs: use bitmask-based dominator computation.
    // Let dom[i] be a bitmask of blocks that dominate i.
    std::vector<unsigned long long> dom(n, 0);
    dom[0] = 1ULL << 0;
    bool changed2 = true;
    while (changed2) {
        changed2 = false;
        for (int v = 0; v < n; ++v) {
            if (v == 0) continue;
            unsigned long long new_dom = ~0ULL;
            for (int p : blocks[v].predecessors) {
                new_dom &= dom[p];
            }
            new_dom |= (1ULL << v);
            if (new_dom != dom[v]) {
                dom[v] = new_dom;
                changed2 = true;
            }
        }
    }
    // Compute immediate dominator: the unique dominator that is dominated by all other dominators except itself.
    for (int v = 1; v < n; ++v) {
        unsigned long long preds_dom = ~0ULL;
        for (int p : blocks[v].predecessors) {
            preds_dom &= dom[p];
        }
        // idom[v] is the dominator in preds_dom that is closest to v (i.e., the one that dominates all others in preds_dom).
        int best = -1;
        for (int d = 0; d < n; ++d) {
            if (d == v) continue;
            if ((dom[v] & (1ULL << d)) == 0) continue; // d dominates v
            if ((preds_dom & (1ULL << d)) == 0) continue; // d dominates all preds
            // d is a candidate; check if it dominates all other candidates
            bool dominates_all = true;
            for (int d2 = 0; d2 < n; ++d2) {
                if (d2 == v || d2 == d) continue;
                if ((dom[v] & (1ULL << d2)) == 0) continue;
                if ((preds_dom & (1ULL << d2)) == 0) continue;
                if ((dom[d2] & (1ULL << d)) == 0) { // d does not dominate d2
                    dominates_all = false;
                    break;
                }
            }
            if (dominates_all) {
                best = d;
                break;
            }
        }
        idom[v] = best;
    }
    return idom;
}

// Check if block a dominates block b given idom array.
bool dominates(int a, int b, const std::vector<int>& idom) {
    int cur = b;
    while (cur != -1) {
        if (cur == a) return true;
        if (cur == 0) break;
        cur = idom[cur];
    }
    return false;
}

// Main solution function.
bool canSinkInstruction(
        const std::vector<BasicBlockInfo>& blocks,
        int sourceBlock,
        bool instructionMayWrite,
        const std::vector<int>& useBlocks) {
    int n = static_cast<int>(blocks.size());
    if (sourceBlock < 0 || sourceBlock >= n) return false;

    // Build idom.
    std::vector<int> idom = computeImmediateDominators(blocks, n);
    if (idom[sourceBlock] == -1) {
        // source not reachable from entry? treat as cannot sink.
        return false;
    }

    const auto& source = blocks[sourceBlock];
    for (int succ : source.successors) {
        if (succ < 0 || succ >= n) continue;
        if (succ == sourceBlock) continue; // cannot sink into itself

        // Condition: acceptable target.
        // Check critical edge condition.
        bool hasUniquePred = (blocks[succ].predecessors.size() == 1 &&
                              blocks[succ].predecessors[0] == sourceBlock);
        if (!hasUniquePred) {
            // Must be safe to speculate and source must dominate succ.
            if (instructionMayWrite) continue;
            if (!dominates(sourceBlock, succ, idom)) continue;
        }

        // All uses must be dominated by succ.
        bool allDominated = true;
        for (int useBlock : useBlocks) {
            if (useBlock < 0 || useBlock >= n) { allDominated = false; break; }
            if (!dominates(succ, useBlock, idom)) {
                allDominated = false;
                break;
            }
        }
        if (allDominated) {
            return true;
        }
    }
    return false;
}
// The solution requires building a dominator tree from the given CFG. A standard algorithm is to compute immediate dominators using an iterative data-flow approach (the classic Lengauer-Tarjan algorithm is overkill for a self-contained task; a simpler `O(n^2)` or `O(n * e)` iterative method suffices for small graphs). After computing immediate dominators, build the children lists to represent the dominator tree. For each candidate successor `S` of the source block, first check `S == source` (skip). Then compute the set of blocks dominated by `S`; this can be done by walking the dominator tree from `S` downward (i.e., all descendants of `S` in the tree are dominated by `S`). For the "critical edge" condition: if `S` has the source block as its unique predecessor, then condition (3) automatically holds (no need to check dominance or speculativity). If not, then we must check that the source block dominates `S` (which is equivalent to `S` being in the subtree of the source in the dominator tree), and that the instruction is safe to speculate (i.e., `!mayWrite`). Finally, for every use block ID, check that it is in the set of blocks dominated by `S` (i.e., `S` dominates the use block). The algorithm is straightforward: for each successor (at most `|succs|`), we do a dominator-tree traversal to collect dominated blocks (O(n)) and then check each use (O(u)) — total O(|succs| * (n + u)) per query, and building dominators is O(n * e) or O(n^2) with the iterative method. Space complexity is O(n + e + n) for the graph, dominator tree, and visited sets.
