/*
Write a C++ function `ll countWays(ll n)` that, given an even positive integer `n`, returns the number of distinct ways to divide a set of `n` distinct elements into `n/2` unordered pairs (i.e., the number of perfect matchings on a complete graph with `n` vertices). The result must be computed exactly as a 64-bit integer (use `long long`). For `n = 2`, there is exactly 1 way. For `n = 4`, there are 3 ways. For `n = 6`, there are 15 ways. The input `n` will always be even and in the range `2 ≤ n ≤ 34`. The function must return the result as a `long long`. (Inspired by the provided snippet: it computes `calc(n, n/2)` times the product of squares of 1..(n/2-1) divided by 2, which simplifies to the double factorial formula for perfect matchings.)
*/

#include <cstdint>

// Returns the number of perfect matchings on a complete graph with n vertices.
// n must be even and 2 <= n <= 34. The result fits in a 64-bit signed integer.
// Equivalent to (n-1)!! = 1 * 3 * 5 * ... * (n-1).
std::int64_t countPerfectMatchings(int n) {
    if (n % 2 != 0 || n < 2) {
        return 0; // Invalid input per constraints, but safe guard.
    }
    std::int64_t result = 1;
    // Multiply odd numbers from 1 up to n-1.
    for (int odd = 1; odd < n; odd += 2) {
        result *= odd;
    }
    return result;
}

#include <cassert>
#include <cstdint>

std::int64_t countPerfectMatchings(int n) {
    if (n % 2 != 0 || n < 2) {
        return 0;
    }
    std::int64_t result = 1;
    for (int odd = 1; odd < n; odd += 2) {
        result *= odd;
    }
    return result;
}

int main() {
    assert(countPerfectMatchings(2) == 1);
    assert(countPerfectMatchings(4) == 3);
    assert(countPerfectMatchings(6) == 15);
    assert(countPerfectMatchings(8) == 105);
    assert(countPerfectMatchings(10) == 945);
    assert(countPerfectMatchings(12) == 10395);
    assert(countPerfectMatchings(14) == 135135);
    assert(countPerfectMatchings(16) == 2027025);
    assert(countPerfectMatchings(18) == 34459425);
    assert(countPerfectMatchings(20) == 654729075);
    assert(countPerfectMatchings(34) == 6332659870762850625LL);
    return 0;
}

// The number of perfect matchings in a complete graph with `n` vertices (where `n` is even) is given by the double factorial `(n-1)!! = (n-1)*(n-3)*...*3*1`. Alternatively, it equals `n! / (2^(n/2) * (n/2)!)`. The provided snippet computes it differently: it calculates `C(n, n/2)` (binomial coefficient) and then multiplies by `(i*i)` for `i=1` to `n/2-1`, and divides by 2. That formula is equivalent to `(n/2)!` times `C(n, n/2)` divided by 2? Let’s verify: For n=6: C(6,3)=20, product of i^2 for i=1..2 gives 1*4=4, so 20*4/2=40? That’s wrong. Actually the code multiplies by `i*i` for i from 1 to `n/2-1`, then divides by 2. For n=6: calc(6,3)=20, i=1..2 gives 1*4=4, ans=80, ans/2=40, but correct answer is 15. So the snippet is actually buggy for general n? Wait, let's trace: calc(n, r) computes binomial coefficient n choose r using integer division sequentially: ans=1; for i=0..r-1: ans *= (n-i); ans /= (i+1). That works correctly for binomial. For n=6, r=3: i=0: ans=6; ans/=1 =>6; i=1: ans*=5=30; ans/=2=15; i=2: ans*=4=60; ans/=3=20. So calc=20. Then loop i=1 to (n/2 -1) = 1 to 2: ans *= i*i; so ans *= 1; ans *= 4 => 80. Then ans/2 = 40. But the correct answer is 15. So the provided snippet is actually wrong? Let's check n=4: calc(4,2) = 6; loop i=1..1: ans*=1 =>6; ans/2=3 correct. n=2: calc(2,1)=2; loop i=1..0: no loop; ans=2; ans/2=1 correct. n=6 gives 40, but correct is 15. So snippet is incorrect for n>=6. The task should be to implement a correct function. The formula for perfect matchings is (n-1)!!. For n=34, (33)!! is about 3.7e18? Let's check: (33)!! = 33*31*...*3*1. Approximate: product of odd numbers up to 33. Let's compute approximate: 33*31=1023, *29=29667, *27=801009, *25=20,025,225, *23=460,580,175, *21=9,672,183,675, *19=183,771,489,825, *17=3,124,115,327,025, *15=46,861,729,905,375, *13=609,202,488,769,875, *11=6,701,227,376,468,625, *9=60,311,046,388,217,625, *7=422,177,324,717,523,375, *5=2,110,886,623,587,616,875, *3=6,332,659,870,762,850,625, *1=6,332,659,870,762,850,625 which is about 6.3e18, still within 64-bit (max ~9.22e18). So for n≤34, result fits in long long. The correct algorithm: compute product of odd numbers from 1 to n-1, or use the combinatorial formula. Simpler: result = 1; for (int i = 1; i <= n/2; ++i) result = result * (2*i -1); But careful: (n-1)!! = product_{k=1}^{n/2} (2k-1). So for n=2: product of 1 =1; n=4: 1*3=3; n=6: 1*3*5=15. That’s correct. Time O(n), space O(1). Edge case: n must be even; if not, return 0 (but constraint says even). Also ensure no overflow: use long long multiplication; for n=34, product fits. We'll implement `long long countPerfectMatchings(int n)`.
