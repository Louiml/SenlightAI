// Given three positive integers `a`, `b`, and `c` (each between 1 and 10⁹), write a C++ function `bool canMeasure(int a, int b, int c)` that returns `true` if it is possible to obtain exactly `c` units using an unlimited number of two jugs with capacities `a` and `b`. This is the classic "jug problem" where you can fill, empty, or pour water between jugs. The function should determine whether `c` is representable as a non-negative integer combination of `a` and `b` (i.e., there exist non-negative integers `x` and `y` such that `a*x + b*y = c`). Note that the original snippet used a brute-force loop, but your solution should be efficient. Also, handle the special case where `c` equals 0 (return `true`), and the case where `c` exceeds the sum of both jugs (return `false`).
The key insight is that any amount that can be measured with two jugs of capacities `a` and `b` must be a multiple of their greatest common divisor (GCD). This is a well-known number theory result: the set of all achievable amounts is exactly the set of non-negative integer multiples of `gcd(a,b)` that do not exceed `a+b`. However, the original snippet's brute-force approach iterates over `i` from 0 to `a` and checks if `(c - a*i)` is divisible by `b`; that works because one of the jugs is poured multiple times. A more general and efficient method is to compute `g = gcd(a,b)` and check if `c % g == 0` and `c <= a + b`. But careful: the condition `c <= a + b` is necessary because you cannot store more than the total capacity of both jugs at once. However, if `c` equals exactly `a + b`, that is achievable by filling both. For `c = 0`, always true. Edge cases: when `a` or `b` is 0 (not in constraints but defensively), handle gcd accordingly; when `c` is 0, return true. The time complexity is O(log(min(a,b))) for computing GCD, and O(1) auxiliary space.
#include <numeric>   // for std::gcd (C++17)

// Returns true if exactly c units can be measured using jugs of capacities a and b.
bool canMeasure(int a, int b, int c) {
    // Base case: zero amount is always measurable.
    if (c == 0) return true;
    // If c exceeds total combined capacity, impossible.
    if (c > a + b) return false;
    // A positive amount c is measurable iff c is a multiple of gcd(a,b).
    int g = std::gcd(a, b);
    return (c % g == 0);
}
#include <cassert>

int main() {
    // Basic multiples
    assert(canMeasure(3, 5, 4) == true);  // 3*2 + 5*(-1?) but 4 = 5 - 3? Actually 4 = 5*2 - 3*2? Wait, 5+5-3-3=4, yes possible.
    assert(canMeasure(3, 5, 7) == true);  // 3*4 + 5*(-1)? Actually 7 = 5+2, 2 not multiple of 1? gcd=1 so yes.
    assert(canMeasure(4, 6, 8) == true);  // gcd=2, 8%2==0, 8 <= 10, yes.
    assert(canMeasure(4, 6, 9) == false); // gcd=2, 9%2 != 0.
    // Edge cases
    assert(canMeasure(3, 5, 0) == true);
    assert(canMeasure(3, 5, 8) == true);  // 3+5
    assert(canMeasure(3, 5, 9) == false); // >8
    assert(canMeasure(1, 1, 2) == true);  // gcd=1, 2%1==0, 2<=2
    assert(canMeasure(2, 4, 3) == false); // gcd=2, 3%2!=0
    // Large values avoiding overflow (but constants within int)
    assert(canMeasure(1000000000, 999999999, 1) == true); // gcd=1? Actually gcd(1e9, 999999999)=1? 1e9 - 999999999 = 1, so yes.
    return 0;
}
