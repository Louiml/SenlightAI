/*
Write a C++ function `findTriangleInTournament` that takes an integer `n` (3 ≤ n ≤ 5000) and a 2D vector of strings `adjacency` of size `n`, where each string has length `n` and contains only '0' or '1'. The input represents a tournament graph (a complete directed graph): for every pair of distinct vertices `i` and `j` (0-indexed), exactly one of `adjacency[i][j]` or `adjacency[j][i]` is '1'. The function must find and return a vector of three distinct integers (vertex indices, 0-indexed) that form a directed cycle of length 3 (i.e., there exist edges a→b, b→c, and c→a). If no such triangle exists, return an empty vector. If multiple triangles exist, any valid one may be returned. The function must run efficiently for the given constraints.
*/

#include <vector>
#include <string>
#include <algorithm>

// Finds a directed cycle of length 3 in a tournament graph.
// Returns the three vertex indices (0-indexed) if such a cycle exists,
// otherwise returns an empty vector.
std::vector<int> findTriangleInTournament(int n, const std::vector<std::string>& adjacency) {
    if (n < 3) return {};
    
    std::vector<int> outDeg(n, 0), inDeg(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (adjacency[i][j] == '1') {
                outDeg[i]++;
                inDeg[j]++;
            }
        }
    }
    
    // Mark vertices that could belong to a transitive ordering.
    std::vector<bool> marked(n, false);
    for (int targetDeg = n - 1; targetDeg >= 0; --targetDeg) {
        for (int v = 0; v < n; ++v) {
            if (!marked[v] && inDeg[v] == targetDeg) {
                marked[v] = true;
                break;
            }
        }
    }
    
    // If all vertices are marked, the tournament is transitive -> no triangle.
    bool allMarked = true;
    for (bool m : marked) {
        if (!m) { allMarked = false; break; }
    }
    if (allMarked) return {};
    
    // Pick an unmarked vertex with the largest out-degree.
    int pos = -1;
    for (int v = 0; v < n; ++v) {
        if (!marked[v] && (pos == -1 || outDeg[v] > outDeg[pos])) {
            pos = v;
        }
    }
    
    // Search for a triangle containing pos: pos -> i -> j -> pos.
    for (int i = 0; i < n; ++i) {
        if (i == pos || adjacency[pos][i] != '1') continue;
        for (int j = 0; j < n; ++j) {
            if (j == pos || j == i) continue;
            if (adjacency[i][j] == '1' && adjacency[j][pos] == '1') {
                return {pos, i, j};
            }
        }
    }
    
    // Should not reach here for a non-transitive tournament, but for safety:
    return {};
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here for linkage.
std::vector<int> findTriangleInTournament(int n, const std::vector<std::string>& adjacency);

int main() {
    // 3 vertices, cycle: 0->1, 1->2, 2->0
    std::vector<std::string> t1 = {"010", "001", "100"};
    std::vector<int> r1 = findTriangleInTournament(3, t1);
    assert(r1.size() == 3);
    // Check that it's a valid triangle
    int a = r1[0], b = r1[1], c = r1[2];
    assert(t1[a][b] == '1' && t1[b][c] == '1' && t1[c][a] == '1');

    // 3 vertices, transitive: 0->1, 0->2, 1->2 -> no triangle
    std::vector<std::string> t2 = {"011", "001", "000"};
    assert(findTriangleInTournament(3, t2).empty());

    // 4 vertices with a triangle among 0,1,2 and 3 beats everyone
    std::vector<std::string> t3 = {"0100", "0010", "1000", "1110"};
    // Edges: 0->1, 1->2, 2->0 (triangle), 0->3, 1->3, 2->3, 3 beats all? Actually row 3 is "1110": 3->0,3->1,3->2, and 3->? self ignore. So triangle still exists.
    std::vector<int> r3 = findTriangleInTournament(4, t3);
    assert(r3.size() == 3);
    int x = r3[0], y = r3[1], z = r3[2];
    assert(t3[x][y] == '1' && t3[y][z] == '1' && t3[z][x] == '1');

    // Large transitive tournament (n=5) all edges forward
    std::vector<std::string> t4 = {"01111", "00111", "00011", "00001", "00000"};
    assert(findTriangleInTournament(5, t4).empty());

    // Random-ish tournament with known cycle: 4 vertices, 0->1,1->2,2->0, 3->0,3->1,3->2, and 0,2? Actually make 3 beat everyone except cycle still exists.
    std::vector<std::string> t5 = {"0100", "0010", "1000", "1110"};
    std::vector<int> r5 = findTriangleInTournament(4, t5);
    assert(r5.size() == 3);

    // Mixed tournament: 5 vertices with one triangle at indices 1,2,3
    std::vector<std::string> t6 = {
        "01111", // 0 beats 1,2,3,4
        "00110", // 1 beats 2,3
        "00001", // 2 beats 4
        "10000", // 3 beats 0
        "10000"  // 4 beats 0
    };
    // Check edges: 1->2,2->4,4->1? 4->1 is '0' (since t6[4][1]='0'), so not. Let's define a proper one:
    // Let n=5, edges: 0->1, 0->2, 0->3, 0->4; 1->2, 1->3; 2->3; 3->4; 4->1? Then cycle 1->2->3->1? 3->1 is false. Better use a known non-transitive: 0->1,1->2,2->0, and 3->0,3->1,3->2, 4->3,4->0,4->1,4->2? But tournament requires exactly one direction between each pair. Let's construct carefully:
    // For i<j, we can set edge direction to create a cycle among 0,1,2:
    // 0->1, 1->2, 2->0. For vertex 3, let 3 beat 0,1,2 and also 3->4? Then 4 must lose to 0,1,2,3? But need 4 vs 0,1,2. Let 0,1,2 beat 4. Then 3 beats 4. Check: 3 beats 0,1,2,4 so row3="11110"? Actually 3 beats 0,1,2,4 -> row3 = "11110"? But 3 vs 3 is self. So t3[3] = "11110" meaning 3->0,3->1,3->2,3->4. 0 beats 1,4 but loses to 2? Actually 2->0, so 0 loses to 2. So rows:
    // 0: 0->1,0->4, loses to 2,3 -> "01010"? Let's just not overcomplicate; we already have enough tests.
    // Use a simple guaranteed cycle on 3 vertices within a 4-vertex tournament where 3 beats everyone.
    std::vector<std::string> t7 = {
        "0100", // 0->1
        "0010", // 1->2
        "1000", // 2->0
        "1110"  // 3->0,1,2
    };
    std::vector<int> r7 = findTriangleInTournament(4, t7);
    assert(r7.size() == 3);
    
    // Self-consistency: verify returned triple forms a cycle.
    auto verify = [&](const std::vector<std::string>& g, const std::vector<int>& tri) {
        if (tri.size() != 3) return false;
        int u=tri[0], v=tri[1], w=tri[2];
        return g[u][v]=='1' && g[v][w]=='1' && g[w][u]=='1';
    };
    assert(verify(t7, r7));

    // Bigger transitive tournament (n=10) all forward edges
    std::vector<std::string> t8(10, std::string(10, '0'));
    for (int i = 0; i < 10; ++i)
        for (int j = i+1; j < 10; ++j)
            t8[i][j] = '1';
    assert(findTriangleInTournament(10, t8).empty());

    // Tournament with exactly one triangle at (0,1,2) but 3 beats all of them
    std::vector<std::string> t9 = {
        "0100", // 0->1
        "0010", // 1->2
        "1000", // 2->0
        "1110"  // 3->0,1,2
    };
    assert(findTriangleInTournament(4, t9).size() == 3);

    return 0;
}

// The key observation is that a tournament graph always contains a directed triangle unless it is a transitive tournament (which is a total order with edges always pointing from the earlier to the later vertex). In a transitive tournament, each vertex has a distinct out-degree (number of outgoing edges): the "first" vertex has out-degree n−1, the next has n−2, ..., the last has 0. So we can identify the transitive case by checking if the multiset of out-degrees is exactly {0,1,...,n−1}. If yes, no triangle exists. Otherwise, a triangle must exist.
//
// Algorithm:
// 1. Compute out-degree for each vertex: `deg[i]` = count of '1' in row `i`.
// 2. Sort the degrees and check if they equal `[0,1,2,...,n-1]`. If they do, return empty vector (transitive tournament, no triangle).
// 3. Otherwise, pick a vertex `p` that is not the unique vertex with the maximum possible out-degree? Simpler: find any vertex `p` such that its out-degree is not n−1 (since if a vertex has out-degree n−1, it beats everyone and cannot be part of a cycle). But careful: if a vertex has out-degree n−1, it cannot be part of a triangle. So among vertices with out-degree < n−1, pick one (any). Let it be `p`.
// 4. Since `p` does not beat everyone, there exists a vertex `q` such that `q` beats `p` (i.e., `adjacency[q][p] == '1'`). Also, since `p` has out-degree at least 0, and if all vertices that beat `p` are also beaten by `p`? That would mean `p` beats all vertices that beat it, but that's impossible because any vertex that beats `p` is not beaten by `p` (since tournament is complete). So we need a smarter approach.
//
// Better standard proof: Pick a vertex with maximum out-degree. Let `m` be that vertex. If `m` has out-degree n−1, then it beats everyone. Then consider the subgraph induced by the remaining n−1 vertices. That subgraph is also a tournament. If it contains a triangle, we are done. If not, it is transitive. But in a transitive tournament, the vertex with the second-highest out-degree (among the remaining) beats all others except `m`. So we can recursively find. But easier iterative approach: We can find a triangle by a known algorithm for tournaments:
// - Pick any vertex `a`.
// - Find a vertex `b` such that `b` beats `a` (if none, i.e., `a` beats everyone, then `a` cannot be in a triangle; remove it conceptually and repeat on remaining).
// - Among all vertices that beat `a`, find a vertex `c` such that `a` beats `c`? Actually, we need a->b, b->c, c->a. So pick `a`, then pick `b` that beats `a`. Then among vertices that beat `a`, look for a `c` such that `b` beats `c` and `c` beats `a`? That is a 3-cycle directly.
//
// Simpler constructive algorithm from the code snippet: The given code identifies vertices with in-degrees (db) equal to n−1, n−2, ... to check transitivity. Then it picks a vertex `pos` that is not marked as part of the transitive chain and has the maximum out-degree among unmarked. Then it searches for two other vertices `i` and `j` such that pos→i, i→j, j→pos. Why does this work? Because if the tournament is not transitive, there exists a cycle. The vertex `pos` chosen in the code is one that is not forced into a transitive chain; and we can always find a triangle containing that vertex? Actually, the code tries all pairs (i,j) to find a triangle containing pos. If it finds one, output. If not, the code outputs nothing (but the problem guarantees a solution unless transitive; the code handles that earlier). For our function, we can implement a direct search: for each possible `a`, for each `b` that `a` beats (a→b), for each `c` that `b` beats (b→c) and `c` beats `a` (c→a), return {a,b,c}. This is O(n^3) = 125e9 for n=5000, too slow. So we need a faster method.
//
// Fast approach: Since a tournament always has a Hamiltonian path, but not necessarily a triangle. Actually, a tournament has a triangle if and only if it is not transitive. There is an O(n + m) algorithm: compute out-degrees. If all out-degrees are distinct, it's transitive. Otherwise, we can find a triangle in O(n^2) by: pick a vertex `v` with out-degree not equal to n-1 and not equal to 0? Let's do this: Pick any vertex `v`. Let `S` = set of vertices that `v` beats (out-neighbors), `T` = set of vertices that beat `v` (in-neighbors). If both S and T are non-empty and there exists an edge from some vertex in S to some vertex in T, then we have a triangle (v→s, s→t, t→v). If for every s in S and every t in T, the edge goes from t to s (i.e., all T beat all S), then we cannot find a triangle involving v. But then v's out-degree is |S| and all vertices in T have out-degree at least |S|+1 (because they beat all S plus v). Actually, we can prove if no triangle exists, the tournament is transitive. So the algorithm: For a candidate vertex `v`, if both in-neighbors and out-neighbors exist, check if there is any edge from an out-neighbor to an in-neighbor. If yes, triangle found. If no, then all edges between out-neighbors and in-neighbors go from in to out. In that case, v beats all out-neighbors, all in-neighbors beat v and all out-neighbors? Not necessarily all in-neighbors beat all out-neighbors, but they might. If we pick v with out-degree not equal to n-1 and not equal to 0, and we find no triangle, then we can discard v? Actually, we can repeatedly pick a vertex with out-degree between 1 and n-2. If it has both in and out neighbors, and no edge from out to in, then we can "merge" v into the in-set? This is a bit complex. 
//
// The given code's approach is: It first marks all vertices with in-degree exactly n−1, n−2, ... in that order, which effectively identifies a potential total order chain. If all vertices are marked, it's transitive. Otherwise, it picks a vertex `pos` that is not marked and has the largest out-degree. Then it brute-force tries O(n^2) pairs to find a triangle containing pos. This is O(n^2) after O(n^2) preprocessing. That's acceptable for n=5000 (25 million operations). So we can adopt that: 
// 1. Compute in-degree (number of '1' in column j) for each vertex.
// 2. Simulate the marking: for k from n-1 down to 0, find any unmarked vertex with in-degree == k, mark it. This is O(n^2) if done naively with a loop over vertices for each k, but total O(n^2) because we have n values of k and each time we scan n vertices, total O(n^2) = 25e6 which is fine.
// 3. If all vertices marked, return empty vector.
// 4. Find `pos` = unmarked vertex with maximum out-degree (out-degree = n - in-degree). 
// 5. For each i from 0 to n-1, if i != pos and adjacency[pos][i] == '1', for each j from 0 to n-1, if j != pos && j != i && adjacency[i][j] == '1' && adjacency[j][pos] == '1', return {pos, i, j}.
// 6. If not found, return empty (should not happen for non-transitive).
//
// Edge cases: The graph is a tournament, so no self-loops and exactly one direction between any pair. The input strings are length n. The function must return vector<int> (0-indexed). If no triangle, return {}.
//
// Time complexity: O(n^2) for marking (the inner loop for each k scans all vertices; total n^2). The final search is O(n^2) in worst case (two nested loops over n). So total O(n^2) = 25e6 operations, acceptable. Space: O(n) for degrees and marks, O(n^2) for the adjacency matrix (input) – but the function receives it as parameter, so no extra copying if we use const ref.
