Write a C++ function named `countOddsInRange` that takes two integer parameters `low` and `high` (inclusive) and returns the number of odd integers between them. The function must handle all integer inputs, including negative numbers, zero, and large ranges, and must not use loops or recursion (i.e., derive the answer using direct mathematical calculation). The function should be declared with appropriate `const` correctness for parameters where possible and return an `int`. The function does not need to handle invalid input (e.g., `low > high` is not expected, but if it occurs, return 0).
// The problem is to count odd numbers in an inclusive range `[low, high]`. A direct loop over the range would be O(n) and might be too slow for very large ranges (e.g., `low = -2,000,000,000`, `high = 2,000,000,000`). Instead, we can use a formula based on the observation that odd numbers occur every second integer. The count of odd numbers from 1 to `x` (inclusive) is `(x + 1) / 2` for positive `x`. However, for negative numbers, we need a different approach because C++ integer division truncates toward zero, not floor. A robust approach: define a helper function `countOddsUpTo(n)` that counts odd numbers in `[0, n]` for `n >= 0`, and for negative `n`, the count is `countOddsUpTo(-n - 1)`? That is messy. Simpler: use the formula `(high - low + 1)` total numbers, then subtract even count. Even numbers in range are such that their absolute value parity matters. A cleaner mathematical formula: The number of odd integers between `low` and `high` inclusive is `(high + 1) / 2 - low / 2` when using floor division, but C++ division truncates toward zero. To use floor division, we can adjust for negatives. An alternative: count odds in `[0, high]` minus odds in `[0, low-1]`. Define `oddCountUpTo(n)`:
// - If `n < 0`, then `oddCountUpTo(n) = n / 2` with truncation? Let's test: For `n = -1`, odd numbers up to -1 in the non-positive range from -1 to 0? Actually we want count of odd numbers in `[0, n]` when n negative? Better: count odd numbers in the integer range from 1 to n (including negatives). We can use a known formula: For any integer `n`, the count of odd integers from 1 to n (inclusive) is `(n + 1) / 2` if we use floor division. In C++, floor division can be emulated by `n >= 0 ? (n+1)/2 : - ( (-n)/2 )`? Let's derive properly.
//
// Observe that odd numbers are of form `2k+1`. For a positive `n`, count odds `<= n` is `(n+1)/2` (integer division). For negative `n`, the odds between `n` and `-1` inclusive: e.g., `n = -5` => odds are -5, -3, -1 => 3 odds. Count = `(-n)/2`? For -5, (-n)/2 = 2? No, that's wrong. Actually for negative `n`, the odds from `n` to -1 inclusive: if `n` is odd, then count = `(-n+1)/2`? For -5: (-(-5)+1)/2=6/2=3 correct. For -4: odds are -3,-1 => 2, formula (-n)/2 = 4/2=2 correct. So for negative `n`, count of odd numbers in `[n, -1]` is `(-n)/2` if n even? Let's create a clean helper: `countOddsUpTo(n)` returns count of odd numbers in `[1, n]` for n>0, and for n<0 returns negative count? Actually we want count of odds in `[0, n]`. Let's define `countOddsInclusive(n)` as count of odd numbers in `[0, n]` for n>=0, and for n<0, count of odd numbers in `[n, -1]`? That is asymmetric.
//
// Simpler: Use the fact that the number of integers in `[low, high]` is `total = high - low + 1`. The number of even numbers in that range can be computed similarly. Alternatively, we can use the formula: `((high + 1) / 2) - (low / 2)` when using floor division. To get floor division in C++ for negative numbers, we can write a helper `floorDiv2(int x)` that returns `x/2` for x>=0, and for x<0 returns `- ((-x + 1) / 2)` if x is odd? Actually floor division by 2: for any integer x, floor(x/2) = (x >= 0) ? x/2 : - (( -x + 1 ) / 2). Let's verify: x=-5 => floor(-2.5) = -3, formula: - ((-(-5)+1)/2) = - ((5+1)/2) = -3 correct. x=-4 => floor(-2) = -2, formula: - ((4+1)/2) = -2 correct. So we can implement that. Then count odds = floorDiv2(high) - floorDiv2(low-1)? Because odds are numbers where integer division by 2 truncates? Actually the count of odd numbers <= n is floor((n+1)/2) = floor(n/2) + 1 if n even? Let's derive: For n >= 0, count of odds in [0,n] is (n+1)//2. Using floor division: (n+1)//2 = floor((n+1)/2). For n<0, count of odds in [0,n] is 0 because odds are positive? But we need inclusive range possibly negative. The standard trick: countOdds(low, high) = countOddsUpTo(high) - countOddsUpTo(low-1) where countOddsUpTo(n) returns the number of odd integers in [1, n] for n>0, and for n<0 returns the negative of number of odds in [n, -1]? That gets messy.
//
// Better: Use a formula that works for all integers: The number of odd integers in [low, high] inclusive is `((high + 1) / 2) - (low / 2)` assuming integer division truncates toward zero? Let's test with C++ truncation: 
// - Example low=1, high=5: high+1=6, 6/2=3; low/2=0; result 3. Correct (1,3,5).
// - low=2, high=6: high+1=7, 7/2=3 (truncate); low/2=1; result 2. Correct (3,5).
// - low=-3, high=3: high+1=4, 4/2=2; low/2 = -3/2 = -1 (C++ truncates toward zero, so -3/2 = -1); result 3. Actual odds: -3,-1,1,3 = 4. Incorrect. So truncation fails for negatives.
//
// Therefore we need floor division. So implement a helper `floorDiv2(int x)` that returns floor(x/2). Then the number of odds in [low, high] is `floorDiv2(high+1) - floorDiv2(low)`? Let's test: 
// - low=1, high=5: floorDiv2(6)=3, floorDiv2(1)=0 => 3 correct.
// - low=2, high=6: floorDiv2(7)=3, floorDiv2(2)=1 => 2 correct.
// - low=-3, high=3: floorDiv2(4)=2, floorDiv2(-3)= floor(-1.5) = -2 => 2 - (-2)=4 correct.
// - low=-5, high=-1: floorDiv2(0)=0, floorDiv2(-5)= floor(-2.5)=-3 => 0 - (-3)=3, odds: -5,-3,-1 => 3 correct.
// - low=0, high=0: floorDiv2(1)=0, floorDiv2(0)=0 => 0 correct.
// - low=1, high=1: floorDiv2(2)=1, floorDiv2(1)=0 => 1 correct.
//
// So the formula is: `count = floorDiv2(high + 1) - floorDiv2(low)`. Careful with overflow: `high + 1` could overflow for INT_MAX. Since `high` is int, `high+1` could overflow. To avoid, we can use long long for intermediate computation: `long long h = high; long long l = low;` then use `floorDiv2LL` for long long. Since the function returns int, the result will fit in int because range is at most 2^32? Actually range can be up to 2,000,000,001 (if low = -2e9, high=2e9) which fits in int (max ~2.1e9). So safe.
//
// Time complexity O(1), space O(1). Edge cases: negative numbers, zero, single-element range, overflow.
#include <cstdint>

// Helper: floor division by 2 for any 64-bit integer.
// Returns the greatest integer less than or equal to x / 2.
long long floorDiv2(long long x) {
    if (x >= 0) {
        return x / 2;
    }
    // For negative x, C++ division truncates toward zero.
    // Use: floor(x/2) = -ceil(-x/2) = -((-x + 1) / 2) for integer division.
    return -((-x + 1) / 2);
}

// Count the number of odd integers in the inclusive range [low, high].
// The range is assumed valid (low <= high); if not, returns 0.
int countOddsInRange(int low, int high) {
    if (low > high) {
        return 0;
    }
    // Use 64-bit to avoid overflow when adding 1 to high.
    long long l = static_cast<long long>(low);
    long long h = static_cast<long long>(high);
    // Number of odds = floor((high+1)/2) - floor(low/2)
    long long oddsUpToHigh = floorDiv2(h + 1);
    long long oddsBeforeLow = floorDiv2(l);
    return static_cast<int>(oddsUpToHigh - oddsBeforeLow);
}
#include <cassert>

int main() {
    // Basic positive range
    assert(countOddsInRange(1, 5) == 3);
    // Even boundaries
    assert(countOddsInRange(2, 6) == 2);
    // Single odd
    assert(countOddsInRange(7, 7) == 1);
    // Single even
    assert(countOddsInRange(8, 8) == 0);
    // Range with zero
    assert(countOddsInRange(0, 3) == 2);  // 1 and 3
    // Negative range
    assert(countOddsInRange(-5, -1) == 3); // -5, -3, -1
    // Mixed negative/positive
    assert(countOddsInRange(-3, 3) == 4);  // -3, -1, 1, 3
    // Large range
    assert(countOddsInRange(-2000000000, 2000000000) == 2000000000);
    // Invalid input (low > high) returns 0
    assert(countOddsInRange(5, 1) == 0);
    return 0;
}
