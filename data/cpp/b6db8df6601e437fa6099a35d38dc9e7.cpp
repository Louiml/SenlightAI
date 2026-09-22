/*
Write a C++ function named `calculateLCMAndGCD` that takes two unsigned long integers `a` and `b` as input and returns a `std::pair<unsigned long, unsigned long>` where the first element is the least common multiple (LCM) and the second element is the greatest common divisor (GCD) of the two numbers. The function must handle the case where either input is zero gracefully (the GCD of zero and any number is the non-zero number, and the LCM of zero with any number is conventionally zero to avoid division by zero). Your task is to implement this function without using any external libraries beyond `<utility>` and `<limits>`, and it must use the Euclidean algorithm for GCD computation recursively or iteratively. The function should be `const`-correct and avoid integer overflow in the LCM calculation by dividing before multiplying.
*/

#include <utility>  // for std::pair

// Returns a pair (lcm, gcd) for two unsigned long integers.
// Handles zero inputs: gcd(0, x) = x, lcm = 0 to avoid division by zero.
std::pair<unsigned long, unsigned long> calculateLCMAndGCD(unsigned long a, unsigned long b) {
    // Compute GCD using iterative Euclidean algorithm.
    unsigned long x = a;
    unsigned long y = b;
    while (y != 0) {
        unsigned long temp = y;
        y = x % y;
        x = temp;
    }
    unsigned long gcd = x;

    // Compute LCM safely: divide first to avoid overflow.
    // If gcd is 0, both inputs are 0, so LCM is 0.
    unsigned long lcm = 0;
    if (gcd != 0) {
        lcm = (a / gcd) * b;
    }

    return {lcm, gcd};
}

#include <cassert>
#include <utility>

// Solution function declaration from the provided code above.
std::pair<unsigned long, unsigned long> calculateLCMAndGCD(unsigned long a, unsigned long b);

int main() {
    // Normal cases
    assert(calculateLCMAndGCD(4, 6) == std::make_pair(12, 2));
    assert(calculateLCMAndGCD(15, 20) == std::make_pair(60, 5));
    assert(calculateLCMAndGCD(7, 5) == std::make_pair(35, 1));
    // One zero
    assert(calculateLCMAndGCD(0, 5) == std::make_pair(0, 5));
    assert(calculateLCMAndGCD(8, 0) == std::make_pair(0, 8));
    // Both zero
    assert(calculateLCMAndGCD(0, 0) == std::make_pair(0, 0));
    // Large numbers (no overflow check)
    assert(calculateLCMAndGCD(1000000000UL, 1000000000UL) == std::make_pair(1000000000UL, 1000000000UL));
    // Equal numbers
    assert(calculateLCMAndGCD(12, 12) == std::make_pair(12, 12));
    // One divides the other
    assert(calculateLCMAndGCD(9, 3) == std::make_pair(9, 3));
    assert(calculateLCMAndGCD(2, 9) == std::make_pair(18, 1));
    return 0;
}

// The solution uses the Euclidean algorithm to compute the GCD: repeatedly replace `(a, b)` with `(b, a % b)` until `b` becomes zero, at which point `a` is the GCD. For the LCM, the standard formula is `lcm(a, b) = |a * b| / gcd(a, b)`, but to avoid overflow, we compute `a / gcd(a, b) * b` (or `b / gcd * a`) since the GCD always divides one of the numbers exactly. Edge cases: if either input is zero, the GCD is the non-zero value (or zero if both are zero), and the LCM is zero (since any multiple of zero is zero, and the formula with division by GCD would be invalid when GCD is zero). Time complexity is `O(log(min(a, b)))` due to the Euclidean algorithm's logarithmic steps, and space complexity is `O(1)` for an iterative version or `O(log n)` for recursive due to call stack, but the iterative version is preferred here. The numbers are `unsigned long` so overflow is less of a concern, but the division-first approach still protects against edge cases where `a * b` could exceed the range.
