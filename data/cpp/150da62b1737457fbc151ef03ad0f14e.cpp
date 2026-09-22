/*
Given positive integers `D` (number of days) and `K` (total number of rabbits on day `D`), where on day 1 there are `a` rabbits and on day 2 there are `b` rabbits (with `a` and `b` unknown positive integers, and `a <= b`), and each subsequent day the rabbit count is the sum of the previous two days (Fibonacci-like recurrence), write a C++ function `std::pair<int,int> findInitialPair(int D, int K)` that returns the pair `(a, b)` satisfying these conditions. The input guarantees that a valid solution exists and that `a` and `b` are within the range 1 to 1000. Your function must handle any `D` from 2 to 35, and any `K` up to say 1,000,000 (but the return values are always ≤ 1000). Return the pair with the smallest possible `a`; if multiple `(a,b)` pairs are valid, pick the one with the smallest `a`, and for that `a` the corresponding `b` is unique. If no valid pair exists (though problem guarantees it), return `{0,0}`.
*/
#include <utility>
#include <vector>

// Given days D and total rabbits K on day D, return the pair (a,b) where
// day1=a, day2=b, and recurrence count_i = count_{i-1}+count_{i-2}.
// Assumes a,b are positive integers <=1000 and a<=b, and solution exists.
std::pair<int,int> findInitialPair(int D, int K) {
    const int MAX_D = 35;
    std::vector<long long> A(MAX_D+1, 0), B(MAX_D+1, 0);
    if (D >= 1) A[1] = 1;
    if (D >= 2) B[2] = 1;
    for (int i = 3; i <= D; ++i) {
        A[i] = A[i-1] + A[i-2];
        B[i] = B[i-1] + B[i-2];
    }
    // Special case: B[D]==0 (only occurs when D==1 or D==0, but D>=2 from spec)
    if (B[D] == 0) {
        if (A[D] != 0 && K % A[D] == 0) {
            int a = static_cast<int>(K / A[D]);
            if (a >= 1 && a <= 1000) return {a, 1000}; // choose any b>=a; simplest is max
        }
        return {0,0};
    }
    for (int a = 1; a <= 1000; ++a) {
        long long remain = static_cast<long long>(K) - A[D] * a;
        if (remain < 0) continue;
        if (remain % B[D] != 0) continue;
        long long b_ll = remain / B[D];
        if (b_ll < 1 || b_ll > 1000) continue;
        int b = static_cast<int>(b_ll);
        if (a <= b) return {a, b};
    }
    return {0,0};
}
#include <cassert>
#include <utility>

// Declaration from solution
std::pair<int,int> findInitialPair(int D, int K);

int main() {
    // Test from original snippet with D=3, K=3 -> a=1,b=2 (day1=1,day2=2,day3=3)
    assert(findInitialPair(3, 3) == std::make_pair(1, 2));
    // D=4, K=5 -> day4 = 3a+2b = 5 -> a=1,b=1 gives 3+2=5 (but a<=b holds)
    assert(findInitialPair(4, 5) == std::make_pair(1, 1));
    // D=5, K=8 -> day5 = 5a+3b = 8 -> a=1,b=1 gives 5+3=8
    assert(findInitialPair(5, 8) == std::make_pair(1, 1));
    // D=6, K=13 -> day6 = 8a+5b = 13 -> a=1,b=1 gives 8+5=13
    assert(findInitialPair(6, 13) == std::make_pair(1, 1));
    // D=2, K=5 -> day2 = b = 5, a can be 1..5, smallest a=1
    assert(findInitialPair(2, 5) == std::make_pair(1, 5));
    // D=7, K=21 -> day7 = 13a+8b = 21 -> a=1,b=1 gives 13+8=21
    assert(findInitialPair(7, 21) == std::make_pair(1, 1));
    // D=8, K=34 -> day8 = 21a+11b = 34 -> a=1,b=1 gives 21+11=32 not match, a=2? 42+11b=34 no; a=1,b=2? 21+22=43; a=1,b=1 gives 32 not 34. Actually a=1,b=2? 21+22=43; a=2,b=1? 42+11=53; a=1,b=1 is 32. Try a=1,b=3 -> 21+33=54; a=1,b=1? 21+11=32. Note original comment says day8 = 21A+11B? But Fibonacci: day8 count = day7+day6 = (13a+8b)+(8a+5b)=21a+13b, not 11b. The snippet had a typo. Correct recurrence: day8 = 21a+13b. Then for K=34, a=1,b=1 gives 21+13=34. So test that.
    assert(findInitialPair(8, 34) == std::make_pair(1, 1));
    // D=9, K=55 -> day9 = 34a+21b = 55 -> a=1,b=1 gives 34+21=55
    assert(findInitialPair(9, 55) == std::make_pair(1, 1));
    // A non-trivial case: D=4, K=7 -> 3a+2b=7, solutions: a=1,b=2 (3+4=7) → a=1<=2 valid
    assert(findInitialPair(4, 7) == std::make_pair(1, 2));
    return 0;
}
// Let `a` be day 1 count and `b` be day 2 count. For day `i` (i ≥ 3), the count is `A[i]*a + B[i]*b`, where `A[i]` and `B[i]` follow the same Fibonacci recurrence: `A[1]=1,A[2]=0`, `B[1]=0,B[2]=1`, and `A[i]=A[i-1]+A[i-2]`, `B[i]=B[i-1]+B[i-2]`. So we need to solve `A[D]*a + B[D]*b = K` for positive integers `a,b` with `a ≤ b`, and `a,b ≤ 1000`. Because coefficients grow exponentially with `D`, but `D ≤ 35` and `K` fits in 32-bit integer, we can precompute arrays `A` and `B` up to `D`. Then iterate `a` from 1 to 1000, compute `rem = K - A[D]*a`. If `rem` is non-negative and divisible by `B[D]`, then compute `b = rem / B[D]`. Check that `b ≥ 1`, `b ≤ 1000`, and `a ≤ b`. Since we iterate `a` in increasing order, the first valid pair gives the smallest `a`. Complexity: precomputing O(D), scanning `a` up to 1000 (constant) → O(D + 1000) time, O(D) space. Edge cases: `D=2` gives `A[2]=0, B[2]=1`, so equation reduces to `b = K`; if `K ≤ 1000` and `a ≤ b` (any `a` from 1..K) we pick smallest `a=1`, but that must satisfy `a≤b` so `K≥1`. For `D=1`? Not in spec but our function can handle: `A[1]=1,B[1]=0` so `a=K`, and any `b` ≥ `a` but our loop won't find because `B[1]=0` leads division by zero; but we can add a special case: if `B[D]==0` then only valid when `a=K` and `b≥K`; but we'll assume valid input. Another edge: coefficients may exceed int for D=35, but 2^33 ≈ 8.5e9 > 2^31, but K ≤ 1e6, and `a` ≤ 1000 so product `A[D]*a` might overflow int. Use `long long` for intermediate computations and store coefficients as `long long`.
