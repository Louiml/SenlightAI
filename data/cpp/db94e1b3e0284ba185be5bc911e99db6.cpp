/*
You are given a set of function definitions and calls. Each function has a unique integer ID, a name, and a list of callee function IDs that it calls (directly or indirectly, but represented as direct edges). Some functions may be declared but not defined. Write a C++ function `buildCallDAG(std::vector<FunctionInfo> functions, std::vector<size_t>& order, std::vector<size_t>& idToIndex)` where `FunctionInfo` is a struct containing `int id`, `std::string name`, `bool defined`, and `std::vector<int> callees`. The function must perform a topological sort of the defined functions based on their call dependencies, assigning each a sequential index starting from 0. If there is a recursive call among defined functions, return `false` and set `order` to an empty vector. If any defined function calls an undefined function, also return `false`. Otherwise, fill `order` with function IDs in a valid topological order (i.e., for every edge callee→caller, the callee appears before the caller), and fill `idToIndex` such that `idToIndex[id]` gives the assigned index. Functions that are undefined are ignored entirely. The input is guaranteed to have no duplicate IDs and callee IDs will always refer to existing function entries.
*/

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

struct FunctionInfo {
    int id;
    std::string name;
    bool defined;
    std::vector<int> callees;
};

// Performs topological sort of defined functions based on call graph.
// Returns false if recursion or call to undefined function detected.
bool buildCallDAG(const std::vector<FunctionInfo>& functions,
                  std::vector<size_t>& order,
                  std::vector<size_t>& idToIndex) {
    // Map from id to index in the functions vector
    std::map<int, size_t> idToPos;
    for (size_t i = 0; i < functions.size(); ++i) {
        idToPos[functions[i].id] = i;
    }

    // For each defined function, we will assign an index
    size_t currentIndex = 0;
    std::vector<int> indexAssigned(functions.size(), -1); // -1 = not assigned
    std::vector<bool> visiting(functions.size(), false);

    // Temporary storage for results
    std::vector<int> tmpOrder;

    // We'll iterate over all defined functions as roots
    for (size_t root = 0; root < functions.size(); ++root) {
        if (!functions[root].defined || indexAssigned[root] != -1) {
            continue;
        }

        // Stack for DFS. Each element is a pair (functionPos, state)
        // state=false means we just entered, state=true means we are leaving to finalize
        std::vector<std::pair<size_t, bool>> stack;
        stack.push_back({root, false});

        while (!stack.empty()) {
            auto [pos, leaving] = stack.back();
            stack.pop_back();

            if (leaving) {
                // Finalize this function
                visiting[pos] = false;
                indexAssigned[pos] = static_cast<int>(currentIndex);
                ++currentIndex;
                tmpOrder.push_back(functions[pos].id);
                continue;
            }

            if (indexAssigned[pos] != -1) {
                // Already processed
                continue;
            }

            if (visiting[pos]) {
                // Cycle detected
                order.clear();
                idToIndex.clear();
                return false;
            }

            visiting[pos] = true;

            // Check callees first
            for (int calleeId : functions[pos].callees) {
                size_t calleePos = idToPos[calleeId];
                if (!functions[calleePos].defined) {
                    // Call to undefined function
                    order.clear();
                    idToIndex.clear();
                    return false;
                }
                // Push callee if not yet assigned
                if (indexAssigned[calleePos] == -1 && !visiting[calleePos]) {
                    stack.push_back({calleePos, false});
                } else if (visiting[calleePos]) {
                    // Cycle detection: callee is currently being visited
                    order.clear();
                    idToIndex.clear();
                    return false;
                }
            }

            // Push this function again to finalize after callees are processed
            stack.push_back({pos, true});
        }
    }

    // Build the output order (already in topological order from tmpOrder)
    order = std::vector<size_t>(tmpOrder.begin(), tmpOrder.end());

    // Build idToIndex map
    idToIndex.clear();
    idToIndex.resize(functions.size(), static_cast<size_t>(-1));
    for (size_t i = 0; i < functions.size(); ++i) {
        if (functions[i].defined && indexAssigned[i] != -1) {
            idToIndex[functions[i].id] = static_cast<size_t>(indexAssigned[i]);
        }
    }

    return true;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution here (FunctionInfo and buildCallDAG)

int main() {
    // Test 1: simple linear chain
    std::vector<FunctionInfo> f1 = {
        {1, "a", true, {2}},
        {2, "b", true, {3}},
        {3, "c", true, {}}
    };
    std::vector<size_t> order1, idToIndex1;
    assert(buildCallDAG(f1, order1, idToIndex1) == true);
    assert(order1.size() == 3);
    // Topological order must have c (id=3) before b (id=2) before a (id=1)
    assert(order1 == std::vector<size_t>({3,2,1}));
    assert(idToIndex1[1] == 2 && idToIndex1[2] == 1 && idToIndex1[3] == 0);

    // Test 2: recursion among defined functions
    std::vector<FunctionInfo> f2 = {
        {1, "a", true, {2}},
        {2, "b", true, {1}}
    };
    std::vector<size_t> order2, idToIndex2;
    assert(buildCallDAG(f2, order2, idToIndex2) == false);
    assert(order2.empty() && idToIndex2.empty());

    // Test 3: call to undefined function
    std::vector<FunctionInfo> f3 = {
        {1, "a", true, {2}},
        {2, "b", true, {3}},
        {3, "c", false, {}}
    };
    std::vector<size_t> order3, idToIndex3;
    assert(buildCallDAG(f3, order3, idToIndex3) == false);
    assert(order3.empty() && idToIndex3.empty());

    // Test 4: independent functions (no calls)
    std::vector<FunctionInfo> f4 = {
        {1, "a", true, {}},
        {2, "b", false, {}},
        {3, "c", true, {}}
    };
    std::vector<size_t> order4, idToIndex4;
    assert(buildCallDAG(f4, order4, idToIndex4) == true);
    assert(order4.size() == 2);
    // Both defined functions should appear, undefined is skipped
    assert(std::find(order4.begin(), order4.end(), 1) != order4.end());
    assert(std::find(order4.begin(), order4.end(), 3) != order4.end());
    assert(idToIndex4[1] < order4.size() && idToIndex4[3] < order4.size());

    // Test 5: mixed chain where a defined function calls both defined and undefined
    std::vector<FunctionInfo> f5 = {
        {1, "a", true, {2, 3}},
        {2, "b", true, {}},
        {3, "c", false, {}}
    };
    std::vector<size_t> order5, idToIndex5;
    assert(buildCallDAG(f5, order5, idToIndex5) == false);

    // Test 6: longer chain with diamond shape, no recursion
    std::vector<FunctionInfo> f6 = {
        {1, "a", true, {2, 3}},
        {2, "b", true, {4}},
        {3, "c", true, {4}},
        {4, "d", true, {}}
    };
    std::vector<size_t> order6, idToIndex6;
    assert(buildCallDAG(f6, order6, idToIndex6) == true);
    assert(order6.size() == 4);
    // d must come before b and c; b and c must come before a
    size_t pos_d = std::find(order6.begin(), order6.end(), 4) - order6.begin();
    size_t pos_b = std::find(order6.begin(), order6.end(), 2) - order6.begin();
    size_t pos_c = std::find(order6.begin(), order6.end(), 3) - order6.begin();
    size_t pos_a = std::find(order6.begin(), order6.end(), 1) - order6.begin();
    assert(pos_d < pos_b && pos_d < pos_c);
    assert(pos_b < pos_a && pos_c < pos_a);

    // Test 7: self-recursion
    std::vector<FunctionInfo> f7 = {
        {1, "a", true, {1}}
    };
    std::vector<size_t> order7, idToIndex7;
    assert(buildCallDAG(f7, order7, idToIndex7) == false);

    return 0;
}

// The core algorithm is essentially the same as the provided `CallDAGCreator::assignIndicesInternal` but simplified because we don’t need to traverse an AST. We can model each function as a node in a graph with directed edges from callee to caller (or equivalently from caller to callee, depending on desired topological order). I choose edges from callee → caller, so that a topological sort gives callees first. We need to detect two error conditions: (1) recursion among defined functions, and (2) a defined function calling an undefined function. For (1), we can use a DFS-based topological sort with three states: unvisited, visiting, visited. If we encounter a node already in the "visiting" state, that’s a cycle. For (2), while processing a defined function, if any callee is not defined, that’s an error. We process only defined functions; undefined functions are ignored, but if a defined function references an undefined one, we fail. The algorithm is iterative to avoid recursion depth issues, similar to the original. We maintain a stack of nodes to process, where each node can either be marked as visiting or finalized. When we pop a node, we check its state. If it’s already assigned an index, skip. If it’s undefined, that’s an error (since we only push defined ones, but we need to check callees). If it’s currently visiting, that means a cycle. Otherwise, mark it visiting, push all its callees (that are defined; undefined callees cause immediate error), and then push the node itself again to finalize it later. The order of processing ensures that after all callees are assigned, we assign the caller’s index.
//
// Time complexity: O(V + E) where V is number of defined functions and E is total number of callee edges. Space: O(V) for the stack and maps.
//
// Edge cases: single function with no callees; multiple independent chains; undefined function that is never called (ignored); a function that calls itself directly; a longer cycle; a function calling an undefined function; a function that is defined but never called (still gets an index). The returned order should list all defined functions exactly once.
