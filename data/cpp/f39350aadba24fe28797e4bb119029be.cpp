// Write a C++ function `smallestPrimeGreaterThan` that, given a positive integer `start` (which may be composite, prime, or 1), returns the smallest prime number strictly greater than `start`. The function must use trial division by odd numbers up to the square root of the candidate, skipping even numbers (except 2), and must handle small inputs efficiently (e.g., `start = 1` returns 2, `start = 2` returns 3, `start = 3` returns 5). The function should be declared as `long long smallestPrimeGreaterThan(long long start)` and should not include a `main` function.

int main() {
    assert(smallestPrimeGreaterThan(1) == 2);
    assert(smallestPrimeGreaterThan(2) == 3);
    assert(smallestPrimeGreaterThan(3) == 5);
    assert(smallestPrimeGreaterThan(4) == 5);
    assert(smallestPrimeGreaterThan(10) == 11);
    assert(smallestPrimeGreaterThan(14) == 17);
    assert(smallestPrimeGreaterThan(100) == 101);
    assert(smallestPrimeGreaterThan(101) == 103);
    assert(smallestPrimeGreaterThan(200) == 211);
    assert(smallestPrimeGreaterThan(1000) == 1009);
}

#include <cmath>

// Returns the smallest prime strictly greater than start.
long long smallestPrimeGreaterThan(long long start) {
    if (start < 2) return 2;
    long long candidate = start + 1;
    while (true) {
        if (candidate == 2) return candidate;
        if (candidate % 2 == 0) {
            ++candidate;
            continue;
        }
        bool isPrime = true;
        for (long long divisor = 3; divisor * divisor <= candidate; divisor += 2) {
            if (candidate % divisor == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) return candidate;
        ++candidate;
    }
}

// The algorithm is straightforward: check numbers starting from `start + 1` upward. For each candidate `n`, if `n` is less than 2, skip; if `n` equals 2, it's prime; if `n` is even and greater than 2, skip because it cannot be prime. For odd numbers, test divisibility by all odd integers from 3 up to the square root of `n`. If any divisor is found, the number is composite; otherwise, it is prime, and we return it. Edge cases: `start = 0` or `start = 1` returns 2; `start = 2` returns 3; large `start` values may require many primality tests, but trial division is acceptable for typical test inputs. Time complexity: worst-case O(k sqrt(m)) where k is the number of candidates tested and m is the largest candidate; space complexity O(1).
