Implement a C++ function `int findRoot(int node, const std::map<int, int>& parentMap)` that, given a node identifier and a parent map representing a disjoint-set forest (where each key maps to its parent, and a root maps to itself), returns the root of the tree containing that node. The function must handle cases where the map does not contain the given node by treating the node as its own root. It should also work for non-contiguous node IDs (e.g., 1, 2, 3, 4 as in the snippet) and must not modify the input map (const correctness). Additionally, implement a helper `void unionNodes(int a, int b, std::map<int, int>& parentMap)` that performs a union by attaching the root of `b` under the root of `a` (do nothing if already in same set). The main task is to write `findRoot` and `unionNodes` functions that correctly simulate the provided snippet’s behavior, then test them with assertions.
The core algorithm is a basic disjoint-set (union-find) without path compression or union by rank, as in the snippet. `findRoot` recursively traverses parent pointers: if `node` is not in the map, or `parentMap[node] == node`, then `node` is a root (return it). Otherwise, recurse on `parentMap[node]`. Because the map is const, we cannot cache results, so worst-case time for a single `findRoot` is `O(h)` where `h` is the height of the tree. In the worst case (e.g., unioning in a chain), height can be `O(n)` for `n` nodes. The space complexity is `O(h)` for recursion stack (or `O(1)` if iterative). For `unionNodes`, we find roots of both nodes (each costs `O(h)`), then if different, set `parentMap[rootB] = rootA`. Edge cases include: nodes not present in map (treated as roots), unioning a node with itself (no change), and unioning two nodes already in the same set (no change). The provided test sequence from the snippet: After making sets 1..4, `findRoot(2)`=2; union(1,2) then `findRoot(2)`=1; union(4,3) then `findRoot(3)`=4; union(2,3) (i.e., union 1's set with 4's set) then `findRoot(3)`=1 (since root of 2 is 1, root of 3 is 4, attach 4 under 1). Also test missing node: `findRoot(5)`=5.
#include <map>

// Return the root of the tree containing 'node' in the disjoint-set forest.
// If 'node' is not present in parentMap, treat it as a root (its own parent).
int findRoot(int node, const std::map<int, int>& parentMap) {
    auto it = parentMap.find(node);
    if (it == parentMap.end() || it->second == node) {
        return node;
    }
    return findRoot(it->second, parentMap);
}

// Union the sets containing 'a' and 'b' by attaching root of 'b' under root of 'a'.
// If they are already in the same set, do nothing.
void unionNodes(int a, int b, std::map<int, int>& parentMap) {
    int rootA = findRoot(a, parentMap);
    int rootB = findRoot(b, parentMap);
    if (rootA != rootB) {
        parentMap[rootB] = rootA;
    }
}
#include <cassert>
#include <map>

// function declarations (as defined above)
int findRoot(int, const std::map<int, int>&);
void unionNodes(int, int, std::map<int, int>&);

int main() {
    std::map<int, int> parentMap;
    // makeSet for 1..4
    for (int i = 1; i <= 4; ++i) parentMap[i] = i;

    assert(findRoot(2, parentMap) == 2);
    unionNodes(1, 2, parentMap);
    assert(findRoot(2, parentMap) == 1);

    unionNodes(4, 3, parentMap);
    assert(findRoot(3, parentMap) == 4);

    unionNodes(2, 3, parentMap);
    assert(findRoot(3, parentMap) == 1);
    assert(findRoot(1, parentMap) == 1);
    assert(findRoot(4, parentMap) == 1);

    // Node not in map is its own root
    assert(findRoot(5, parentMap) == 5);

    // Union with missing node still works
    unionNodes(5, 2, parentMap);
    assert(findRoot(5, parentMap) == 1); // root of 2 is 1, attach 5 under 1
    assert(findRoot(5, parentMap) == 1);

    // Union same set does nothing but remains consistent
    unionNodes(3, 4, parentMap);
    assert(findRoot(4, parentMap) == 1);

    return 0;
}
