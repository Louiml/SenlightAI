Write a C++ function named `printNumberTriangle` that takes an integer `n` as input and returns a string containing a right-aligned triangle pattern of digits. The triangle should consist of `n` lines. On the first line, the digit `n` is printed `n` times. On each subsequent line, the number of times the digit `n` is printed decreases by 1, until the last line contains only a single `n`. Each line is terminated by a newline character (`\n`). The input `n` is a positive integer (1 ≤ n ≤ 9). The function must be standalone and not rely on any global variables. For example, if `n` is 4, the returned string should be: `"4444\n444\n44\n4\n"`.

#include <cassert>
#include <string>

// Declare the function under test (it could be included from the solution header).
std::string printNumberTriangle(int n);

int main() {
    assert(printNumberTriangle(1) == "1\n");
    assert(printNumberTriangle(2) == "22\n2\n");
    assert(printNumberTriangle(3) == "333\n33\n3\n");
    assert(printNumberTriangle(4) == "4444\n444\n44\n4\n");
    assert(printNumberTriangle(5) == "55555\n5555\n555\n55\n5\n");
    assert(printNumberTriangle(9) == "999999999\n99999999\n9999999\n999999\n99999\n9999\n999\n99\n9\n");
    return 0;
}

#include <string>

// Builds a descending triangle pattern where each line i contains i copies of digit n.
// n must be between 1 and 9 inclusive.
std::string printNumberTriangle(int n) {
    std::string result;
    const char digit = static_cast<char>('0' + n); // Since n is 1-9, this is a single digit.

    for (int i = n; i >= 1; --i) {
        for (int j = 0; j < i; ++j) {
            result.push_back(digit);
        }
        result.push_back('\n');
    }

    return result;
}

// The task is straightforward: generate a string representation of a descending triangular pattern where each row `i` (from `n` down to 1) contains exactly `i` copies of the digit `n`. The main algorithm uses a nested loop: an outer loop iterates from `i = n` down to `i = 1`, and an inner loop appends the character `char('0' + n)` exactly `i` times to a result string. After the inner loop, a newline character `'\n'` is appended. Edge cases include `n = 1`, which produces just `"1\n"`, and the maximum allowed `n = 9`, which yields a multi-line string with consistent structure. Since the function only uses string concatenation, the time complexity is O(n²) because the total number of characters appended is the sum `n + (n-1) + ... + 1 = n(n+1)/2`. The space complexity is O(n²) as well, since the constructed string must store all those characters. Using `std::string` and `push_back` ensures efficient appending without repeated reallocation in practice.
