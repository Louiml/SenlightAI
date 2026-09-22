// Given an undirected tree with `n` nodes (numbered `0` to `n-1`), represented by a list of `n-1` undirected edges, and a list of directed guesses `(u, v)` meaning that the guesser correctly guessed that `u` is the parent of `v` in a rooted tree, write a C++ function `int countGoodRoots(int n, vector<pair<int,int>> edges, vector<pair<int,int>> guesses, int k)` that returns the number of nodes that, when chosen as the root of the tree, produce at least `k` correct guesses among the provided guesses. A guess `(u,v)` is correct for a given root if in the tree rooted at that root, `u` is the immediate parent of `v`. The input may contain duplicate guesses, but each duplicate should be counted separately (i.e., if the same directed edge appears twice, it contributes 2 to the count of correct guesses).

// The solution uses a two-pass tree traversal technique. First, build the undirected adjacency list. Also store guesses in a hash map keyed by a unique encoding `(u * n + v)` mapping to the count of that directed guess. In the first DFS (rooted at node 0 arbitrarily), compute `cnt`, the number of correct guesses when the tree is rooted at node 0. This is done by traversing the tree from root 0, and for each edge from parent `i` to child `j`, adding `guesses[(i,j)]` to `cnt`. The second DFS performs a rerooting operation: when moving the root from node `i` to its neighbor `j`, only the orientation of the edge between `i` and `j` changes. Previously, if the tree was rooted at `i`, the direction was `(i,j)`; after rerooting to `j`, the direction becomes `(j,i)`. So we subtract the count of guesses `(i,j)` and add the count of guesses `(j,i)`. This gives the correct count for root `j` in O(1) per neighbor. During this second DFS, each node is considered a root, and we add 1 to the answer if its count `cnt` meets or exceeds `k`. The algorithm runs in O(n + |guesses|) time and O(n + |guesses|) space. Edge cases include duplicate guesses (handled by counts), a tree with a single node (no edges, only root 0, and guesses may be empty or irrelevant), and `k` possibly zero (then every root is valid, since count ≥ 0 always).

#include <vector>
#include <unordered_map>
#include <functional>

// Given an undirected tree with n nodes, a list of directed guesses,
// and a threshold k, return the number of nodes that can be chosen as root
// such that at least k guesses are correct.
int countGoodRoots(int n,
                   const std::vector<std::pair<int,int>>& edges,
                   const std::vector<std::pair<int,int>>& guesses,
                   int k) {
    // Build undirected adjacency list
    std::vector<std::vector<int>> graph(n);
    for (const auto& e : edges) {
        graph[e.first].push_back(e.second);
        graph[e.second].push_back(e.first);
    }

    // Store guess counts using a unique key = u * n + v
    std::unordered_map<long long, int> guessCount;
    for (const auto& g : guesses) {
        long long key = 1LL * g.first * n + g.second;
        guessCount[key]++;
    }

    int correct = 0; // number of correct guesses for current root (initially root 0)
    int answer = 0;

    // First DFS: compute correct guesses when root = 0
    std::function<void(int,int)> dfs1 = [&](int node, int parent) {
        for (int neighbor : graph[node]) {
            if (neighbor != parent) {
                correct += guessCount[1LL * node * n + neighbor];
                dfs1(neighbor, node);
            }
        }
    };

    // Second DFS: reroot to each node and count valid roots
    std::function<void(int,int)> dfs2 = [&](int node, int parent) {
        // For current root 'node', check if correct >= k
        if (correct >= k) {
            answer++;
        }
        for (int neighbor : graph[node]) {
            if (neighbor != parent) {
                // Move root from 'node' to 'neighbor'
                int forward = guessCount[1LL * node * n + neighbor];
                int backward = guessCount[1LL * neighbor * n + node];
                correct -= forward;
                correct += backward;
                dfs2(neighbor, node);
                // Restore for other branches (undo reroot)
                correct -= backward;
                correct += forward;
            }
        }
    };

    dfs1(0, -1);
    dfs2(0, -1);
    return answer;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Test case 1: tree 0-1-2, guesses (0,1),(1,2), k=2 -> only root 0 gives both correct
    {
        int n = 3;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2}};
        std::vector<std::pair<int,int>> guesses = {{0,1},{1,2}};
        assert(countGoodRoots(n, edges, guesses, 2) == 1);
        assert(countGoodRoots(n, edges, guesses, 1) == 2); // roots 0 and 1 give at least 1
        assert(countGoodRoots(n, edges, guesses, 0) == 3);
    }

    // Test case 2: single node, no edges, empty guesses
    {
        int n = 1;
        std::vector<std::pair<int,int>> edges;
        std::vector<std::pair<int,int>> guesses;
        assert(countGoodRoots(n, edges, guesses, 0) == 1);
        assert(countGoodRoots(n, edges, guesses, 1) == 0);
    }

    // Test case 3: duplicate guesses count separately
    {
        int n = 2;
        std::vector<std::pair<int,int>> edges = {{0,1}};
        std::vector<std::pair<int,int>> guesses = {{0,1},{0,1}};
        assert(countGoodRoots(n, edges, guesses, 2) == 1); // root 0 gives 2 correct
        assert(countGoodRoots(n, edges, guesses, 1) == 1); // only root 0 gives >=1
    }

    // Test case 4: star with center 0, guesses from center to leaves, k=3
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{0,2},{0,3}};
        std::vector<std::pair<int,int>> guesses = {{0,1},{0,2},{0,3}};
        assert(countGoodRoots(n, edges, guesses, 3) == 1); // only root 0
        assert(countGoodRoots(n, edges, guesses, 2) == 1);
        assert(countGoodRoots(n, edges, guesses, 1) == 1);
    }

    // Test case 5: more complex tree, some guesses wrong for all roots
    {
        int n = 4;
        std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3}};
        std::vector<std::pair<int,int>> guesses = {{3,2},{3,0}};
        // Let's manually check: root 0: no guesses correct (0), root 1: (3,2)? no, (3,0)? no ->0,
        // root 2: (3,2)? yes ->1, root 3: (3,2)? yes, (3,0)? no ->1
        assert(countGoodRoots(n, edges, guesses, 1) == 2);
        assert(countGoodRoots(n, edges, guesses, 0) == 4);
    }

    return 0;
}
