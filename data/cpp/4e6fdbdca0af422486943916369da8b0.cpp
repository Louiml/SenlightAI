// Write a C++ function `printMultiplicationTable` that takes an integer `n` and an integer `limit` (both non-negative, with `limit` ≥ 1) and returns a `std::string` containing the multiplication table for `n` from 1 to `limit`, formatted exactly as: each line is `"n x i = product"` followed by a newline character, where `i` goes from 1 to `limit` inclusive. The function must use a `do-while` loop to generate the table (mirroring the original snippet’s loop structure), and the returned string must have no trailing extra spaces (only newlines). For example, for `n = 6` and `limit = 10`, the output should match the original snippet’s printed text exactly (including newlines after each line). Ensure the function is `const`‑correct (parameters by value or const reference as appropriate) and does not perform any console output—it only builds and returns the string. Handle edge cases where `limit` is 1 (only one line) and where `n` is 0 (products are all 0). The function must be standalone and not rely on global state.
// The solution uses a `do-while` loop because at least one iteration is always required since `limit >= 1`. We initialize a counter `i = 1` and an empty output string. In each iteration, append the formatted line `n << " x " << i << " = " << (n * i)` followed by `'\n'`, then increment `i`. The loop continues while `i <= limit` (note: the original snippet used `i != 11`, but for general `limit` we need `i <= limit` to handle any positive limit). After the loop, return the accumulated string. Edge cases: if `limit == 1`, the loop runs exactly once; if `n == 0`, all products are 0, which is handled naturally by integer multiplication. Time complexity is O(limit) because we perform one iteration per multiplier, and each iteration does constant work (string append and arithmetic). Space complexity is O(limit * digits) for the returned string, but auxiliary space (excluding the return value) is O(1) because we only keep a counter and the output string. There are no special edge cases beyond ensuring the loop condition is correct for `limit` values greater than 1 (the original snippet hardcoded 11, but we generalize). One subtlety: using `std::to_string` for each line would require concatenation; instead, we can use `std::ostringstream` to build lines efficiently, but a simple string append with `std::to_string` works fine. To mirror the original formatting exactly (no extra spaces), we append each line as `std::to_string(n) + " x " + std::to_string(i) + " = " + std::to_string(n * i) + "\n"`. The complexity remains linear.
#include <string>

// Returns a string containing the multiplication table of n from 1 to limit.
// Each line is formatted as "n x i = product" followed by a newline.
// Uses a do-while loop to ensure at least one line is generated (limit >= 1).
std::string printMultiplicationTable(int n, int limit) {
    std::string result;
    int i = 1;
    do {
        result += std::to_string(n) + " x " + std::to_string(i) + " = " + std::to_string(n * i) + "\n";
        ++i;
    } while (i <= limit);
    return result;
}
#include <cassert>
#include <string>

// Function under test (declared here for testing purposes; in a real project it would be included from header)
std::string printMultiplicationTable(int n, int limit);

int main() {
    // Test with original snippet's values: n=6, limit=10
    assert(printMultiplicationTable(6, 10) ==
           "6 x 1 = 6\n6 x 2 = 12\n6 x 3 = 18\n6 x 4 = 24\n6 x 5 = 30\n"
           "6 x 6 = 36\n6 x 7 = 42\n6 x 8 = 48\n6 x 9 = 54\n6 x 10 = 60\n");

    // Test limit = 1 (only one line)
    assert(printMultiplicationTable(7, 1) == "7 x 1 = 7\n");

    // Test n = 0 (all products are zero)
    assert(printMultiplicationTable(0, 3) ==
           "0 x 1 = 0\n0 x 2 = 0\n0 x 3 = 0\n");

    // Test a different n and limit
    assert(printMultiplicationTable(3, 2) ==
           "3 x 1 = 3\n3 x 2 = 6\n");

    // Test limit = 5, n = -2 (negative n works naturally)
    assert(printMultiplicationTable(-2, 3) ==
           "-2 x 1 = -2\n-2 x 2 = -4\n-2 x 3 = -6\n");

    // Test large limit to ensure loop termination and correctness
    std::string large = printMultiplicationTable(1, 3);
    assert(large == "1 x 1 = 1\n1 x 2 = 2\n1 x 3 = 3\n");

    return 0;
}
