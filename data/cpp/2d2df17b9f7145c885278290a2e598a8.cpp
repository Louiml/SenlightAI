// Write a C++ function that, given two integers `n` and `m` (both between 1 and 1000), returns a vector of strings representing a checkerboard-like pattern of `n` rows and `m` columns. The pattern alternates between two row types: in the first type, cells have `'1'` at odd columns (1-indexed) and `'0'` at even columns; in the second type, the pattern is inverted (`'0'` at odd, `'1'` at even). The row types alternate every two rows: rows 1-2 use the first type, rows 3-4 use the second type, rows 5-6 use the first type again, and so on. Each string in the vector is exactly `m` characters long, containing only `'0'` and `'1'`. The function should handle all valid inputs gracefully, and the returned vector must have exactly `n` strings.

// The problem mimics the provided code’s output pattern. The key is to detect the row group: each group consists of two consecutive rows that share the same column pattern. The column pattern for a group is determined by the group index (starting from 0): if the group index is even, use pattern A (odd columns get `'1'`, even get `'0'`); if odd, use pattern B (odd columns get `'0'`, even get `'1'`). For each row `i` (0-indexed), compute `group = i / 2`. If `group % 2 == 0`, then for each column `j` (0-indexed), the character is `'1'` if `j % 2 == 0` (since 0-indexed odd? Wait, the code uses 1-indexed: column 1 is odd, so in 0-indexed, column 0 is odd. So for pattern A, `'1'` when `j % 2 == 0`; for pattern B, `'1'` when `j % 2 == 1`.). Edge cases: `n` and `m` can be 1, and the pattern must still alternate correctly. Time complexity is O(n*m) because we generate each character exactly once; space complexity is O(n*m) for the output vector (excluding the output itself, it's O(1) auxiliary if we build each string on the fly, but we store the result so O(n*m) total). There are no tricky edge cases beyond ensuring the group logic uses integer division correctly.

#include <string>
#include <vector>

// Generate an alternating checkerboard pattern of n rows and m columns.
// Rows are grouped in pairs: rows 0-1 share pattern A, rows 2-3 share pattern B,
// rows 4-5 share pattern A, and so on.
// Pattern A: '1' at even columns (0-indexed), '0' at odd columns.
// Pattern B: '0' at even columns, '1' at odd columns.
std::vector<std::string> generateCheckerboard(int n, int m) {
    std::vector<std::string> result;
    result.reserve(n);

    for (int row = 0; row < n; ++row) {
        std::string line;
        line.reserve(m);
        const int group = row / 2;
        const bool usePatternA = (group % 2 == 0);

        for (int col = 0; col < m; ++col) {
            const bool isEvenColumn = (col % 2 == 0);
            if (usePatternA) {
                line += (isEvenColumn ? '1' : '0');
            } else {
                line += (isEvenColumn ? '0' : '1');
            }
        }
        result.push_back(line);
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Test 1x1
    assert(generateCheckerboard(1, 1) == std::vector<std::string>{"1"});
    
    // Test 1x3 (single pair start, pattern A)
    assert(generateCheckerboard(1, 3) == std::vector<std::string>{"101"});
    
    // Test 2x2 (exactly one pair, pattern A)
    assert(generateCheckerboard(2, 2) == std::vector<std::string>{"10", "10"});
    
    // Test 3x1 (first pair A, then second row of pair? Actually rows 0-1 A, row 2 B)
    assert(generateCheckerboard(3, 1) == std::vector<std::string>{"1", "1", "0"});
    
    // Test 4x4 (two pairs: first A, second B)
    std::vector<std::string> expected4x4 = {
        "1010",
        "1010",
        "0101",
        "0101"
    };
    assert(generateCheckerboard(4, 4) == expected4x4);
    
    // Test 5x2 (first pair A, second pair B, fifth row starts new A group)
    std::vector<std::string> expected5x2 = {
        "10",
        "10",
        "01",
        "01",
        "10"
    };
    assert(generateCheckerboard(5, 2) == expected5x2);
    
    // Test 2x5
    assert(generateCheckerboard(2, 5) == std::vector<std::string>{"10101", "10101"});
    
    // Test large dimensions but small pattern to ensure no crash
    auto big = generateCheckerboard(1000, 1000);
    assert(big.size() == 1000);
    assert(big[0].size() == 1000);
    assert(big[0][0] == '1');
    assert(big[2][0] == '0');
    assert(big[999][999] == '1'); // row 999 group 499 odd => pattern B, col 999 odd => '1'
    
    return 0;
}
