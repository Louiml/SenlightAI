/*
Write a C++ function named `isPerfectNumber` that takes an integer parameter `num` and returns a `bool` indicating whether `num` is a perfect number. A perfect number is a positive integer that is equal to the sum of its proper positive divisors (excluding the number itself). The function should return `false` for any non-positive input (including zero and negative numbers). For example, `6` is perfect because `1 + 2 + 3 = 6`, while `12` is not perfect because `1 + 2 + 3 + 4 + 6 = 16 ≠ 12`. The function must be standalone (no `main`), and the caller will handle input/output.
*/

#include <cstddef> // for size_t if needed, but not required here

// Returns true if num is a perfect positive integer, false otherwise.
// A perfect number equals the sum of its proper divisors (excluding itself).
bool isPerfectNumber(int num) {
    if (num <= 0) {
        return false;
    }

    int sum = 0;
    for (int divisor = 1; divisor < num; ++divisor) {
        if (num % divisor == 0) {
            sum += divisor;
        }
    }
    return sum == num;
}

#include <cassert>

// Function declaration (the solution is assumed to be included above)
bool isPerfectNumber(int num);

int main() {
    // Known perfect numbers
    assert(isPerfectNumber(6) == true);
    assert(isPerfectNumber(28) == true);
    assert(isPerfectNumber(496) == true);
    assert(isPerfectNumber(8128) == true);

    // Non-perfect numbers
    assert(isPerfectNumber(1) == false);
    assert(isPerfectNumber(12) == false);
    assert(isPerfectNumber(100) == false);

    // Edge cases: non-positive input
    assert(isPerfectNumber(0) == false);
    assert(isPerfectNumber(-6) == false);
    assert(isPerfectNumber(-1) == false);

    return 0;
}

// The main algorithm iterates from 1 to `num - 1` and sums all divisors of `num`. After the loop, compare the accumulated sum to `num` and return `true` if they are equal, otherwise `false`. Critical edge cases: (1) `num <= 0` must immediately return `false` because perfect numbers are defined only for positive integers (and 0 has infinite divisors conceptually); (2) `num == 1` is not perfect because its only proper divisor is 1, but 1 ≠ 1 (the sum of divisors less than 1 is 0, so sum=0 ≠ 1); this is handled naturally because the loop starts at 1 and the condition `i <= num - 1` is false for `num == 1`, resulting in `sum=0`. Time complexity is O(n) for a given number `n` (the loop runs `n-1` times), and space complexity is O(1) since only a few integer variables are used. For very large `n`, the algorithm is inefficient, but it matches the simple specification; optimization (e.g., looping to sqrt(n)) is not required but could be mentioned as a note.
