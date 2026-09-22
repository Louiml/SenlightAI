// Write a C++ function named `sumOfProperDivisors` that takes a single positive integer `n` and returns the sum of all positive divisors of `n` that are strictly less than `n`. For example, for `n = 12`, the proper divisors are 1, 2, 3, 4, and 6, so the function should return 16. The function must handle `n = 1` correctly (returning 0, since 1 has no proper divisors), and must work for any positive integer up to at least 10^6. The function should be efficient enough to be called repeatedly in a loop without excessive overhead, and must not modify any input or global state.

The main algorithm iterates through all integers from 1 up to `n-1` and tests whether each divides `n` evenly using the modulus operator. If `i` divides `n` (i.e., `n % i == 0`), then `i` is a proper divisor and is added to an accumulating sum. Since we start from 1 and stop before `n`, we automatically exclude `n` itself. Edge cases include `n = 1`, where the loop runs from 1 to 0, so no iterations occur and the sum remains 0, which is correct. For time complexity, the algorithm runs in O(n) time because it performs a constant amount of work per integer from 1 to `n-1`. The space complexity is O(1) because only a single integer accumulator is used, plus the loop variable. This approach is straightforward and correct, though not the most optimized possible (a divisor-pair approach could reduce to O(√n)), but it suffices for the constraints given and is clear for a beginner-level task.

#include <cstdint>

// Returns the sum of all proper divisors of n (divisors strictly less than n).
// For n = 1, returns 0 because 1 has no proper divisors.
int64_t sumOfProperDivisors(int n) {
    int64_t sum = 0;  // Use int64_t to avoid overflow for larger n.
    for (int i = 1; i < n; ++i) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum;
}

#include <cassert>

int main() {
    // n = 1: no proper divisors
    assert(sumOfProperDivisors(1) == 0);
    // n = 2: proper divisor is 1
    assert(sumOfProperDivisors(2) == 1);
    // n = 6: proper divisors 1,2,3 -> sum 6
    assert(sumOfProperDivisors(6) == 6);
    // n = 12: proper divisors 1,2,3,4,6 -> sum 16
    assert(sumOfProperDivisors(12) == 16);
    // n = 28: proper divisors 1,2,4,7,14 -> sum 28 (perfect number)
    assert(sumOfProperDivisors(28) == 28);
    // n = 100: proper divisors 1,2,4,5,10,20,25,50 -> sum 117
    assert(sumOfProperDivisors(100) == 117);
    // n = 1000000: proper divisors sum is large; just check it's > 0
    assert(sumOfProperDivisors(1000000) > 0);
    return 0;
}
