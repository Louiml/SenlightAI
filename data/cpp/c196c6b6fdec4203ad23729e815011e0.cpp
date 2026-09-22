// Write a C++ function named `classifyPrime` that takes a single `long long` integer `x` and returns a `std::string` containing either `"Prime"` or `"Not Prime"` based on whether `x` is a prime number. The function must handle the special case where `x` is 1 (which is not prime) and correctly identify all other positive integers. The function should be efficient for numbers up to \(2 \times 10^9\), using trial division only up to the square root of `x`. Do not include any I/O inside the function; the caller (test code) will handle input and output.
The core algorithm is trial division: to check if `x` is prime, we test divisibility by every integer `j` from 2 up to \(\sqrt{x}\). This works because any composite number \(n\) must have a factor less than or equal to \(\sqrt{n}\); if no such factor exists, the number is prime. Important edge cases: `x == 1` is not prime; `x == 2` is prime (the loop starts at 2 and `j * j <= 2` is false, so it remains prime); `x` even and greater than 2 is quickly rejected by the first modulo check. We can slightly optimize by handling the even case separately and then iterating only odd divisors from 3 upward, but the straightforward loop is sufficient for the given constraints. The loop condition `j * j <= x` uses integer multiplication to avoid floating-point issues (e.g., using `sqrt` with double could introduce precision errors). Time complexity is \(O(\sqrt{x})\) per call, and space complexity is \(O(1)\).
#include <string>

// Determines if a given positive integer x is prime.
// Returns "Prime" if x is prime, otherwise "Not Prime".
std::string classifyPrime(long long x) {
    if (x <= 1) {
        return "Not Prime";
    }
    if (x == 2) {
        return "Prime";
    }
    if (x % 2 == 0) {
        return "Not Prime";
    }
    // Check odd divisors from 3 up to sqrt(x)
    for (long long j = 3; j * j <= x; j += 2) {
        if (x % j == 0) {
            return "Not Prime";
        }
    }
    return "Prime";
}
#include <cassert>
#include <string>

// (The function declaration is assumed to be included from above.)
std::string classifyPrime(long long x);

int main() {
    assert(classifyPrime(1) == "Not Prime");
    assert(classifyPrime(2) == "Prime");
    assert(classifyPrime(3) == "Prime");
    assert(classifyPrime(4) == "Not Prime");
    assert(classifyPrime(97) == "Prime");
    assert(classifyPrime(100) == "Not Prime");
    assert(classifyPrime(999999937) == "Prime"); // large prime
    assert(classifyPrime(1000000000) == "Not Prime"); // large even
    assert(classifyPrime(17) == "Prime");
    assert(classifyPrime(0) == "Not Prime");
    return 0;
}
