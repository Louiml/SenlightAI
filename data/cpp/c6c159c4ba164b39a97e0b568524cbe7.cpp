// Write a C++ function named `findMostFrequentDivisor` that takes a positive integer `n` (where `1 <= n <= 10^9`) and returns the divisor of `n` that appears most frequently as a divisor in the set of all positive integers from 1 to `n`. More precisely, for each divisor `d` of `n`, count how many integers `k` in the range `[1, n]` are divisible by `d` (i.e., `k % d == 0`). The function should return the divisor `d` that maximizes this count. If multiple divisors have the same maximum count, return the largest such divisor. For example, for `n = 12`, divisors are {1,2,3,4,6,12}. Counts: 1 divides 12 numbers, 2 divides 6, 3 divides 4, 4 divides 3, 6 divides 2, 12 divides 1. The maximum count is 12 for divisor 1, so return 1. For `n = 6`, divisors {1,2,3,6}: counts 6,3,2,1 → return 1. For `n = 10`, divisors {1,2,5,10}: counts 10,5,2,1 → return 1. Note: the function must handle large `n` efficiently without iterating over all integers up to `n`. The implementation should compute all divisors of `n`, then for each divisor compute the count as `n / d` (since numbers divisible by `d` in [1,n] are exactly `floor(n/d)`), and pick the divisor with the largest count; ties broken by larger divisor.
// The key observation is that for a given divisor `d` of `n`, the count of integers in `[1,n]` divisible by `d` is simply `n / d` (integer division). Therefore, we only need to enumerate all divisors of `n`. Enumerate divisors by iterating `i` from 1 to `sqrt(n)`, and for each `i` dividing `n`, add both `i` and `n/i` (if distinct). For each divisor, compute its count as `n / d`, and track the maximum count. If a tie occurs, choose the larger divisor. Since the counts are decreasing as divisor increases (because `n/d` is smaller for larger `d`), the maximum count will always be achieved at the smallest divisor, which is `1` (count `n`). However, ties occur when multiple divisors have the same `n/d`. For example, for `n=4`, divisors {1,2,4}, counts: 4,2,1 → max is 1. For `n=8`, divisors {1,2,4,8}, counts: 8,4,2,1 → max is 1. In general, the divisor `1` always has the highest count `n`, and no other divisor can match that because for any `d>1`, `n/d < n`. Therefore, the answer is always `1` for any `n >= 1`. This is trivially true, but if we want to make the task more interesting, we could modify the problem, but per specification, the answer is always 1. However, to make the task nontrivial, we can change the definition: instead, find the divisor `d` of `n` that maximizes the number of divisors of `n` that are multiples of `d`? But no, the provided description is fixed. So the solution is simply enumerate divisors and compute `n/d`, then pick max count (which will always be 1). Time complexity is `O(sqrt(n))` for enumerating divisors, space `O(number_of_divisors)`. Edge case: n=1, divisor {1}, count=1, return 1.
#include <vector>
#include <algorithm>

// Given a positive integer n, return the divisor of n that divides the most
// integers in [1, n]. Since every divisor d divides exactly floor(n/d) integers,
// and this count is maximized for d=1, the answer is always 1, but we compute
// it via full enumeration to be robust.
long long mostFrequentDivisor(long long n) {
    std::vector<long long> divisors;
    for (long long i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) {
                divisors.push_back(n / i);
            }
        }
    }

    long long bestDivisor = -1;
    long long bestCount = -1;
    for (long long d : divisors) {
        long long count = n / d;
        if (count > bestCount || (count == bestCount && d > bestDivisor)) {
            bestCount = count;
            bestDivisor = d;
        }
    }
    return bestDivisor;
}
#include <cassert>

int main() {
    // For any positive n, divisor 1 divides all n numbers.
    assert(mostFrequentDivisor(1) == 1);
    assert(mostFrequentDivisor(2) == 1);
    assert(mostFrequentDivisor(3) == 1);
    assert(mostFrequentDivisor(4) == 1);
    assert(mostFrequentDivisor(6) == 1);
    assert(mostFrequentDivisor(10) == 1);
    assert(mostFrequentDivisor(12) == 1);
    assert(mostFrequentDivisor(100) == 1);
    assert(mostFrequentDivisor(1000000000LL) == 1);
    // Also test a prime number
    assert(mostFrequentDivisor(17) == 1);
}
