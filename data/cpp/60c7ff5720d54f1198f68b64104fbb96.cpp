Write a C++ function that takes a single positive integer `n` and returns a `std::pair<int, int>` containing the sum of all even numbers from 1 to `n` (inclusive) as the first element, and the sum of all odd numbers from 1 to `n` (inclusive) as the second element. The function should handle the case where `n` is zero or negative by returning `{0, 0}`. The function must not use loops or recursion; instead, derive the sums directly using arithmetic formulas based on the count of even and odd numbers up to `n`. Assume `n` is small enough that the sums fit into an `int`.
The task requires computing the sum of evens and odds without iteration. For any positive integer `n`, the number of even numbers up to `n` is `kE = n/2` (integer division), and the number of odd numbers is `kO = (n+1)/2`. The sum of the first `kE` even numbers (2,4,6,...,2*kE) is `kE*(kE+1)`, because the sum of the first `m` even numbers is `m*(m+1)`. The sum of the first `kO` odd numbers (1,3,5,...,2*kO-1) is `kO*kO`, because the sum of the first `m` odd numbers is `m^2`. For `n <= 0`, both sums are zero, which aligns with the formula since `n/2` would be non-positive and produce incorrect values, so we handle that with an early return. Edge case: `n=1` gives `kE=0`, `kO=1`, sums `0` and `1`. `n=2` gives `kE=1` sum `2`, `kO=1` sum `1`. The algorithm uses only constant arithmetic operations, so it runs in `O(1)` time and `O(1)` auxiliary space.
#include <utility> // for std::pair

// Returns {sumEven, sumOdd} for numbers 1..n inclusive.
// If n <= 0, returns {0, 0}.
std::pair<int, int> sumsOfEvensAndOdds(int n) {
    if (n <= 0) {
        return {0, 0};
    }
    
    const int countEven = n / 2;
    const int countOdd = (n + 1) / 2;
    
    const int sumEven = countEven * (countEven + 1);  // sum of first k even numbers
    const int sumOdd = countOdd * countOdd;          // sum of first k odd numbers
    
    return {sumEven, sumOdd};
}
#include <cassert>

int main() {
    // Basic positive cases
    assert(sumsOfEvensAndOdds(1) == std::pair<int,int>(0, 1));
    assert(sumsOfEvensAndOdds(2) == std::pair<int,int>(2, 1));
    assert(sumsOfEvensAndOdds(3) == std::pair<int,int>(2, 4));
    assert(sumsOfEvensAndOdds(4) == std::pair<int,int>(6, 4));
    assert(sumsOfEvensAndOdds(5) == std::pair<int,int>(6, 9));
    
    // Larger number: n=10, evens: 2+4+6+8+10=30, odds: 1+3+5+7+9=25
    assert(sumsOfEvensAndOdds(10) == std::pair<int,int>(30, 25));
    
    // n=100: sumEven = 50*51 = 2550, sumOdd = 50*50 = 2500
    assert(sumsOfEvensAndOdds(100) == std::pair<int,int>(2550, 2500));
    
    // Zero and negative
    assert(sumsOfEvensAndOdds(0) == std::pair<int,int>(0, 0));
    assert(sumsOfEvensAndOdds(-5) == std::pair<int,int>(0, 0));
    
    // n=6: evens 2+4+6=12, odds 1+3+5=9
    assert(sumsOfEvensAndOdds(6) == std::pair<int,int>(12, 9));
}
