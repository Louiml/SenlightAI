// Write a C++ function named `printMultiplicationTable` that accepts a positive integer `n` and returns a `std::vector<std::string>` where each string is a line of the multiplication table from 1 to 10 for that number, formatted exactly as `"n * i = product"` (e.g., for n=5, the first line is `"5 * 1 = 5"`). The function must handle the case where `n` is zero or negative by returning an empty vector, and must not print anything to the console—the result is returned for the caller to display. The output strings must use integer multiplication, not string concatenation of the product.
#include <cassert>
#include <vector>
#include <string>

// Include the solution function (or paste it here)

int main() {
    // Test with positive number
    std::vector<std::string> result = printMultiplicationTable(3);
    assert(result.size() == 10);
    assert(result[0] == "3 * 1 = 3");
    assert(result[1] == "3 * 2 = 6");
    assert(result[9] == "3 * 10 = 30");

    // Test with 1
    result = printMultiplicationTable(1);
    assert(result.size() == 10);
    assert(result[0] == "1 * 1 = 1");

    // Test with zero
    result = printMultiplicationTable(0);
    assert(result.empty());

    // Test with negative number
    result = printMultiplicationTable(-5);
    assert(result.empty());

    // Test with a larger number
    result = printMultiplicationTable(12);
    assert(result[5] == "12 * 6 = 72");
    assert(result[9] == "12 * 10 = 120");

    return 0;
}
#include <vector>
#include <string>

// Returns a vector of strings representing the multiplication table of n from 1 to 10.
// If n <= 0, returns an empty vector.
std::vector<std::string> printMultiplicationTable(const int n) {
    std::vector<std::string> table;
    if (n <= 0) {
        return table; // empty vector
    }
    for (int i = 1; i <= 10; ++i) {
        int product = n * i;
        std::string line = std::to_string(n) + " * " + std::to_string(i) + " = " + std::to_string(product);
        table.push_back(line);
    }
    return table;
}
// The solution is straightforward: initialize an empty vector of strings. If `n` is less than or equal to 0, return the empty vector immediately. Otherwise, loop from `i = 1` to `i = 10` inclusive. For each iteration, compute the product as `n * i`, and construct a string using `std::to_string` for each part (n, i, product) and concatenating with `" * "` and `" = "` separators. Push the resulting string into the vector. The main edge case is the input `n <= 0`, where no multiplication lines are valid, so return an empty vector. There are no other special cases because the range is fixed and small. Time complexity is O(10) = O(1) (constant), space complexity is O(10) for the vector (constant). The function is marked `const` where appropriate (no member variables, but the parameter is passed by value).
