Write a C++ function named `goldbachPair` that, given an integer `n` greater than or equal to 6, returns a string representing the first Goldbach pair (two odd primes that sum to `n`) in the format `"n = p + q"`, where `p` is the smallest odd prime in the pair and `q = n - p`. If `n` is odd or less than 6, return an empty string. The function must handle only even `n` values correctly, and for even `n` there is always at least one valid pair (by Goldbach's conjecture for this range), but in the weird case no pair is found (e.g., due to an input mistake), return an empty string as well. The primes must be odd — note that 2 is a prime but is excluded from being either `p` or `q` in this task (the original snippet only tests odd numbers for primality). The function must be efficient enough to handle `n` up to 10,000. Do not include any I/O inside the function; just compute and return the string.
The main algorithm starts by verifying that `n` is even and at least 6; if not, return an empty string. Then iterate over possible `j` from 2 to `n/2` — but since we require odd primes, skip even values of `j` (start from 3 and step by 2). For each candidate `j`, check if it is prime using trial division: compute the integer square root of `j`, then test divisibility only by odd divisors from 3 up to that root (since `j` is odd, we skip testing division by 2). If `j` is prime, compute `l = n - j`. If `l` is odd (which it will be since odd + odd = even, so `l` is odd if `j` is odd), check primality of `l` similarly: compute its square root and test odd divisors from 3 upward. If `l` is prime, then we have found the pair, so construct the string `"n = j + l"` and return it. Because we iterate `j` in increasing order, the first found pair will have the smallest odd prime. Important edge cases: if `n` itself is even but less than 6 (e.g., 4), return empty; if `n` is odd, return empty; if `n` is even and >= 6, the loop will always find a pair (as per Goldbach's conjecture for numbers up to 10,000, which has been verified), but to be safe, if the loop ends without finding a pair, return an empty string. Complexity: The outer loop iterates at most `n/2` times (actually roughly `n/4` odd numbers), and for each candidate prime check, trial division up to the square root of the candidate gives O(sqrt(n)) operations. Overall worst-case time is O(n sqrt(n)). Space usage is O(1) aside from the returned string.
#include <string>
#include <cmath>

// Return a string of the form "n = p + q" for the first Goldbach pair of even n
// where p and q are odd primes (p <= q). If n is not an even number >= 6, return "".
std::string goldbachPair(int n) {
    if (n < 6 || (n % 2) != 0) {
        return "";
    }

    // Helper lambda to check if an odd number is prime.
    auto isOddPrime = [](int value) -> bool {
        if (value < 2) return false;
        if (value == 2) return true;
        if ((value % 2) == 0) return false;

        int limit = static_cast<int>(std::sqrt(static_cast<double>(value)));
        for (int d = 3; d <= limit; d += 2) {
            if ((value % d) == 0) {
                return false;
            }
        }
        return true;
    };

    // Iterate over odd candidates for j (the smaller prime).
    for (int j = 3; j <= n / 2; j += 2) {
        if (isOddPrime(j)) {
            int l = n - j;
            if (isOddPrime(l)) {
                return std::to_string(n) + " = " + std::to_string(j) + " + " + std::to_string(l);
            }
        }
    }

    return ""; // Should not be reached for valid even n >= 6.
}
#include <cassert>
#include <string>

// The solution function is declared above (or included from a header).
int main() {
    assert(goldbachPair(6) == "6 = 3 + 3");
    assert(goldbachPair(8) == "8 = 3 + 5");
    assert(goldbachPair(10) == "10 = 3 + 7");
    assert(goldbachPair(12) == "12 = 5 + 7");
    assert(goldbachPair(14) == "14 = 3 + 11");
    assert(goldbachPair(100) == "100 = 3 + 97");
    assert(goldbachPair(7) == "");
    assert(goldbachPair(4) == "");
    assert(goldbachPair(-8) == "");
    assert(goldbachPair(10000) == "10000 = 3 + 9997");
    return 0;
}
