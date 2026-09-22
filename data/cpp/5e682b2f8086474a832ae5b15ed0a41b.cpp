// Write a C++ function `int countPrimeCandidates(int n, int k)` that converts the decimal integer `n` to base `k` (where `2 ≤ k ≤ 9`), splits the resulting base-`k` representation at every occurrence of the digit `0`, and counts how many of the resulting numeric substrings (interpreted as decimal numbers) are prime numbers greater than 1. For example, if `n = 437674` and `k = 3`, the base-3 representation is `211020101011`, splitting on zeros gives `["211", "2", "1", "1", "11"]`, and the primes are `211`, `2`, and `11` (since `1` is not prime), so the function returns `3`. Assume `n` is a non-negative integer; if `n == 0`, the function should return `0` because the base-`k` representation is just `"0"`, and no non‑zero substrings occur. Ignore leading zeros that may appear after a split (e.g., `"001"` becomes `1` and is not prime). You may use the `sqrt` primality check for numbers up to `10^7`.
The solution has three phases. First, compute the base-`k` digits of `n` using repeated division, storing digits in a vector from least significant to most significant; if `n == 0`, immediately return `0` to avoid an empty vector. Second, traverse the digits from most significant (reverse iteration) to build substrings separated by `0`s: accumulate non-zero digits into a string, and whenever a `0` is encountered (or the digit list ends), convert the accumulated string to a `long long` using `stoll` and push it into a list of candidate numbers; reset the string for the next substring. Third, for each candidate number greater than 1, test primality by checking divisibility from 2 up to `sqrt(candidate)` (using integer arithmetic with `static_cast<long long>(std::sqrt(...))` to avoid floating precision issues); if no divisor is found, increment the answer. Edge cases include `n == 0` (no candidates), `n` being a single digit with no zeros (one candidate), consecutive zeros (skip empty substrings), and numbers like `1` or `0` which are not prime. Time complexity is `O(d √m)` where `d` is the number of base-`k` digits and `m` is the maximum candidate value; space complexity is `O(d)` for digit storage and candidate list.
#include <string>
#include <vector>
#include <cmath>

// Convert n to base k, split at zeros, and count prime substrings.
int countPrimeCandidates(int n, int k) {
    if (n == 0) return 0;

    // Extract base-k digits (least significant first).
    std::vector<int> digits;
    while (n > 0) {
        digits.push_back(n % k);
        n /= k;
    }

    // Build candidate numbers by splitting at zeros.
    std::vector<long long> candidates;
    std::string current;
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        if (*it != 0) {
            current += static_cast<char>('0' + *it);
        } else {
            if (!current.empty()) {
                candidates.push_back(std::stoll(current));
                current.clear();
            }
        }
    }
    if (!current.empty()) {
        candidates.push_back(std::stoll(current));
    }

    // Count primes among candidates.
    int primeCount = 0;
    for (long long candidate : candidates) {
        if (candidate <= 1) continue;
        bool isPrime = true;
        long long limit = static_cast<long long>(std::sqrt(static_cast<double>(candidate)));
        for (long long divisor = 2; divisor <= limit; ++divisor) {
            if (candidate % divisor == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) ++primeCount;
    }
    return primeCount;
}
#include <cassert>

int main() {
    // 437674 in base 3 = "211020101011" → candidates: 211,2,1,1,11 → primes: 211,2,11
    assert(countPrimeCandidates(437674, 3) == 3);

    // 110011 in base 10 → candidates: 11, 11 → both prime
    assert(countPrimeCandidates(110011, 10) == 2);

    // No zeros → entire number as candidate; 17 is prime
    assert(countPrimeCandidates(17, 10) == 1);

    // Candidate is 1 → not prime
    assert(countPrimeCandidates(1, 2) == 0);

    // n = 0 → return 0
    assert(countPrimeCandidates(0, 5) == 0);

    // 101 in base 2 = "101" → only candidate 5 → prime
    assert(countPrimeCandidates(5, 2) == 1);

    // 1001 in base 2 → candidates: 1, 1 → none prime
    assert(countPrimeCandidates(9, 2) == 0);

    // Base 8 example: 33 in base 8 = "41" → candidate 41 (prime)
    assert(countPrimeCandidates(33, 8) == 1);

    // Large candidate: 999999937 is prime, but 999999937 in decimal has no zeros → count 1
    assert(countPrimeCandidates(999999937, 10) == 1);

    // Multiple zeros: 2002 in base 10 → candidates: 2, 2 → both prime
    assert(countPrimeCandidates(2002, 10) == 2);
}
