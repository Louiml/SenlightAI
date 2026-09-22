// Write a C++ function named `printNumbersUpTo` that takes a positive integer `n` and returns a string containing the numbers from 1 to `n` inclusive, each separated by a newline character `'\n'`. The function must handle the case where `n` is 1 (returning just `"1\n"` or `"1"` — choose one and be consistent), and should not produce a trailing newline after the last number if you decide so, or you may include it. For simplicity, specify that the output should have a newline after every number including the last. The function should validate that `n` is positive; if `n` is 0 or negative, return an empty string. Ensure the solution is efficient for large `n` (up to 10^6) and uses `std::string` concatenation appropriately (avoid O(n^2) issues). Do not write any `main` function or any code that performs I/O; just the free function.

// The core task is to generate a newline-separated sequence of integers from 1 to `n`. The naive approach of repeatedly appending `std::to_string(i) + "\n"` to a `std::string` using `+=` can be O(n^2) in the worst case because strings may reallocate and copy existing content. To avoid this, we can reserve the approximate capacity of the final string upfront. The length of the string for numbers from 1 to n is the sum of the digits of each number plus n newlines. A simple upper bound is n * (number of digits of n + 1), but we can compute a tighter bound or just reserve(`n * 6`) for safety since numbers up to 10^6 have at most 7 digits plus newline. Then we use a loop from 1 to n, converting each integer to a string and appending. This yields O(n * average digits) time, which is effectively O(n log n) but for n up to 10^6 it's fine, and O(n) auxiliary space for the result. Edge cases: n=0 or negative → empty string; n=1 → "1\n". No other exceptions.

#include <string>

// Returns a string containing numbers 1 to n, each on a new line.
// If n is non-positive, returns an empty string.
std::string printNumbersUpTo(int n) {
    if (n <= 0) {
        return "";
    }
    
    // Reserve enough space to reduce reallocations.
    // Each number up to n has at most 10 digits for n up to 1e9, plus newline.
    // For n up to 1e6, 7 digits + newline = 8. Use a safe estimate.
    std::string result;
    result.reserve(static_cast<size_t>(n) * 8);
    
    for (int i = 1; i <= n; ++i) {
        result += std::to_string(i);
        result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>

std::string printNumbersUpTo(int n);

int main() {
    // n=1
    assert(printNumbersUpTo(1) == "1\n");
    
    // n=5
    assert(printNumbersUpTo(5) == "1\n2\n3\n4\n5\n");
    
    // n=0
    assert(printNumbersUpTo(0) == "");
    
    // n=-3
    assert(printNumbersUpTo(-3) == "");
    
    // n=10
    std::string expected10;
    for (int i = 1; i <= 10; ++i) {
        expected10 += std::to_string(i) + "\n";
    }
    assert(printNumbersUpTo(10) == expected10);
    
    // n=100 (verify length and content)
    std::string result100 = printNumbersUpTo(100);
    assert(result100.size() > 0);
    assert(result100.front() == '1');
    assert(result100.back() == '\n');
    assert(result100.find("\n10\n") != std::string::npos);
    
    // n=1000
    std::string result1000 = printNumbersUpTo(1000);
    assert(result1000.find("\n1000\n") != std::string::npos);
    
    return 0;
}
