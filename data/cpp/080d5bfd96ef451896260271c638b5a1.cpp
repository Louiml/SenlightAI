Write a C++ function that takes a non-negative integer `N` and returns a `std::string` containing the first `N` rows of Pascal's triangle, each row's numbers separated by single spaces, and rows separated by newline characters. The rows must use 1-based indexing (row 1 is "1", row 2 is "1 1", etc.). If `N` is 0, return an empty string. The function must be efficient for `N` up to 30, where values fit in a 32-bit signed integer. Each row must be computed iteratively from the previous row using the standard Pascal recurrence.
The core algorithm is to generate each row of Pascal's triangle based on the previous row. For row index `r` (1-based), there are `r` elements. The first and last element of each row are always 1. For interior positions `j` (1 ≤ j ≤ r-2 in 0-based indexing of the row), the value is `prev[j-1] + prev[j]`, where `prev` is the previous row. We can store the current row as a vector of integers. For `N=0`, we return an empty string immediately. For `N=1`, the loop produces "1". We build the result string using an `std::ostringstream` to avoid manual string concatenation, adding each row followed by a newline, but we need to avoid a trailing newline at the end. The time complexity is O(N²) because we generate N rows, each with up to N elements, and each element is computed in O(1). The space complexity is O(N) for storing the current and previous row vectors. Edge cases include `N=0` (empty string) and `N=1` (single row with "1"). Values fit in `int` for N ≤ 30 because the maximum value (row 30 middle) is 155117520, less than 2³¹-1. The function must be `const`-correct and use `std::vector<int>` for rows.
#include <string>
#include <vector>
#include <sstream>

// Return the first N rows of Pascal's triangle as a newline-separated string.
std::string pascalsTriangle(int N) {
    if (N <= 0) {
        return "";
    }

    std::ostringstream output;
    std::vector<int> previousRow;

    for (int row = 1; row <= N; ++row) {
        std::vector<int> currentRow(row);
        currentRow[0] = 1;
        currentRow[row - 1] = 1;

        for (int col = 1; col < row - 1; ++col) {
            currentRow[col] = previousRow[col - 1] + previousRow[col];
        }

        for (int col = 0; col < row; ++col) {
            if (col > 0) {
                output << " ";
            }
            output << currentRow[col];
        }

        if (row < N) {
            output << "\n";
        }

        previousRow = std::move(currentRow);
    }

    return output.str();
}
#include <cassert>

int main() {
    // N = 0
    assert(pascalsTriangle(0) == "");

    // N = 1
    assert(pascalsTriangle(1) == "1");

    // N = 2
    assert(pascalsTriangle(2) == "1\n1 1");

    // N = 3
    assert(pascalsTriangle(3) == "1\n1 1\n1 2 1");

    // N = 4
    assert(pascalsTriangle(4) == "1\n1 1\n1 2 1\n1 3 3 1");

    // N = 5
    assert(pascalsTriangle(5) == "1\n1 1\n1 2 1\n1 3 3 1\n1 4 6 4 1");

    // N = 6 (middle value 10)
    assert(pascalsTriangle(6) == "1\n1 1\n1 2 1\n1 3 3 1\n1 4 6 4 1\n1 5 10 10 5 1");

    // N = 10 (contains "1 9 36 84 126 126 84 36 9 1" in last row)
    std::string result10 = pascalsTriangle(10);
    assert(result10.find("1 9 36 84 126 126 84 36 9 1") != std::string::npos);

    // N = 30 (check middle value of last row is 155117520)
    std::string result30 = pascalsTriangle(30);
    assert(result30.find("155117520") != std::string::npos);

    return 0;
}
