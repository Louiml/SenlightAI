// Write a standalone C++ function `generateAllTopologicalOrders` that, given a string of distinct lowercase letters (the set of items, up to 26) and a string of dependency pairs in the format `"AB CD EF"` where `AB` means `A` must come before `B` (i.e., `B` depends on `A`), returns a vector of strings containing all valid topological orderings of the letters, sorted lexicographically. The input strings contain only letters; spaces in the dependency string are ignored. A valid ordering must place every dependency source before its target. If the graph has a cycle or no valid order exists, return an empty vector. The function must handle any number of letters (including zero) and any number of dependencies, and must not modify the input strings.
The problem is to generate all topological sorts of a directed acyclic graph (DAG) where nodes are distinct lowercase letters and edges are given as ordered pairs. The key is to use backtracking with a priority of letters sorted lexicographically. Maintain an adjacency list and an in-degree count for each node. At each step, choose the smallest available letter whose in-degree is zero (no unresolved dependencies), append it to the current permutation, decrease the in-degree of all its neighbors, recurse, then backtrack by restoring in-degrees and removing the letter. Because we always pick the smallest possible letter at each step, the resulting orderings are produced in lexicographic order. Edge cases: an empty set of letters should return one empty string; if there is a cycle, no valid ordering exists—this can be detected if at some step no zero-in-degree nodes remain but not all nodes are used, or by counting the number of generated orders (if zero and the graph is non-empty, there is a cycle). Time complexity is \(O(V! \cdot E)\) in the worst case for the number of permutations generated, but with pruning it is fine for up to ~10 nodes; space complexity is \(O(V+E)\) for adjacency and in-degree plus \(O(V)\) for recursion depth.
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

// Generate all lexicographically sorted topological orderings of the given letters
// with dependency pairs of the form "XY" meaning X must appear before Y.
std::vector<std::string> generateAllTopologicalOrders(const std::string& letters, const std::string& dependencies) {
    // Collect unique letters
    std::set<char> nodeSet;
    for (char c : letters) {
        if (std::isalpha(c)) nodeSet.insert(c);
    }
    // Also include letters from dependencies (in case not listed in 'letters')
    for (char c : dependencies) {
        if (std::isalpha(c)) nodeSet.insert(c);
    }
    std::vector<char> nodes(nodeSet.begin(), nodeSet.end());
    if (nodes.empty()) return {""};

    // Build adjacency and in-degree
    std::map<char, std::vector<char>> adj;
    std::map<char, int> indeg;
    for (char c : nodes) {
        adj[c] = {};
        indeg[c] = 0;
    }
    // Parse dependencies: "AB CD" -> A before B, C before D
    std::string clean;
    for (char c : dependencies) {
        if (std::isalpha(c)) clean.push_back(c);
    }
    for (size_t i = 0; i + 1 < clean.size(); i += 2) {
        char before = clean[i];
        char after = clean[i+1];
        // Check both exist
        if (nodeSet.count(before) && nodeSet.count(after)) {
            adj[before].push_back(after);
            indeg[after]++;
        }
    }

    std::vector<std::string> result;
    std::string current;
    std::vector<bool> used(nodes.size(), false);

    // Recursive backtracking
    std::function<void()> backtrack = [&]() {
        if (current.size() == nodes.size()) {
            result.push_back(current);
            return;
        }
        bool hasZero = false;
        for (size_t i = 0; i < nodes.size(); ++i) {
            if (!used[i] && indeg[nodes[i]] == 0) {
                hasZero = true;
                // Choose this node
                used[i] = true;
                current.push_back(nodes[i]);
                // Decrease indegree of neighbors
                for (char neighbor : adj[nodes[i]]) {
                    indeg[neighbor]--;
                }
                backtrack();
                // Restore
                for (char neighbor : adj[nodes[i]]) {
                    indeg[neighbor]++;
                }
                current.pop_back();
                used[i] = false;
                // Once we pick the smallest lexicographically available at this step,
                // do not try larger ones? Actually we must try all to generate all orders
                // But to get lexicographic order globally, we should loop in sorted order,
                // which we do because nodes is sorted. However we must not break after one pick.
            }
        }
        if (!hasZero && current.size() < nodes.size()) {
            // cycle detected, result stays empty
        }
    };

    backtrack();
    return result;
}
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

// Include the solution function declaration/definition here (or link it)

int main() {
    // Basic two-node dependency
    std::vector<std::string> r1 = generateAllTopologicalOrders("abc", "ab");
    assert(r1.size() == 3);
    assert(r1[0] == "abc");
    assert(r1[1] == "acb");
    assert(r1[2] == "cab");
    // Note: "bac" and "bca" invalid because a must be before b.

    // Linear chain a->b->c
    std::vector<std::string> r2 = generateAllTopologicalOrders("abc", "ab bc");
    assert(r2.size() == 1);
    assert(r2[0] == "abc");

    // Cycle a->b, b->a
    std::vector<std::string> r3 = generateAllTopologicalOrders("ab", "ab ba");
    assert(r3.empty());

    // Single letter
    std::vector<std::string> r4 = generateAllTopologicalOrders("z", "");
    assert(r4.size() == 1);
    assert(r4[0] == "z");

    // Empty set
    std::vector<std::string> r5 = generateAllTopologicalOrders("", "");
    assert(r5.size() == 1);
    assert(r5[0] == "");

    // Independent nodes, sorted output
    std::vector<std::string> r6 = generateAllTopologicalOrders("ba", "");
    assert(r6.size() == 2);
    assert(r6[0] == "ab");
    assert(r6[1] == "ba");

    // Complex case: 4 nodes with multiple valid orders
    std::vector<std::string> r7 = generateAllTopologicalOrders("abcd", "ab cd");
    assert(r7.size() == 6);
    assert(r7[0] == "abcd");
    assert(r7[1] == "abdc");
    assert(r7[2] == "acbd");
    assert(r7[3] == "acdb");
    assert(r7[4] == "cabd");
    assert(r7[5] == "cadb");

    std::cout << "All tests passed.\n";
    return 0;
}
