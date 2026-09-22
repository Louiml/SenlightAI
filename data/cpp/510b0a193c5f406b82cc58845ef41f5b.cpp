/*
Write a C++ function that takes a positive integer `n` and prints (or returns as a string for testing purposes) a right-aligned triangular pattern of uppercase letters. For each row `i` from 1 to `n`, the row contains `n - i` leading spaces followed by `i` letters. The letters in a row form a descending sequence starting from a character determined by the row index: the starting character of row 1 is `'A'`, row 2 is `'B'` (since one extra letter is added per row), row 3 is `'D'`, row 4 is `'G'`, and so on, meaning the starting character of row `i` is the 1-based cumulative sum of 1 through `i` added to `'A'` (i.e., `'A' + i*(i-1)/2`). The descending sequence within a row goes from the starting character down by 1 for each column, producing letters like `A`, `BA`, `DCB`, `GFED`, etc. The function should accept the integer and return the complete multiline pattern as a `std::string` (with each row followed by `\n`). Assume `n` is at least 1, and handle the case where the letter index exceeds `'Z'` by wrapping around to `'A'` (e.g., using modulo 26). Your solution must be a standalone free function without a `main`; provide edge-case handling for large `n` where the cumulative sum may overflow an `int` — use a 64-bit type internally.
*/
#include <string>
#include <cstdint>

// Generate a right-aligned triangular pattern of uppercase letters.
// Row i (1-indexed) starts at 'A' + (i-1)*i/2 (mod 26) and descends by 1 each column.
std::string generateLetterTriangle(int n) {
    std::string result;
    result.reserve(static_cast<std::size_t>(n) * n); // approximate

    for (int i = 1; i <= n; ++i) {
        // Leading spaces: n - i
        result.append(static_cast<std::size_t>(n - i), ' ');

        // Starting offset for this row: triangular number (i-1)*i/2
        std::int64_t startOffset = static_cast<std::int64_t>(i - 1) * i / 2;

        // Print i letters descending from the starting character
        for (int k = 0; k < i; ++k) {
            char letter = 'A' + static_cast<char>((startOffset - k) % 26);
            // Ensure non-negative modulo for safety (startOffset >= k in valid pattern)
            result.push_back(letter);
        }

        result.push_back('\n');
    }

    return result;
}
#include <cassert>
#include <string>

// Assume generateLetterTriangle is declared above.

int main() {
    // Basic small cases
    assert(generateLetterTriangle(1) == "A\n");
    assert(generateLetterTriangle(2) == " A\nBA\n");
    assert(generateLetterTriangle(3) == "  A\n BA\nDCB\n");
    assert(generateLetterTriangle(4) == "   A\n  BA\n DCB\nGFED\n");

    // Check wrapping beyond 'Z'
    // Row 6 starts at offset 5*6/2 = 15 => 'P', row 7 starts at 21 => 'V', row 8 starts at 28 => 'B' (since 28%26=2)
    assert(generateLetterTriangle(8).substr(0, 1) == " "); // just sanity, full pattern check below
    // Let's verify the last line of n=8: row 8 has 0 spaces, starting 'B', descending to 'U' (since 8 letters)
    assert(generateLetterTriangle(8).substr(generateLetterTriangle(8).size() - 9) == "BZYXWVUTS\n");

    // Check large n doesnt crash and patterns are valid length
    std::string big = generateLetterTriangle(50);
    // For n=50, total rows = 50, each row i has (50-i) spaces + i letters + newline = 51 chars per row (consistent)
    assert(big.size() == static_cast<std::size_t>(50) * 51);

    // Verify each row has correct space count and letter count
    std::string pattern = generateLetterTriangle(5);
    // Expected: "    A\n   BA\n  DCB\n GFED\nJIHGF\n"
    assert(pattern == "    A\n   BA\n  DCB\n GFED\nJIHGF\n");

    // Test n=6: last row starts at offset 15 => 'P', then O,N,M,L,K
    assert(generateLetterTriangle(6).substr(generateLetterTriangle(6).size() - 7) == "PONMLK\n");

    // Test n=10: check first row and last row
    std::string t10 = generateLetterTriangle(10);
    assert(t10.substr(0, 1) == " ");
    // Row 10 starts at offset 45, 45%26=19 => 'T', descending 10 letters: T S R Q P O N M L K
    assert(t10.substr(t10.size() - 11) == "TSRQPONMLK\n");
}
// The pattern generation follows a clear mathematical rule: for row `i` (1-indexed), the starting letter’s offset from `'A'` is the triangular number `T(i-1) = (i-1)*i/2`. This is because each row adds one more letter than the previous row, so the cumulative shift is the sum of integers from 0 to `i-1`. Inside the row, we output `i` letters descending by 1 each step. To handle letters beyond `'Z'`, we use modulo 26 arithmetic: `char ch = 'A' + ((offset) % 26)`. For each step in the row, we decrease the offset by 1 (mod 26). Build the result string by appending the appropriate spaces and letters for each row, then a newline. Edge cases: (1) `n=1` produces just `A`; (2) large `n` such that `(i-1)*i/2` could exceed 32-bit int — use `long long` for the offset; (3) because of wrapping, consecutive rows may reuse letters, but that is correct. Time complexity is `O(n^2)` because the total number of printed characters is `sum(i) = n(n+1)/2` per row’s letters plus spaces of similar order. Space complexity is `O(n^2)` for the output string itself (necessary). The algorithm is iterative and straightforward.
