Write a C++ function that processes multiple test cases of square numeric matrices. For each matrix of size `N` (N >= 1), the function must compute the trace (sum of main diagonal elements), the number of rows that contain repeated values, and the number of columns that contain repeated values. The input is provided as a sequence of test cases: first an integer `T` (number of test cases), then for each test case an integer `N`, followed by `N*N` integers read in row-major order (each integer 0-9, representing a single digit). The function should return a `std::vector<std::string>` where each string is formatted as `"Case #i: trace rows cols"` (with `i` starting at 1), exactly as in the original snippet. The function must handle matrices with values 0-9, and a value is considered repeated if it appears more than once in a row or column. Assume `T >= 1` and all input values are valid digits.

The solution reads all test cases sequentially. For each matrix, the row-major values are stored in a flat string (or a 2D vector for clarity). To compute the trace, sum the element at index `(i, i)` for each `i` from 0 to N-1. To detect repeated rows, for each row `i`, use a temporary boolean array of size 10 (since digits 0-9) and mark each value; if a value is already marked, that row has a repeat, so increment the row count and break to the next row. Similarly, for each column `j`, use a fresh boolean array and iterate rows `i`; if a value is already marked, increment the column count and break. Edge cases: N=1 has no repeats (both row and column counts are 0), and the trace is the single element. Duplicates within a row/column are only counted once per row/column. Time complexity: For each matrix of size N, row checking is O(N^2) and column checking is O(N^2), plus O(N) for trace, so overall O(T * N^2). Space complexity: O(N^2) for storing the matrix, plus O(1) for the boolean arrays (size 10).

#include <vector>
#include <string>
#include <cstddef>

// Process T test cases of N x N numeric matrices and return formatted results.
std::vector<std::string> matrixAnalysis(const std::vector<int>& flatInput) {
    std::vector<std::string> answers;
    std::size_t pos = 0;
    int T = flatInput[pos++];

    for (int caseIdx = 1; caseIdx <= T; ++caseIdx) {
        int N = flatInput[pos++];
        std::vector<int> matrix(N * N);
        for (int idx = 0; idx < N * N; ++idx) {
            matrix[idx] = flatInput[pos++];
        }

        int trace = 0;
        int repeatedRows = 0;
        int repeatedCols = 0;

        // Compute trace and detect repeated rows
        for (int i = 0; i < N; ++i) {
            trace += matrix[i * N + i];

            bool seenRow[10] = {false};
            for (int j = 0; j < N; ++j) {
                int val = matrix[i * N + j];
                if (seenRow[val]) {
                    ++repeatedRows;
                    break;
                }
                seenRow[val] = true;
            }
        }

        // Detect repeated columns
        for (int j = 0; j < N; ++j) {
            bool seenCol[10] = {false};
            for (int i = 0; i < N; ++i) {
                int val = matrix[i * N + j];
                if (seenCol[val]) {
                    ++repeatedCols;
                    break;
                }
                seenCol[val] = true;
            }
        }

        answers.push_back("Case #" + std::to_string(caseIdx) + ": " +
                          std::to_string(trace) + " " +
                          std::to_string(repeatedRows) + " " +
                          std::to_string(repeatedCols));
    }
    return answers;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be included above.
int main() {
    // Test 1: Single 1x1 matrix, no repeats
    std::vector<int> input1 = {1, 1, 5};
    std::vector<std::string> out1 = matrixAnalysis(input1);
    assert(out1.size() == 1);
    assert(out1[0] == "Case #1: 5 0 0");

    // Test 2: 2x2 matrix with a repeated row and a repeated column
    // Matrix: [1 2; 2 1] -> trace=2, row0 no repeat, row1 no repeat, col0 no repeat, col1 no repeat
    std::vector<int> input2 = {1, 2, 1, 2, 2, 1};
    std::vector<std::string> out2 = matrixAnalysis(input2);
    assert(out2[0] == "Case #1: 2 0 0");

    // Test 3: 2x2 matrix with repeats
    // Matrix: [1 1; 2 2] -> trace=3, row0 repeat, row1 no, col0 no, col1 repeat
    std::vector<int> input3 = {1, 2, 1, 1, 2, 2};
    std::vector<std::string> out3 = matrixAnalysis(input3);
    assert(out3[0] == "Case #1: 3 1 1");

    // Test 4: 3x3 matrix with all rows/columns having repeats
    // Matrix: [1 1 1; 2 2 2; 3 3 3] -> trace=6, rows=3, cols=0 (each column has distinct values)
    std::vector<int> input4 = {1, 3, 1, 1, 1, 2, 2, 2, 3, 3, 3};
    std::vector<std::string> out4 = matrixAnalysis(input4);
    assert(out4[0] == "Case #1: 6 3 0");

    // Test 5: Multiple test cases
    // Case1: 1x1 [7] -> 7 0 0
    // Case2: 2x2 [1 2; 2 1] -> trace=2, 0 0
    // Case3: 2x2 [0 0; 0 0] -> trace=0, rows=2, cols=2
    std::vector<int> input5 = {3, 1, 7, 2, 1, 2, 2, 1, 2, 0, 0, 0, 0};
    std::vector<std::string> out5 = matrixAnalysis(input5);
    assert(out5.size() == 3);
    assert(out5[0] == "Case #1: 7 0 0");
    assert(out5[1] == "Case #2: 2 0 0");
    assert(out5[2] == "Case #3: 0 2 2");

    // Test 6: N=1 with digit 0
    std::vector<int> input6 = {1, 1, 0};
    std::vector<std::string> out6 = matrixAnalysis(input6);
    assert(out6[0] == "Case #1: 0 0 0");

    return 0;
}
