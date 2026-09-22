/*
Write a C++ function `generateLetterTriangle` that takes a positive integer `n` as input and returns a `std::string` containing a right-aligned triangle pattern of uppercase letters, with `n` rows. The first row contains a single letter starting at the letter `'A' + (n - 1)` (i.e., the letter whose distance from `'A'` is `n-1`), and each subsequent row shifts the starting letter one step earlier in the alphabet while increasing the row length by one. For example, if `n = 3`, the output should be `"C\nBC\nABC\n"` (with newline characters between rows). The pattern uses letters only from `'A'` to `'Z'`, so `n` must be between 1 and 26 inclusive. If `n` is outside this range, return an empty string.
*/
#include <string>

// Returns a right-aligned letter triangle pattern with n rows.
// The triangle starts at letter 'A' + (n - 1) on the first row.
// If n is not between 1 and 26 inclusive, returns an empty string.
std::string generateLetterTriangle(int n) {
    if (n < 1 || n > 26) {
        return "";
    }

    std::string result;
    result.reserve(static_cast<size_t>(n * (n + 1) / 2 + n)); // space for letters plus newlines

    for (int row = 1; row <= n; ++row) {
        char current = static_cast<char>('A' + (n - row));
        for (int col = 1; col <= row; ++col) {
            result.push_back(current);
            ++current;
        }
        result.push_back('\n'); // newline after each row (including last)
    }

    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above, but include it here for completeness.
std::string generateLetterTriangle(int n);

int main() {
    // Basic 3-row case
    assert(generateLetterTriangle(3) == "C\nBC\nABC\n");

    // Single-row case
    assert(generateLetterTriangle(1) == "A\n");

    // Four-row case explicitly checked
    assert(generateLetterTriangle(4) == "D\nCD\nBCD\nABCD\n");

    // Boundary n = 26 should be valid (first row starts at 'Z')
    // We only check the first character and total size to keep the test concise.
    std::string big = generateLetterTriangle(26);
    assert(big.size() == 26 * 27 / 2 + 26); // 351 letters + 26 newlines = 377
    assert(big.front() == 'Z');
    assert(big.find('\n') == 1); // after first row of size 1

    // Invalid n values return empty string
    assert(generateLetterTriangle(0) == "");
    assert(generateLetterTriangle(-5) == "");
    assert(generateLetterTriangle(27) == "");

    // Check exact output for n=2
    assert(generateLetterTriangle(2) == "B\nAB\n");
}
// The solution works by iterating over rows from 1 to `n`. For each row (say row index `row` starting at 1), the starting character is computed as `'A' + (n - row)`. This gives the correct initial letter: for `n=3`, row 1 starts at `'C'`, row 2 at `'B'`, and row 3 at `'A'`. Then, within each row, we append `row` consecutive characters, incrementing the character by 1 each time. We append a newline after each row except possibly the last (though including it is fine for testing). The main edge case is when `n` is less than 1 or greater than 26, which would produce letters outside `'A'`–`'Z'`; in that case, return an empty string. The time complexity is O(n²) because the total number of characters printed is `1 + 2 + ... + n = n(n+1)/2`, and each character operation is O(1). The space complexity is O(n²) because we build and return the full result string, but the auxiliary space beyond the output is O(1).
