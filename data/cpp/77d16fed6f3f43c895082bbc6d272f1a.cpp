/*
Write a C++ function named `printPrimeNumbersUpTo` that takes a single positive integer `n` as input and returns a `std::vector<int>` containing all prime numbers from 2 up to and including `n`, in ascending order. The function must correctly handle edge cases: if `n` is less than 2, the returned vector should be empty; if `n` is 2, the vector should contain just `{2}`. The primality test used inside must be efficient enough for `n` up to 10^6, meaning it should check divisors only up to the square root of the candidate, skipping even numbers after testing 2 as a special case. The function should be standalone, not rely on global variables, and should use `const` where appropriate for parameters and local variables that are not modified.
*/

#include <vector>
#include <cmath>

// Helper: returns true if the given positive integer is prime.
bool isPrime(int number) {
    if (number < 2) return false;
    if (number == 2) return true;
    if (number % 2 == 0) return false;

    const int limit = static_cast<int>(std::sqrt(number));
    for (int divisor = 3; divisor <= limit; divisor += 2) {
        if (number % divisor == 0) return false;
    }
    return true;
}

// Return all prime numbers from 2 up to n (inclusive), in ascending order.
std::vector<int> printPrimeNumbersUpTo(const int n) {
    std::vector<int> primes;
    for (int candidate = 2; candidate <= n; ++candidate) {
        if (isPrime(candidate)) {
            primes.push_back(candidate);
        }
    }
    return primes;
}

#include <cassert>
#include <vector>

int main() {
    // Edge case: n < 2 → empty vector
    assert(printPrimeNumbersUpTo(1) == std::vector<int>{});
    assert(printPrimeNumbersUpTo(0) == std::vector<int>{});

    // n = 2 → only 2
    assert(printPrimeNumbersUpTo(2) == std::vector<int>{2});

    // n = 10 → {2, 3, 5, 7}
    assert(printPrimeNumbersUpTo(10) == std::vector<int>({2, 3, 5, 7}));

    // n = 20 → {2, 3, 5, 7, 11, 13, 17, 19}
    assert(printPrimeNumbersUpTo(20) == std::vector<int>({2, 3, 5, 7, 11, 13, 17, 19}));

    // n = 100 → check first and last primes
    auto primes = printPrimeNumbersUpTo(100);
    assert(primes.size() == 25);
    assert(primes.front() == 2);
    assert(primes.back() == 97);

    // n = 1000 → check known count (168 primes)
    assert(printPrimeNumbersUpTo(1000).size() == 168);

    return 0;
}

// The solution iterates from `2` to `n`, and for each number `i`, determines if it is prime using a helper function `isPrime`. The primality check works as follows: any number less than 2 is not prime; 2 is prime; even numbers greater than 2 are not prime (to avoid redundant checks); for odd numbers, test divisibility by odd divisors from 3 up to `sqrt(i)` — if any divides evenly, it is not prime. The helper returns `true` for prime numbers, and the main function appends those to the result vector. Edge cases include `n < 2` (empty vector) and `n == 2` (single element). Time complexity is \(O(n \sqrt{n})\) in the worst case due to repeated square‑root checks, but in practice with the even‑skip optimization it is acceptable for \(n\) up to \(10^6\). Space complexity is \(O(\pi(n))\) for the output vector, and \(O(1)\) auxiliary space besides the output.
