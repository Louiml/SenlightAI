// Write a C++ function named `printPositiveSeries` that takes a single integer parameter `limit` and prints all positive integers from 1 up to and including `limit`, each on its own line. The function should not return any value (void). The parameter may be any integer, including zero, negative values, or very large values. If the parameter is less than 1, the function should print nothing and simply return. The function must be standalone, with no reliance on global state, and must use `std::cout` for output. The main program will handle user input and call the function.
// The solution is straightforward: the function accepts an integer `limit`. If `limit` is less than 1, the function should return immediately, because there are no positive integers to print. Otherwise, use a `for` loop that starts at `i = 1` and continues while `i <= limit`, incrementing `i` each iteration, and output the current value of `i` followed by a newline using `std::cout << i << "\n"`. Edge cases include `limit = 0` or negative values, where the loop condition would fail immediately (since 1 <= 0 is false), so no output occurs—this matches the requirement naturally. For very large `limit`, the loop iterates exactly `limit` times, which is the expected behavior. Time complexity is O(n) where n = `limit` (number of iterations), and space complexity is O(1) as only a single loop variable is used.
#include <iostream>

// Prints all positive integers from 1 to 'limit' inclusive, one per line.
// If 'limit' is less than 1, prints nothing.
void printPositiveSeries(int limit) {
    for (int i = 1; i <= limit; ++i) {
        std::cout << i << "\n";
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Helper function to capture output of printPositiveSeries
std::string captureOutput(int limit) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printPositiveSeries(limit);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Test with positive limit
    assert(captureOutput(3) == "1\n2\n3\n");
    assert(captureOutput(1) == "1\n");
    // Test with limit = 0 (should print nothing)
    assert(captureOutput(0) == "");
    // Test with negative limit (should print nothing)
    assert(captureOutput(-5) == "");
    // Test with a larger limit
    assert(captureOutput(5) == "1\n2\n3\n4\n5\n");
    // Test with limit = 2 (small)
    assert(captureOutput(2) == "1\n2\n");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
