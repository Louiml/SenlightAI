/*
Write a C++ function named `generateDiamondPattern` that takes a single positive odd integer `n` and returns a `std::string` representing a diamond shape made of asterisks (`*`) and spaces. The diamond should have `n` rows in total: the top half includes `n` rows with an increasing odd number of asterisks (1, 3, 5, ..., n), and the bottom half includes `n-1` rows with decreasing odd numbers (n-2, n-4, ..., 1). Each row must be centered by preceding the asterisks with the appropriate number of spaces. The returned string should include newline characters (`\n`) at the end of each row, but no trailing newline after the last row. If the input is not a positive odd integer, the function should return an empty string.
*/
#include <string>

// Generates a centered diamond pattern of asterisks with odd number of rows.
// Returns an empty string if n is not a positive odd integer.
std::string generateDiamondPattern(int n) {
    if (n <= 0 || n % 2 == 0) {
        return "";
    }
    
    std::string result;
    // Top half (including middle row)
    for (int i = 1; i <= n; i += 2) {
        result += std::string((n - i) / 2, ' ') + std::string(i, '*') + "\n";
    }
    // Bottom half (excluding middle row)
    for (int i = n - 2; i > 0; i -= 2) {
        result += std::string((n - i) / 2, ' ') + std::string(i, '*') + "\n";
    }
    
    // Remove the final newline to avoid trailing newline
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the function under test
std::string generateDiamondPattern(int n);

int main() {
    // Test for n = 1 (smallest valid)
    assert(generateDiamondPattern(1) == "*");
    
    // Test for n = 3
    assert(generateDiamondPattern(3) == " *\n***\n *");
    
    // Test for n = 5
    assert(generateDiamondPattern(5) == "  *\n ***\n*****\n ***\n  *");
    
    // Test for n = 7 (verify symmetry)
    assert(generateDiamondPattern(7) == "   *\n  ***\n *****\n*******\n *****\n  ***\n   *");
    
    // Test invalid inputs return empty string
    assert(generateDiamondPattern(0) == "");
    assert(generateDiamondPattern(-3) == "");
    assert(generateDiamondPattern(4) == "");
    assert(generateDiamondPattern(10) == "");
    
    // Confirm no trailing newline
    std::string result = generateDiamondPattern(5);
    assert(result.back() != '\n');
    
    return 0;
}
// The solution involves constructing a string row by row. For the top half, iterate `i` from 1 to `n` stepping by 2, and for each value compute the number of leading spaces as `(n - i) / 2`. Append that many spaces, then `i` asterisks, then a newline. For the bottom half, iterate `i` from `n-2` down to 1 stepping by -2, applying the same space count formula. Edge cases: if `n` is even, negative, or zero, return an empty string. Also ensure no trailing newline after the last row—this can be handled by building the string normally and then removing the final newline character if present, or by carefully appending newlines only between rows. Time complexity is O(n^2) in terms of character output size (since the total number of characters is roughly n^2/2), and auxiliary space is O(1) excluding the returned string.
