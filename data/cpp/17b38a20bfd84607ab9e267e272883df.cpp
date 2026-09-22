/*
Write a C++ function `std::vector<std::vector<int>> generateNumberTriangle(int n)` that, given a non-negative integer `n`, returns a vector of `n` vectors (rows) where row `i` (0-indexed) contains exactly `i+1` consecutive positive integers starting from 1 and continuing from the last number used in the previous row. The first row contains the single number `1`, the second row contains `2 3`, the third row contains `4 5 6`, and so on. The function should return an empty vector when `n` is 0. The input may be as large as 1000, so the returned structure must be built efficiently. Edge cases include `n = 0` (empty result) and large `n` where the last number equals `n*(n+1)/2`.
*/
#include <vector>

// Return a triangular array where each row i contains i+1 consecutive integers
// continuing sequentially from the previous row's last number.
std::vector<std::vector<int>> generateNumberTriangle(int n) {
    std::vector<std::vector<int>> triangle;
    if (n <= 0) {
        return triangle;
    }

    int current_number = 1;
    for (int row = 0; row < n; ++row) {
        std::vector<int> current_row;
        current_row.reserve(row + 1); // Optimize by pre-allocating row capacity
        for (int col = 0; col <= row; ++col) {
            current_row.push_back(current_number);
            ++current_number;
        }
        triangle.push_back(current_row);
    }

    return triangle;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.
int main() {
    // n = 0 returns empty
    assert(generateNumberTriangle(0).empty());

    // n = 1: single row [1]
    std::vector<std::vector<int>> t1 = generateNumberTriangle(1);
    assert(t1.size() == 1);
    assert(t1[0] == std::vector<int>({1}));

    // n = 3: [[1], [2,3], [4,5,6]]
    std::vector<std::vector<int>> t3 = generateNumberTriangle(3);
    assert(t3.size() == 3);
    assert(t3[0] == std::vector<int>({1}));
    assert(t3[1] == std::vector<int>({2, 3}));
    assert(t3[2] == std::vector<int>({4, 5, 6}));

    // n = 4: last element should be 10
    std::vector<std::vector<int>> t4 = generateNumberTriangle(4);
    assert(t4[3].back() == 10);
    assert(t4[3].size() == 4);
    assert(t4[2].back() == 6);

    // n = 5: total elements = 15, check sequential order across rows
    std::vector<std::vector<int>> t5 = generateNumberTriangle(5);
    int expected = 1;
    for (const auto& row : t5) {
        for (int val : row) {
            assert(val == expected);
            ++expected;
        }
    }
    assert(expected == 16); // 1 + 15

    // n = 1000: just check size and last value for performance validation
    std::vector<std::vector<int>> t1000 = generateNumberTriangle(1000);
    assert(t1000.size() == 1000);
    assert(t1000[999].back() == 1000 * 1001 / 2);
}
// The algorithm is straightforward: iterate from row index `i = 0` to `n-1`. For each row, create a vector of size `i+1` and fill it with consecutive integers starting from a running counter that begins at 1 and increments after each insertion. The running counter is shared across all rows, so we simply push back the current counter value into the row's vector and then increment it. After filling a row, push the row into the result. The main edge case is `n = 0`: the loop does not execute, and we return an empty vector. Another edge case is very large `n` (e.g., 1000), but the total number of integers is `n*(n+1)/2`, which for `n=1000` is about 500,500 — well within typical memory limits. The time complexity is `O(n^2)` because the total number of elements is `n*(n+1)/2`, which grows quadratically. The auxiliary space complexity is also `O(n^2)` because we store all those integers in the result. No auxiliary data structures beyond the output are needed.
