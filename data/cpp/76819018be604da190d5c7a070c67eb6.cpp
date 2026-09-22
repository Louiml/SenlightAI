Write a C++ function named `sumOfSquaresUpTo` that takes an integer parameter `limit` and returns the sum of the squares of all non-negative integers from 0 up to and including `limit`. For example, if `limit` is 3, the function should return 0² + 1² + 2² + 3² = 14. The function must handle negative `limit` values gracefully by returning 0, since there are no non-negative integers less than or equal to a negative number. Use a loop (not a closed-form formula) and ensure the computation does not overflow for reasonable input values (limit ≤ 10000).

#include <cassert>

// The function declaration is assumed to be available from the solution above.
long long sumOfSquaresUpTo(int limit);

int main() {
    // Basic positive cases
    assert(sumOfSquaresUpTo(0) == 0);
    assert(sumOfSquaresUpTo(1) == 1);   // 0+1
    assert(sumOfSquaresUpTo(2) == 5);   // 0+1+4
    assert(sumOfSquaresUpTo(3) == 14);  // 0+1+4+9
    assert(sumOfSquaresUpTo(10) == 385); // known result

    // Negative limit edge case
    assert(sumOfSquaresUpTo(-1) == 0);
    assert(sumOfSquaresUpTo(-100) == 0);

    // Larger value to ensure no overflow
    assert(sumOfSquaresUpTo(10000) == 333383335000LL);

    // Repeat a few to ensure consistency
    assert(sumOfSquaresUpTo(5) == 55);
    assert(sumOfSquaresUpTo(5) == 55);
}

#include <cstddef> // for std::size_t (optional, not needed)

// Returns the sum of squares of all integers from 0 to 'limit' inclusive.
// If 'limit' is negative, returns 0.
long long sumOfSquaresUpTo(int limit) {
    if (limit < 0) {
        return 0;
    }
    long long total = 0;
    for (int i = 0; i <= limit; ++i) {
        total += static_cast<long long>(i) * i; // promote to avoid overflow in multiplication
    }
    return total;
}

// The algorithm is straightforward: iterate from 0 to `limit` inclusive, accumulating the square of each integer into a `long long` sum. Start with an edge case check—if `limit` is negative, immediately return 0 because no non-negative integers satisfy the condition. Otherwise, initialize a `long long` accumulator to 0 and use a `for` loop with an index from 0 to `limit`. For each iteration, compute `i * i` (integer multiplication, which is safe for i up to 10000) and add it to the accumulator. The time complexity is O(n) where n = `limit` (or O(1) if `limit` is negative), and the auxiliary space complexity is O(1) since only a few scalar variables are used. Edge cases include `limit = 0` (returns 0) and large `limit` values where the sum grows as roughly n³/3; using a 64-bit `long long` ensures no overflow for n ≤ 10000 (max sum ≈ 3.33 × 10¹¹, well below 2⁶³).
