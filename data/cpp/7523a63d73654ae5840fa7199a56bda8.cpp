Write a C++ function `long long sumOfPrimesBelow(long long limit)` that returns the sum of all prime numbers strictly less than the given positive integer `limit`. The function must use a sieve-based approach to mark composite numbers, and it must handle the edge case where `limit` is 0, 1, or 2 (in which cases the sum is 0). The implementation should be efficient for limits up to 2,000,000 and must not allocate more memory than necessary by using a dynamic boolean-type container that is sized appropriately.
#include <cassert>

int main() {
    assert(sumOfPrimesBelow(0) == 0);
    assert(sumOfPrimesBelow(1) == 0);
    assert(sumOfPrimesBelow(2) == 0);
    assert(sumOfPrimesBelow(3) == 2);          // primes: 2
    assert(sumOfPrimesBelow(4) == 2);          // primes: 2,3
    assert(sumOfPrimesBelow(10) == 17);        // 2+3+5+7
    assert(sumOfPrimesBelow(20) == 77);        // 2+3+5+7+11+13+17+19
    assert(sumOfPrimesBelow(100) == 1060);
    assert(sumOfPrimesBelow(1000) == 76127);
    assert(sumOfPrimesBelow(2000000) == 142913828922LL);
}
#include <vector>

// Return the sum of all prime numbers strictly less than limit.
// Uses a sieve of Eratosthenes. Returns 0 if there are no primes below limit.
long long sumOfPrimesBelow(long long limit) {
    if (limit <= 2) {
        return 0;
    }
    
    std::vector<bool> isComposite(limit, false);
    long long sum = 0;
    
    for (long long i = 2; i < limit; ++i) {
        if (!isComposite[i]) {
            sum += i;
            for (long long multiple = i * 2; multiple < limit; multiple += i) {
                isComposite[multiple] = true;
            }
        }
    }
    
    return sum;
}
// The algorithm is a classic Sieve of Eratosthenes applied to find all primes less than `limit`. We maintain a `std::vector<bool>` (or `std::vector<char>` for speed) of size `limit` (indices 0 to limit-1), where `false` denotes a potential prime. We start at `i = 2` and if `isComposite[i]` is false, then `i` is prime: add it to the sum, and mark all multiples of `i` starting from `i*2` (or `i*i` as an optimization, but the original snippet uses `i*2`) as composite by setting them to `true`. The loop continues up to `limit-1`. Edge cases: if `limit <= 2`, there are no primes, so return 0. This works because the vector is zero-initialized by default. Time complexity is \(O(n \log \log n)\) for marking composites, and space complexity is \(O(n)\) where \(n = \text{limit}\). The main pitfall is forgetting to handle limits ≤ 2 and accidentally accessing index out of bounds when limit is very small.
