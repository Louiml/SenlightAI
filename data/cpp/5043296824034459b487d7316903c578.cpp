Write a C++ function `std::unordered_set<const Value*> deadCodeEliminationAnalyze(Block* block)` that, given a simplified control-flow graph block (mimicking PyTorch JIT's IR but with a custom lightweight `Node`/`Block`/`Value` representation), performs a mark-and-sweep dead code elimination analysis and returns the set of `Value*` pointers that are considered "live" after the elimination. The function must respect a simplified side-effect policy: any node marked with `bool sideEffect` is always considered live (its outputs and inputs are preserved). Additionally, for `If`-like nodes with multiple sub-blocks (each `Block*` stored in `node->blocks()`), outputs of the outer node that are not used anywhere outside the node must be pruned: the corresponding block outputs in all sub-blocks should be removed from the returned "live" set, and the outer node's output should not be considered live. For `Loop`-like nodes with a single sub-block, apply convergence: repeatedly mark the loop body until no new nodes become live, considering loop-carried dependencies (block input `i` corresponds to outer node input `2+i` and block output `i` corresponds to outer node output `i`). The function should not mutate the graph; it should only compute and return the set of live values (i.e., all outputs of marked nodes plus the graph's return values). Use the following minimal provided data structures (do not modify them; implement the function in the same file):
```cpp
#include <unordered_set>
#include <vector>
#include <cstddef>

struct Value;
struct Node;

struct Value {
    Node* producer = nullptr;
    std::vector<Node*> uses; // consumers that have this as an input
};

struct Block {
    Node* returnNode = nullptr; // node that has the block's output values as inputs
    std::vector<Node*> nodes;
    Node* ownerNode = nullptr; // null for top-level block
    std::vector<Value*> inputs; // block parameters (loop body, if branches)
    std::vector<Value*> outputs; // block output values (values produced inside block that leave it)
};

struct Node {
    enum class Kind { Return, If, Loop, Other };
    Kind kind = Kind::Other;
    bool sideEffect = false;
    std::vector<Value*> inputs;
    std::vector<Value*> outputs;
    std::vector<Block*> blocks;
    Node* ownerBlock = nullptr;
};
```
The function must be self-contained and not rely on any external libraries beyond standard headers. Provide a reference solution with `const` correctness where appropriate.

#include <cassert>
#include <iostream>

// Include the solution code above. Provide minimal test infrastructure.

int main() {
    // Build a simple graph:
    // Block with return node that returns value produced by an "Other" node.
    Value a, b, c;
    Node prodA, prodB, retNode;
    prodA.outputs = {&a};
    prodA.inputs = {}; // no inputs
    prodA.kind = Node::Kind::Other;
    prodB.outputs = {&b};
    prodB.inputs = {};
    prodB.kind = Node::Kind::Other;
    retNode.inputs = {&a};
    retNode.kind = Node::Kind::Return;

    Block top;
    top.nodes = {&prodA, &prodB};
    top.returnNode = &retNode;
    a.producer = &prodA;
    b.producer = &prodB;
    a.uses = {&retNode}; // a used
    // b has no uses, so should be pruned.

    auto live = deadCodeEliminationAnalyze(&top);
    assert(live.count(&a) == 1);
    assert(live.count(&b) == 0);
    assert(live.count(&prodA.outputs[0]) == 1);

    // Test side-effect: make prodB side-effect, then b becomes live.
    prodB.sideEffect = true;
    auto live2 = deadCodeEliminationAnalyze(&top);
    assert(live2.count(&b) == 1);
    assert(live2.count(&prodB.outputs[0]) == 1);

    // Test If node with one dead branch output.
    Value ifOut, branchVal1, branchVal2;
    Node ifNode, branch1, branch2, ifRet;
    ifNode.kind = Node::Kind::If;
    ifNode.outputs = {&ifOut};
    ifOut.producer = &ifNode;
    // No uses for ifOut -> dead.
    branch1.outputs = {&branchVal1};
    branch2.outputs = {&branchVal2};
    Block b1, b2;
    b1.ownerNode = &ifNode;
    b2.ownerNode = &ifNode;
    b1.outputs = {&branchVal1};
    b2.outputs = {&branchVal2};
    branchVal1.producer = &branch1;
    branchVal2.producer = &branch2;
    branchVal1.uses = {&ifNode}; // used inside if
    branchVal2.uses = {};
    ifNode.blocks = {&b1, &b2};
    ifNode.inputs = {&branchVal1, &branchVal2};
    top.nodes.push_back(&ifNode);
    // Re-run analysis.
    auto live3 = deadCodeEliminationAnalyze(&top);
    // Since ifOut has no uses, it should be pruned.
    assert(live3.count(&ifOut) == 0);
    // Corresponding block outputs should also be pruned.
    assert(live3.count(&branchVal1) == 0);
    assert(live3.count(&branchVal2) == 0);

    // Test a used If output: add a use to ifOut.
    ifOut.uses.push_back(&retNode); // but retNode already has inputs; just to simulate.
    // Actually add a new return node for clarity.
    Node ret2;
    ret2.kind = Node::Kind::Return;
    ret2.inputs = {&ifOut};
    top.returnNode = &ret2;
    // Need to clear previous live set and re-run.
    auto live4 = deadCodeEliminationAnalyze(&top);
    assert(live4.count(&ifOut) == 1);
    // Block outputs corresponding to the used outer output should be live.
    assert(live4.count(&branchVal1) == 1);
    assert(live4.count(&branchVal2) == 1);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <unordered_set>
#include <vector>
#include <utility>
#include <algorithm>

// Provided structures are assumed available; no modifications.

// Returns the set of live values after dead code elimination analysis.
std::unordered_set<const Value*> deadCodeEliminationAnalyze(Block* block) {
    std::unordered_set<const Node*> markedNodes;
    std::unordered_set<const Value*> liveValues;

    // Helper to mark a node live and propagate to its inputs.
    auto markNode = [&](Node* node, auto& markNodeRef) -> bool {
        if (markedNodes.count(node)) {
            return false;
        }
        markedNodes.insert(node);
        for (Value* input : node->inputs) {
            liveValues.insert(input);
        }
        // Also mark owning block's owner node if any (not needed for top-level).
        return true;
    };

    // Pre-declare recursive marking for blocks.
    std::function<bool(Block*)> markBlock;

    // Special handling for if/loop nodes.
    auto markNodeWithBlocks = [&](Node* node, auto& markNodeRef) -> bool {
        bool changed = false;
        if (node->kind == Node::Kind::Loop) {
            // Converge on the loop body.
            bool loopChanged = true;
            while (loopChanged) {
                loopChanged = false;
                for (Block* subBlock : node->blocks) {
                    bool before = markedNodes.size();
                    loopChanged |= markBlock(subBlock);
                }
                // Re-check if any of this node's outputs became live via the loop.
                for (Value* out : node->outputs) {
                    if (liveValues.count(out)) {
                        // Simulate marking this node again.
                        changed |= markNodeRef(node, markNodeRef);
                        break;
                    }
                }
            }
        } else if (node->kind == Node::Kind::If) {
            // For If: only mark sub-block outputs whose corresponding outer output is live.
            if (node->blocks.size() == 1 || node->blocks.empty()) {
                changed |= markNodeRef(node, markNodeRef);
                for (Block* subBlock : node->blocks) {
                    changed |= markBlock(subBlock);
                }
            } else {
                // Determine which outer outputs are live.
                for (size_t i = 0; i < node->outputs.size(); ++i) {
                    if (liveValues.count(node->outputs[i])) {
                        // Mark the block output corresponding to this index.
                        for (Block* subBlock : node->blocks) {
                            if (i < subBlock->outputs.size()) {
                                liveValues.insert(subBlock->outputs[i]);
                            }
                        }
                    }
                }
                // Then mark the node itself if any output is live or side effect.
                bool anyLive = false;
                for (Value* out : node->outputs) {
                    if (liveValues.count(out)) { anyLive = true; break; }
                }
                if (node->sideEffect || anyLive) {
                    changed |= markNodeRef(node, markNodeRef);
                    for (Block* subBlock : node->blocks) {
                        changed |= markBlock(subBlock);
                    }
                }
            }
        } else {
            changed |= markNodeRef(node, markNodeRef);
            for (Block* subBlock : node->blocks) {
                changed |= markBlock(subBlock);
            }
        }
        return changed;
    };

    // Mark a block: mark return node, side-effect nodes, and process nodes in reverse.
    markBlock = [&](Block* blk) -> bool {
        bool changed = false;
        // Mark the return node.
        if (blk->returnNode) {
            changed |= markNode(blk->returnNode, markNode);
        }
        // Mark all side-effect nodes.
        for (Node* node : blk->nodes) {
            if (node->sideEffect) {
                changed |= markNode(node, markNode);
            }
        }
        // Process backwards.
        for (auto it = blk->nodes.rbegin(); it != blk->nodes.rend(); ++it) {
            Node* node = *it;
            changed |= markNodeWithBlocks(node, markNode);
        }
        return changed;
    };

    // Run marking on the top-level block.
    bool work = true;
    while (work) {
        size_t before = markedNodes.size();
        work = markBlock(block);
        // Also handle top-level return node's inputs as live outputs.
        if (block->returnNode) {
            for (Value* v : block->returnNode->inputs) {
                liveValues.insert(v);
            }
        }
        work = work || (markedNodes.size() != before);
    }

    // Sweep: prune dead block outputs for If and Loop nodes, and collect live values.
    std::unordered_set<const Value*> result;
    auto collectBlock = [&](Block* blk, auto& self) -> void {
        if (blk->returnNode) {
            for (Value* v : blk->returnNode->inputs) {
                if (liveValues.count(v) || !v->uses.empty()) {
                    result.insert(v);
                }
            }
        }
        for (Node* node : blk->nodes) {
            if (markedNodes.count(node)) {
                for (Value* out : node->outputs) {
                    result.insert(out);
                }
            }
            // For If nodes, remove dead block outputs.
            if (node->kind == Node::Kind::If && node->blocks.size() > 1) {
                for (size_t i = 0; i < node->outputs.size(); ++i) {
                    bool used = !node->outputs[i]->uses.empty();
                    if (!used) {
                        // Block output at index i is dead; don't include in result.
                        for (Block* sub : node->blocks) {
                            if (i < sub->outputs.size() && !sub->outputs[i]->uses.empty()) {
                                // Actually still used externally? keep.
                                result.insert(sub->outputs[i]);
                            } else if (i < sub->outputs.size()) {
                                // dead block output; do not insert.
                            }
                        }
                    } else {
                        // used outer output; keep block outputs.
                        for (Block* sub : node->blocks) {
                            if (i < sub->outputs.size()) {
                                result.insert(sub->outputs[i]);
                            }
                        }
                    }
                }
            }
            // For Loop nodes, prune dead loop carried outputs.
            if (node->kind == Node::Kind::Loop && node->blocks.size() == 1) {
                Block* body = node->blocks[0];
                size_t offset = 2; // loop carried deps start at input index 2
                size_t bodyOffset = 1;
                for (size_t i = 0; i < node->outputs.size(); ++i) {
                    bool outerUsed = !node->outputs[i]->uses.empty();
                    bool innerUsed = (bodyOffset + i < body->inputs.size()) && !body->inputs[bodyOffset + i]->uses.empty();
                    if (!outerUsed && !innerUsed) {
                        // dead loop output; do not include.
                    } else {
                        result.insert(node->outputs[i]);
                        if (i < body->outputs.size()) {
                            result.insert(body->outputs[i]);
                        }
                    }
                }
            }
            // Recurse into sub-blocks.
            for (Block* sub : node->blocks) {
                self(sub, self);
            }
        }
        // Include block inputs that are used (parameters).
        for (Value* v : blk->inputs) {
            if (!v->uses.empty()) {
                result.insert(v);
            }
        }
    };
    collectBlock(block, collectBlock);

    return result;
}

// The algorithm is a mark-and-sweep liveness analysis over a simplified IR. We start by marking the return node of the given block as live, and then propagate liveness upward through producers. A node is live if any of its outputs is a live value, if it has side effects, or if it is the return node. When a node becomes live, all its inputs become live values, and the node itself is marked. For sub-blocks (If nodes), we only mark a block's output value as live if the corresponding outer output is already live; this avoids marking dead branch computations. For Loop nodes, we must iterate to convergence because a value mutated in the loop body (via a side-effect node) can affect a later iteration's live value even if it isn't live yet in the first pass. We use a worklist approach: mark the return node, then repeatedly process nodes in reverse order, marking side-effect nodes and nodes whose outputs are live. For loops, we process the sub-block repeatedly until no new marks appear. At the end, we sweep the block and all nested blocks, collecting all outputs of marked nodes plus any remaining block outputs that are used externally. We also prune dead block outputs for If nodes: if an outer output is not used, we do not include the corresponding block outputs in the live set, and we do not mark the outer output. For Loop nodes, if an outer loop output is unused and the corresponding block input is unused, we prune it as well. The returned set contains all `Value*` that are considered live (i.e., not eliminable). Time complexity is O(N * L) where N is number of nodes and L is number of loop iterations until convergence; worst-case O(N^2). Space complexity O(N) for the sets and worklists.
