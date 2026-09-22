/*
Write a C++ function `findMSTLikeWeight(int n, const std::vector<std::tuple<int,int,int>>& edges, int k)` that, given a connected undirected graph with `n` vertices (labeled 0 to n-1), `m` edges (each as a tuple of two endpoints and a positive weight), and a special vertex count `k` (where k ≤ n), returns the minimum possible total weight of a spanning tree that contains exactly the first `k` vertices as leaves? No, that's not from the snippet. Instead, base it on the validation: the snippet validates an undirected simple graph with n vertices, m edges, no self-loops, no multiple edges, and a list of k vertex labels (1..n) that are "special". The task: Given an undirected graph with n vertices and m weighted edges (no self-loops, no multiple edges), and a set of k special vertices (given as labels 1..n), write a function `bool isValidGraph(int n, int m, int k, const std::vector<std::tuple<int,int,int>>& edges, const std::vector<int>& specialVertices)` that returns `true` if and only if the input satisfies all constraints from the validator: n between 1 and 100000, m between 1 and 1000000, k between 1 and n, each edge endpoints in [0,n-1] with no self-loop and no duplicate undirected edge, each weight between 1 and 1e9, and special vertices are each between 1 and n (inclusive). The function should return `false` if any constraint is violated. This is essentially reimplementing the validation logic in a pure function without using testlib.
*/

#include <vector>
#include <tuple>
#include <set>
#include <utility>

/**
 * Validates the constraints of a graph problem.
 * 
 * @param n number of vertices
 * @param m number of edges
 * @param k number of special vertices
 * @param edges vector of tuples (u, v, weight) for each edge
 * @param specialVertices vector of length k with vertex labels 1..n
 * @return true if all constraints are satisfied, false otherwise
 */
bool isValidGraph(int n, int m, int k,
                  const std::vector<std::tuple<int,int,int>>& edges,
                  const std::vector<int>& specialVertices) {
    // Check basic bounds
    if (n < 1 || n > 100000) return false;
    if (m < 1 || m > 1000000) return false;
    if (k < 1 || k > n) return false;
    
    // Check edge count matches
    if (static_cast<int>(edges.size()) != m) return false;
    if (static_cast<int>(specialVertices.size()) != k) return false;
    
    std::set<std::pair<int,int>> seen;
    
    for (const auto& [a, b, w] : edges) {
        // Check vertex range and self-loop
        if (a < 0 || a >= n) return false;
        if (b < 0 || b >= n) return false;
        if (a == b) return false;
        
        // Check weight range
        if (w < 1 || w > 1000000000) return false;
        
        // Normalize pair for duplicate check
        int u = a, v = b;
        if (u > v) std::swap(u, v);
        auto res = seen.insert({u, v});
        if (!res.second) {
            return false; // duplicate edge
        }
    }
    
    // Check special vertices range
    for (int v : specialVertices) {
        if (v < 1 || v > n) return false;
    }
    
    return true;
}

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Valid simple case
    assert(isValidGraph(3, 2, 1, 
        {{0,1,5},{1,2,7}}, {2}) == true);
    
    // Self-loop
    assert(isValidGraph(2, 1, 1, 
        {{0,0,10}}, {1}) == false);
    
    // Duplicate edge (undirected)
    assert(isValidGraph(3, 2, 1, 
        {{0,1,5},{1,0,5}}, {1}) == false);
    
    // Out-of-range vertex
    assert(isValidGraph(3, 1, 1, 
        {{0,3,5}}, {1}) == false);
    
    // Weight too low
    assert(isValidGraph(2, 1, 1, 
        {{0,1,0}}, {1}) == false);
    
    // Weight too high
    assert(isValidGraph(2, 1, 1, 
        {{0,1,1000000001}}, {1}) == false);
    
    // k out of range
    assert(isValidGraph(2, 1, 3, 
        {{0,1,5}}, {1}) == false);
    
    // Special vertex out of range
    assert(isValidGraph(3, 1, 1, 
        {{0,1,5}}, {0}) == false);
    
    // n out of upper bound
    assert(isValidGraph(100001, 1, 1, 
        {{0,1,5}}, {1}) == false);
    
    // Valid with multiple edges and random order
    assert(isValidGraph(4, 3, 2, 
        {{1,2,10},{3,0,20},{2,1,5}}, {4,2}) == true);
    
    return 0;
}

// The solution must check each constraint in order. First verify bounds: n in [1,100000], m in [1,1000000], k in [1,n]. Then for each edge tuple, check both endpoints are in [0,n-1], and that they are not equal (no self-loop). To detect duplicate undirected edges efficiently, use a `std::set<std::pair<int,int>>` where each pair is normalized so the smaller endpoint comes first. Insert each normalized pair; if insertion fails (element already exists), then a duplicate edge exists and we return false. Also check the weight is in [1,1000000000]. Finally, for each special vertex, check it is in [1,n]. If any check fails, return false; otherwise return true. Complexity: O(m log m) due to set operations; space O(m) for the set. Edge cases: m=1 with n=1 would be a self-loop? Actually n>=1 and m>=1, but if n=1 then any edge would be a self-loop (since endpoints must be 0), so such input would be invalid because self-loop is forbidden. Also k can equal n, and special vertices may repeat? The validator only reads them, no uniqueness check, so repeats are allowed. So we just check range.
