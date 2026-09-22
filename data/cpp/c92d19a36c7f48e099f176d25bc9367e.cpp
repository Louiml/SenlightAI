/*
Given a 2D vector `a` of size `arr_size x 10`, where each row contains exactly 10 integers in the range 0 to 3 (inclusive), write a C++ function `radixSortColumns` that sorts the rows lexicographically using radix sort, processing columns from the last (index 9) to the first (index 0), exactly as demonstrated in the provided snippet. The function must modify the input vector in place and use counting sort for each digit position with a digit range of 0–3. The rows are ordered such that after sorting, the sequence of rows is sorted by the entire 10-digit vector lexicographic order (i.e., compare row[0], then row[1], ..., up to row[9]). The input will always have a positive number of rows, and each element is guaranteed to be 0, 1, 2, or 3. The function must not print anything. Provide a free function named `radixSortColumns` that takes a `std::vector<std::vector<int>>&` as its only parameter.
*/

#include <vector>

// Sort rows of a 2D vector lexicographically using LSD radix sort on columns (0..9).
// Each element must be in the range 0..3 (digit range k=4).
void radixSortColumns(std::vector<std::vector<int>>& a) {
    const int numDigits = 10;  // each row has 10 columns
    const int digitRange = 4;  // values 0,1,2,3

    for (int p = numDigits - 1; p >= 0; --p) {
        int n = a.size();
        // Bucket vector b, same size as a, initialized with zeros in each row.
        std::vector<std::vector<int>> b(n, std::vector<int>(numDigits, 0));

        // Count occurrences of each digit value at column p.
        std::vector<int> c(digitRange, 0);
        for (int i = 0; i < n; ++i) {
            ++c[a[i][p]];
        }

        // Cumulative counts to determine final positions.
        for (int i = 1; i < digitRange; ++i) {
            c[i] += c[i - 1];
        }

        // Stable placement from the end.
        for (int i = n - 1; i >= 0; --i) {
            int digit = a[i][p];
            b[c[digit] - 1] = a[i];
            --c[digit];
        }

        // Copy back.
        a = b;
    }
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test.
void radixSortColumns(std::vector<std::vector<int>>& a);

int main() {
    // Test 1: Single row (no change).
    std::vector<std::vector<int>> t1 = {{0,1,2,3,0,1,2,3,0,1}};
    radixSortColumns(t1);
    assert(t1[0][0] == 0 && t1[0][9] == 1);

    // Test 2: Two rows already sorted.
    std::vector<std::vector<int>> t2 = {
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,1}
    };
    radixSortColumns(t2);
    assert(t2[0][9] == 0 && t2[1][9] == 1);

    // Test 3: Reverse order, last column differs.
    std::vector<std::vector<int>> t3 = {
        {1,1,1,1,1,1,1,1,1,3},
        {1,1,1,1,1,1,1,1,1,1}
    };
    radixSortColumns(t3);
    assert(t3[0][9] == 1 && t3[1][9] == 3);

    // Test 4: Sort by earlier columns when later columns equal.
    std::vector<std::vector<int>> t4 = {
        {0,2,0,0,0,0,0,0,0,0},
        {0,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    };
    radixSortColumns(t4);
    assert(t4[0][1] == 0 && t4[1][1] == 1 && t4[2][1] == 2);

    // Test 5: All identical rows.
    std::vector<std::vector<int>> t5 = {
        {3,3,3,3,3,3,3,3,3,3},
        {3,3,3,3,3,3,3,3,3,3}
    };
    radixSortColumns(t5);
    assert(t5[0] == t5[1]);
    assert(t5[0][0] == 3 && t5[0][9] == 3);

    // Test 6: Mixed multi-column sort (lexicographic check).
    std::vector<std::vector<int>> t6 = {
        {2,3,1,0,2,1,3,0,1,2},
        {2,3,1,0,2,1,3,0,1,1},
        {2,3,1,0,2,1,3,0,1,0}
    };
    radixSortColumns(t6);
    assert(t6[0][9] == 0 && t6[1][9] == 1 && t6[2][9] == 2);

    // Test 7: Larger set, verify stable sort by first column.
    std::vector<std::vector<int>> t7 = {
        {3,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,0,0,0},
        {2,0,0,0,0,0,0,0,0,0}
    };
    radixSortColumns(t7);
    assert(t7[0][0] == 0 && t7[1][0] == 1 && t7[2][0] == 2 && t7[3][0] == 3);

    // Test 8: Random-like small case.
    std::vector<std::vector<int>> t8 = {
        {0,1,2,3,0,1,2,3,3,2},
        {0,1,2,3,0,1,2,3,3,1},
        {0,1,2,3,0,1,2,3,3,0}
    };
    radixSortColumns(t8);
    assert(t8[0][9] == 0 && t8[1][9] == 1 && t8[2][9] == 2);

    // Test 9: Verify entire row order for known case.
    std::vector<std::vector<int>> t9 = {
        {0,0,0,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,0,0,0}
    };
    radixSortColumns(t9);
    assert(t9[0][9] == 0 && t9[1][9] == 1);

    // Test 10: Edge case with many rows (10 rows), simple pattern.
    std::vector<std::vector<int>> t10(10, std::vector<int>(10, 0));
    for (int i = 0; i < 10; ++i) {
        t10[i][9] = i % 4;
    }
    radixSortColumns(t10);
    assert(t10[0][9] == 0 && t10[3][9] == 0 && t10[4][9] == 1 && t10[9][9] == 3);

    return 0;
}

// The algorithm is a standard least-significant-digit (LSD) radix sort applied to each column of the 2D vector. Since each row has exactly 10 digits, we iterate `p` from 9 down to 0. For each digit position, we perform a stable counting sort: first, we create an empty bucket vector `b` of the same size as `a`, and fill each sub-vector with 10 zeros (though only the current column's placement matters, the code in the snippet copies the whole row). Then we count occurrences of each digit value (0..3) using a `c` array of size 4, compute cumulative counts, and place rows from the end of the input to the beginning into `b` at positions determined by `c`, decrementing the count. Finally, we copy the sorted rows from `b` back into `a`. This ensures stability, so lexicographic order over all 10 columns is achieved. Edge cases: each row must have exactly 10 elements; if not, the function would access out-of-bounds, but the problem guarantees this. Complexity: for each of the 10 digits, counting sort runs in O(N + k) where N is the number of rows and k = 4 (digit range), so total time is O(10 * (N + 4)) = O(N) with a small constant. Space complexity is O(N * 10) for the bucket vector `b`, plus O(4) for counts, so O(N) auxiliary space.
