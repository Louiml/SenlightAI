/*
Write a C++ function `findPair` that takes an integer `n` (where 1 ≤ n ≤ 10^6) and returns a `std::pair<int, int>` representing two positive integers `(a, b)` such that:
- `a` is a divisor of `b` (i.e., `b % a == 0`),
- `a ≥ 1`, `b ≥ a`,
- `a * b > n`,
- `a / b < n` (note: since `a ≤ b`, this condition is always true for positive integers, but it's from the original code),
- and among all such valid pairs, return the one with the smallest `a`. If multiple pairs have the same smallest `a`, return the one with the smallest `b`. If no such pair exists, return `{-1, -1}`. The original code iterates `i` from 1 to `n` and `j` from 1 to `i`, checking `i % j == 0 && i * j > n && i / j < n`, returning `(i, j)` on first hit. Note that the original condition uses `i/j` which in integer division might be 0 for `j > i`, but since `j` only goes up to `i`, it's fine. We want a standalone function that matches this behavior.
*/
#include <utility> // for std::pair

// Given an integer n, return a pair (a, b) such that:
// a divides b, a*b > n, a/b < n, with minimal a then minimal b.
// If no such pair exists, return {-1, -1}.
std::pair<int, int> findPair(int n) {
    for (int a = 1; a <= n; ++a) {
        for (int b = 1; b <= a; ++b) {
            if (a % b == 0 && a * b > n && a / b < n) {
                return {a, b};
            }
        }
    }
    return {-1, -1};
}
#include <cassert>
#include <utility>

int main() {
    // n=1: no pair satisfies a*b > 1 with a<=b and a divides b.
    assert(findPair(1) == std::make_pair(-1, -1));

    // n=2: smallest a=2, b=2 gives 2*2=4>2, 2/2=1<2.
    assert(findPair(2) == std::make_pair(2, 2));

    // n=3: a=2, b=2 gives 4>3, 1<3.
    assert(findPair(3) == std::make_pair(2, 2));

    // n=4: a=2, b=2 gives 4>4? No, need strictly >, so try a=2,b=2 no; a=3,b=1? 3%1==0, 3*1=3 not>4; a=3,b=3? 9>4 and 1<4, so (3,3).
    assert(findPair(4) == std::make_pair(3, 3));

    // n=5: a=3,b=3 gives 9>5, 1<5.
    assert(findPair(5) == std::make_pair(3, 3));

    // n=9: a=3,b=3 gives 9>9? No, 9 not >9; a=4,b=2? 4%2==0, 8 not>9; a=4,b=4? 16>9, 1<9, so (4,4).
    assert(findPair(9) == std::make_pair(4, 4));

    // n=10: a=4,b=4 gives 16>10, works.
    assert(findPair(10) == std::make_pair(4, 4));

    // n=100: sqrt is 10, but a=11? Check a=10,b=5? 10%5==0, 50 not>100; a=11,b=11? 121>100, works.
    assert(findPair(100) == std::make_pair(11, 11));

    // n=1000000: a=1001 works as shown.
    assert(findPair(1000000) == std::make_pair(1001, 1001));

    // n=2^31-1 large, just check it doesn't crash and returns something valid.
    auto result = findPair(2147483647);
    assert(result.first != -1);
    assert(result.first * result.second > 2147483647);
    assert(result.first % result.second == 0);
}
// The original code loops `i` from 1 to `n` and `j` from 1 to `i`, checking `i % j == 0`, `i * j > n`, and `i / j < n`. Since `j` ranges from 1 to `i`, `i / j` is at least 1 (when `j = i`) and at most `i` (when `j = 1`). The condition `i / j < n` is equivalent to `i/n < j` but since `i ≤ n` and `j ≥ 1`, this is always true except possibly when `j = 0` but that's not possible. Actually, for `i ≤ n`, `i / j ≤ i ≤ n`, and if `i = n` and `j = 1`, then `i/j = n`, which is not `< n`, so the condition fails only in that edge case. The first valid pair found in the nested loops is the answer. The loop order ensures minimal `i` first, and for same `i`, minimal `j` (since `j` increases). So the solution is simply to implement the same logic efficiently. For `n` up to 10^6, the naive double loop is O(n^2) which is too slow. But we can optimize: we need to find any pair `(i, j)` with `j` dividing `i`, `i * j > n`, and `i / j < n`. Since `j` divides `i`, let `i = j * k`, then `i/j = k`, so conditions become: `j*k * j > n` => `j^2 * k > n`, and `k < n`. Also `j ≤ i` means `j ≤ j*k` => `k ≥ 1`. We can iterate over `i` from 1 to `n`, and for each `i`, iterate over its divisors `j` up to `i`, but we only need the smallest `j` divisor that satisfies `i * j > n` and `i / j < n`. Since `j` is a divisor of `i`, the smallest divisor is always 1, but `1 * i = i > n` only if `i > n`, but `i ≤ n`, so `i * 1 ≤ n`, fails. The next divisors increase. Actually, we can break out early. For a given `i`, the divisors `j` are increasing, so once `i * j > n`, we can check `i / j < n`. The first `j` that satisfies `i * j > n` is `j = floor(n/i) + 1`, but `j` must divide `i`. So we need to find the smallest divisor `j` of `i` such that `j > n / i` (since integer arithmetic, `i * j > n` means `j > n / i`). Also need `i / j < n`, which is almost always true. So for each `i`, we can iterate over divisors up to sqrt(i), but that's still O(n sqrt(n)) worst case, which for n=10^6 is about 10^9 operations, too slow. A simpler approach is to just iterate `i` from 1 to n and break early because the first found `i` is minimal, and for each `i`, the first `j` that works is found quickly. In practice, for many `i`, there is a small `j` that works. But worst case, if no pair exists? Let's analyze: For i=1, j=1, i*j=1 ≤ n, fail. For i=2, j=1 gives 2≤n, j=2 gives 4>n and i/j=1<n, so (2,2) works for n < 4? Actually if n=3, i=2, j=2: 2*2=4>3 and 2/2=1<3, so works. So for n≥2, (2,2) works? For n=2: i=2, j=2 gives 4>2 and 1<2, works. For n=1: i=1, j=1 gives 1>1? false. i=1, no other. So for n=1, no pair, return -1. So always exists for n≥2. For n=2, i=2, j=2 works. So we can just loop i from 1 to n and for each i, loop j from 1 to i, but break early when i*j>n and i%j==0, then check i/j<n. Since we want minimal i and minimal j, and we find the first such pair in the nested loops, we can just do that. For n up to 10^6, worst case, how many iterations? The first valid pair is often found quickly. For n=10^6, i=2, j=2 works immediately because 4>10^6? No, 4 is not > 10^6. Actually for n=10^6, need i*j > 1e6. For i=2, j=2 gives 4, too small. j must be at least 500001 for i=2, but j must divide i=2, only j=1,2. So i=2 fails. i=3, j must be > 1e6/3 ~333333, but j≤3, fails. In general, for i small, i*j > n requires j > n/i, but j≤i, so we need i^2 > n => i > sqrt(n). So minimal i is floor(sqrt(n))+1. For n=1e6, sqrt=1000, so i starts at 1001. For i=1001, j must be > 1e6/1001 ≈ 999, and j must divide 1001, and j≤1001. 1001 = 7*11*13, divisors: 1,7,11,13,77,91,143,1001. The smallest divisor >999 is 1001 itself, so i=1001, j=1001 works: 1001*1001 > 1e6? 1,002,001 > 1,000,000 yes, and i/j=1 < n. So for n=1e6, (1001,1001) works. So the first found will be (ceil(sqrt(n))? Actually i=1001 works. For any n, smallest i is the smallest integer such that there exists a divisor j of i with j > n/i. The smallest such i is around sqrt(n). So we can just simulate the double loop but break early when we find a pair. The number of iterations until we find a pair is roughly O(sqrt(n) * number of divisors) which is small. Even if no pair exists (n=1), loop runs once. So naive double loop with early break is efficient enough for n up to 1e6. However, to be safe, we can add a break after finding the first pair. The original code returns immediately. So time complexity in worst case is O(n log n) if we loop all i and j as divisors, but with early break, it's O(sqrt(n) * d(sqrt(n))) where d is number of divisors, very small. Space O(1). Edge case: n=1 returns {-1,-1}. Ensure integer division works correctly.
