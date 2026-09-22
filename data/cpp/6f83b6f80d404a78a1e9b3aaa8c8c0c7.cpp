Write a C++ function named `printMultiplicationTable` that takes a positive integer `n` and returns a `std::string` containing the multiplication table for `n` from 1 to 10, with each line formatted exactly as `n x i = product` (where `n` and `i` are integers and `product` is their multiplication), each on its own line, and ending with a newline character after the last line. The function must work correctly for any integer `n` within the range of a 32-bit signed integer, and the returned string must match the exact formatting specified.
#include <cassert>
#include <string>

int main() {
    // Simple case
    assert(printMultiplicationTable(2) == "2 x 1 = 2\n2 x 2 = 4\n2 x 3 = 6\n2 x 4 = 8\n2 x 5 = 10\n2 x 6 = 12\n2 x 7 = 14\n2 x 8 = 16\n2 x 9 = 18\n2 x 10 = 20\n");

    // Case with n = 1
    assert(printMultiplicationTable(1) == "1 x 1 = 1\n1 x 2 = 2\n1 x 3 = 3\n1 x 4 = 4\n1 x 5 = 5\n1 x 6 = 6\n1 x 7 = 7\n1 x 8 = 8\n1 x 9 = 9\n1 x 10 = 10\n");

    // Case with n = 0 (not positive but still works)
    assert(printMultiplicationTable(0) == "0 x 1 = 0\n0 x 2 = 0\n0 x 3 = 0\n0 x 4 = 0\n0 x 5 = 0\n0 x 6 = 0\n0 x 7 = 0\n0 x 8 = 0\n0 x 9 = 0\n0 x 10 = 0\n");

    // Large n to check no overflow (n = 1,000,000)
    std::string large = printMultiplicationTable(1000000);
    assert(large.find("1000000 x 10 = 10000000") != std::string::npos);

    // Negative n (not required but valid)
    assert(printMultiplicationTable(-3).find("-3 x 2 = -6") != std::string::npos);
}
#include <string>

// Return the multiplication table of n from 1 to 10 as a formatted string.
std::string printMultiplicationTable(int n) {
    std::string table;
    for (int i = 1; i <= 10; ++i) {
        long long product = static_cast<long long>(n) * i;  // avoid overflow
        table += std::to_string(n) + " x " + std::to_string(i) + " = " + std::to_string(product) + "\n";
    }
    return table;
}
// The solution is straightforward: iterate through integers `i` from 1 to 10 inclusive, compute the product `n * i`, format each line as `n x i = product`, and append it to a string, adding a newline after each line. The most important considerations are (a) ensuring the output exactly matches the required format with spaces around the `x` and `=`, and (b) handling the possibility of very large products (since `n` can be up to the maximum 32-bit integer, `n * 10` may overflow a 32-bit signed int, so we must use `long long` for the multiplication to avoid undefined behavior). The algorithm runs in O(1) time because it always performs exactly 10 iterations, and uses O(1) auxiliary space aside from the returned string itself, which has length proportional to the number of digits in `n` (but is bounded). Edge cases include `n` being 0 (which is allowed since the problem says "positive integer", but if given 0, the table would print `0 x i = 0` correctly) and negative `n` (though not expected, the function would still produce correct output as multiplication works fine with negatives). No other edge cases exist because the range is fixed.
