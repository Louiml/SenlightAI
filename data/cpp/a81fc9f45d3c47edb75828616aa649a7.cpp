// Write a C++ function that takes as parameters a square matrix `A` of size `n x n` where each element is a pointer to an integer, and returns a boolean value. The function should return `true` if there exist two distinct positions in the matrix (different row and/or column indices) whose pointed-to integer values are equal, and `false` otherwise. The matrix may contain `nullptr` entries, which should be treated as non-matching (i.e., two `nullptr` entries do not count as a match). The function must handle any size `n >= 1`, and you may assume all non-null pointers point to valid integers.
// The solution compares every pair of distinct matrix positions. For each position `(i, j)`, iterate over all later positions `(x, y)` to avoid double-checking the same pair. For each comparison, first check that both pointers are non-null; if both are non-null and the dereferenced values are equal, return `true` immediately. If the loop completes without finding a match, return `false`. Edge cases include: the matrix containing only `nullptr` entries (return `false`), a single element matrix (return `false` since no distinct pair exists), and duplicate values at different positions (return `true`). The algorithm runs in `O(n^4)` time in the worst case because it checks all pairs of positions, and uses `O(1)` auxiliary space, ignoring the input matrix itself.
#include <cstddef>

// Returns true if any two distinct positions in the matrix point to equal integer values.
// Null pointers are ignored and never considered matching.
bool hasDuplicatePointedValues(int* const* const* A, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (A[i][j] == nullptr) continue;
            for (int x = i; x < n; ++x) {
                for (int y = (x == i ? j + 1 : 0); y < n; ++y) {
                    if (A[x][y] != nullptr && *A[i][j] == *A[x][y]) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
#include <cassert>

int main() {
    // Test 1: duplicate values at different positions -> true
    int v1 = 5, v2 = 5, v3 = 7;
    int* row0[] = {&v1, &v2};
    int* row1[] = {&v3, nullptr};
    int** A1[] = {row0, row1};
    assert(hasDuplicatePointedValues(A1, 2) == true);

    // Test 2: all distinct values -> false
    int a = 1, b = 2, c = 3, d = 4;
    int* r0[] = {&a, &b};
    int* r1[] = {&c, &d};
    int** A2[] = {r0, r1};
    assert(hasDuplicatePointedValues(A2, 2) == false);

    // Test 3: only null pointers -> false
    int* r0n[] = {nullptr, nullptr};
    int* r1n[] = {nullptr, nullptr};
    int** A3[] = {r0n, r1n};
    assert(hasDuplicatePointedValues(A3, 2) == false);

    // Test 4: single element matrix -> false
    int solo = 42;
    int* rs[] = {&solo};
    int** A4[] = {rs};
    assert(hasDuplicatePointedValues(A4, 1) == false);

    // Test 5: duplicate with a null in between -> true
    int x = 9, y = 9;
    int* p0[] = {&x, nullptr, &y};
    int* p1[] = {nullptr, nullptr, nullptr};
    int* p2[] = {&y, nullptr, &x};
    int** A5[] = {p0, p1, p2};
    assert(hasDuplicatePointedValues(A5, 3) == true);

    // Test 6: duplicate appears only in same pointer (not distinct) -> false
    int z = 8;
    int* q0[] = {&z, &z};
    int* q1[] = {nullptr, nullptr};
    int** A6[] = {q0, q1};
    assert(hasDuplicatePointedValues(A6, 2) == false);
}
