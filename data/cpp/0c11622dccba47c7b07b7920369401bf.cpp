/*
Write a C++ function that, given a positive integer `n`, prints all prime factors of `n` in ascending order, with each prime factor repeated according to its multiplicity. For example, for `n = 12`, the output should be `2 2 3`. The function must handle edge cases such as `n = 1` (no output), `n = 2` (output `2`), and `n` being a prime number itself (output just that number). The solution must be self-contained with a clearly named function (e.g., `printPrimeFactors`) and should not rely on global variables. Do not include a `main` function in the solution code; only the function implementation.
*/

#include <iostream>

// Prints all prime factors of n in ascending order, each repeated according to its multiplicity.
// For n = 1, prints nothing. Assumes n is positive.
void printPrimeFactors(int n) {
    // Handle factor 2 separately to reduce iterations.
    while (n % 2 == 0) {
        std::cout << 2 << " ";
        n /= 2;
    }
    // Now n is odd. Check odd divisors up to sqrt(n).
    for (int i = 3; i * i <= n; i += 2) {
        while (n % i == 0) {
            std::cout << i << " ";
            n /= i;
        }
    }
    // If n is still > 1, it is a prime factor (possibly the original n if n was prime).
    if (n > 1) {
        std::cout << n << " ";
    }
}

#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output of printPrimeFactors.
std::string captureOutput(int n) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    printPrimeFactors(n);
    std::cout.rdbuf(oldCout);
    return oss.str();
}

int main() {
    assert(captureOutput(1) == "");
    assert(captureOutput(2) == "2 ");
    assert(captureOutput(3) == "3 ");
    assert(captureOutput(4) == "2 2 ");
    assert(captureOutput(12) == "2 2 3 ");
    assert(captureOutput(13) == "13 ");
    assert(captureOutput(100) == "2 2 5 5 ");
    assert(captureOutput(97) == "97 ");
    assert(captureOutput(360) == "2 2 2 3 3 5 ");
    assert(captureOutput(1024) == "2 2 2 2 2 2 2 2 2 2 ");
}

// The problem requires factorization of an integer into its prime factors with correct multiplicities. The provided snippet has bugs: it checks `isPrime(n)` inside the loop (not the candidate factor), and its while loop logic is incorrect (`x = x * i` instead of dividing). A correct approach: iterate `i` from 2 upward while `i*i <= n`. For each `i`, while `n % i == 0`, print `i` and divide `n` by `i`. After the loop, if the remaining `n` is greater than 1, it is a prime factor greater than the square root of the original `n` (or exactly the last prime factor). This avoids any separate primality test. Edge cases: `n = 1` prints nothing (loop condition `i*i <= n` is false immediately), `n` being prime prints the number itself after the loop. Time complexity: \(O(\sqrt{n})\) iterations in the worst case (when `n` is prime, the loop runs up to \(\sqrt{n}\)), and each division is constant time. Space complexity: \(O(1)\) auxiliary space, excluding output stream overhead.
