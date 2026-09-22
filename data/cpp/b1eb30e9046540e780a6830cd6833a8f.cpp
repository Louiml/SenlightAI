Write a C++ function `printDecreasingLetters(int n)` that takes a positive integer `n` and returns a `std::string` representing the pattern where each line `i` (from 1 to `n`) contains the letters from `'A'` to `'A' + (n - i)` separated by a single space, with each line terminated by a newline character (except the last line, which should have no trailing newline). For example, if `n = 3`, the output should be the string `"A B C\nA B\nA"`. The function must handle `n >= 1`; for invalid input (e.g., `n <= 0`), return an empty string. Use only standard library facilities.
#include <cassert>
#include <string>

std::string printDecreasingLetters(int n); // declaration

int main() {
    assert(printDecreasingLetters(1) == "A");
    assert(printDecreasingLetters(2) == "A B\nA");
    assert(printDecreasingLetters(3) == "A B C\nA B\nA");
    assert(printDecreasingLetters(4) == "A B C D\nA B C\nA B\nA");
    assert(printDecreasingLetters(5) == "A B C D E\nA B C D\nA B C\nA B\nA");
    assert(printDecreasingLetters(0) == "");
    assert(printDecreasingLetters(-3) == "");
    return 0;
}
#include <string>

// Returns a string containing the letter pattern for input n.
// For each row i (1 to n), prints letters from 'A' to 'A'+(n-i).
// Rows are separated by newlines; no trailing newline at the end.
// Returns empty string if n <= 0.
std::string printDecreasingLetters(int n) {
    if (n <= 0) {
        return "";
    }
    
    std::string result;
    result.reserve(static_cast<size_t>(n * (n + 1) / 2 * 2)); // rough estimate for characters plus spaces
    
    for (int i = 1; i <= n; ++i) {
        int maxChar = 'A' + (n - i); // inclusive upper bound
        for (char ch = 'A'; ch <= maxChar; ++ch) {
            result.push_back(ch);
            if (ch < maxChar) {
                result.push_back(' ');
            }
        }
        if (i < n) {
            result.push_back('\n');
        }
    }
    return result;
}
// The task is a direct adaptation of the given snippet, which prints a triangular pattern of letters where the number of letters decreases by one each row. The key is to build a string result string efficiently. For each row `i` (1-indexed), we need to output characters from `'A'` to `'A' + (n - i)`. The number of characters in row `i` is `(n - i + 1)`. We iterate `i` from 1 to `n`, and for each row, loop `ch` from `'A'` to `'A' + (n - i)`. For each character, append it to the result string, and if it is not the last character in the row, append a space. After finishing a row, if it is not the last row (`i < n`), append a newline. Edge cases: `n = 1` produces `"A"` with no spaces or newlines. Invalid `n <= 0` returns empty string. The algorithm runs in `O(n^2)` time because the total number of letters printed is `n + (n-1) + ... + 1 = n(n+1)/2`, and each letter takes constant time. Space complexity is `O(n^2)` for the resulting string, which is required to store the output.
