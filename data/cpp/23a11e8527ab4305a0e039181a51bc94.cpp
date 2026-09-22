Write a C++ function `int countDistinctPrimePermutations(const std::string& digits)` that takes a non-empty string of decimal digits (each character between '0' and '9', possibly with leading zeros) and returns the number of **distinct prime numbers** that can be formed using any non‑empty prefix of any permutation of the given digits. That is, for every permutation of the original string (after sorting it lexicographically), consider all prefixes of that permutation (length 1 up to the full length), convert each prefix to an integer (with `stoi` semantics, so leading zeros are dropped), and count each distinct prime value only once. For example, if the input is `"17"`, the permutations are `"17"` and `"71"`. Their prefixes are `1`, `17`, `7`, `71`. The primes among these are `17` and `7` (and `71` is prime too, but note `71` is also a prefix of the permutation `"71"`). The distinct primes are `{7, 17, 71}` — so the answer is `3`. The input length is between 1 and 7, inclusive, to keep runtime reasonable. A number is prime if it is greater than 1 and has no divisors other than 1 and itself. The order of digits in the permutations matters, and duplicate permutations are allowed but primes are counted only once.

#include <cassert>
#include <string>

int main() {
    // Single digit: '2' is prime, '3' is prime, '0' and '1' are not
    assert(countDistinctPrimePermutations("2") == 1);
    assert(countDistinctPrimePermutations("3") == 1);
    assert(countDistinctPrimePermutations("0") == 0);
    assert(countDistinctPrimePermutations("1") == 0);
    
    // Two digits "17": permutations "17" (prefixes: 1,17), "71" (prefixes: 7,71)
    // Distinct primes: 7, 17, 71 -> 3
    assert(countDistinctPrimePermutations("17") == 3);
    
    // "23": permutations "23" (2,23), "32" (3,32). Primes: 2,23,3 -> 3
    assert(countDistinctPrimePermutations("23") == 3);
    
    // "011": permutations "011" (0,1,11), "101" (1,10,101), "110" (1,11,110)
    // Values: 0,1,11,1,10,101,1,11,110 → distinct primes: 11,101 → 2
    assert(countDistinctPrimePermutations("011") == 2);
    
    // "7" repeated: "77" → permutations "77" (7,77), "77" (7,77) → distinct: 7 and 77? 77 not prime, so just 7
    assert(countDistinctPrimePermutations("77") == 1);
    
    // "999" → permutations all "999" → prefixes: 9,99,999 (none prime) → 0
    assert(countDistinctPrimePermutations("999") == 0);
    
    // "13" → permutations "13" (1,13), "31" (3,31) → primes: 13,3,31 → 3
    assert(countDistinctPrimePermutations("13") == 3);
    
    // "10" → permutations "01" (0,1), "10" (1,10) → no primes → 0
    assert(countDistinctPrimePermutations("10") == 0);
    
    return 0;
}

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cmath>

// Helper: returns true if n is prime (n > 1 and has no divisors except 1 and itself)
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

// Main function: counts distinct prime numbers formed from prefixes of all permutations
int countDistinctPrimePermutations(const std::string& digits) {
    std::string sortedDigits = digits;
    std::sort(sortedDigits.begin(), sortedDigits.end());
    std::unordered_set<int> primes;
    
    do {
        for (size_t len = 1; len <= sortedDigits.size(); ++len) {
            int value = std::stoi(sortedDigits.substr(0, len));
            if (isPrime(value)) {
                primes.insert(value);
            }
        }
    } while (std::next_permutation(sortedDigits.begin(), sortedDigits.end()));
    
    return static_cast<int>(primes.size());
}

// The core approach is to generate all distinct permutations of the sorted digit string using `std::next_permutation`. For each permutation, we iterate over every possible prefix length from 1 to the string length. For each prefix, we convert it to an integer using `std::stoi` (this automatically discards leading zeros, e.g., `"007"` becomes `7`). Then we test primality with a simple trial division up to the square root of the number. If the number is prime, we insert it into a `std::unordered_set<int>` to deduplicate. After processing all permutations and all prefixes, the answer is the size of that set. Edge cases: the input may contain zeros, so many prefixes will produce the same integer (e.g., `"00"` → 0, which is not prime). We must handle numbers like `0` and `1` as non-prime. Since the maximum length is 7, the maximum integer formed is at most 9,999,999, so `int` is sufficient. Time complexity: There are at most 7! = 5040 permutations, and for each we try at most 7 prefixes, giving about 35,000 conversions and primality tests. Each primality test is O(sqrt(N)) ≤ O(3162). Overall it's well within limits. Space complexity is O(number of distinct primes), which is small, plus the set overhead.
