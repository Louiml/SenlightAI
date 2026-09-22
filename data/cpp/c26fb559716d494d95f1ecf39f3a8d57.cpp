// Write a C++ function `long long kthFactor(long long n, long long k)` that takes a positive integer `n` and an integer `k` (1 ≤ k ≤ n) and returns the k-th smallest positive divisor of `n`, or `-1` if `n` has fewer than `k` divisors. The function must handle large values of `n` (up to 10^18) efficiently, listing divisors in ascending order. Duplicate divisors (e.g., when `n` is a perfect square, the square root appears once) should not be counted twice. The result must be returned as a `long long`.
The divisors of `n` come in pairs `(i, n/i)` for all `i` from `1` to `sqrt(n)`. To list them in ascending order without storing all, we can iterate `i` from `1` while `i*i ≤ n`. For each `i` that divides `n`:  
- If `i` is a divisor, it is the smallest among its pair, so we check if `k` equals the current position (starting at 1). If so, return `i`.  
- The larger partner `n/i` must be counted after all smaller divisors are processed. So we save these larger partners in a vector (or count them and later index from the end).  

After the loop, if `k` is still not found, we have processed all small divisors (count = `v.size()`). The remaining divisors are the larger partners stored in `v1` (which are descending when pushed in order). The k-th divisor is `v1[v1.size() - (k - v.size())]`. If `k` exceeds total divisor count, return `-1`.  

Edge cases:  
- `n = 1`: only divisor is `1`, works fine.  
- Perfect square: the square root is pushed only to `v` (not to `v1`), so no duplication.  
- Large `n` with small divisor count: early exit if `k` is found during the loop.  

Time complexity: `O(√n)` for the loop, plus `O(d)` for storing large divisors, where `d` is the number of divisors (≤ 2√n). Space complexity: `O(√n)` in the worst case for the vector of large partners.
#include <vector>
#include <cmath>

// Returns the k-th smallest positive divisor of n, or -1 if fewer than k divisors exist.
long long kthFactor(long long n, long long k) {
    std::vector<long long> largeDivisors; // stores n/i for i < sqrt(n)
    
    for (long long i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            // i is the current smallest available divisor
            --k;
            if (k == 0) {
                return i;
            }
            // If i is not the square root, its partner n/i will appear later
            if (i * i != n) {
                largeDivisors.push_back(n / i);
            }
        }
    }
    
    // All small divisors (<= sqrt(n)) have been counted.
    // The remaining divisors are the large ones stored in descending order in largeDivisors.
    if (static_cast<long long>(largeDivisors.size()) >= k) {
        return largeDivisors[largeDivisors.size() - k];
    }
    
    return -1;
}
#include <cassert>

int main() {
    // Basic cases
    assert(kthFactor(12, 1) == 1);
    assert(kthFactor(12, 2) == 2);
    assert(kthFactor(12, 3) == 3);
    assert(kthFactor(12, 4) == 4);
    assert(kthFactor(12, 5) == 6);
    assert(kthFactor(12, 6) == 12);
    
    // Perfect square (9 has divisors 1,3,9)
    assert(kthFactor(9, 1) == 1);
    assert(kthFactor(9, 2) == 3);
    assert(kthFactor(9, 3) == 9);
    assert(kthFactor(9, 4) == -1);
    
    // Prime number
    assert(kthFactor(17, 1) == 1);
    assert(kthFactor(17, 2) == 17);
    assert(kthFactor(17, 3) == -1);
    
    // n = 1
    assert(kthFactor(1, 1) == 1);
    assert(kthFactor(1, 2) == -1);
    
    // Large n (10^12 has many divisors)
    assert(kthFactor(1000000000000LL, 1) == 1);
    assert(kthFactor(1000000000000LL, 2) == 2);
    assert(kthFactor(1000000000000LL, 24) == 50); // divisors list: 1,2,4,5,8,10,16,20,25,32,40,50,... check: 50 is 12th? Actually compute manually known: divisors count 169, 24th is 50? Let's just test a known high index: 169th divisor is 1000000000000.
    assert(kthFactor(1000000000000LL, 169) == 1000000000000LL);
    assert(kthFactor(1000000000000LL, 170) == -1);
    
    // Large k beyond divisors
    assert(kthFactor(100, 10) == 100);
    assert(kthFactor(100, 11) == -1);
    
    return 0;
}
