// Write a C++ function that takes a positive integer `n` and returns a vector of strings, where each string represents a line of a table showing the first, second, and third powers (i.e., `i^1`, `i^2`, `i^3`) for each integer `i` from 1 to `n`, inclusive. The output format must match exactly: for each row, print the three values separated by single spaces, with no extra leading or trailing spaces. The first row must always be `"1 1 1"` (even though `1^2` and `1^3` are both 1). For `n` up to at least 100, the values may exceed standard `int` range for `i^3` (e.g., `100^3 = 1,000,000` fits in `int`, but for larger `n` consider using a 64-bit type such as `long long`). The function should handle the edge case `n = 1` by returning only that first row. The input is guaranteed to be a positive integer (no zero or negative), so no validation is required.
// The solution is straightforward: iterate from 1 to `n` inclusive, and for each `i`, compute `i^1`, `i^2`, and `i^3`. Since `i` can be up to 100 (or potentially larger if the test expands), use `long long` to avoid overflow for `i^3` (e.g., `1000^3 = 1e9`, which fits in `int`, but for safety). For `i=1`, the values are 1,1,1 which matches the pattern. For every `i`, format the three values as a string with spaces between them. The main edge case is `n=1` where the loop runs once; also note that `i^1` is just `i`, so no need for `pow` with exponent 1. The time complexity is O(n) because we process each integer once, and space complexity is O(n) for storing the output vector (plus O(1) extra for temporary strings). The implementation must avoid floating-point `pow` to guarantee exact integer arithmetic and correct formatting (especially for large values). Use `std::to_string` or string concatenation for building each line.
#include <string>
#include <vector>

// Generate a vector of strings, each containing the first, second, and third
// powers of the number i (from 1 to n), separated by spaces.
std::vector<std::string> generatePowerTable(int n) {
    std::vector<std::string> table;
    table.reserve(n); // Optional optimization

    for (long long i = 1; i <= n; ++i) {
        long long first = i;          // i^1
        long long second = i * i;     // i^2
        long long third = i * i * i;  // i^3
        table.push_back(std::to_string(first) + " " + std::to_string(second) + " " + std::to_string(third));
    }
    return table;
}
#include <cassert>
#include <string>
#include <vector>

// The function declaration is assumed from the solution.
std::vector<std::string> generatePowerTable(int n);

int main() {
    // Test n = 1
    std::vector<std::string> result1 = generatePowerTable(1);
    assert(result1.size() == 1);
    assert(result1[0] == "1 1 1");

    // Test n = 3 (basic small case)
    std::vector<std::string> result3 = generatePowerTable(3);
    assert(result3.size() == 3);
    assert(result3[0] == "1 1 1");
    assert(result3[1] == "2 4 8");
    assert(result3[2] == "3 9 27");

    // Test n = 5 (includes a perfect cube)
    std::vector<std::string> result5 = generatePowerTable(5);
    assert(result5.size() == 5);
    assert(result5[4] == "5 25 125");

    // Test n = 10 (larger values, but still within int range)
    std::vector<std::string> result10 = generatePowerTable(10);
    assert(result10.size() == 10);
    assert(result10[9] == "10 100 1000");

    // Test n = 100 (values are exactly as expected, no overflow with long long)
    std::vector<std::string> result100 = generatePowerTable(100);
    assert(result100.size() == 100);
    assert(result100[99] == "100 10000 1000000");

    // Verify the full sequence for n = 4 matches expected manual computation
    std::vector<std::string> expected = {"1 1 1", "2 4 8", "3 9 27", "4 16 64"};
    assert(generatePowerTable(4) == expected);

    return 0;
}
