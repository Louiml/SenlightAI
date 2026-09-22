// Write a C++ function that takes a positive integer `n` and returns the sum of all multiples of 3 or 5 that are strictly less than `n`. The function should handle large values of `n` (up to 10^9) efficiently without iterating through each number. If `n` is 1 or less, the sum should be 0. The function must use a formula-based approach rather than brute-force looping.

// The solution relies on the mathematical formula for the sum of an arithmetic progression: the sum of multiples of `k` less than `n` is `k * m * (m + 1) / 2`, where `m = (n - 1) / k` (the count of multiples). To avoid double-counting numbers that are multiples of both 3 and 5 (i.e., multiples of 15), we compute the sum for multiples of 3, add the sum for multiples of 5, and subtract the sum for multiples of 15. All arithmetic uses `long long` to avoid overflow since `n` can be up to 10^9, making intermediate products up to ~10^18. Edge cases: when `n <= 1`, there are no multiples, so return 0; when `n` is small (e.g., 2), the counts are zero, and the formula naturally yields 0. Time complexity is O(1) constant time, and space complexity is O(1) auxiliary.

#include <cstdint>

// Returns the sum of all multiples of 3 or 5 strictly less than n.
// Uses arithmetic progression formulas; handles n up to 1e9 safely.
long long sumMultiplesOf3Or5(long long n) {
    if (n <= 1) {
        return 0;
    }
    
    auto sumMultiples = [](long long limit, long long k) -> long long {
        long long count = (limit - 1) / k;  // number of multiples < limit
        return k * count * (count + 1) / 2;
    };
    
    long long sum3 = sumMultiples(n, 3);
    long long sum5 = sumMultiples(n, 5);
    long long sum15 = sumMultiples(n, 15);
    
    return sum3 + sum5 - sum15;
}

#include <cassert>

int main() {
    assert(sumMultiplesOf3Or5(10) == 23);      // 3+5+6+9
    assert(sumMultiplesOf3Or5(1) == 0);        // no multiples
    assert(sumMultiplesOf3Or5(2) == 0);        // only 1, no multiples
    assert(sumMultiplesOf3Or5(3) == 0);        // multiples strictly less than 3 → none
    assert(sumMultiplesOf3Or5(4) == 3);        // only 3
    assert(sumMultiplesOf3Or5(20) == 78);      // 3+5+6+9+10+12+15+18
    assert(sumMultiplesOf3Or5(1000) == 233168); // known Project Euler result
    assert(sumMultiplesOf3Or5(1000000000LL) == 233333333166666668LL); // large input check
    assert(sumMultiplesOf3Or5(0) == 0);        // edge case
    assert(sumMultiplesOf3Or5(-5) == 0);       // negative edge case
    return 0;
}
