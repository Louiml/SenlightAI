// Write a C++ function `countPrimesUpTo` that takes a positive integer `n` and returns the number of prime numbers in the range `[2, n]`. The function must implement an efficient version of the sieve of Eratosthenes that processes the sieve in fixed-size windows to keep memory usage low (approximately `O(sqrt(n))`), rather than allocating a full array of size `n`. The function should correctly handle edge cases such as `n = 0`, `n = 1`, and `n = 2`, and must return the count as an `unsigned long long` to accommodate large inputs. The function must not use external parallel libraries; a serial implementation is sufficient. Include a helper class or struct that manages the prime factors and window processing.

The solution is based on a segmented (windowed) sieve of Eratosthenes. The core idea: first, enumerate all primes up to `sqrt(n)` using a simple sieve on a small boolean array. Then, process the range `[sqrt(n), n]` in consecutive windows of size `W` (typically `W = sqrt(n)` or a fixed constant like 256 or 1024), striking out multiples of the small primes within each window. The small primes are stored in a vector, and for each window we maintain an array of booleans of size `W`. For each small prime `p`, we compute the first multiple of `p` within the current window and mark it and all subsequent multiples as composite. After striking out all multiples, we count the unmarked numbers in the window (which are primes, adjusted for parity and the fact that we skip even numbers).

Important edge cases:
- `n < 2`: return 0 because there are no primes.
- `n = 2`: return 1.
- When the window size is larger than the remaining range, shrink the window accordingly.
- Handle even numbers separately (we only need to consider odd numbers beyond 2 to halve memory and time).

Time complexity: The outer sieve up to `sqrt(n)` takes `O(sqrt(n) log log sqrt(n))` time. For each of the `n / W` windows, we strike out multiples of each small prime. The total work across all windows is roughly `O(n log log n)` because each composite number is struck out once per prime factor. Thus the overall time is `O(n log log n)`. Space complexity: We store primes up to `sqrt(n)` (`O(sqrt(n))` space) and a window of size `W` (constant or `O(sqrt(n))`), giving `O(sqrt(n))` auxiliary space.

#include <vector>
#include <cmath>
#include <cstdint>

// Counts the number of primes in [2, n] using a segmented sieve.
// Input n is a non-negative integer.
// Returns the count as unsigned long long.
unsigned long long countPrimesUpTo(unsigned long long n) {
    if (n < 2) return 0;
    if (n == 2) return 1;

    // Step 1: find all primes up to sqrt(n) using a simple sieve.
    unsigned long long limit = static_cast<unsigned long long>(std::sqrt(static_cast<double>(n)));
    // Only need odd numbers, so size = (limit / 2) + 1, but we'll use a vector of bool for simplicity.
    std::vector<bool> isPrimeSmall(limit + 1, true);
    isPrimeSmall[0] = isPrimeSmall[1] = false;
    for (unsigned long long i = 2; i * i <= limit; ++i) {
        if (isPrimeSmall[i]) {
            for (unsigned long long j = i * i; j <= limit; j += i) {
                isPrimeSmall[j] = false;
            }
        }
    }

    // Collect the odd primes up to limit (we already know 2 is prime).
    std::vector<unsigned long long> smallPrimes;
    for (unsigned long long i = 3; i <= limit; i += 2) {
        if (isPrimeSmall[i]) {
            smallPrimes.push_back(i);
        }
    }

    // Count 2 as a prime.
    unsigned long long count = 1; // for prime 2

    // Step 2: segment sieve from limit+1 to n.
    // We skip even numbers, so the "window" covers only odd numbers.
    // The first odd number after limit is: limit+1 if limit even, else limit+2.
    unsigned long long low = (limit % 2 == 0) ? limit + 1 : limit + 2;
    // Ensure low is odd (if limit even, limit+1 is odd; if limit odd, limit+2 is odd).
    if (low % 2 == 0) low++; // safety, though above ensures it

    // Choose a window size: use sqrt(n) or a constant like 1<<16.
    const unsigned long long WINDOW_SIZE = 1ULL << 16; // 65536
    // Alternatively, could use limit, but a fixed size is fine.

    while (low <= n) {
        unsigned long long high = std::min(low + WINDOW_SIZE - 1, n);
        // Ensure high is odd
        if (high % 2 == 0) high--;

        // Boolean array for odd numbers in [low, high].
        // Index i corresponds to number low + 2*i.
        std::vector<bool> isComposite((high - low) / 2 + 1, false);

        // For each small prime p, strike out multiples in this window.
        for (unsigned long long p : smallPrimes) {
            // Find the first multiple of p that is >= low and odd.
            // Since p is odd, multiples alternate parity? Actually p is odd, so p*odd = odd, p*even = even.
            // We need the smallest odd multiple of p that is >= low.
            unsigned long long start;
            if (p * p > high) break; // no need to go further
            // Compute first multiple >= low
            unsigned long long firstMultiple = (low + p - 1) / p * p;
            // If firstMultiple is even, add p to make it odd (since p is odd).
            if (firstMultiple % 2 == 0) firstMultiple += p; // now odd
            // Strike out all odd multiples: start from firstMultiple, step by 2*p.
            for (unsigned long long multiple = firstMultiple; multiple <= high; multiple += 2 * p) {
                // Index in isComposite: (multiple - low) / 2
                isComposite[(multiple - low) / 2] = true;
            }
        }

        // Count primes in this window.
        for (unsigned long long i = low; i <= high; i += 2) {
            if (!isComposite[(i - low) / 2]) {
                ++count;
            }
        }

        // Move to next window.
        low = high + 2; // next odd number
    }

    return count;
}

#include <cassert>

// Forward declaration of the function under test (or include the header).
unsigned long long countPrimesUpTo(unsigned long long n);

int main() {
    // Basic edge cases
    assert(countPrimesUpTo(0) == 0);
    assert(countPrimesUpTo(1) == 0);
    assert(countPrimesUpTo(2) == 1);
    assert(countPrimesUpTo(3) == 2);

    // Small known values
    assert(countPrimesUpTo(10) == 4);   // 2,3,5,7
    assert(countPrimesUpTo(20) == 8);   // 2,3,5,7,11,13,17,19
    assert(countPrimesUpTo(100) == 25);
    assert(countPrimesUpTo(1000) == 168);
    assert(countPrimesUpTo(10000) == 1229);

    // Larger value to test segmentation
    assert(countPrimesUpTo(100000) == 9592);
    assert(countPrimesUpTo(1000000) == 78498);

    // Symmetry check with a square number
    assert(countPrimesUpTo(97) == 25); // 97 is prime, 100 has 25 primes
    assert(countPrimesUpTo(98) == 25);

    // Very small n after the limit
    assert(countPrimesUpTo(4) == 2); // 2,3
    assert(countPrimesUpTo(5) == 3); // 2,3,5
    assert(countPrimesUpTo(6) == 3);

    return 0;
}
