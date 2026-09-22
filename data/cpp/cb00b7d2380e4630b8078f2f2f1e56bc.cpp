// Write a C++ function that takes a positive integer `n` and returns a string containing `n` lines, where each line `i` (starting from 1) contains the integers from `i` down to `1`, each immediately followed by the next (no spaces), and each line ends with a newline character. For example, for `n = 3`, the output should be the string `"1\n21\n321\n"`. The function should handle `n = 0` by returning an empty string, and should be robust for any non-negative integer `n` (the input `n` may be large, but you can assume that the resulting string fits in memory).
// The core algorithm is a straightforward nested loop: for each row from 1 to `n`, we print decreasing numbers from `row` down to 1, each as a single digit (since `row` can be > 9, we must handle multi-digit numbers by converting each integer to a string via `std::to_string`). For each row `i`, we start `value = i` and decrement it while `value >= 1`, appending `std::to_string(value)` to the result string, then append a newline after the inner loop ends. Edge cases: when `n = 0`, the loop does not execute and the function returns an empty string. For `n = 1`, the output is `"1\n"`. Complexity: the total number of value productions is `n*(n+1)/2`, so time is O(n²), and the result string also has O(n²) characters, so space is O(n²) as well, which is unavoidable given the output size.
#include <string>

// Return a string with n lines, where line i contains numbers i down to 1.
std::string buildDescendingTriangle(int n) {
    std::string result;
    result.reserve(static_cast<size_t>(n) * (n + 1) / 2 * 2); // rough capacity hint

    for (int row = 1; row <= n; ++row) {
        for (int value = row; value >= 1; --value) {
            result += std::to_string(value);
        }
        result += '\n';
    }
    return result;
}
#include <cassert>
#include <string>

// Declaration of the solution function (assumed from the solution section)
std::string buildDescendingTriangle(int n);

int main() {
    assert(buildDescendingTriangle(0) == "");
    assert(buildDescendingTriangle(1) == "1\n");
    assert(buildDescendingTriangle(2) == "1\n21\n");
    assert(buildDescendingTriangle(3) == "1\n21\n321\n");
    assert(buildDescendingTriangle(4) == "1\n21\n321\n4321\n");
    assert(buildDescendingTriangle(5) == "1\n21\n321\n4321\n54321\n");
    assert(buildDescendingTriangle(10).find("10987654321\n") != std::string::npos);
    assert(buildDescendingTriangle(10).find("9876543210\n") == std::string::npos); // no zero
    assert(buildDescendingTriangle(100).back() == '\n');
    assert(buildDescendingTriangle(100).size() > 5000); // sanity check
    return 0;
}
