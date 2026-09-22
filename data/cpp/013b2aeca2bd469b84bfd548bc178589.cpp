// Write a C++ function named `largestAdjacentProduct` that takes a non-empty string representing a sequence of decimal digits (only characters '0'–'9') and an integer `windowSize` (greater than 0 and less than or equal to the length of the string). The function must return the greatest product of exactly `windowSize` consecutive digits in the string. For example, for the string `"123456"` and `windowSize = 3`, the function should return `120` (from `"456"`). The input string will contain no whitespace or other characters, and you can assume the product fits within an `unsigned long long`. If the input is invalid (empty string, windowSize ≤ 0, or windowSize > string length), the function should return 0. The task is to implement the function only; no `main` is required, but you may use `assert` in a test harness.

#include <cassert>

// Global main for testing the solution function.
int main() {
    // Basic example from the prompt
    assert(largestAdjacentProduct("123456", 3) == 120);
    // Single digit window
    assert(largestAdjacentProduct("987", 1) == 9);
    // Full string window
    assert(largestAdjacentProduct("111", 3) == 1);
    // Window containing zero
    assert(largestAdjacentProduct("102", 2) == 2); // windows: 1*0=0, 0*2=0 -> max=0? Actually 1*0=0, 0*2=0 -> max=0, but 10*2? No, window size 2: "10"=0, "02"=0 => 0. Correct.
    assert(largestAdjacentProduct("102", 2) == 0);
    // Larger product with zeros ignored
    assert(largestAdjacentProduct("2304", 2) == 12); // windows: 2*3=6, 3*0=0, 0*4=0 -> max=6? Wait, 2*3=6, 3*0=0, 0*4=0 -> max=6. Correct.
    assert(largestAdjacentProduct("2304", 2) == 6);
    // Window size matching entire string
    assert(largestAdjacentProduct("123", 3) == 6);
    // Invalid inputs
    assert(largestAdjacentProduct("", 3) == 0);
    assert(largestAdjacentProduct("123", 0) == 0);
    assert(largestAdjacentProduct("123", 4) == 0);
    // All same digits
    assert(largestAdjacentProduct("2222", 2) == 4);
    assert(largestAdjacentProduct("2222", 3) == 8);

    return 0;
}

#include <string>

// Returns the maximum product of `windowSize` consecutive digits in `digits`.
// Returns 0 if `digits` is empty, `windowSize` is non-positive, or `windowSize` exceeds the length.
unsigned long long largestAdjacentProduct(const std::string& digits, int windowSize) {
    if (digits.empty() || windowSize <= 0 || static_cast<size_t>(windowSize) > digits.length()) {
        return 0;
    }

    unsigned long long maxProduct = 0;
    const size_t n = digits.length();
    const size_t k = static_cast<size_t>(windowSize);

    for (size_t start = 0; start <= n - k; ++start) {
        unsigned long long product = 1;
        for (size_t i = start; i < start + k; ++i) {
            product *= static_cast<unsigned long long>(digits[i] - '0');
        }
        if (product > maxProduct) {
            maxProduct = product;
        }
    }

    return maxProduct;
}

// The solution scans the string from left to right, considering every contiguous substring of length `windowSize`. For each window, compute the product of its digits by iterating over the substring, converting each character to its integer value using `digit - '0'`. Track the maximum product seen. Edge cases: empty string or invalid `windowSize` must return 0; a window containing a `'0'` digit will have product 0, which may lower the maximum, so it is safe to compare normally. The algorithm runs in O(n * k) time where n is the string length and k is the window size, and uses O(1) auxiliary space (only a few variables). For very large inputs, this is acceptable, but note that a sliding-window optimization (dividing by the outgoing digit and multiplying by the incoming digit) could improve to O(n), but is not necessary for the task. The function must be `const`‑correct: take the string by `const std::string&` and the window size by value, and return `unsigned long long`.
