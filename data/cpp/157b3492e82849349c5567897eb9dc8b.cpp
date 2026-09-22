Write a standalone C++ function that, given a 2D integer array with a fixed number of columns (3) and a dynamic number of rows, along with the row count, returns a `std::pair<int,int>` where the first element is the maximum value and the second element is the minimum value across all elements in the array. The array may contain negative numbers, positive numbers, zeros, and duplicate values. Assume the array is non-empty (at least 1 row). The function must be `const`-correct (take the array as `const` reference or pointer to `const`, and not modify it). The return type should be a `std::pair` with `first` = maximum, `second` = minimum. The function must not accept a column count parameter (since columns are fixed to 3). Provide a single free function (no global variables, no `main`).

// The solution requires iterating through every element of a 2D array with a compile-time fixed number of columns (3) and a runtime number of rows. Because columns are fixed, the function signature can be `std::pair<int,int> findMinMax(const int arr[][3], int rows)` or equivalently `std::pair<int,int> findMinMax(const int (*arr)[3], int rows)`. The algorithm initializes `maxValue` to the smallest possible `int` (e.g., `INT_MIN` from `<climits>`) and `minValue` to the largest possible `int` (e.g., `INT_MAX`). Then it loops over each row index `i` from 0 to `rows-1` and each column index `j` from 0 to 2 (since columns = 3). For each element `arr[i][j]`, it updates `maxValue` if the element is greater, and `minValue` if the element is smaller. This works correctly for arrays containing negative values, zeros, and duplicates because the initialization to extreme sentinels ensures any real value will replace them on the first comparison. Edge cases: if `rows == 0` (which the task says won't happen, but if it did, the function would return `{INT_MIN, INT_MAX}` — an invalid pair; we can choose to ignore or handle gracefully, but given the task guarantees non-empty, we can assume `rows > 0`). Time complexity is `O(n)` where `n = rows * 3` (number of elements), and space complexity is `O(1)` extra space.

#include <utility>   // for std::pair
#include <climits>   // for INT_MIN and INT_MAX
#include <algorithm> // for std::max and std::min (optional, but used)

// Returns a pair containing (maximum, minimum) of all elements in a 3-column array.
// Assumes rows > 0. The array is not modified (const correctness).
std::pair<int, int> findMinMax(const int arr[][3], int rows) {
    int maxValue = INT_MIN;
    int minValue = INT_MAX;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < 3; ++j) {
            const int current = arr[i][j];
            maxValue = std::max(maxValue, current);
            minValue = std::min(minValue, current);
        }
    }

    return {maxValue, minValue};
}

#include <cassert>
#include <utility>

// Declare the solution function (as if from included header)
std::pair<int, int> findMinMax(const int arr[][3], int rows);

int main() {
    // Test 1: Simple positive increasing array
    int arr1[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
    auto res1 = findMinMax(arr1, 3);
    assert(res1.first == 9 && res1.second == 1);

    // Test 2: Negative numbers and zeros
    int arr2[2][3] = {{-5, 0, -3}, {10, -8, 2}};
    auto res2 = findMinMax(arr2, 2);
    assert(res2.first == 10 && res2.second == -8);

    // Test 3: Single row
    int arr3[1][3] = {{42, -100, 7}};
    auto res3 = findMinMax(arr3, 1);
    assert(res3.first == 42 && res3.second == -100);

    // Test 4: All identical values
    int arr4[2][3] = {{7,7,7},{7,7,7}};
    auto res4 = findMinMax(arr4, 2);
    assert(res4.first == 7 && res4.second == 7);

    // Test 5: Extreme values (but within int range)
    int arr5[1][3] = {{INT_MIN, 0, INT_MAX}};
    auto res5 = findMinMax(arr5, 1);
    assert(res5.first == INT_MAX && res5.second == INT_MIN);

    // Test 6: Larger rows, mixed signs
    int arr6[4][3] = {{1,-1,2},{-2,3,-3},{4,-4,5},{-5,6,-6}};
    auto res6 = findMinMax(arr6, 4);
    assert(res6.first == 6 && res6.second == -6);

    return 0;
}
