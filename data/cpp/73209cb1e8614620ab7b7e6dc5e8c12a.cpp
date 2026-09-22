// Write a C++ function named `gcdUsingEuclidean` that computes the greatest common divisor (GCD) of two non-negative integers using Euclid's algorithm. The function must handle edge cases: if both inputs are zero, return 0 (representing an undefined GCD in a practical sense, or you may document that GCD(0,0) is undefined, but for this task return 0); if one input is zero, return the other non-zero input. The function should be `const`-correct, meaning it takes two `int` values by value and returns an `int`. It must not modify its inputs, and it must be implemented iteratively (not recursively) to avoid stack overflow for very large inputs. The function should also handle negative inputs by taking their absolute value before computing. Provide a self-contained implementation with necessary includes and a descriptive comment.

The solution uses Euclid’s algorithm: repeatedly replace the larger number with the remainder of dividing the larger by the smaller until one becomes zero. The GCD is then the non-zero number. For example, GCD(48, 18): 48 % 18 = 12 → GCD(18, 12) → 18 % 12 = 6 → GCD(12, 6) → 12 % 6 = 0 → return 6. Edge cases: if both are zero, return 0 (undefined but handled). If one is zero, return the other. Negative inputs are made positive via `std::abs`. The iterative loop runs in O(log(min(a,b))) time in the worst case (since the numbers reduce logarithmically) and O(1) auxiliary space. Testing with `assert` covers zero, equal numbers, primes, and negative values.

#include <cstdlib>  // for std::abs

// Computes the greatest common divisor of two non-negative integers using Euclid's algorithm.
// Handles negative inputs by taking absolute values. Returns 0 if both inputs are 0 (undefined).
int gcdUsingEuclidean(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);

    // GCD(0, b) = b, GCD(a, 0) = a, GCD(0, 0) = 0 (undefined but defined here)
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

#include <cassert>

int gcdUsingEuclidean(int, int); // forward declaration

int main() {
    // Basic cases
    assert(gcdUsingEuclidean(48, 18) == 6);
    assert(gcdUsingEuclidean(17, 13) == 1);  // primes
    assert(gcdUsingEuclidean(100, 10) == 10);
    assert(gcdUsingEuclidean(7, 0) == 7);
    assert(gcdUsingEuclidean(0, 5) == 5);
    assert(gcdUsingEuclidean(0, 0) == 0);  // undefined but defined as 0

    // Equal numbers
    assert(gcdUsingEuclidean(12, 12) == 12);

    // Negative inputs
    assert(gcdUsingEuclidean(-48, 18) == 6);
    assert(gcdUsingEuclidean(48, -18) == 6);
    assert(gcdUsingEuclidean(-48, -18) == 6);

    // Large numbers
    assert(gcdUsingEuclidean(123456, 7890) == 6); // 123456 = 2^6 * 3 * 643, 7890 = 2*3*5*263, common 2*3=6
    assert(gcdUsingEuclidean(1000000000, 1) == 1);

    return 0;
}
