Write a standalone C++ function named `sumOfDivisors` that takes a single positive integer `n` as input and returns the sum of all positive divisors of `n` (including 1 and `n`). For example, if `n = 12`, the divisors are 1, 2, 3, 4, 6, 12, so the sum is 28. The function must handle large values of `n` up to \(10^{12}\) efficiently, avoiding a naive loop up to `n`. Also, consider edge cases such as `n = 1`, prime numbers, and perfect squares. Do not include a `main` function; the solution should only contain the free function and necessary headers.

#include <cassert>

int main() {
    // Simple cases
    assert(sumOfDivisors(1) == 1);
    assert(sumOfDivisors(2) == 3);   // 1+2
    assert(sumOfDivisors(3) == 4);   // 1+3
    assert(sumOfDivisors(4) == 7);   // 1+2+4
    
    // Composite and perfect square
    assert(sumOfDivisors(12) == 28); // 1+2+3+4+6+12
    assert(sumOfDivisors(16) == 31); // 1+2+4+8+16
    
    // Prime number
    assert(sumOfDivisors(17) == 18); // 1+17
    
    // Larger value to check efficiency and correctness
    assert(sumOfDivisors(100) == 217); // 1+2+4+5+10+20+25+50+100
    
    // Very large value (10^12) – result is known: 1+2+4+5+10+20+25+50+100+125+... but we just verify it's > 0
    assert(sumOfDivisors(1000000000000) > 0);
    
    // Edge case: n = 0 is not allowed per task, but if passed, sqrt(0)=0, loop doesn't run, sum=0
    // Not testing since specification says positive integer.
}

#include <cmath>

// Returns the sum of all positive divisors of a positive integer n.
// Works efficiently for n up to 10^12 using the divisor-pair method.
long long sumOfDivisors(long long n) {
    long long sum = 0;
    long long limit = static_cast<long long>(std::sqrt(n));
    
    for (long long i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            sum += i;
            if (i != n / i) {
                sum += n / i;
            }
        }
    }
    return sum;
}

// The straightforward approach of iterating from 1 to `n` and checking divisibility is too slow for \(n \leq 10^{12}\), so we use the divisor-pair property: for every divisor `d` of `n` where `d ≤ sqrt(n)`, the paired divisor is `n/d`. We loop `i` from 1 to `sqrt(n)`. If `i` divides `n`, we add `i`. If `i != n/i` (to avoid double-counting when `n` is a perfect square), we add `n/i` as well. This reduces the number of iterations to \(O(\sqrt{n})\), which is at most \(10^6\) iterations for \(10^{12}\), easily manageable. Edge cases: `n = 1` returns 1 (only divisor is itself); perfect squares like `n = 16` (divisors: 1,2,4,8,16 → sum 31) are handled by the inequality check. Time complexity is \(O(\sqrt{n})\), space complexity \(O(1)\).
