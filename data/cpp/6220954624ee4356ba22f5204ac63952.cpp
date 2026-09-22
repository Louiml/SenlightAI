Write a C++ function `printNumberTriangle` that takes an integer `n` (where `n >= 0`) and returns a `std::string` containing a right-aligned number triangle. For each row `i` from 0 to `n-1`, the row must start with `i` spaces (no trailing spaces), followed by `(n-i)` copies of the digit `i+1` (if `i+1` is greater than 9, use the last digit of that number, i.e., `(i+1) % 10`). Each row is separated by a newline character `'\n'`, and the last row ends with a newline as well. For example, if `n = 4`, the output should be `"1111\n 222\n  33\n   4\n"` (note: row 0 has no leading spaces, row 1 has 1 space, etc.). If `n = 0`, return an empty string. The function must build the string efficiently using `std::ostringstream` or manual concatenation, and must not print to the console; it only returns the formatted string. The function should handle `n` up to at least 10,000 without excessive memory or time, and must be robust for any non-negative integer input. Edge cases include `n=1` (returns `"1\n"`), `n=2` (returns `"11\n 2\n"`), and large `n` where the digit wrap-around occurs (e.g., `n=12`, row index 9 uses digit `0` because `(9+1)%10 == 0`).
// The task is to generate a triangular pattern where each row `i` has exactly `i` leading spaces and `(n-i)` occurrences of a digit derived from `(i+1) % 10`. The main algorithm iterates from row `i = 0` to `i = n-1`. For each row, append `i` spaces, then append the character `char('0' + ((i+1)%10))` exactly `(n-i)` times, then append a newline. The total number of characters is the sum over rows of `(i + (n-i) + 1)` = sum over rows of `(n+1)` = `n*(n+1)` characters, so the output size is `O(n^2)`. Time complexity is `O(n^2)` because we must generate each character. Space complexity is `O(n^2)` for the returned string (but auxiliary space beyond that is `O(1)` if we build directly into the string or `ostringstream`). Edge cases: `n=0` returns empty string (loop does nothing). `n=1` produces `"1\n"` (no leading spaces, one `1`, newline). For large `n`, the digit wraps correctly using modulo 10. The use of `std::string::append(size_t count, char ch)` is efficient as it reserves the needed space internally. No special handling for negative `n` is required because the task specifies `n >= 0`.
#include <string>

// Generate a right-aligned number triangle as a string.
// For each row i (0 <= i < n): i spaces, then (n-i) copies of digit ((i+1)%10), then newline.
std::string printNumberTriangle(int n) {
    if (n <= 0) {
        return "";
    }
    
    std::string result;
    // Reserve enough space to avoid reallocations: n*(n+1) characters (approx).
    result.reserve(static_cast<size_t>(n) * (n + 1));
    
    for (int i = 0; i < n; ++i) {
        // Append i leading spaces
        result.append(static_cast<size_t>(i), ' ');
        // Append (n-i) copies of the digit
        char digit = static_cast<char>('0' + ((i + 1) % 10));
        result.append(static_cast<size_t>(n - i), digit);
        // Append newline
        result.push_back('\n');
    }
    
    return result;
}
#include <cassert>
#include <string>

// Declare the function from the solution (included via a separate header normally).
std::string printNumberTriangle(int n);

int main() {
    // Test n=0
    assert(printNumberTriangle(0) == "");
    
    // Test n=1
    assert(printNumberTriangle(1) == "1\n");
    
    // Test n=2
    assert(printNumberTriangle(2) == "11\n 2\n");
    
    // Test n=4 (matching the snippet's output)
    assert(printNumberTriangle(4) == "1111\n 222\n  33\n   4\n");
    
    // Test n=5 (digit wrap not yet)
    assert(printNumberTriangle(5) == "11111\n 2222\n  333\n   44\n    5\n");
    
    // Test n=10 (row 9 uses digit '0' because (9+1)%10=0)
    std::string expected10;
    expected10 += "1111111111\n";
    expected10 += " 222222222\n";
    expected10 += "  33333333\n";
    expected10 += "   4444444\n";
    expected10 += "    555555\n";
    expected10 += "     66666\n";
    expected10 += "      7777\n";
    expected10 += "       888\n";
    expected10 += "        99\n";
    expected10 += "         0\n";
    assert(printNumberTriangle(10) == expected10);
    
    // Test n=11 (row 10 uses digit '1')
    std::string expected11 = expected10 + "          1\n";
    assert(printNumberTriangle(11) == expected11);
    
    return 0;
}
