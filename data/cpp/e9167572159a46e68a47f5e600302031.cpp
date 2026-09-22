// Write a C++ function `bool isPerfectCubeProduct(int a, int b)` that determines whether the product of two positive integers `a` and `b` is a perfect cube, and additionally whether both `a` and `b` are divisible by that cube root. More precisely, return `true` if there exists an integer `x` such that `x * x * x == a * b`, and also `a % x == 0` and `b % x == 0`. Otherwise return `false`. The function must handle values up to \(10^6\) without overflow by using 64-bit arithmetic internally. Do not use floating-point functions like `cbrt`; instead, use integer binary search to find the cube root.

#include <cassert>

int main() {
    // Basic perfect cube cases
    assert(isPerfectCubeProduct(1, 1) == true);          // 1^3 = 1
    assert(isPerfectCubeProduct(2, 4) == true);          // 2^3 = 8, 2 divides both
    assert(isPerfectCubeProduct(3, 9) == true);          // 3^3 = 27, 3 divides both
    assert(isPerfectCubeProduct(4, 2) == true);          // same as above, order irrelevant

    // Non-cube product
    assert(isPerfectCubeProduct(2, 3) == false);

    // Cube product but root does not divide both factors
    assert(isPerfectCubeProduct(1, 8) == false);         // root=2, 1%2 != 0
    assert(isPerfectCubeProduct(8, 1) == false);

    // Larger values
    assert(isPerfectCubeProduct(1000, 1000) == true);    // 1000000 = 100^3, root=100
    assert(isPerfectCubeProduct(999999, 1) == false);    // not a cube

    // Edge: product is cube and both divisible
    assert(isPerfectCubeProduct(64, 125) == true);       // 8000 = 20^3, 64%20 != 0? Actually 64%20=4, false
    // Correction: 64*125 = 8000, 20^3=8000, but 64%20=4 so false
    assert(isPerfectCubeProduct(64, 125) == false);

    // Correct example: a=8, b=27 -> 216=6^3, 8%6=2 so false
    assert(isPerfectCubeProduct(8, 27) == false);
    // a=8, b=27 also 216=6^3, so false due to divisibility

    // Correct both divisible: a=8, b=1 -> 8=2^3, 8%2=0,1%2=1? false
    // Better: a=4, b=2 -> 8=2^3, both divisible by 2 -> true (already tested)

    // Final check with a=27, b=8 -> 216, root=6, 27%6=3 -> false
    assert(isPerfectCubeProduct(27, 8) == false);

    // Perfect cube with root dividing both: a=16, b=4 -> 64=4^3, 16%4=0,4%4=0 -> true
    assert(isPerfectCubeProduct(16, 4) == true);

    return 0;
}

#include <cstdint>

// Returns true if a*b is a perfect cube and both a and b are divisible by its cube root.
bool isPerfectCubeProduct(int a, int b) {
    const int64_t product = static_cast<int64_t>(a) * static_cast<int64_t>(b);
    int left = 1;
    int right = 1000001; // Upper bound for cube root search

    // Binary search for the largest integer whose cube does not exceed product.
    while (left < right - 1) {
        const int mid = left + (right - left) / 2;
        const int64_t cube = static_cast<int64_t>(mid) * mid * mid;
        if (cube > product) {
            right = mid;
        } else {
            left = mid;
        }
    }

    const int64_t rootCube = static_cast<int64_t>(left) * left * left;
    if (rootCube != product) {
        return false; // Not a perfect cube
    }
    if (a % left != 0 || b % left != 0) {
        return false; // Cube root does not divide both factors
    }
    return true;
}

// The key idea is to compute the integer cube root of the product \(p = a \times b\) using binary search on the range \([1, 10^6+1]\). Since \(a\) and \(b\) are at most \(10^6\), their product is at most \(10^{12}\), which fits in a 64-bit integer. The cube root of that product is at most \(10^4\) (since \(10^4^3 = 10^{12}\)), but to be safe we search up to \(10^6+1\). Binary search finds the largest integer `mid` such that `(long long)mid * mid * mid <= p`. After the loop, we check three conditions: (1) the exact cube equality holds, i.e., `(long long)l * l * l == p`; (2) `a` is divisible by `l`; (3) `b` is divisible by `l`. If all hold, the answer is `true`, otherwise `false`. Edge cases include `a == 1` or `b == 1`, products that are not cubes (e.g., `2 * 3 = 6`), and products that are cubes but where one factor is not divisible by the root (e.g., `a=2, b=4` gives product `8`, root `2`, but `4 % 2 == 0` and `2 % 2 == 0` actually is true; a better counterexample is `a=1, b=8` where root is `2`, `1 % 2 != 0` → false). The algorithm runs in \(O(\log 10^6) \approx O(20)\) per query, which is constant time, and uses \(O(1)\) auxiliary space.
