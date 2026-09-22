/*
Write a C++ function `primeFactorizationToString` that takes a positive integer `n` and returns a `std::string` containing its prime factorization in the format `"n = p1^k1 * p2^k2 * ... * pm^km"`, where each prime factor `p` is listed in increasing order with its exponent `k` (use `p^1` even if the exponent is 1). If `n` is 1, return `"1 = 1"`. The function must handle any positive `int` input (including edge cases like 2, large primes, and numbers with repeated prime factors), and the output must not have trailing spaces or extra multiplication signs. For example, for input 60, the output should be `"60 = 2^2 * 3^1 * 5^1"`. The function should not print anything; it must build and return the string.
*/

#include <string>

// Return the prime factorization of n as a string in the form "n = p1^k1 * ...".
std::string primeFactorizationToString(int n) {
    if (n == 1) {
        return "1 = 1";
    }
    
    std::string result;
    int original = n;
    
    for (int j = 2; j <= n; ++j) {
        int count = 0;
        while (n % j == 0) {
            ++count;
            n /= j;
        }
        if (count > 0) {
            result += std::to_string(j) + "^" + std::to_string(count);
            if (n > 1) {
                result += " * ";
            }
        }
    }
    
    return std::to_string(original) + " = " + result;
}

#include <cassert>
#include <string>

// Function declaration from solution
std::string primeFactorizationToString(int n);

int main() {
    assert(primeFactorizationToString(1) == "1 = 1");
    assert(primeFactorizationToString(2) == "2 = 2^1");
    assert(primeFactorizationToString(60) == "60 = 2^2 * 3^1 * 5^1");
    assert(primeFactorizationToString(100) == "100 = 2^2 * 5^2");
    assert(primeFactorizationToString(97) == "97 = 97^1");
    assert(primeFactorizationToString(12) == "12 = 2^2 * 3^1");
    assert(primeFactorizationToString(360) == "360 = 2^3 * 3^2 * 5^1");
    assert(primeFactorizationToString(1024) == "1024 = 2^10");
    assert(primeFactorizationToString(7) == "7 = 7^1");
    assert(primeFactorizationToString(210) == "210 = 2^1 * 3^1 * 5^1 * 7^1");
    return 0;
}

// The solution uses trial division by all integers from 2 up to the current value of `n`. For each divisor `j`, count how many times it divides `n` (using a while loop that repeatedly divides `n` by `j`). If the count is positive, append `j^count` to the result. Between factors, append `" * "` but only if more factors will follow—this is handled by checking if the remaining `n` is greater than 1 before adding the separator. The loop increments `j` regardless, so all divisors are tested, but only prime divisors will ever divide `n` because any composite divisor would have been completely divided out earlier by its prime factors. Edge cases: input 1 is handled with a special return at the start. For prime numbers, the only factor is the number itself with exponent 1. For numbers like 2, the loop runs once. Time complexity is O(sqrt(n)) in the worst case if we optimized by stopping at sqrt, but here we iterate up to the original `n` because `n` decreases during division; in practice, the loop runs at most the original value of `n`, but for large primes it is O(n). However, since `n` is an `int` (max ~2^31), this is acceptable for typical test cases. Worst-case time is O(n) and space is O(k) where k is the number of distinct prime factors (for the output string). An optimized version would stop at sqrt(n), but this straightforward method matches the snippet and is clear.
