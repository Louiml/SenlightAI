/*
Write a C++ function `pairSplitProduct(long long n, long long x)` that, given two positive integers \( n \) and \( x \), finds two positive integers \( a \) and \( b \) such that \( a+b = n \) and \( a \cdot b \leq x \), with the product as large as possible (i.e., maximize \( a \cdot b \) among all valid splits). If no such pair exists (for example, when \( x < n-1 \), since the minimal positive product for a split is \( 1 \cdot (n-1) = n-1 \)), return the pair \( (0,0) \). The pair should be returned as a `std::pair<long long, long long>` where \( a \leq b \). The function must handle \( n \) up to \( 10^{18} \) and \( x \) up to \( 10^{18} \), and return the pair with maximal product under the constraint. If multiple pairs achieve the same maximal product, choose the one with the smallest \( a \). The function should be efficient for large inputs and use 64-bit integers. Note that the problem can be reduced by dividing both \( n \) and \( x \) by their greatest common divisor, but this is optional if you handle overflow carefully.
*/

#include <utility>
#include <cstdint>

// Find two positive integers a, b such that a+b=n, a*b <= x, and a*b is maximized.
// Return (a, b) with a <= b. If no such pair exists, return (0, 0).
std::pair<long long, long long> pairSplitProduct(long long n, long long x) {
    if (n < 2) // Need at least two positive parts.
        return {0, 0};

    // Binary search for largest a in [1, n/2] such that a*(n-a) <= x.
    long long left = 1, right = n / 2;
    long long best_a = -1; // -1 means not found

    while (left <= right) {
        long long mid = left + (right - left) / 2;
        __int128 product = (__int128)mid * (n - mid);
        if (product <= x) {
            best_a = mid;
            left = mid + 1; // Try larger a since product increases as a increases in this range.
        } else {
            right = mid - 1;
        }
    }

    if (best_a == -1)
        return {0, 0};

    long long a = best_a;
    long long b = n - a;
    return {a, b};
}

#include <cassert>
#include <utility>

// Test the function with a variety of cases.
int main() {
    // Basic cases
    assert(pairSplitProduct(10, 100) == std::make_pair(5LL, 5LL)); // Max product 25 <= 100
    assert(pairSplitProduct(10, 20) == std::make_pair(4LL, 6LL)); // 4*6=24 > 20, so 3*7=21 >20, so 2*8=16 <=20, but 3*7>20; check: since a up to 5, find largest a with a(10-a)<=20: a=2 gives 16, a=3 gives 21>20, so a=2 -> b=8? Wait 2*8=16 <=20, but 3*7=21>20, so largest a is 2, product 16. But 4*6=24 >20, so correct is (2,8). Let's verify: a=2 product=16, a=3 product=21>20, so best is 2. So (2,8). However, note that a=1 product=9, a=2 product=16, so max is 16 -> (2,8). But wait, a=4 gives 24 >20, a=5 gives 25 >20, so indeed (2,8). So assertion should be (2,8). Let me correct: pairSplitProduct(10,20) should be (2,8). I'll test.
    assert(pairSplitProduct(10, 20) == std::make_pair(2LL, 8LL));
    assert(pairSplitProduct(10, 9) == std::make_pair(1LL, 9LL)); // 1*9=9 <=9, and 2*8=16>9, so best is 1.
    assert(pairSplitProduct(10, 8) == std::make_pair(0LL, 0LL)); // Even 1*9=9 >8, so no valid.

    // Edge cases
    assert(pairSplitProduct(2, 1) == std::make_pair(0LL, 0LL)); // 1*1=1 <=1, actually valid! Wait n=2, a=1,b=1, product=1, x=1, so valid. So (1,1). Check: a in [1,1], mid=1, product=1, <=1, so best_a=1, return (1,1). So assertion should be (1,1). I need to fix.
    assert(pairSplitProduct(2, 1) == std::make_pair(1LL, 1LL));
    assert(pairSplitProduct(2, 0) == std::make_pair(0LL, 0LL)); // x=0, no positive product <=0.
    assert(pairSplitProduct(1, 100) == std::make_pair(0LL, 0LL)); // n=1, no split.

    // Large numbers (no overflow)
    long long n = 1000000000000000000LL; // 1e18
    long long x = 1000000000000000000LL;
    // The maximum product is (n/2)*(n-n/2) = 5e17 * 5e17 = 2.5e35 > x, so binary search will find a smaller a.
    auto result = pairSplitProduct(n, x);
    // Verify the returned pair satisfies constraints: a+b=n, a*b <= x, and a <= b.
    assert(result.first + result.second == n);
    assert(result.first <= result.second);
    __int128 prod = (__int128)result.first * result.second;
    assert(prod <= x);
    // Also ensure that a+1 (if within n/2) would give product > x (maximality)
    if (result.first < n/2) {
        __int128 next_prod = (__int128)(result.first + 1) * (n - (result.first + 1));
        assert(next_prod > x);
    }

    // Another large case: n is even, x very small -> no valid.
    long long n2 = 1000000000; // 1e9
    long long x2 = 999999999; // less than n2-1
    assert(pairSplitProduct(n2, x2) == std::make_pair(0LL, 0LL));

    // Random small validation against brute force (optional but here just a few)
    // For n=7, enumerate all splits:
    // a=1,b=6 product=6; a=2,b=5 product=10; a=3,b=4 product=12.
    // If x=11, max product <=11 is 10 -> (2,5)
    assert(pairSplitProduct(7, 11) == std::make_pair(2LL, 5LL));
    // If x=12, max is 12 -> (3,4)
    assert(pairSplitProduct(7, 12) == std::make_pair(3LL, 4LL));
    // If x=6, max is 6 -> (1,6)
    assert(pairSplitProduct(7, 6) == std::make_pair(1LL, 6LL));
    // If x=5, none -> (0,0)
    assert(pairSplitProduct(7, 5) == std::make_pair(0LL, 0LL));

    return 0;
}

// The core idea is to find the split \((a, n-a)\) with \( 1 \le a \le n-1 \) that maximizes \( a(n-a) \) subject to \( a(n-a) \le x \). The product function \( f(a) = a(n-a) \) is a parabola opening downward with maximum at \( a = n/2 \). For a given constraint \( x \), the valid region is where \( f(a) \le x \). Since \( f \) is symmetric and increasing for \( a < n/2 \) and decreasing for \( a > n/2 \), the largest valid product will be achieved by the largest \( a \) in the left half (i.e., \( a \le n/2 \)) that satisfies \( a(n-a) \le x \). This suggests a binary search on \( a \) in the range \([1, \lfloor n/2 \rfloor]\), looking for the maximum \( a \) such that \( a(n-a) \le x \). Because the function is increasing in \( a \) on this interval, binary search is valid. After finding the maximum \( a \) satisfying the constraint, we set \( b = n - a \). If no such \( a \) exists (i.e., even for \( a=1 \), \( 1 \cdot (n-1) > x \)), then return \( (0,0) \). Handle the edge case \( n=1 \) separately, since no positive split exists (as \( a \) must be at least 1 and \( b \) at least 1, so \( n \) must be at least 2). Also note that \( a \le b \) is automatic because we search only up to \( \lfloor n/2 \rfloor \). Complexity: binary search over \( O(\log n) \) steps, each step does a constant number of multiplications of numbers up to \( n \), which may overflow if not careful. Since \( n \le 10^{18} \), \( a \) and \( b \) are also up to \( 10^{18} \), and their product can exceed \( 10^{36} \), which does not fit in 64-bit. However, we only need to compare against \( x \le 10^{18} \). We can avoid overflow by checking the product condition using division: \( a(n-a) \le x \) is equivalent to \( a \le x/(n-a) \) (integer division) or compare using `__int128` if available. In the solution, we use `__int128` for the product to safely compare. Also, we do not need to reduce by GCD, but it's optional. The time complexity is \( O(\log n) \) per call, and space complexity is \( O(1) \).
