// Write a C++ function `gcd(int a, int b)` that computes the greatest common divisor of two integers using the Euclidean algorithm (with recursion). The function must handle non‑negative inputs, including the case where one or both arguments are zero, and it must return the correct non‑negative GCD. For example, `gcd(0, 5)` should return 5, and `gcd(0, 0)` should be treated as returning 0 (since mathematically GCD(0,0) is undefined, but in this task assume it returns 0). The function must be a free function, not a member of a class, and must not rely on any standard library GCD implementation. Provide a demonstration via a simple `main` that prints the result of `gcd(27, 45)`.

The Euclidean algorithm is based on the property that `gcd(a, b) = gcd(b % a, a)` for `a > 0`. The base case occurs when `a == 0`, in which case the GCD is `b`. This works because if one number is zero, the GCD is the other non‑zero number. Edge cases:  
- `gcd(0, b)` returns `b` directly via the base case.  
- `gcd(a, 0)` becomes `gcd(0, a)` in the first recursive call, returning `a`.  
- `gcd(0, 0)` hits the base case and returns `0` (since `b` is 0), which is accepted in this task.  
The recursion depth is proportional to the number of steps in the Euclidean algorithm, which is at most \(O(\log \min(a,b))\) for typical numbers, so the time complexity is \(O(\log(\min(a,b)))\) and the space complexity is \(O(\log(\min(a,b)))\) due to recursion stack. No negative numbers are expected; if they appear, the algorithm would still work but might return a negative GCD — for this task we assume inputs are non‑negative.

#include <iostream> // included for completeness; not required for the free function

// Computes the greatest common divisor of two non‑negative integers using recursion.
// Precondition: a and b are non‑negative integers.
// Returns: the GCD; if both are 0, returns 0.
int gcd(int a, int b) {
    if (a == 0) {
        return b;
    }
    return gcd(b % a, a);
}

#include <cassert>

// Free function declaration (should match the solution's signature)
int gcd(int a, int b);

int main() {
    // Basic tests
    assert(gcd(27, 45) == 9);
    assert(gcd(45, 27) == 9);
    assert(gcd(100, 10) == 10);
    assert(gcd(17, 19) == 1);
    assert(gcd(0, 5) == 5);
    assert(gcd(5, 0) == 5);
    assert(gcd(0, 0) == 0);
    assert(gcd(1, 1) == 1);
    assert(gcd(12, 8) == 4);
    assert(gcd(81, 27) == 27);
    return 0;
}
