Write a standalone C++ function `computeLowestPrimeFactors(int n)` that returns a `std::vector<int>` where the element at index `i` (for `2 <= i <= n`) is the smallest prime factor of `i`, and for index `0` and `1` the value should be `0`. The function must use an efficient sieve-based approach and handle `n` up to at least `1e6`. Additionally, implement a helper function `printPrimeFactors(int x, const std::vector<int>& spf)` that prints all prime factors of `x` (with multiplicity) separated by spaces using the precomputed smallest-prime-factor table. The task must demonstrate both correct factorization logic and efficient precomputation without modifying the input.

// The core algorithm is a standard smallest-prime-factor (SPF) sieve. We initialize a vector `spf` of size `n+1` filled with zeros. For each integer `i` from `2` to `n`, if `spf[i]` is still `0` (meaning `i` is prime), we set `spf[i] = i` and then mark all multiples `j` of `i` (starting from `i*i` to avoid redundant work) with `spf[j] = i`—but only if `spf[j]` is still `0`, so the first (smallest) prime found remains. This ensures each composite gets its smallest prime factor. Edge cases: `n < 2` returns a vector with zeros; `x` in `printPrimeFactors` must be between `2` and the size of `spf - 1`; the inner loop for factor extraction uses `while (n % spf[n] == 0)` to catch repeated factors. Time complexity precomputation is `O(n log log n)` due to the sieve, and each factorization in `printPrimeFactors` is `O(log x)` in the worst case. Space complexity is `O(n)` for the SPF vector.

#include <vector>
#include <iostream>

// Returns a vector where index i holds the smallest prime factor of i (for i>=2), else 0.
std::vector<int> computeLowestPrimeFactors(int n) {
    std::vector<int> spf(n + 1, 0);
    for (int i = 2; i <= n; ++i) {
        if (spf[i] == 0) { // i is prime
            spf[i] = i;
            // start from i*i to avoid redundant marking of smaller multiples
            for (long long j = 1LL * i * i; j <= n; j += i) {
                if (spf[static_cast<int>(j)] == 0) {
                    spf[static_cast<int>(j)] = i;
                }
            }
        }
    }
    return spf;
}

// Prints all prime factors of x (with multiplicity) using spf table.
// Assumes spf is valid and x is in [2, spf.size()-1].
void printPrimeFactors(int x, const std::vector<int>& spf) {
    int num = x;
    while (num > 1) {
        int p = spf[num];
        // p must be non-zero because spf is properly computed
        while (num % p == 0) {
            std::cout << p << ' ';
            num /= p;
        }
    }
}

#include <cassert>
#include <vector>
#include <sstream>

int main() {
    // Test small values
    std::vector<int> spf_small = computeLowestPrimeFactors(10);
    assert(spf_small[0] == 0);
    assert(spf_small[1] == 0);
    assert(spf_small[2] == 2);
    assert(spf_small[3] == 3);
    assert(spf_small[4] == 2);
    assert(spf_small[5] == 5);
    assert(spf_small[6] == 2);
    assert(spf_small[7] == 7);
    assert(spf_small[8] == 2);
    assert(spf_small[9] == 3);
    assert(spf_small[10] == 2);

    // Test factorization output via stringstream
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf(ss.rdbuf());
    printPrimeFactors(12, spf_small);
    std::cout.rdbuf(old_cout);
    assert(ss.str() == "2 2 3 ");

    ss.str("");
    ss.clear();
    old_cout = std::cout.rdbuf(ss.rdbuf());
    printPrimeFactors(18, spf_small);
    std::cout.rdbuf(old_cout);
    assert(ss.str() == "2 3 3 ");

    // Test larger n
    std::vector<int> spf_large = computeLowestPrimeFactors(100);
    assert(spf_large[97] == 97); // prime
    assert(spf_large[99] == 3);  // 99 = 3*33
    assert(spf_large[100] == 2); // 100 = 2^2 * 5^2

    // Test edge: n < 2
    std::vector<int> spf_zero = computeLowestPrimeFactors(1);
    assert(spf_zero.size() == 2);
    assert(spf_zero[0] == 0 && spf_zero[1] == 0);

    return 0;
}
