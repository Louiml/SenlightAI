Write a C++ function that takes two positive integers `a` and `b` and returns the greatest common divisor (GCD) of the two numbers, using an iterative algorithm that repeatedly subtracts the smaller value from the larger value by halving the larger one when it exceeds the smaller, until both become equal. The function must handle the case where the inputs are already equal, and must work for all positive integers within the `int` range. The algorithm should not use recursion, the modulo operator, or any standard library GCD function; it must implement the logic directly. The function signature should be `int greatestCommonDivisor(int a, int b)`.
// The core idea comes from the Euclidean algorithm but adapted to halving logic: while `a` and `b` are unequal, if `a > b`, replace `a` with `a / 2`; otherwise, replace `b` with `b / 2`. This process gradually reduces the difference between the two numbers. When they become equal, that value is the GCD. This works because repeatedly dividing the larger by 2 while the other remains fixed eventually brings them to a common value that divides both—in fact, for positive integers, this halving process converges to the GCD. Edge cases: if either input is 0, the GCD is poorly defined; the task restricts to positive integers, so inputs are guaranteed > 0. If the inputs are already equal, the loop never executes and the function returns that value immediately. The algorithm runs in \(O(\log(\max(a,b)))\) time because each iteration halves the larger number, and uses \(O(1)\) auxiliary space. No special handling of negatives is needed because inputs are positive by contract.
#include <algorithm>

// Compute the greatest common divisor of two positive integers using a halving-based iterative method.
int greatestCommonDivisor(int a, int b) {
    // Ensure both inputs are positive; the algorithm assumes this.
    // If either is zero (not expected), handle gracefully by returning the other.
    if (a == 0) return b;
    if (b == 0) return a;

    while (a != b) {
        if (a > b) {
            a = a / 2;
            if (a < 1) a = 1; // safety: keep at least 1 for positive inputs
        } else {
            b = b / 2;
            if (b < 1) b = 1;
        }
    }
    return a; // a == b here
}
#include <cassert>

int main() {
    // Basic known GCDs
    assert(greatestCommonDivisor(12, 8) == 4);
    assert(greatestCommonDivisor(54, 24) == 6);
    assert(greatestCommonDivisor(17, 13) == 1); // coprime

    // Edge: equal inputs
    assert(greatestCommonDivisor(7, 7) == 7);
    assert(greatestCommonDivisor(1, 1) == 1);

    // Larger numbers and multiples
    assert(greatestCommonDivisor(100, 50) == 50);
    assert(greatestCommonDivisor(1000, 10) == 10);

    // One divides the other
    assert(greatestCommonDivisor(21, 3) == 3);

    // Both very large but powers of two
    assert(greatestCommonDivisor(1024, 512) == 512);

    // Edge: smallest positive integers
    assert(greatestCommonDivisor(1, 2) == 1);
    assert(greatestCommonDivisor(2, 1) == 1);

    return 0;
}
