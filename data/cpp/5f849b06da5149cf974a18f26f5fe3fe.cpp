Write a C++ function that takes a 4x4 integer matrix (represented as `std::array<std::array<int, 4>, 4>` or a simple 2D array) and a parameter `k` (where 0 < k < 4). The function should modify the matrix so that the top `k` rows are set to zero, while preserving all other rows unchanged. The function must operate in-place (i.e., mutate the input matrix) and return `void`. The function should handle arbitrary integer values including negatives and zeros, and should not use external libraries other than the standard C++ headers.
// The core operation is to zero out the first `k` rows of a 4x4 matrix. The algorithm is straightforward: iterate over rows 0 through k-1, and for each such row, iterate over all 4 columns, setting each element to 0. Rows from k to 3 remain unchanged. No special edge cases exist beyond validating `k` is between 1 and 3 inclusive (the function can assume valid input per the specification, but it's safe to clamp or handle gracefully if needed). Time complexity is O(k * 4) = O(1) since the matrix size is fixed at 4x4; space complexity is O(1) because we modify the input in-place without extra storage. The solution uses `const` where appropriate (e.g., not modifying the `k` parameter, and passing the matrix by reference to allow in-place mutation).
#include <array>
#include <cstddef>

// Zero out the top k rows (0-based indexing: rows 0 to k-1) of a 4x4 integer matrix.
// The matrix is modified in-place. k must be in [1, 3] for meaningful operation.
void zeroTopRows(std::array<std::array<int, 4>, 4>& matrix, int k) {
    for (int row = 0; row < k; ++row) {
        for (int col = 0; col < 4; ++col) {
            matrix[row][col] = 0;
        }
    }
}
#include <cassert>

int main() {
    // Test case 1: k=2, mixed values
    std::array<std::array<int, 4>, 4> m1 = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};
    zeroTopRows(m1, 2);
    assert(m1[0] == std::array<int, 4>{0, 0, 0, 0});
    assert(m1[1] == std::array<int, 4>{0, 0, 0, 0});
    assert(m1[2] == std::array<int, 4>{9, 10, 11, 12});
    assert(m1[3] == std::array<int, 4>{13, 14, 15, 16});

    // Test case 2: k=1, negative values
    std::array<std::array<int, 4>, 4> m2 = {{
        {{-1, -2, -3, -4}},
        {{5, 6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};
    zeroTopRows(m2, 1);
    assert(m2[0] == std::array<int, 4>{0, 0, 0, 0});
    assert(m2[1] == std::array<int, 4>{5, 6, 7, 8});
    assert(m2[2] == std::array<int, 4>{9, 10, 11, 12});
    assert(m2[3] == std::array<int, 4>{13, 14, 15, 16});

    // Test case 3: k=3, all rows zeroed except last
    std::array<std::array<int, 4>, 4> m3 = {{
        {{1, 1, 1, 1}},
        {{2, 2, 2, 2}},
        {{3, 3, 3, 3}},
        {{4, 4, 4, 4}}
    }};
    zeroTopRows(m3, 3);
    assert(m3[0] == std::array<int, 4>{0, 0, 0, 0});
    assert(m3[1] == std::array<int, 4>{0, 0, 0, 0});
    assert(m3[2] == std::array<int, 4>{0, 0, 0, 0});
    assert(m3[3] == std::array<int, 4>{4, 4, 4, 4});

    // Test case 4: k=0 (no change)
    std::array<std::array<int, 4>, 4> m4 = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};
    zeroTopRows(m4, 0);
    assert(m4[0] == std::array<int, 4>{1, 2, 3, 4});
    assert(m4[1] == std::array<int, 4>{5, 6, 7, 8});
    assert(m4[2] == std::array<int, 4>{9, 10, 11, 12});
    assert(m4[3] == std::array<int, 4>{13, 14, 15, 16});

    // Test case 5: all zeros already
    std::array<std::array<int, 4>, 4> m5 = {{
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}},
        {{0, 0, 0, 0}}
    }};
    zeroTopRows(m5, 2);
    assert(m5[0] == std::array<int, 4>{0, 0, 0, 0});
    assert(m5[1] == std::array<int, 4>{0, 0, 0, 0});
    assert(m5[2] == std::array<int, 4>{0, 0, 0, 0});
    assert(m5[3] == std::array<int, 4>{0, 0, 0, 0});

    return 0;
}
