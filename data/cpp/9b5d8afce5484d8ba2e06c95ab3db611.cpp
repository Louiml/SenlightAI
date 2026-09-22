Write a C++ function that takes a square matrix represented as a `std::vector<std::vector<int>>` and returns a `std::vector<int>` containing the product of each row, in the same order as the rows. Each row's product is computed by multiplying all its elements together. The matrix may contain zeros, negative numbers, and rows of any length (though all rows have the same length in a valid matrix). The function must handle potential integer overflow by returning `long long` values. If the matrix is empty (no rows), return an empty vector. If a row is empty, its product should be 1 (the multiplicative identity). The function should be named `rowProducts` and should accept the matrix by `const` reference.

// The solution iterates over each row of the matrix. For each row, initialize a `long long` accumulator to 1, then multiply each element into the accumulator. Since the matrix is square but rows may conceptually be empty in edge cases, handle that with the identity product. If the matrix is empty, return an empty vector. All rows have the same length by contract, but we do not assume non-emptiness. Complexity: let `n` be the number of rows and `m` be the number of columns (or row length). Time is `O(n*m)` because each element is visited once. Space is `O(n)` for the output vector, plus `O(1)` auxiliary. Edge cases: empty matrix → empty output; empty row → product 1; overflow: use `long long` to reduce risk, but note that extremely large values may still overflow `long long`, so the type is chosen to accommodate typical test cases.

#include <vector>
#include <cstdint>

// Compute the product of each row of a matrix, returning products as long long.
std::vector<long long> rowProducts(const std::vector<std::vector<int>>& matrix) {
    std::vector<long long> result;
    result.reserve(matrix.size());

    for (const auto& row : matrix) {
        long long product = 1;
        for (int value : row) {
            product *= static_cast<long long>(value);
        }
        result.push_back(product);
    }

    return result;
}

#include <cassert>
#include <vector>

// Prototype (or include header with rowProducts definition)
std::vector<long long> rowProducts(const std::vector<std::vector<int>>& matrix);

int main() {
    // Basic 3x3 test
    std::vector<std::vector<int>> m1 = {
        {1, 2, 3},
        {4, 5, 6},
        {-1, 0, 2}
    };
    auto p1 = rowProducts(m1);
    assert(p1.size() == 3);
    assert(p1[0] == 6);
    assert(p1[1] == 120);
    assert(p1[2] == 0);

    // Single row with negative values
    std::vector<std::vector<int>> m2 = {{-2, -3, 4}};
    auto p2 = rowProducts(m2);
    assert(p2.size() == 1);
    assert(p2[0] == 24);

    // Empty matrix
    std::vector<std::vector<int>> m3;
    auto p3 = rowProducts(m3);
    assert(p3.empty());

    // Matrix with an empty row and a normal row
    std::vector<std::vector<int>> m4 = {{}, {5, 7}};
    auto p4 = rowProducts(m4);
    assert(p4.size() == 2);
    assert(p4[0] == 1);
    assert(p4[1] == 35);

    // Matrix with zeros and negative numbers
    std::vector<std::vector<int>> m5 = {
        {0, 0, 0},
        {-1, -1, -1},
        {2, -3, 4}
    };
    auto p5 = rowProducts(m5);
    assert(p5[0] == 0);
    assert(p5[1] == -1);
    assert(p5[2] == -24);

    // Large values to test long long usage
    std::vector<std::vector<int>> m6 = {
        {100000, 100000},
        {-100000, -100000}
    };
    auto p6 = rowProducts(m6);
    assert(p6[0] == 10000000000LL);
    assert(p6[1] == 10000000000LL);

    // Single element matrix
    std::vector<std::vector<int>> m7 = {{42}};
    auto p7 = rowProducts(m7);
    assert(p7.size() == 1);
    assert(p7[0] == 42);

    // Two rows, one with single zero
    std::vector<std::vector<int>> m8 = {{0}, {1, 2, 3}};
    auto p8 = rowProducts(m8);
    assert(p8[0] == 0);
    assert(p8[1] == 6);

    return 0;
}
