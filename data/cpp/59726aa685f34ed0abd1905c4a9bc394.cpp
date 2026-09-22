Write a C++ function `void printDecreasing(int n)` that takes a non-negative integer `n` and prints all integers from `n` down to `1` in decreasing order, each on its own line. The function must not print anything when `n` is 0, and must not accept negative inputs (if given a negative number, it should simply do nothing). The function should use recursion (no loops allowed) and output directly to `std::cout`. The task is to implement the recursive logic cleanly, ensuring the base case is correct and that the output format matches exactly (one number per line, no extra spaces or blank lines).
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function under test (already defined elsewhere)
void printDecreasing(int n);

// Helper to capture output of printDecreasing for a given input
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printDecreasing(n);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Test cases for printDecreasing
    assert(captureOutput(5) == "5\n4\n3\n2\n1\n");
    assert(captureOutput(1) == "1\n");
    assert(captureOutput(0) == "");
    assert(captureOutput(-3) == "");
    assert(captureOutput(3) == "3\n2\n1\n");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <iostream>   // for std::cout

// Recursively prints integers from n down to 1, each on a new line.
// If n <= 0, prints nothing.
void printDecreasing(int n) {
    if (n <= 0) {
        return;
    }
    std::cout << n << '\n';
    printDecreasing(n - 1);
}
// The solution uses a simple recursion. The base case occurs when `n == 0`, at which point the function returns without printing anything. For any `n > 0`, the function first prints the current value of `n`, then recursively calls `printDecreasing(n-1)`. This ensures the numbers are printed in decreasing order because we print before recursing. The recursion depth is exactly `n` for positive inputs, so for large `n` this could cause stack overflow, but for typical test cases it is fine. Negative inputs are handled by an initial guard: if `n <= 0`, the function returns immediately. Time complexity is `O(n)` because we perform one print and one recursive call per value from `n` down to `1`. Space complexity is `O(n)` due to the recursion stack. Edge cases include `n = 0` (nothing printed), `n = 1` (prints just "1"), and negative inputs (nothing printed).
