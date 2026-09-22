Write a C++ function that takes a 2D integer array with fixed dimensions (N_FIL rows and N_COL columns, where N_FIL = 30 and N_COL = 10), along with a row index, and returns a pair containing the maximum value and the sum of all elements in that row. The function must be pure (no side effects), properly const-correct, and handle the edge case where the row contains negative numbers. The input array is guaranteed to have at least one element per row, so no empty-row handling is needed. The function should be named `rowMaxAndSum` and return a `std::pair<int, int>` where the first element is the maximum and the second is the sum of the row.
#include <cassert>
#include <utility>

// Declare the function from the solution (in practice, this would be in a header)
std::pair<int, int> rowMaxAndSum(const int D[N_FIL][N_COL], int rowIndex);

int main() {
    // Test case 1: Simple increasing values
    int D1[N_FIL][N_COL] = {};
    for (int col = 0; col < N_COL; ++col) D1[0][col] = col + 1;
    auto result1 = rowMaxAndSum(D1, 0);
    assert(result1.first == 10 && result1.second == 55);

    // Test case 2: All negative values
    int D2[N_FIL][N_COL] = {};
    for (int col = 0; col < N_COL; ++col) D2[0][col] = -(col + 1);
    auto result2 = rowMaxAndSum(D2, 0);
    assert(result2.first == -1 && result2.second == -55);

    // Test case 3: Mixed values with negative numbers
    int D3[N_FIL][N_COL] = {};
    D3[0][0] = -5; D3[0][1] = 3; D3[0][2] = -2; D3[0][3] = 7;
    for (int col = 4; col < N_COL; ++col) D3[0][col] = 0;
    auto result3 = rowMaxAndSum(D3, 0);
    assert(result3.first == 7 && result3.second == 3);

    // Test case 4: All identical values
    int D4[N_FIL][N_COL] = {};
    for (int col = 0; col < N_COL; ++col) D4[0][col] = 5;
    auto result4 = rowMaxAndSum(D4, 0);
    assert(result4.first == 5 && result4.second == 50);

    // Test case 5: Different row (row 2) with descending values
    int D5[N_FIL][N_COL] = {};
    for (int col = 0; col < N_COL; ++col) D5[2][col] = 10 - col;
    auto result5 = rowMaxAndSum(D5, 2);
    assert(result5.first == 10 && result5.second == 55);

    return 0;
}
#include <utility>
#include <cstddef>

// Fixed dimensions as specified in the problem context
constexpr std::size_t N_FIL = 30;
constexpr std::size_t N_COL = 10;

// Pre: rowIndex is a valid row index (0 <= rowIndex < N_FIL)
// Post: Returns a pair where first = maximum element in that row,
//       second = sum of all elements in that row
std::pair<int, int> rowMaxAndSum(const int D[N_FIL][N_COL], int rowIndex) {
    int maxVal = D[rowIndex][0];
    int sumVal = D[rowIndex][0];
    
    for (std::size_t col = 1; col < N_COL; ++col) {
        int currentVal = D[rowIndex][col];
        if (currentVal > maxVal) {
            maxVal = currentVal;
        }
        sumVal += currentVal;
    }
    
    return {maxVal, sumVal};
}
// The solution iterates through each column of the specified row exactly once. Initialize both the maximum and the sum to the value of the first element (column 0) to avoid the need for special handling of the first iteration. Then for each subsequent column, update the maximum by comparing the current element to the running maximum, and add the element to the running sum. This approach naturally handles negative numbers because the maximum is only updated when a strictly larger value is encountered, and the sum accumulates all values regardless of sign. The algorithm runs in O(N_COL) time per row since it visits each column exactly once, and uses O(1) auxiliary space because it only stores two integer accumulators. Since the dimensions are fixed constants, the complexity is effectively O(1) in the context of the problem but theoretically linear in the number of columns. Edge cases to consider: single-column rows (handle by the initialization), all-negative rows (maximum will be the largest negative value), and rows with large magnitudes (no overflow assumptions beyond standard int range).
