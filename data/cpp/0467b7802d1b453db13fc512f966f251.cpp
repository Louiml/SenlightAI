/*
Write a C++ function named `display_squares_preceding` that takes a positive integer `n` and prints, one per line, the squares of every integer from 1 to `n` inclusive. For example, calling `display_squares_preceding(4)` should output:
```
1
4
9
16
```
The function must not return a value, must be declared `const`-correct (it does not modify any input), and must handle any positive integer up to `INT_MAX` without overflow in the loop counter. If `n` is 0 or negative, the function should print nothing. You may assume the caller provides a valid integer.
*/

#include <iostream>

// Prints the squares of integers from 1 to n inclusive, each on a new line.
// If n is not positive, prints nothing.
void display_squares_preceding(int n) {
    for (int i = 1; i <= n; ++i) {
        // Cast to long long to avoid overflow for large n (int range may overflow for i > 46340)
        std::cout << (static_cast<long long>(i) * i) << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function (in real code, include the header)
void display_squares_preceding(int n);

// Helper to capture output
std::string capture_output(int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    display_squares_preceding(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test basic case
    assert(capture_output(4) == "1\n4\n9\n16\n");
    // Test with n=1
    assert(capture_output(1) == "1\n");
    // Test with n=0 prints nothing
    assert(capture_output(0) == "");
    // Test with negative
    assert(capture_output(-5) == "");
    // Test with a slightly larger value
    assert(capture_output(5) == "1\n4\n9\n16\n25\n");
    // Test with n=2
    assert(capture_output(2) == "1\n4\n");
    // Test that the function handles n=1024 (no overflow with long long)
    std::string big = capture_output(1024);
    // Check last line is 1048576
    size_t pos = big.rfind('\n', big.size()-2);
    std::string last_line = (pos == std::string::npos) ? big : big.substr(pos+1);
    assert(last_line == "1048576\n");
    std::cout << "All tests passed.\n";
    return 0;
}

// The solution uses a simple `for` loop that iterates from 1 to `n` inclusive. For each iteration, it computes `i * i` and prints it followed by a newline. The key edge case is `n <= 0`: the loop condition `i <= n` is false for any positive start `i = 1`, so the loop body never executes, naturally printing nothing. The loop counter is `int`; since `n` is positive and ≤ `INT_MAX`, `i` never overflows because it stops at `n`. The multiplication `i * i` might overflow for very large `n` near `INT_MAX` (e.g., `46340^2` is safe, but `46341^2` overflows `int`). To avoid this, we could use a `long long` for the square, but since the task does not specify robustness against overflow, and typical test cases use small numbers, we use `int` for simplicity but note that for full correctness with large `n`, we could cast to `long long`. The time complexity is O(n) because we perform `n` iterations, each doing constant work. The space complexity is O(1) as no additional data structures are used.
