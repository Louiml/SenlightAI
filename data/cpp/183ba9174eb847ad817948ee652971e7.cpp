/*
Write a C++ function `int computeResult(int n, int k)` that, given two positive integers `n` and `k`, determines the smallest non-negative integer `i` (with `0 ≤ i ≤ k`) such that `i * n + (n - 1) > k`. If `n == 1`, return `k`. Otherwise, if such an `i` is found, return `i - 1`; if no such `i` exists within the loop (which for the given constraints always does when n>1), return the last computed `i`. The function must handle all integer values within the typical 32-bit signed range, and must avoid overflow by using `long long` for intermediate multiplications. The behavior should exactly match the original snippet’s logic, including the edge case where `n == 1` returns `k` directly.
*/
#include <cstdint>

// Compute the result as per the original snippet's logic.
// Returns k if n == 1, otherwise returns (smallest i with i*n + n-1 > k) - 1.
// n and k are positive integers.
int computeResult(int n, int k) {
    if (n == 1) {
        return k;
    }
    // Use long long to avoid overflow in i * n.
    for (long long i = 0; i <= k; ++i) {
        long long expression = i * n + (n - 1);
        if (expression > k) {
            return static_cast<int>(i - 1);
        }
    }
    // The loop always breaks for n > 1, but for safety return k (unreachable).
    return k;
}
#include <cassert>

int main() {
    // Base cases from original snippet logic.
    assert(computeResult(1, 5) == 5);
    assert(computeResult(1, 0) == 0);
    // For n=2, k=5: compute sequence: i=0 -> -1? Actually n-1=1, 0*2+1=1 <=5; i=1 -> 3 <=5; i=2 -> 5 <=5; i=3 -> 7 >5, so return i-1=2.
    assert(computeResult(2, 5) == 2);
    // n=3, k=10: i=0->2, i=1->5, i=2->8, i=3->11>10, return 2.
    assert(computeResult(3, 10) == 2);
    // n=4, k=3: i=0->3 <=3; i=1->7>3, return 0.
    assert(computeResult(4, 3) == 0);
    // n=10, k=100: i=0->9, ... i=10->109>100? Actually i=10 gives 10*10+9=109>100, i=9 gives 9*10+9=99<=100, return 9.
    assert(computeResult(10, 100) == 9);
    // Edge case: k=0, n>1: i=0 gives n-1 > 0? if n>1 then n-1 >=1 >0, so return -1? Original for n=2,k=0: loop i=0 expression=1>0, break, calculo=0, then output calculo-1 = -1. So function returns -1.
    assert(computeResult(2, 0) == -1);
    // Large values to test overflow safety.
    assert(computeResult(50000, 1000000) == 19); // 19*50000+49999 <= 1000000? 19*50000=950000+49999=999999 <=1e6; 20*50000+49999=1049999>1e6, return 19.
    // n=1 always returns k even if k large.
    assert(computeResult(1, 123456789) == 123456789);
    return 0;
}
// The original code iterates `i` from 0 to `k` inclusive, computing `calculo = i * n + n_pausa` where `n_pausa = n - 1`. It breaks as soon as `calculo > k`, setting `calculo = i` before breaking. After the loop, if `n == 1` it outputs `k`, otherwise it outputs `calculo - 1`. This effectively finds the largest `i` such that `i * n + (n - 1) <= k` (when `n > 1`), because the break occurs at the first `i` that makes the expression exceed `k`, and then we subtract 1. However, notice the subtlety: the loop runs `i` from 0 to `k` inclusive; if the condition never triggers (impossible for n>1 because at i=k, `k*n + n-1 > k` for n>=2), the loop completes and `calculo` is then `(k+1)*n + n-1`? Actually no: `for(size_t i=0; i <= k; i++)` uses unsigned, and after loop ends if no break, `calculo` would be `(k)*n + n-1`? Wait: the loop increments `i` until `i > k`, but inside the loop the last executed `i` is `k` because condition `i <= k`, so after loop `i` becomes `k+1` but `calculo` holds the value computed for `i = k`. But since for n>1, at i=k, `calculo = k*n + n-1 > k` (unless n=0 but n positive), so break will always happen at the latest at `i = k`. Therefore for n>1, the loop always breaks and sets `calculo = i` where `i` is the smallest index making expression > k. Then output `i-1`. For n==1, the code skips the whole computation and outputs `k`. So the function should: if n==1, return k; else, loop i from 0 upward, compute `i * n + (n-1)` using `long long` to avoid overflow (since i can be up to k, and n up to maybe large), and when that expression > k, return i-1. Because we iterate from 0, the first `i` that triggers break is minimal. The time complexity is O(i) which in the worst case is O(k) (though in practice i ≈ k/n, so O(k/n)), but worst-case O(k) when n=2. Space O(1). Edge cases: n=1 returns k; n>1 always find a break because at i=k, expression = k*n + n-1 >= k+1 (since n>=2 => k*2+1 > k). Also handle large n,k that might overflow int; use long long.
