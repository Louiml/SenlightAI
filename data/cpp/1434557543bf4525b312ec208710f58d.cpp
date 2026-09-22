Write a C++ function named `invertedNumberPattern` that takes a single positive integer `n` as input and returns a `std::string` containing the inverted number pattern described below. The pattern for a given `n` consists of `n` lines: the first line contains the digit `n` repeated `n` times, the second line contains the digit `n-1` repeated `n-1` times, and so on, until the last line contains the digit `1` exactly once. Each line must be terminated by a newline character (`'\n'`), and there should be no extra spaces, leading or trailing whitespace. For example, when `n = 4`, the returned string should be `"4444\n333\n22\n1\n"`. The function must handle `n = 1` correctly, producing `"1\n"`. You may assume the input `n` is always a positive integer. The function should be self-contained, use only standard library facilities, and should not read from or write to any external stream; it must perform all its work in memory and return the result as a `std::string`.
// The core algorithm is straightforward: iterate from `current = n` down to `1`. For each `current` value, build a line consisting of the character representation of `current` (which is `'0' + current` since `current` is between 1 and 9 in any reasonable test case, but the pattern works for any digit from 1 to 9; if `n` could be 10 or more, it would produce multi-digit numbers, but the problem statement implies single digits, so we will handle that assumption). Append that line to the result string, followed by a newline character. The pattern is symmetric in the sense that each line's length equals the digit value it prints. Edge cases: `n = 1` yields a single line with one `'1'`. The loop naturally terminates when `current` becomes 0, but we stop at 1 because the pattern ends at `1`. Time complexity is \(O(n^2)\) because the total number of characters output is the sum of 1 through n, which is \(n(n+1)/2\). Space complexity is \(O(n^2)\) for the returned string, but the auxiliary space used during construction (besides the string itself) is \(O(1)\) apart from the string's dynamic growth. We use a `std::string` and `operator+=` to accumulate the result, which is efficient enough for typical constraints. No external I/O is performed inside the function, making it testable with assertions.
#include <string>

// Builds an inverted number pattern as a string.
// For n = 4, returns "4444\n333\n22\n1\n".
std::string invertedNumberPattern(int n) {
    std::string result;
    // Iterate from n down to 1.
    for (int current = n; current >= 1; --current) {
        // Append the digit 'current' repeated 'current' times.
        // current is between 1 and 9, so char('0' + current) is correct.
        char digit = static_cast<char>('0' + current);
        result.append(current, digit);
        result.push_back('\n');
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared here for testing.
std::string invertedNumberPattern(int n);

int main() {
    // Test n = 1
    assert(invertedNumberPattern(1) == "1\n");
    // Test n = 2
    assert(invertedNumberPattern(2) == "22\n1\n");
    // Test n = 3
    assert(invertedNumberPattern(3) == "333\n22\n1\n");
    // Test n = 4
    assert(invertedNumberPattern(4) == "4444\n333\n22\n1\n");
    // Test n = 5
    assert(invertedNumberPattern(5) == "55555\n4444\n333\n22\n1\n");
    // Test n = 6, verify length and first/last characters
    std::string pattern6 = invertedNumberPattern(6);
    assert(pattern6.size() == 6 + 5 + 4 + 3 + 2 + 1 + 6); // sum 1..6 = 21 digits + 6 newlines = 27
    assert(pattern6.substr(0, 6) == "666666");
    assert(pattern6.back() == '\n');
    // Test n = 7, check each line separately
    std::string pattern7 = invertedNumberPattern(7);
    assert(pattern7.find("7777777\n") == 0);
    assert(pattern7.find("666666\n") == 8); // 7 digits + newline = 8
    // Test n = 8, ensure no extra characters
    std::string pattern8 = invertedNumberPattern(8);
    assert(pattern8.find("88888888\n") == 0);
    assert(pattern8[pattern8.size() - 1] == '\n');
    // Test n = 9, final line is "1"
    std::string pattern9 = invertedNumberPattern(9);
    assert(pattern9.find("1\n") == pattern9.size() - 2);
    return 0;
}
