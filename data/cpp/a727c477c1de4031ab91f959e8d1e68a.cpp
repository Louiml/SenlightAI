// Write a C++ function `countMaxTruffles(int n, int rows, int cols)` where `n` is the number of test cases to process and `rows` and `cols` are positive integers representing the dimensions of a rectangular truffle field. The function should return a `std::vector<int>` containing, for each test case, the maximum number of whole 3×3 square patches that can be fully placed inside the field when the field is partitioned into 3×3 tiles from one corner. For each field, the answer is calculated as `(rows / 3) * (cols / 3)` (integer division, truncating toward zero). Assume `n ≥ 0`, and each `rows` and `cols` is at least 0. If `n` is 0, return an empty vector. The function must be self-contained, use only standard libraries, and avoid any input/output operations (no `printf`, `scanf`, `cin`, or `cout` in the function itself).

The problem derives from a counting exercise: given a rectangle of size `rows × cols`, how many non-overlapping 3×3 squares can be placed in it if aligned to a corner? Since each 3×3 square requires exactly 3 consecutive rows and 3 consecutive columns, the maximum number of such squares that fit along the width is `cols / 3` (integer division, discarding any remainder), and along the height is `rows / 3`. Multiplying these two counts gives the total number of 3×3 squares that tile the top-left portion of the rectangle. This works because the squares are placed in a grid from one corner, and any unused remainder rows or columns (less than 3) cannot form another full square. Edge cases: if `rows < 3` or `cols < 3`, then one of the factors becomes 0, resulting in a 0 answer; if both dimensions are multiples of 3, the answer is exactly `(rows/3)*(cols/3)`. The algorithm processes each test case independently in constant time, so for `n` cases, time complexity is `O(n)` and space complexity is `O(n)` for the output vector (or `O(1)` auxiliary if ignoring output storage).

#include <vector>

// For each test case, compute the number of full 3x3 blocks that fit in a rows x cols field.
// Returns a vector of results, one per test case.
std::vector<int> countMaxTruffles(int n, const std::vector<int>& rows, const std::vector<int>& cols) {
    std::vector<int> result;
    result.reserve(n);
    for (int i = 0; i < n; ++i) {
        // Integer division truncates toward zero; for non-negative values, it floors.
        const int blocksPerRow = rows[i] / 3;
        const int blocksPerCol = cols[i] / 3;
        result.push_back(blocksPerRow * blocksPerCol);
    }
    return result;
}

#include <cassert>
#include <vector>

// Solution function declaration (assume it's defined in the same translation unit or included above)
std::vector<int> countMaxTruffles(int n, const std::vector<int>& rows, const std::vector<int>& cols);

int main() {
    // Case 1: Normal dimensions
    assert(countMaxTruffles(1, {10}, {10}) == std::vector<int>{9}); // (3)*(3)=9
    // Case 2: Dimensions smaller than 3
    assert(countMaxTruffles(2, {2, 5}, {5, 2}) == std::vector<int>{0, 0});
    // Case 3: Exact multiples of 3
    assert(countMaxTruffles(1, {9}, {6}) == std::vector<int>{6}); // (3)*(2)=6
    // Case 4: Mixed sizes, multiple cases
    assert(countMaxTruffles(3, {1, 3, 7}, {3, 1, 8}) == std::vector<int>{0, 0, 4}); // (2)*(2)=4
    // Case 5: Zero cases
    assert(countMaxTruffles(0, {}, {}) == std::vector<int>{});
    // Case 6: Large values (still safe with int for typical sizes)
    assert(countMaxTruffles(1, {1000}, {999}) == std::vector<int>{111222}); // (333)*(334)=111222
    // Case 7: One dimension exactly 3, the other not
    assert(countMaxTruffles(1, {3, 4}, {4, 3}) == std::vector<int>{1, 1}); // (1)*(1)=1 for both
    // Case 8: Repeated values
    assert(countMaxTruffles(2, {6, 6}, {6, 6}) == std::vector<int>{4, 4});
    return 0;
}
