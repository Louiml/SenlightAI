// Write a C++ function named `classifyNumber` that takes a single positive integer `n` as input and returns a string indicating whether the number is "prime", "composite", or, if the number is 1, "neither prime nor composite". The function must handle all positive integers, including 1 and 2, correctly, and must not read from standard input or print to standard output. The return value must be exactly one of the three specified strings (lowercase, space-separated as shown).

#include <cassert>
#include <string>

// Function declaration (provided by solution)
std::string classifyNumber(int n);

int main() {
    // Test edge cases and basic classification
    assert(classifyNumber(1) == "neither prime nor composite");
    assert(classifyNumber(2) == "prime");
    assert(classifyNumber(3) == "prime");
    assert(classifyNumber(4) == "composite");
    assert(classifyNumber(9) == "composite");
    assert(classifyNumber(17) == "prime");
    assert(classifyNumber(100) == "composite");
    assert(classifyNumber(97) == "prime");
    assert(classifyNumber(121) == "composite");
    assert(classifyNumber(2 * 3 * 5 * 7 * 11 * 13) == "composite"); // 30030
    return 0;
}

#include <string>
#include <cmath>

// Classify a positive integer as "prime", "composite", or "neither prime nor composite".
// Precondition: n >= 1.
std::string classifyNumber(int n) {
    if (n == 1) {
        return "neither prime nor composite";
    }
    // Start checking from 2; if n is 2, the loop condition fails immediately.
    for (int i = 2; i <= std::sqrt(n); ++i) {
        if (n % i == 0) {
            return "composite";
        }
    }
    return "prime";
}

// The core algorithm is trial division. For a number `n`, we check divisibility by every integer from 2 up to `n-1` (or more efficiently up to `sqrt(n)`). If any divisor is found, the number is composite; otherwise, it is prime. The special case is `n == 1`, which is neither prime nor composite. Edge cases include `n == 2` (the smallest prime), where the loop range is empty and the flag remains true. Also, for `n == 0` or negative numbers, the problem specification says positive integer; but to be safe, we could treat non-positive numbers as invalid; however, since the task specifies positive integers, we assume valid input. Time complexity: O(n) if we check up to n-1, or O(√n) if we check up to sqrt(n), which is better. Space complexity is O(1) auxiliary. I will implement the optimized version checking up to `sqrt(n)` to be efficient.
