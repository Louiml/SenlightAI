Write a C++ function `bool isAbundant(int n)` that takes a positive integer `n` and returns `true` if the sum of its proper divisors (all positive divisors excluding the number itself) is strictly greater than `n`, otherwise `false`. For example, 12 has proper divisors 2, 3, 4, and 6 (and 1), whose sum is 16, which is greater than 12, so `isAbundant(12)` should return `true`. The function must handle edge cases such as `n = 1` (sum of proper divisors is 0, so `false`) and prime numbers (sum of proper divisors is 1, so `false`). The function should be efficient enough for `n` up to at least 10,000, and must be pure (no side effects, no I/O).
// The simplest approach is to iterate through all integers from 2 up to `n-1` and check if each divides `n` evenly. If it does, add it to a running sum. However, this is O(n) per call, which is fine for small `n` but can be optimized. A better approach is to only iterate up to `sqrt(n)` and add both `i` and `n/i` when they are distinct divisors. This reduces the time complexity to O(√n). The divisor `1` is always a proper divisor and should be included in the sum, but since it is included in the iteration when `i` starts from 2 (we miss it), we should initialize the sum to 1 (for `n > 1`) to account for divisor 1. For `n = 1`, there are no proper divisors (excluding itself), so sum = 0. Edge cases: if `n` is 1, return false; if `n` is prime, the only proper divisor is 1, so sum = 1, which is not greater than `n` (for `n` ≥ 2), so false. Also handle perfect squares: when `i*i == n`, do not add `i` twice. The algorithm runs in O(√n) time and O(1) auxiliary space.
#include <cmath>

// Returns true if the sum of proper divisors of n is strictly greater than n.
bool isAbundant(int n) {
    if (n <= 1) {
        return false; // No proper divisors for n=1; for n<=0, undefined but return false.
    }
    
    int sum = 1; // Always include divisor 1 (since n>1).
    int limit = static_cast<int>(std::sqrt(n));
    
    for (int i = 2; i <= limit; ++i) {
        if (n % i == 0) {
            sum += i;
            int other = n / i;
            if (other != i) {
                sum += other;
            }
        }
    }
    
    return sum > n;
}
int main() {
    assert(isAbundant(12) == true);   // Divisors: 1,2,3,4,6 sum=16 >12
    assert(isAbundant(1) == false);   // Sum of proper divisors = 0
    assert(isAbundant(2) == false);   // Prime: sum=1
    assert(isAbundant(6) == false);   // Perfect number: sum=1+2+3=6 not >6
    assert(isAbundant(28) == false);  // Perfect number: sum=1+2+4+7+14=28
    assert(isAbundant(18) == true);   // Divisors: 1,2,3,6,9 sum=21 >18
    assert(isAbundant(20) == true);   // Divisors: 1,2,4,5,10 sum=22 >20
    assert(isAbundant(945) == true);  // Smallest odd abundant: sum=975 >945
    assert(isAbundant(97) == false);  // Prime
    assert(isAbundant(100) == true);  // Divisors sum=117 >100
}
