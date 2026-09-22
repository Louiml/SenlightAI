// Write a C++ function named `isPrime` that takes a single positive integer `n` as input and returns a boolean value indicating whether `n` is a prime number. A prime number is a natural number greater than 1 that has no positive divisors other than 1 and itself. Your function must efficiently determine primality by checking divisibility only up to the square root of `n`, and it must handle edge cases such as `n = 1` (not prime) and `n = 2` (prime) correctly. The function should accept `int` or `long long` and be `const`-correct where appropriate.
// The standard primality test algorithm works as follows. If `n` is less than 2, it is not prime. Otherwise, count the number of divisors found by iterating `i` from 1 to `sqrt(n)` (inclusive). For each `i` that divides `n`, increment a counter. If at any point the counter exceeds 1, break early (since more than one divisor means it is not prime). After the loop, if exactly one divisor was found (which must be `i = 1`), then `n` is prime; otherwise, it is composite. Important edge cases: `n = 1` (loop finds divisor `1` only, counter = 1, but 1 is not prime, so we explicitly return false for `n < 2`), `n = 2` (loop checks `i=1`, counter=1, then `i=2` counts as divisor, counter becomes 2 → not prime? Wait: careful – the divisor `n` itself is checked only up to `sqrt(n)`, so for `n=2`, `sqrt(2)≈1.414`, so the loop runs only for `i=1`. That counts one divisor, and since counter == 1, we would incorrectly return true for 2? Actually that is correct: 2 is prime. The code from the snippet breaks when counter > 1, so for `n=2` it only counts `i=1`, returns true. For `n=4`, `i=1` (counter=1), `i=2` (counter=2, break), returns false. For `n=1`, the loop runs `i=1` (counter=1) but we must return false because 1 is not prime – handle that with `if (n < 2) return false;`. Time complexity is O(√n) due to the loop, and space complexity is O(1) as only a few variables are used.
#include <cmath>

// Return true if n is a prime number, false otherwise.
// n must be a non-negative integer.
bool isPrime(long long n) {
    if (n < 2) return false;

    long long divisorCount = 0;
    for (long long i = 1; i <= static_cast<long long>(std::sqrt(n)); ++i) {
        if (n % i == 0) {
            ++divisorCount;
            if (divisorCount > 1) break;
        }
    }
    return divisorCount == 1;
}
#include <cassert>

int main() {
    assert(isPrime(1) == false);
    assert(isPrime(2) == true);
    assert(isPrime(3) == true);
    assert(isPrime(4) == false);
    assert(isPrime(17) == true);
    assert(isPrime(25) == false);
    assert(isPrime(97) == true);
    assert(isPrime(100) == false);
    assert(isPrime(999983) == true);
    assert(isPrime(1000000007LL) == true);
    return 0;
}
