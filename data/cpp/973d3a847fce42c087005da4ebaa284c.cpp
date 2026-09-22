Write a C++ function named `primeFactorization` that takes a positive integer `n` and returns a `std::string` containing the prime factorization of `n` in the format "p1^e1 * p2^e2 * ...", where each prime factor `pi` is followed by its exponent `ei` (e.g., for `n = 12`, output `"2^2 * 3^1"`). If `n` is 1, return `"1"`. If `n` is itself a prime number, return it as `"n^1"`. The output should have factors in strictly increasing order of prime values, with no leading or trailing spaces. The function must be const-correct and handle all positive integers up to 1,000,000 efficiently (i.e., no need to check primality separately—just trial divide by all integers starting from 2).

The algorithm performs trial division from `i = 2` upward while `i * i <= n`. For each `i`, count how many times it divides `n` completely, incrementing the exponent and dividing `n` by `i` repeatedly. If the exponent is greater than zero, append `"i^exp"` to the result, with a `" * "` separator between different factors. After the loop, if `n > 1`, it means the remaining value is a prime greater than the last checked `i`, so append `"n^1"`. This approach naturally produces increasing order and handles prime `n` (the loop will finish with `n` unchanged, then the trailing step adds it). Edge cases: `n = 1` returns `"1"`; `n = 2` returns `"2^1"` (loop checks `i=2`, divides once, then `n` becomes 1 and loop stops). Time complexity is \(O(\sqrt{n})\) in the worst case (when `n` is prime), and auxiliary space is \(O(\log n)\) for the output string.

#include <string>

// Returns the prime factorization of positive integer n as a string "p1^e1 * p2^e2 * ...".
// For n = 1, returns "1". Factors are in increasing order.
std::string primeFactorization(int n) {
    if (n <= 1) return "1";

    std::string result;
    bool first = true;

    for (int i = 2; i * i <= n; ++i) {
        int exponent = 0;
        while (n % i == 0) {
            n /= i;
            ++exponent;
        }
        if (exponent > 0) {
            if (!first) result += " * ";
            first = false;
            result += std::to_string(i) + "^" + std::to_string(exponent);
        }
    }

    // If n > 1, the remaining n is a prime factor with exponent 1.
    if (n > 1) {
        if (!first) result += " * ";
        result += std::to_string(n) + "^1";
    }

    return result;
}

#include <cassert>
#include <string>

// Declaration of the function to test (already defined above, but included for clarity)
std::string primeFactorization(int n);

int main() {
    assert(primeFactorization(1) == "1");
    assert(primeFactorization(2) == "2^1");
    assert(primeFactorization(12) == "2^2 * 3^1");
    assert(primeFactorization(100) == "2^2 * 5^2");
    assert(primeFactorization(97) == "97^1");
    assert(primeFactorization(360) == "2^3 * 3^2 * 5^1");
    assert(primeFactorization(1024) == "2^10");
    assert(primeFactorization(999983) == "999983^1");   // a large prime
    assert(primeFactorization(1000000) == "2^6 * 5^6");
    assert(primeFactorization(64) == "2^6");
    return 0;
}
