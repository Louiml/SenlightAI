// Write a C++ function `countPrimesInRange(int l, int r)` that takes two integers `l` and `r` representing an inclusive range \( [l, r] \), with \( 1 \le l \le r \) and \( r \le 10^7 \), and returns the number of prime numbers strictly between `l` and `r` inclusive. Note that the range may be small (e.g., `l = r = 1`) or very large (up to 10 million), so you cannot simply use a full sieve up to `r` for every call if the range is large; however, for this task you should implement an efficient segmented sieve that marks composite numbers only within the given range using primes up to \(\sqrt{r}\). The function must handle the edge case where `l = 1` (since 1 is not prime) and must correctly count primes in a range where the segment size could be as small as 1. The function should be deterministic and return an `int` count. Your implementation must not use any global variables and must be self-contained.

#include <cassert>

int main() {
    // Basic cases
    assert(countPrimesInRange(2, 10) == 4);      // 2,3,5,7
    assert(countPrimesInRange(10, 20) == 4);     // 11,13,17,19
    assert(countPrimesInRange(1, 1) == 0);       // 1 is not prime
    assert(countPrimesInRange(2, 2) == 1);       // 2 is prime
    assert(countPrimesInRange(1, 2) == 1);       // only 2
    
    // Edge cases
    assert(countPrimesInRange(1, 100) == 25);    // known prime count up to 100
    assert(countPrimesInRange(90, 100) == 1);    // only 97
    assert(countPrimesInRange(1000, 1100) == 16); // verified count
    assert(countPrimesInRange(999900, 1000000) == 68); // large range, but small segment
    assert(countPrimesInRange(10000000, 10000000) == 0); // 10^7 is not prime
    
    return 0;
}

#include <vector>
#include <cmath>

// Count the number of primes in the inclusive range [l, r].
// Preconditions: 1 <= l <= r <= 10^7.
int countPrimesInRange(int l, int r) {
    if (r < 2) return 0;
    
    // Step 1: Sieve primes up to sqrt(r)
    const int limit = static_cast<int>(std::sqrt(r));
    std::vector<bool> isPrimeSmall(limit + 1, true);
    std::vector<int> smallPrimes;
    
    if (limit >= 2) {
        isPrimeSmall[0] = isPrimeSmall[1] = false;
        for (int i = 2; i <= limit; ++i) {
            if (isPrimeSmall[i]) {
                smallPrimes.push_back(i);
                if (static_cast<long long>(i) * i <= limit) {
                    for (long long j = static_cast<long long>(i) * i; j <= limit; j += i) {
                        isPrimeSmall[j] = false;
                    }
                }
            }
        }
    }
    
    // Step 2: Segmented sieve for [l, r]
    const int segmentSize = r - l + 1;
    std::vector<bool> isComposite(segmentSize, false);
    
    for (int p : smallPrimes) {
        long long start = std::max(static_cast<long long>(p) * p, (l + p - 1LL) / p * 1LL * p);
        for (long long j = start; j <= r; j += p) {
            isComposite[j - l] = true;
        }
    }
    
    // Handle the special case: 1 is not prime
    if (l <= 1 && r >= 1) {
        isComposite[1 - l] = true;
    }
    
    // Count primes
    int count = 0;
    for (int i = 0; i < segmentSize; ++i) {
        if (!isComposite[i]) {
            ++count;
        }
    }
    return count;
}

// The core idea is a segmented sieve. First, generate all primes up to \(\sqrt{r}\) using a simple sieve (or any efficient method). Then, for each prime `p` from that list, mark all multiples of `p` within the range \([l, r]\) as composite. The starting multiple should be \(\max(p^2, \lceil l/p \rceil \cdot p)\) to avoid marking `p` itself (which may be inside the range) as composite. After marking, iterate through the range and count numbers not marked as composite. Important edge cases: if `l` is 1, then 1 must be explicitly marked as composite (since 1 is not prime). Also, if `l` is less than 2, ensure that negative or zero numbers are not considered (though the problem guarantees `l >= 1`). For efficiency, the sieve for primes up to \(\sqrt{r}\) takes \(O(\sqrt{r} \log \log \sqrt{r})\) time, and the marking step takes \(O((r-l+1) \cdot \log \log r)\) roughly, but more precisely each prime `p` marks about \((r-l+1)/p\) composites, so total marking work is \(O((r-l+1) \cdot \sum_{p \le \sqrt{r}} 1/p)\) which is roughly \(O((r-l+1) \log \log r)\). Space complexity is \(O(\sqrt{r})\) for the prime list plus \(O(r-l+1)\) for the boolean segment vector. For a single call, this is efficient even when the range is large. The function returns an `int` because the maximum possible primes up to 10^7 is about 664,579, which fits within `int`.
