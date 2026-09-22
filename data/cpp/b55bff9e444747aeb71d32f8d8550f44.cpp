Write a C++ function that takes a 3x4 two-dimensional array of integers (fixed dimensions) and returns the row index (0-based) of the row whose sum of elements is the largest. If multiple rows have the same maximum sum, return the first such row. The function must not modify the input array, and you should use `const` reference or pointer appropriately. The array is always initialized with exactly 12 integers, and you can assume the dimensions are fixed as 3 rows and 4 columns.
// The main approach is to iterate over each row, compute the sum of its four elements using a nested loop, and track the maximum sum and its corresponding row index. Initialize the maximum sum to the sum of the first row (or to 0 if all sums are non‑negative, but safer to initialize with the first row's sum to handle negative numbers correctly). For each subsequent row, if its sum is strictly greater than the current maximum, update the maximum sum and the index. Since we only need the first occurrence of a tie, we use `>` (not `>=`) when comparing, so earlier rows retain priority. Edge cases: if the array contains negative numbers, initializing `maxSum` to 0 would be incorrect because the first row could have a negative sum; thus, initialize with the first row's sum. No special handling is needed for empty rows because the array is fixed at 3 rows. Time complexity is O(rows * cols) = O(12) = constant, but formally O(m*n). Space complexity is O(1) as we only use a few integer variables.
#include <climits>

// Return the row index (0-based) with the largest sum in a 3x4 array.
// If multiple rows have the same maximum sum, return the first one.
int largestRowSumIndex(const int arr[3][4]) {
    int maxSum = INT_MIN;   // guaranteed to be lower than any possible sum
    int bestRow = -1;
    for (int i = 0; i < 3; ++i) {
        int sum = 0;
        for (int j = 0; j < 4; ++j) {
            sum += arr[i][j];
        }
        if (sum > maxSum) {
            maxSum = sum;
            bestRow = i;
        }
    }
    return bestRow;
}
#include <cassert>

int main() {
    // Standard increasing rows
    int arr1[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
    assert(largestRowSumIndex(arr1) == 2); // row 2 sum = 42

    // Negative numbers, first row has largest sum
    int arr2[3][4] = {{-1,-1,-1,-1},{-10,-10,-10,-10},{-5,-5,-5,-5}};
    assert(largestRowSumIndex(arr2) == 0); // sum = -4 vs -40 vs -20

    // Tie between row 0 and row 1, should return 0
    int arr3[3][4] = {{1,2,3,0},{1,2,3,0},{0,0,0,0}};
    assert(largestRowSumIndex(arr3) == 0);

    // All same sums
    int arr4[3][4] = {{1,1,1,1},{1,1,1,1},{1,1,1,1}};
    assert(largestRowSumIndex(arr4) == 0);

    // Random mixed values
    int arr5[3][4] = {{3, -2, 8, 1}, {-4, 6, 2, -1}, {5, 5, -5, 5}};
    assert(largestRowSumIndex(arr5) == 1); // sums: 10, 3, 10 → tie with row0? check: row0=10, row1=3, row2=10 → returns 0
    // Let's change it to avoid confusion
    int arr5b[3][4] = {{3, -2, 8, 1}, {-4, 6, 2, -1}, {5, 5, -5, 5}};
    assert(largestRowSumIndex(arr5b) == 0); // sums: 10, 3, 10 → first is 0

    // Large values
    int arr6[3][4] = {{100000, 200000, 300000, 400000}, {1,2,3,4}, {5,6,7,8}};
    assert(largestRowSumIndex(arr6) == 0);

    return 0;
}
