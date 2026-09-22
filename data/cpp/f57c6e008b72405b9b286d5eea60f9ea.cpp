Write a C++ function named `oriented_facets` that accepts a matrix `F` representing triangular faces of a triangle mesh, where each row contains three vertex indices in counter-clockwise order. The function must return a matrix `E` of size `3 * F.rows() × 2` containing the three oriented boundary edges of each triangle, such that for triangle `(a, b, c)`, the edges are output in the order `(b, c)`, `(c, a)`, `(a, b)`. Each edge is a pair of vertex indices that follows the same orientation as the triangle’s winding. The input matrix uses integer indices, and the output matrix must have an integer scalar type. The function should only support triangular faces (exactly 3 columns per row); if any row has a different number of columns, the behavior is undefined. The function signature is: `void oriented_facets(const std::vector<std::vector<int>>& F, std::vector<std::vector<int>>& E);` where `E` is resized by the function to hold exactly `3 * F.size()` rows and 2 columns. The function must be `const`-correct for the input.

// The algorithm iterates over each triangular face in the input matrix. For each face with vertices `a = F[i][0]`, `b = F[i][1]`, `c = F[i][2]`, we append three oriented edges `(b, c)`, `(c, a)`, `(a, b)` to the output list. This preserves the cyclic order of the triangle’s vertices. Since the input is guaranteed to have exactly three columns, no special edge cases arise beyond empty input (which yields an empty output). The time complexity is O(n) where n is the number of triangles, because each triangle produces a constant number of edges. The auxiliary space is O(n) for the output matrix, and O(1) additional space besides that. No sorting or deduplication is required, because edges are emitted per-face with orientation; if duplicate edges appear between adjacent triangles, they will be in opposite directions and are intentionally left as separate entries.

#include <vector>

/**
 * Generates oriented edges from triangular faces.
 * 
 * @param F Input matrix of triangles, each row contains three vertex indices.
 * @param E Output matrix of edges, each row is an oriented pair (v1, v2).
 *        Resized to hold exactly 3 * F.size() edges.
 */
void oriented_facets(const std::vector<std::vector<int>>& F,
                     std::vector<std::vector<int>>& E) {
    E.clear();
    E.reserve(F.size() * 3);
    
    for (const auto& face : F) {
        // Assume face has exactly 3 elements.
        int a = face[0];
        int b = face[1];
        int c = face[2];
        
        // Oriented edges following the winding order.
        E.push_back({b, c});
        E.push_back({c, a});
        E.push_back({a, b});
    }
}

#include <cassert>
#include <vector>

// Declaration of the function under test.
void oriented_facets(const std::vector<std::vector<int>>& F,
                     std::vector<std::vector<int>>& E);

int main() {
    // Test 1: Single triangle.
    std::vector<std::vector<int>> F1 = {{0, 1, 2}};
    std::vector<std::vector<int>> E1;
    oriented_facets(F1, E1);
    assert(E1.size() == 3);
    assert(E1[0] == std::vector<int>({1, 2}));
    assert(E1[1] == std::vector<int>({2, 0}));
    assert(E1[2] == std::vector<int>({0, 1}));

    // Test 2: Multiple triangles.
    std::vector<std::vector<int>> F2 = {{0, 1, 2}, {2, 1, 3}};
    std::vector<std::vector<int>> E2;
    oriented_facets(F2, E2);
    assert(E2.size() == 6);
    assert(E2[0] == std::vector<int>({1, 2}));
    assert(E2[1] == std::vector<int>({2, 0}));
    assert(E2[2] == std::vector<int>({0, 1}));
    assert(E2[3] == std::vector<int>({1, 3}));
    assert(E2[4] == std::vector<int>({3, 2}));
    assert(E2[5] == std::vector<int>({2, 1}));

    // Test 3: Empty input.
    std::vector<std::vector<int>> F3;
    std::vector<std::vector<int>> E3;
    oriented_facets(F3, E3);
    assert(E3.empty());

    // Test 4: Non-consecutive indices and larger values.
    std::vector<std::vector<int>> F4 = {{10, 20, 15}, {15, 20, 25}, {0, 5, 10}};
    std::vector<std::vector<int>> E4;
    oriented_facets(F4, E4);
    assert(E4.size() == 9);
    assert(E4[0] == std::vector<int>({20, 15}));
    assert(E4[1] == std::vector<int>({15, 10}));
    assert(E4[2] == std::vector<int>({10, 20}));
    assert(E4[3] == std::vector<int>({20, 25}));
    assert(E4[4] == std::vector<int>({25, 15}));
    assert(E4[5] == std::vector<int>({15, 20}));
    assert(E4[6] == std::vector<int>({5, 10}));
    assert(E4[7] == std::vector<int>({10, 0}));
    assert(E4[8] == std::vector<int>({0, 5}));

    // Test 5: Reusing output matrix after prior data.
    std::vector<std::vector<int>> E5 = {{999, 999}};
    oriented_facets(F1, E5);
    assert(E5.size() == 3);
    assert(E5[0] == std::vector<int>({1, 2}));
    assert(E5[1] == std::vector<int>({2, 0}));
    assert(E5[2] == std::vector<int>({0, 1}));

    return 0;
}
