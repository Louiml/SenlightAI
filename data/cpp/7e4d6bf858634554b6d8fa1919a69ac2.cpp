Write a C++ function named `PrintNumberPattern` that takes a positive integer `n` and prints (via `std::cout`) an inverted right‑angled triangle pattern of digits. For each row `i` from `n` down to `1`, the row contains the digit `i` repeated exactly `i` times, with each row on a new line. The function must return `void` and must not read any input. The function must handle the smallest valid input `1` correctly, and must not print any leading or trailing spaces. For example, if `n = 3`, the output should be:
```
333
22
1
```
The function should be declared with `const`‑correct parameters (the input is read‑only) and must be placed in the global namespace. Do not include `main` in the solution; only provide the function.
// The core algorithm is a nested loop. The outer loop iterates from `n` down to `1` (inclusive), controlling the current digit and the number of times it must be printed. The inner loop runs from `1` to `i` (inclusive) and prints the digit `i` without a space. After the inner loop finishes, a newline is printed to start the next row. Edge cases: when `n = 1`, the function prints `1` and a newline; no other special handling is needed because the loop condition naturally stops at `1`. The time complexity is \(O(n^2)\) because the total number of printed digits is the sum from 1 to `n`, which is \(n(n+1)/2\). The space complexity is \(O(1)\) because only a loop counter and the constant `n` are used; no additional data structures are allocated.
#include <iostream>

// Prints an inverted pattern of digits from n down to 1.
// Each row i contains the digit i repeated i times.
void PrintNumberPattern(const int n) {
    for (int i = n; i >= 1; --i) {
        for (int j = 1; j <= i; ++j) {
            std::cout << i;
        }
        std::cout << "\n";
    }
}
#include <cassert>
#include <sstream>

// The solution function is declared here (or included from the solution).
void PrintNumberPattern(const int n);

int main() {
    // Capture output using a string stream redirection.
    std::ostringstream buffer;
    std::streambuf* old_cout = std::cout.rdbuf(buffer.rdbuf());

    PrintNumberPattern(1);
    assert(buffer.str() == "1\n");

    buffer.str("");
    PrintNumberPattern(2);
    assert(buffer.str() == "22\n1\n");

    buffer.str("");
    PrintNumberPattern(3);
    assert(buffer.str() == "333\n22\n1\n");

    buffer.str("");
    PrintNumberPattern(5);
    assert(buffer.str() == "55555\n4444\n333\n22\n1\n");

    // Restore original cout.
    std::cout.rdbuf(old_cout);

    // A direct test without redirection to verify no crash.
    PrintNumberPattern(4);
    return 0;
}
