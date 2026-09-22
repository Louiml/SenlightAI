// Given a vector of AST node pointers representing a syntax tree hierarchy, where each node has a unique `id_` (integer) and a `location_` (represented as a simple integer range `{start, end}`), write a C++ function that computes a "finest nodes at location" set exactly as in the Solidity gas estimator: for each location range, only the first encountered node (in depth-first pre-order traversal across all roots in the given order) that claims that location is included. The function must return a `std::set<const Node*>`, where `Node` is a struct with `int id_;`, `int start_;`, `int end_;`, `std::vector<Node*> children_;`. Traversal must be pre-order (visit node before its children) and must consider all roots in the order they appear in the input vector. The location is identified by the pair `(start_, end_)`; two nodes are considered to have the same location only if both `start_` and `end_` are equal. If a location has already been recorded by a previously visited node, the current node is not added, but traversal continues into its children.

The algorithm performs a depth-first pre-order traversal over each root in the given order. Maintain a map from a location pair (e.g., `std::pair<int,int>`) to the first node pointer that recorded it. When visiting a node, check if `(start_, end_)` is not already in the map; if it's new, insert it into the map and add the node to the result set. Then recursively visit all children in their stored order. This ensures that the first node (in traversal order) to claim a location wins, and later nodes with the same location are skipped from the result set but their subtrees are still traversed (because the recursion continues regardless). Edge cases: multiple roots may share locations within a single root's subtree or across roots; empty roots vector yields empty set; nodes with children can have the same location as a descendant – the ancestor wins because it is visited first. Time complexity: O(N) where N is total number of nodes, because each node is visited exactly once, and each map/set operation is O(log M) with M distinct locations but bounded by N. Space complexity: O(D) for recursion depth plus O(M) for the map and set, with M ≤ N.

#include <set>
#include <map>
#include <vector>
#include <utility>

// Simple AST node structure.
struct Node {
    int id_;
    int start_;
    int end_;
    std::vector<Node*> children_;
};

// Perform pre-order DFS to collect the finest nodes at each distinct location.
void collectFinestNodes(Node* node, std::map<std::pair<int,int>, const Node*>& seenLocations, std::set<const Node*>& result) {
    if (!node) return;
    // Compute location key.
    auto location = std::make_pair(node->start_, node->end_);
    if (seenLocations.find(location) == seenLocations.end()) {
        // First node to claim this location.
        seenLocations[location] = node;
        result.insert(node);
    }
    // Recurse into children (pre-order, so children are visited after this node).
    for (Node* child : node->children_) {
        collectFinestNodes(child, seenLocations, result);
    }
}

// Returns the set of finest nodes at each location in the given roots.
std::set<const Node*> finestNodesAtLocation(const std::vector<Node*>& roots) {
    std::set<const Node*> result;
    std::map<std::pair<int,int>, const Node*> seenLocations;
    // Traverse roots in given order.
    for (Node* root : roots) {
        collectFinestNodes(root, seenLocations, result);
    }
    return result;
}

#include <cassert>
#include <set>
#include <vector>

// Node and function declarations are assumed to be included above.

int main() {
    // Build a simple tree:
    // root1: id=1, loc(0,10), children: id=2 loc(2,5), id=3 loc(2,5) (duplicate location)
    // root2: id=4, loc(6,9) (new), child id=5 loc(0,10) (duplicate of root1's location)
    // root3: empty vector
    Node n1{1,0,10,{}}, n2{2,2,5,{}}, n3{3,2,5,{}}, n4{4,6,9,{}}, n5{5,0,10,{}};
    n1.children_ = {&n2, &n3};
    n4.children_ = {&n5};
    std::vector<Node*> roots = {&n1, &n4};

    auto result = finestNodesAtLocation(roots);
    // Expected: n1 (loc 0-10), n2 (loc 2-5), n4 (loc 6-9)
    // n3 is skipped (same loc as n2), n5 is skipped (same loc as n1)
    assert(result.size() == 3);
    assert(result.count(&n1) == 1);
    assert(result.count(&n2) == 1);
    assert(result.count(&n4) == 1);
    assert(result.count(&n3) == 0);
    assert(result.count(&n5) == 0);

    // Test empty roots
    std::vector<Node*> empty;
    auto resultEmpty = finestNodesAtLocation(empty);
    assert(resultEmpty.empty());

    // Test single node with no children
    Node single{10,0,0,{}};
    std::vector<Node*> singleRoot = {&single};
    auto resultSingle = finestNodesAtLocation(singleRoot);
    assert(resultSingle.size() == 1);
    assert(resultSingle.count(&single) == 1);

    // Test roots with identical locations across different roots
    Node a{20,1,1,{}}, b{21,1,1,{}};
    std::vector<Node*> twoRoots = {&a, &b};
    auto resultTwo = finestNodesAtLocation(twoRoots);
    // Only the first (a) should be included
    assert(resultTwo.size() == 1);
    assert(resultTwo.count(&a) == 1);
    assert(resultTwo.count(&b) == 0);

    return 0;
}
