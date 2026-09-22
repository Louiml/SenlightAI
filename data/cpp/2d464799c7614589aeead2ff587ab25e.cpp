Write a C++ function named `reduceFraction` that takes two positive integers `numerator` and `denominator` and returns a `std::pair<int, int>` representing the fraction reduced to its lowest terms (i.e., with the greatest common divisor removed from both). The function must handle cases where the numerator or denominator is 1, where they are equal, and where one is a multiple of the other. Use an iterative division method (not the Euclidean algorithm) that tests potential common divisors starting from 2 upward, dividing both numbers by each common factor as found, and stopping when the test divisor exceeds the smaller of the current two values. The function should not modify the original inputs and must return the reduced pair. For example, `reduceFraction(100, 75)` returns `{4, 3}`, `reduceFraction(7, 9)` returns `{7, 9}`, and `reduceFraction(12, 4)` returns `{3, 1}`.

#include <cassert>
#include <utility>

// Declaration of the function under test.
std::pair<int, int> reduceFraction(int numerator, int denominator);

int main() {
    // Basic reduction
    assert(reduceFraction(100, 75) == std::make_pair(4, 3));
    // Already reduced
    assert(reduceFraction(7, 9) == std::make_pair(7, 9));
    // Denominator divides numerator
    assert(reduceFraction(12, 4) == std::make_pair(3, 1));
    // Numerator divides denominator
    assert(reduceFraction(6, 18) == std::make_pair(1, 3));
    // Equal numbers
    assert(reduceFraction(5, 5) == std::make_pair(1, 1));
    // One is 1
    assert(reduceFraction(1, 8) == std::make_pair(1, 8));
    assert(reduceFraction(8, 1) == std::make_pair(8, 1));
    // Product of common factors (2*2*3)
    assert(reduceFraction(24, 36) == std::make_pair(2, 3));
    // Large numbers with common factor 2
    assert(reduceFraction(1024, 256) == std::make_pair(4, 1));
    return 0;
}

#include <utility>  // for std::pair

// Reduce a fraction to lowest terms by repeatedly dividing by common divisors.
// Returns a pair {reduced_numerator, reduced_denominator}.
// The inputs must be positive integers.
std::pair<int, int> reduceFraction(int numerator, int denominator) {
    int base = numerator;
    int sub = denominator;
    int divisor = 2;

    // The divisor must not exceed the smaller of the two current numbers.
    int lowest = (base < sub) ? base : sub;

    while (divisor <= lowest) {
        if (base % divisor == 0 && sub % divisor == 0) {
            base /= divisor;
            sub /= divisor;
            // Recompute lowest because numbers may have shrunk.
            lowest = (base < sub) ? base : sub;
            // Do NOT increment divisor; the same factor may divide again.
        } else {
            ++divisor;
        }
    }

    return {base, sub};
}

// The core algorithm mirrors the provided snippet’s inner loop: start with a candidate divisor `mask = 2`. While `mask` is less than or equal to the current minimum of the numerator and denominator, check if both are divisible by `mask`. If yes, divide both by `mask` and keep the same `mask` (since the same factor might appear again, e.g., `8` and `12` share a factor `2` twice). If not divisible, increment `mask`. This approach repeatedly extracts common factors in increasing order. Edge cases: if one number is 1, the minimum is 1, so the while loop never runs (since `mask=2` > 1) and the pair is returned unchanged. If both are equal, the loop will reduce them to `{1,1}`. If one is a multiple of the other (e.g., `12` and `4`), the smaller divides the larger, so after dividing, the smaller becomes 1 and the loop exits. The loop condition uses the current values after division, so it correctly re-checks the reduced pair. Time complexity is O(min(n, d) * number_of_common_factors) in the worst case, but typically O(min(n,d)) because each mask that fails increments. In practice it’s linear in the smaller value for fixed-size integers. Space complexity is O(1) auxiliary.
