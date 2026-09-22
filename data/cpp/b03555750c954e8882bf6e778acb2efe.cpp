// Write a C++ function named `sumEveryThirdBelow100` that takes no arguments and returns an integer. The function must calculate the sum of every third integer starting from 2, then 5, 8, 11, and so on, but only for values strictly less than 100. The loop must use a `while` loop (not a `do-while` or `for` loop). The function should not print anything; it should just return the final sum. Ensure the function handles the starting value correctly and respects the strict upper bound (numbers less than 100, so 99 is included if reachable, but 100 is not). The expected sum is 2+5+8+...+98 (since 98 is the last number in the sequence below 100).
#include <cassert>

// Function declaration (the solution function is provided above; this is for testing)
int sumEveryThirdBelow100();

int main() {
    // The sequence: 2,5,8,...,98. 33 terms. Sum = 1650.
    assert(sumEveryThirdBelow100() == 1650);
    
    // Since the function has no parameters and no state, testing it multiple times should yield the same result.
    assert(sumEveryThirdBelow100() == 1650);
    
    // We can also manually compute the sum for verification.
    int manualSum = 0;
    for (int i = 2; i < 100; i += 3) {
        manualSum += i;
    }
    assert(sumEveryThirdBelow100() == manualSum);
    
    // Check that the result is positive and non-zero.
    assert(sumEveryThirdBelow100() > 0);
    
    // Check that the result is less than the sum of all integers from 2 to 98 (since we skip many).
    int sumAll = 0;
    for (int i = 2; i < 100; ++i) sumAll += i;
    assert(sumEveryThirdBelow100() < sumAll);
    
    // Additional trivial check: The result is an integer type.
    static_assert(std::is_same<decltype(sumEveryThirdBelow100()), int>::value, "Return type must be int");
    
    // Result must be exactly 1650, which is 33 * 50 (average of first and last).
    assert(sumEveryThirdBelow100() == 33 * (2 + 98) / 2);
    
    return 0;
}
#include <cstdint>

// Calculates the sum of every third integer starting from 2, while the current value is strictly less than 100.
int sumEveryThirdBelow100() {
    int sum = 0;
    int count = 2;
    while (count < 100) {
        sum += count;
        count += 3;
    }
    return sum;
}
// The sequence is 2, 5, 8, ..., where each term increases by 3. The condition is strict: `count < 100`. The loop should start with `count = 2`. In each iteration, add the current `count` to `sum`, then increment `count` by 3. The loop continues while `count < 100`. The numbers are 2, 5, 8, ..., 98 (since 98+3=101 which is not <100). The number of terms is ((98 - 2)/3) + 1 = 33 terms. The sum of an arithmetic series is n/2 * (first + last) = 33/2 * (2+98) = 16.5 * 100 = 1650. This is a simple O(n) loop with O(1) space. Edge case: if the starting value were ≥100, the sum would be 0, but that is not the case here; still, the loop handles it gracefully. No special edge cases other than the strict `<` comparison.
