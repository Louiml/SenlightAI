/*
Write a C++ function named `classifyPrime` that takes a positive integer `num` and returns a `std::string` containing either `"<num> is prime"` or `"<num> is not prime"` (with the actual number substituted). The function must correctly identify prime numbers (numbers greater than 1 that have no positive divisors other than 1 and themselves) and handle the edge case of `num = 1` (which is not prime). The function should not use any global state, should be `const`-correct (i.e., not modify input, take parameter by value or const reference), and must be efficient for values up to at least 2,000,000 by checking divisors only up to the square root of `num`. The function must return the exact string format as specified, with a single space before "is" and after "is", and no extra leading or trailing spaces.
*/

#include <string>

// Returns a string indicating whether the given positive integer is prime.
// Precondition: num > 0.
std::string classifyPrime(const int num) {
    if (num < 2) {
        return std::to_string(num) + " is not prime";
    }
    // Check divisors from 2 up to sqrt(num) using i*i <= num to avoid sqrt().
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            return std::to_string(num) + " is not prime";
        }
    }
    return std::to_string(num) + " is prime";
}

#include <cassert>
#include <string>

// Assume classifyPrime is declared above (in a header or same file).
int main() {
    // Edge cases
    assert(classifyPrime(1) == "1 is not prime");
    assert(classifyPrime(2) == "2 is prime");
    assert(classifyPrime(3) == "3 is prime");

    // Small composites
    assert(classifyPrime(4) == "4 is not prime");
    assert(classifyPrime(9) == "9 is not prime");
    assert(classifyPrime(15) == "15 is not prime");

    // Larger primes
    assert(classifyPrime(97) == "97 is prime");
    assert(classifyPrime(7919) == "7919 is prime");  // 1000th prime

    // Larger composite
    assert(classifyPrime(1000000) == "1000000 is not prime");

    // Perfect square composite
    assert(classifyPrime(121) == "121 is not prime");

    return 0;
}

// The solution determines whether `num` is prime by testing divisibility. If `num` is less than 2, it is not prime (since 0 and 1 are not prime, and negative numbers are not considered). For `num >= 2`, we check divisors from 2 up to `sqrt(num)` (inclusive). If any divisor divides `num` evenly, `num` is composite; otherwise it is prime. This works because if `num` has a divisor greater than `sqrt(num)`, the complementary divisor must be less than `sqrt(num)`, so testing only up to the square root covers all possibilities. Edge cases: `num = 1` returns "not prime", `num = 2` and `num = 3` are prime (loop starts at 2, but for 2 the loop runs with `i=2` and `num%2 == 0`? Actually for `num=2`, `i=2` and `sqrt(2)≈1.4`, so loop condition `i <= sqrt(num)` fails because 2 <= 1.4 is false; need to use `i*i <= num` to avoid floating-point issues. The loop runs from 2 to `sqrt(num)` inclusive via `i*i <= num`. Time complexity is O(√n) for each call, space complexity is O(1).
