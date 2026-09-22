// Write a C++ function `bool isAbundantNumber(int n)` that determines whether a given positive integer `n` is an abundant number. An abundant number is a positive integer for which the sum of its proper divisors (all positive divisors excluding the number itself) is greater than the number. The function should handle the case `n = 1` (which has no proper divisors, sum = 0, so it is not abundant) and correctly process perfect squares and small numbers like 2 and 3. Your implementation should avoid counting any divisor more than once and should iterate efficiently up to the square root of `n`. The function must be declared with appropriate `const` correctness (though `n` is passed by value, the function should not modify it). Provide the implementation in a single self-contained function without a `main` function.
#include <cassert>
#include <cmath>

// Include the solution function here (for testing, assume it is defined above)
// bool isAbundantNumber(const int n);

int main() {
    // Basic non-abundant numbers
    assert(isAbundantNumber(1) == false);
    assert(isAbundantNumber(2) == false);
    assert(isAbundantNumber(3) == false);
    assert(isAbundantNumber(4) == false);
    assert(isAbundantNumber(5) == false);
    assert(isAbundantNumber(6) == false); // 6 is perfect, not abundant
    assert(isAbundantNumber(7) == false);
    assert(isAbundantNumber(8) == false);
    assert(isAbundantNumber(9) == false);
    assert(isAbundantNumber(10) == false);
    assert(isAbundantNumber(11) == false);
    assert(isAbundantNumber(12) == true);  // 1+2+3+4+6=16 > 12
    assert(isAbundantNumber(18) == true);  // 1+2+3+6+9=21 > 18
    assert(isAbundantNumber(20) == true);  // 1+2+4+5+10=22 > 20
    assert(isAbundantNumber(21) == false); // 1+3+7=11 < 21
    assert(isAbundantNumber(28) == false); // 28 is perfect (sum = 28)
    assert(isAbundantNumber(30) == true);  // 1+2+3+5+6+10+15=42 > 30
    assert(isAbundantNumber(16) == false); // 1+2+4+8=15 < 16 (perfect square handling)
    assert(isAbundantNumber(100) == true); // 1+2+4+5+10+20+25+50=117 > 100
    assert(isAbundantNumber(1) == false);  // edge case
    return 0;
}
#include <cmath>

// Determine if a positive integer n is an abundant number.
// An abundant number has the sum of its proper divisors greater than itself.
bool isAbundantNumber(const int n) {
    if (n <= 1) {
        return false; // 1 has no proper divisors (sum = 0)
    }
    
    int divisorSum = 1; // 1 is always a proper divisor for n > 1
    for (int i = 2; i <= std::sqrt(n); ++i) {
        if (n % i == 0) {
            divisorSum += i;
            if (i != n / i) {
                divisorSum += n / i;
            }
        }
    }
    
    return divisorSum > n;
}
// The solution computes the sum of proper divisors by iterating from 1 up to the square root of `n`. For each divisor `i` found (where `i < sqrt(n)` or `i <= sqrt(n)`), we add `i` and, if `i != n/i`, we also add the paired divisor `n/i`. The initial sum is set to 1 (since 1 is always a proper divisor for `n > 1`), but for `n = 1`, we must return false because there are no proper divisors (sum should be 0). The loop should run from `i = 2` up to `sqrt(n)` inclusive. For `n = 1`, the loop does not run, but the initial sum of 1 would incorrectly make it abundant (1 > 1 is false, so actually it would not be abundant, but the sum itself is wrong; to be safe, handle `n == 1` explicitly). Edge cases: perfect squares (e.g., 16) require checking `i != n/i` to avoid adding the square root twice. Small numbers like 2, 3, 4 should be handled correctly: 1 is not abundant, 2 is not (sum=1), 3 is not (sum=1), 4 is not (sum=1+2=3 < 4), 12 is abundant (sum=1+2+3+4+6=16 > 12). Time complexity is \(O(\sqrt{n})\) due to the loop up to sqrt(n), and space complexity is \(O(1)\). The function should be `bool` and take `int n` by value, no need for const reference since passing by value is fine for small ints, but we can make it `bool isAbundantNumber(const int n)` to imply const correctness.
