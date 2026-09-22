Write a C++ function named `printPrimesUpTo` that takes an integer `n` (where `0 ≤ n ≤ 10^7`) and returns a `std::string` containing all prime numbers from 2 up to `n`, inclusive, separated by single spaces. If `n < 2`, return an empty string. The function must be efficient for the upper bound using the Sieve of Eratosthenes. The returned string should have no leading or trailing spaces. For example, `printPrimesUpTo(10)` should return `"2 3 5 7"`, and `printPrimesUpTo(1)` should return `""`.
// The solution uses the Sieve of Eratosthenes to mark prime numbers up to the maximum possible bound of `10^7` once, using a static boolean array (or `std::vector<bool>`) to store primality. Since the input `n` can be as large as `10^7`, precomputing up to that fixed maximum is acceptable and ensures repeated calls are fast. The sieve initializes all numbers as prime, then marks `0` and `1` as non-prime, and iterates from `i = 2` to `sqrt(10^7)`; if `i` is still marked prime, it marks all multiples `i*i` and beyond as non-prime. After preprocessing, the function loops from 2 to `n`, appends each prime to a `std::ostringstream` (or builds a string manually) with spaces, and trims the trailing space before returning. Edge cases: `n` below 2 returns an empty string; duplicate primes do not occur; for large `n`, the string may be long (about 6.7 million digits for all primes under 10^7), so memory for the string is O(number of primes) ≈ O(n/log n), which is acceptable. Time complexity: O(10^7 log log 10^7) for the sieve precomputation (done once) plus O(n) for building the output; space complexity: O(10^7) for the sieve and O(length of output) for the string.
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

// Return a string of all primes from 2 to n, separated by spaces.
std::string printPrimesUpTo(int n) {
    static const int MAX_LIMIT = 10000000;
    static std::vector<bool> isPrime(MAX_LIMIT + 1, true);
    static bool sieveInitialized = false;

    if (!sieveInitialized) {
        isPrime[0] = isPrime[1] = false;
        for (int i = 2; i * i <= MAX_LIMIT; ++i) {
            if (isPrime[i]) {
                for (int j = i * i; j <= MAX_LIMIT; j += i) {
                    isPrime[j] = false;
                }
            }
        }
        sieveInitialized = true;
    }

    if (n < 2) return "";

    std::ostringstream output;
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) {
            output << i << ' ';
        }
    }
    std::string result = output.str();
    if (!result.empty()) {
        result.pop_back(); // Remove trailing space
    }
    return result;
}
#include <cassert>
#include <string>

// Function declaration (normally would come from header, but included for test)
std::string printPrimesUpTo(int n);

int main() {
    assert(printPrimesUpTo(0) == "");
    assert(printPrimesUpTo(1) == "");
    assert(printPrimesUpTo(2) == "2");
    assert(printPrimesUpTo(3) == "2 3");
    assert(printPrimesUpTo(10) == "2 3 5 7");
    assert(printPrimesUpTo(20) == "2 3 5 7 11 13 17 19");
    assert(printPrimesUpTo(30) == "2 3 5 7 11 13 17 19 23 29");
    // Test that 50 includes 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
    assert(printPrimesUpTo(50) == "2 3 5 7 11 13 17 19 23 29 31 37 41 43 47");
    // Test a moderate value, ensuring no leading/trailing spaces and correct content
    std::string result = printPrimesUpTo(100);
    assert(result.front() == '2' && result.back() == '9');
    assert(result.find("  ") == std::string::npos);
    return 0;
}
