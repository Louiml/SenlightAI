// You are given a tree with `n` vertices (numbered 1 to n). A "center" is any vertex whose degree (number of neighbors) is exactly 1, i.e., a leaf. Starting from any leaf, perform a depth-first traversal that labels vertices cyclically with labels 0, 1, 2, 0, 1, 2, ... as you move away from the starting leaf (the starting leaf gets label 0, its neighbor label 1, the next label 2, etc.). Collect the degrees of all vertices that receive label 1 during any such traversal that starts at a leaf. However, you should perform this traversal from *only one* leaf (the first leaf you encounter when scanning vertices 1..n in order), and only collect degrees from vertices marked label 1. Sort those collected degrees in non-decreasing order and return them as a vector of integers. The input tree is connected and has at least 2 vertices. Write a function that takes an integer `n` and a vector of undirected edges (each edge as a pair of 1-indexed vertices) and returns the sorted list of degrees of label-1 vertices. If no vertex receives label 1, return an empty vector.

// The key observation is that the tree is unrooted and we start from a leaf. Since the traversal is deterministic given the starting leaf, we can simulate a DFS over the tree. The graph is stored as an adjacency list. We find the first leaf by scanning vertices 1..n and checking if their degree is 1. From that leaf, we run a recursive DFS that carries a label (0,1,2) and increments the label modulo 3 as we move to children (excluding the parent). When the label is 1, we record the degree of that vertex (which is the size of its adjacency list). After the DFS, we sort the collected degrees in ascending order and return them. Edge cases: if the tree has exactly 2 vertices, the starting leaf's neighbor gets label 1, and its degree is 1, so the result is {1}. If the tree is a star with center degree >1, the leaves get label 0, the center gets label 1, and we record the center's degree (which is the number of leaves). Since the traversal starts from the first leaf, we will visit all vertices exactly once. The recursion depth could be up to n, so we might need to consider iterative DFS to avoid stack overflow for large n, but for a typical contest setting recursion is acceptable with n up to 200,000 if the recursion limit is increased. Time complexity: O(n) for DFS and O(k log k) for sorting the collected degrees, where k is the number of label-1 vertices. Space complexity: O(n) for adjacency list and recursion stack.

#include <vector>
#include <algorithm>

// Given a tree with n vertices (1-indexed) and a list of undirected edges,
// return the sorted degrees of vertices that receive label 1 in a DFS starting
// from the first leaf encountered (smallest index with degree 1).
std::vector<int> collectLabelOneDegrees(int n, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> graph(n);
    for (const auto& edge : edges) {
        int u = edge.first - 1;
        int v = edge.second - 1;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // Find the first leaf (degree 1) scanning vertices 0..n-1.
    int start_leaf = -1;
    for (int i = 0; i < n; ++i) {
        if (graph[i].size() == 1) {
            start_leaf = i;
            break;
        }
    }

    std::vector<int> degrees;
    // DFS: label cycles 0,1,2,0,1,2,...
    // Use a lambda with explicit recursion.
    std::function<void(int, int, int)> dfs = [&](int now, int parent, int label) {
        if (label == 1) {
            degrees.push_back(static_cast<int>(graph[now].size()));
        }
        int next_label = (label + 1) % 3;
        for (int neighbor : graph[now]) {
            if (neighbor == parent) continue;
            dfs(neighbor, now, next_label);
        }
    };

    dfs(start_leaf, -1, 0);
    std::sort(degrees.begin(), degrees.end());
    return degrees;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function declaration (assume it's included).
std::vector<int> collectLabelOneDegrees(int n, const std::vector<std::pair<int, int>>& edges);

int main() {
    // Example 1: simple path of 3 vertices: 1-2-3
    // Leaves: 1 and 3. Start from leaf 1 (degree 1, label 0).
    // Vertex 2 gets label 1, degree 2; vertex 3 gets label 2.
    // Result: {2}
    assert(collectLabelOneDegrees(3, {{1,2}, {2,3}}) == std::vector<int>({2}));

    // Example 2: star with center 1 and leaves 2,3,4
    // First leaf is vertex 2 (degree 1). DFS: vertex 2 label 0, vertex 1 label 1 (degree 3), leaves 3,4 label 2.
    // Result: {3}
    assert(collectLabelOneDegrees(4, {{1,2}, {1,3}, {1,4}}) == std::vector<int>({3}));

    // Example 3: path of 2 vertices: 1-2
    // First leaf is 1. Label 0: vertex 1, label 1: vertex 2 (degree 1).
    // Result: {1}
    assert(collectLabelOneDegrees(2, {{1,2}}) == std::vector<int>({1}));

    // Example 4: tree: 1-2, 2-3, 2-4, 4-5 (n=5)
    // Degrees: 1:1, 2:3, 3:1, 4:2, 5:1
    // First leaf is 1. From leaf 1: label0:1, label1:2 (deg3), label2:3,4, label0:5.
    // Only vertex 2 gets label1, degree 3. Result: {3}
    assert(collectLabelOneDegrees(5, {{1,2}, {2,3}, {2,4}, {4,5}}) == std::vector<int>({3}));

    // Example 5: tree with multiple label-1 vertices:
    // n=6, edges: 1-2, 2-3, 3-4, 2-5, 5-6
    // Leaves: 1,4,6. First leaf=1. DFS: label0:1, label1:2(deg3), label2:3,5, label0:4,6? Wait:
    // From 2 label1 -> children: 3 (label2), 5 (label2)
    // From 3 label2 -> child 4 label0
    // From 5 label2 -> child 6 label0
    // So label1 vertices: only vertex 2 (degree 3). Result {3}
    assert(collectLabelOneDegrees(6, {{1,2}, {2,3}, {3,4}, {2,5}, {5,6}}) == std::vector<int>({3}));

    // Example 6: n=7, edges: 1-2, 2-3, 3-4, 2-5, 5-6, 6-7
    // Leaves: 1,4,7. First leaf=1. DFS: label0:1, label1:2(deg3), label2:3,5, label0:4,6, label2:7? Actually:
    // From 2 label1 -> 3 label2, 5 label2
    // From 3 label2 -> 4 label0
    // From 5 label2 -> 6 label0
    // From 6 label0 -> 7 label1, degree 1
    // So label1 vertices: vertex 2 (degree3) and vertex 7 (degree1). Sorted: {1,3}
    assert(collectLabelOneDegrees(7, {{1,2}, {2,3}, {3,4}, {2,5}, {5,6}, {6,7}}) == std::vector<int>({1,3}));

    // Example 7: empty case? n>=2 so not empty. But ensure no crash for a chain of 4: 1-2-3-4
    // First leaf=1, label0:1, label1:2(deg2), label2:3, label0:4 => result {2}
    assert(collectLabelOneDegrees(4, {{1,2}, {2,3}, {3,4}}) == std::vector<int>({2}));

    return 0;
}
