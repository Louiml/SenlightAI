// Write a C++ function named `normalizeReferences` that takes a double value and an integer value as parameters, and returns an integer. The function must declare a reference to the integer parameter, and then modify the referenced integer by assigning it the truncated value of the double parameter (using `static_cast<int>`). The function should return the original integer value (before modification) as its return value. Additionally, you must demonstrate proper use of `const` on the double parameter (since it is read-only) and ensure that the reference correctly reflects changes to the original integer argument. This task reinforces understanding of references, const correctness, and value truncation in C++.

The solution is straightforward: create a reference alias to the integer parameter, store the original integer value in a local variable before modifying it, then assign the truncated double value to the reference (which modifies the original integer). The function returns the saved original value. Edge cases include: when the double is negative, `static_cast<int>` truncates toward zero (e.g., -1.9 becomes -1); when the double is very large or small, truncation follows standard integer conversion rules; and when the double is exactly an integer, no change occurs. The function must be marked as `const` on the double parameter to signal it is not modified. Time complexity is O(1) and space complexity is O(1) since only a few local variables are used.

// Returns the original integer value, and modifies the integer argument
// to the truncated value of the given double.
int normalizeReferences(const double value, int& target) {
    int original = target;          // Save original before modification
    target = static_cast<int>(value); // Truncate double and assign through reference
    return original;
}

#include <cassert>

int main() {
    // Test 1: Basic truncation and reference modification
    int a = 42;
    int ret = normalizeReferences(3.99, a);
    assert(ret == 42);
    assert(a == 3);

    // Test 2: Negative double truncates toward zero
    int b = 100;
    ret = normalizeReferences(-2.7, b);
    assert(ret == 100);
    assert(b == -2);

    // Test 3: Double exactly an integer
    int c = 7;
    ret = normalizeReferences(7.0, c);
    assert(ret == 7);
    assert(c == 7);

    // Test 4: Double is zero
    int d = -5;
    ret = normalizeReferences(0.0, d);
    assert(ret == -5);
    assert(d == 0);

    // Test 5: Large double truncation
    int e = 1;
    ret = normalizeReferences(123456.789, e);
    assert(ret == 1);
    assert(e == 123456);

    // Test 6: Small fractional double
    int f = 0;
    ret = normalizeReferences(0.999, f);
    assert(ret == 0);
    assert(f == 0);

    // Test 7: Negative integer original
    int g = -10;
    ret = normalizeReferences(5.5, g);
    assert(ret == -10);
    assert(g == 5);
}
