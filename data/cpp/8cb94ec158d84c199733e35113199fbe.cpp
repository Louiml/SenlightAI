// Write a C++ function named `isPrime` that takes a single integer `n` and returns `true` if `n` is a prime number, and `false` otherwise. The function must handle non-positive numbers (including zero and negative integers) by returning `false`, and must correctly identify that `1` is not prime. The function should be efficient enough to handle inputs up to 10^9 without iterating through all numbers up to `n`—use the trial division method only up to the square root of `n`. The function should be `const`-correct (meaning it does not modify its input, so the parameter should be passed by value, and the function itself should be marked `const` if it were a member function, but since it is free, just ensure no side effects). The implementation must be self-contained with necessary headers (`<cmath>` for `sqrt`), and must not include a `main` function in the solution code.

The obvious approach from the snippet is trial division: check divisibility of `n` by every integer from 2 to `n-1`. However, that is inefficient for large inputs. The mathematical improvement is to only check divisors up to `√n`, because if `n` has a divisor greater than `√n`, then it must also have a corresponding divisor less than `√n`. So we only need to test up to `√n`. Edge cases: `n <= 1` are not prime by definition. For `n = 2` and `n = 3`, the loop from 2 to `√n` will be skipped or trivially handled correctly. For even numbers greater than 2, we can immediately return `false` after checking divisibility by 2, then only test odd divisors from 3 upwards, stepping by 2, to halve the iterations. The time complexity is O(√n) in the worst case (when `n` is prime) and O(1) auxiliary space. Since `n` can be up to 10^9, `√n` ≈ 31623, which is very fast. Use `int` or `long long` to avoid overflow when computing `i * i` (though `i` is already bounded by `√n`, use `i <= n / i` to avoid overflow entirely). The function will be `bool isPrime(int n)` and will be `const`-correct by not modifying any external state.

#include <cmath>

// Returns true if n is a prime number, false otherwise.
// Handles non-positive numbers and 1 by returning false.
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    // Check odd divisors from 3 up to sqrt(n)
    for (int i = 3; i <= n / i; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

#include <cassert>

// Forward declaration of the solution function (not needed if same file, but included for clarity)
bool isPrime(int n);

int main() {
    // Basic positive primes
    assert(isPrime(2) == true);
    assert(isPrime(3) == true);
    assert(isPrime(5) == true);
    assert(isPrime(7) == true);
    assert(isPrime(11) == true);
    assert(isPrime(13) == true);
    assert(isPrime(17) == true);
    assert(isPrime(19) == true);

    // Non-prime positive numbers
    assert(isPrime(1) == false);
    assert(isPrime(4) == false);
    assert(isPrime(6) == false);
    assert(isPrime(8) == false);
    assert(isPrime(9) == false);
    assert(isPrime(10) == false);
    assert(isPrime(15) == false);
    assert(isPrime(21) == false);

    // Non-positive numbers
    assert(isPrime(0) == false);
    assert(isPrime(-1) == false);
    assert(isPrime(-7) == false);

    // A larger prime (up to 10^9)
    assert(isPrime(2147483647) == true); // 2^31 - 1, known prime
    // A larger composite
    assert(isPrime(1000000000) == false);
    assert(isPrime(999999937) == true); // known prime near 10^9

    return 0;
}
