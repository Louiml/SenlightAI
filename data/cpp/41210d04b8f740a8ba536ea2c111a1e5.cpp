/*
Write a C++ function `recursiveDecrPrint` that takes a non-negative integer `n` and prints the numbers from `n` down to `1` on separate lines (each followed by a newline) using recursion, without using any loops. If `n` is 0, the function should print nothing and return. The function should be const-correct where applicable (though no parameters are modified). Your solution must avoid global variables and must not use `std::endl` (use `'\n'` instead). The function signature should be `void recursiveDecrPrint(int n);`.
*/
#include <iostream>

// Print numbers from n down to 1, each on its own line, using recursion.
// If n is 0, print nothing.
void recursiveDecrPrint(int n) {
    if (n == 0) {
        return;
    }
    std::cout << n << '\n';
    recursiveDecrPrint(n - 1);
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function (normally this would come from a header)
void recursiveDecrPrint(int n);

// Helper to capture output
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    recursiveDecrPrint(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    assert(captureOutput(0) == "");
    assert(captureOutput(1) == "1\n");
    assert(captureOutput(3) == "3\n2\n1\n");
    assert(captureOutput(5) == "5\n4\n3\n2\n1\n");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The core idea is to mimic the provided snippet's recursive countdown. The function prints the current value `n`, then recursively calls itself with `n-1`. The base case is when `n` equals 0; at that point, nothing is printed and the recursion terminates. This ensures that for any positive input, we output `n, n-1, ..., 1` in that order. Edge case: for `n = 0`, no output occurs (the base case triggers immediately). For negative inputs, the problem specification states non-negative integers, so we don't need to handle them; if passed, the function would incorrectly print negative numbers forever unless we guard — but the task constrains input to non-negative, so we optionally can add a check. Time complexity is O(n) because each recursive call prints one number and calls once. Space complexity is O(n) due to the recursion stack depth (each call uses stack memory). For very large n, stack overflow could occur, but typical test values are small.
