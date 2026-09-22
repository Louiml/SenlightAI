Write a C++ function named `reverseEachRow` that takes two integers, `rows` and `cols`, and a two-dimensional vector of characters (where each inner vector represents a row of exactly `cols` characters), and returns a new two-dimensional vector where each row's characters have been reversed in order. For example, a row containing `{'a','b','c'}` should become `{'c','b','a'}`. The function must preserve the original number of rows and columns, work for any valid dimensions (including 0 rows or 0 columns), and must not modify the input vector. Assume all input characters are printable ASCII. The function signature should be: `std::vector<std::vector<char>> reverseEachRow(int rows, int cols, const std::vector<std::vector<char>>& grid);`.
// The solution iterates through each row of the input grid. For each row, we create a new reversed copy by traversing the original row from its last index down to its first index and pushing those characters into a new vector. This is done for every row, producing an output grid of identical dimensions. The algorithm is straightforward: for each row, the inner loop runs `cols` times, and there are `rows` rows, so the total number of character operations is `rows * cols`. The time complexity is therefore O(rows * cols), which is optimal since we must touch every element. The auxiliary space is also O(rows * cols) for the output grid, plus O(cols) for the temporary row vector during construction, but since we build the output directly, the additional space is just the output itself (if we count the output, it's O(rows * cols); if we exclude it, the in-progress temporary is O(cols)). Edge cases include empty grids (rows=0 or cols=0) — in either case, the function should return an empty grid or grid with empty rows accordingly. Also, rows with varying sizes in the input ( if not all of length `cols`) should be handled gracefully — we assume the input is well-formed, but we could also use `row.size()` to be robust rather than relying on `cols` for each row; however, the specification guarantees each row is exactly `cols` characters. We ensure `const` correctness by taking the input grid as a const reference.
#include <vector>
#include <algorithm>

// Reverse each row of a 2D character grid and return the result.
// The input grid is not modified. Rows are assumed to have exactly 'cols' characters.
std::vector<std::vector<char>> reverseEachRow(int rows, int cols, const std::vector<std::vector<char>>& grid) {
    std::vector<std::vector<char>> reversedGrid;
    reversedGrid.reserve(rows);

    for (const auto& row : grid) {
        std::vector<char> reversedRow;
        reversedRow.reserve(cols);
        // Traverse from the end to the beginning
        for (int j = static_cast<int>(row.size()) - 1; j >= 0; --j) {
            reversedRow.push_back(row[j]);
        }
        reversedGrid.push_back(reversedRow);
    }

    return reversedGrid;
}
#include <cassert>
#include <vector>

// (the function reverseEachRow is assumed to be included above)

int main() {
    // Test 1: Normal 3x3 grid
    std::vector<std::vector<char>> grid1 = {
        {'a','b','c'},
        {'d','e','f'},
        {'g','h','i'}
    };
    auto result1 = reverseEachRow(3, 3, grid1);
    assert(result1 == std::vector<std::vector<char>>({
        {'c','b','a'},
        {'f','e','d'},
        {'i','h','g'}
    }));

    // Test 2: Single row with single character
    std::vector<std::vector<char>> grid2 = {{'x'}};
    auto result2 = reverseEachRow(1, 1, grid2);
    assert(result2 == std::vector<std::vector<char>>({{'x'}}));

    // Test 3: Empty grid (rows=0)
    std::vector<std::vector<char>> grid3;
    auto result3 = reverseEachRow(0, 5, grid3);
    assert(result3.empty());

    // Test 4: Grid with zero columns (each row empty)
    std::vector<std::vector<char>> grid4 = {{}, {}, {}};
    auto result4 = reverseEachRow(3, 0, grid4);
    assert(result4 == std::vector<std::vector<char>>({{}, {}, {}}));

    // Test 5: Wide grid with repeating pattern
    std::vector<std::vector<char>> grid5 = {
        {'1','2','3','4','5'},
        {'a','b','c','d','e'}
    };
    auto result5 = reverseEachRow(2, 5, grid5);
    assert(result5 == std::vector<std::vector<char>>({
        {'5','4','3','2','1'},
        {'e','d','c','b','a'}
    }));

    // Test 6: Ensure original grid is not modified
    std::vector<std::vector<char>> original = {{'a','b'}, {'c','d'}};
    auto result6 = reverseEachRow(2, 2, original);
    assert(original == std::vector<std::vector<char>>({{'a','b'}, {'c','d'}}));
    assert(result6 == std::vector<std::vector<char>>({{'b','a'}, {'d','c'}}));

    // Test 7: Grid with special characters
    std::vector<std::vector<char>> grid7 = {{'!','@','#'}, {'$','%','^'}};
    auto result7 = reverseEachRow(2, 3, grid7);
    assert(result7 == std::vector<std::vector<char>>({{'#','@','!'}, {'^','%','$'}}));
}
