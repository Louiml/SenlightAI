// Write a C++ function that computes the least common multiple (LCM) of all integers in the range [1, n], where n is a positive integer parameter. The function should take an integer `upper_limit` (representing the highest number in the range) and return an `unsigned long long` value representing the smallest positive number that is evenly divisible by every integer from 1 to `upper_limit` inclusive. The function must handle edge cases gracefully: for `upper_limit` equal to 1, the result should be 1. Ensure the implementation is robust for values up to at least 43 without overflow, using 64-bit unsigned arithmetic. You may assume the input is always positive (≥ 1).
// The core algorithm is to iteratively compute the LCM of the running result and each integer in the range. The LCM of two numbers `a` and `b` is given by `(a / gcd(a, b)) * b` to avoid overflow, using the standard Euclidean algorithm for GCD. Starting with `result = 1`, for each `i` from 2 to `upper_limit`, we update `result = lcm(result, i)`. This works because LCM is associative: the LCM of a set is the LCM of the running result and the next number. Edge cases: when `upper_limit` is 1, the loop does not execute and `result` remains 1, which is correct. Since we multiply by dividing by the GCD first, the intermediate values stay within the 64-bit range for the tested limit (n ≤ 43 gives a result < 2^64). Time complexity is O(n log min(result, i)) due to the GCD computation per iteration, but practically O(n) for these sizes; space complexity is O(1). The algorithm is straightforward and avoids the need for prime factorization.
#include <cstdint>

// Compute GCD using the Euclidean algorithm.
static std::uint64_t gcd(std::uint64_t a, std::uint64_t b) {
    while (b != 0) {
        std::uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Compute LCM of a and b without overflow risk by dividing first.
static std::uint64_t lcm(std::uint64_t a, std::uint64_t b) {
    return (a / gcd(a, b)) * b;
}

// Return the smallest positive integer divisible by all integers from 1 to upper_limit inclusive.
std::uint64_t smallest_evenly_divisible(unsigned int upper_limit) {
    std::uint64_t result = 1;
    for (unsigned int i = 2; i <= upper_limit; ++i) {
        result = lcm(result, static_cast<std::uint64_t>(i));
    }
    return result;
}
#include <cassert>
#include <cstdint>

int main() {
    assert(smallest_evenly_divisible(1) == 1ULL);
    assert(smallest_evenly_divisible(2) == 2ULL);
    assert(smallest_evenly_divisible(3) == 6ULL);
    assert(smallest_evenly_divisible(4) == 12ULL);
    assert(smallest_evenly_divisible(5) == 60ULL);
    assert(smallest_evenly_divisible(10) == 2520ULL);
    assert(smallest_evenly_divisible(20) == 232792560ULL);
    assert(smallest_evenly_divisible(30) == 2329089562800ULL);
    return 0;
}
