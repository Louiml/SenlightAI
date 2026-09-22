Write a C++ function that determines whether a given positive integer weight can be split into two even positive integer parts, and returns `true` if such a split is possible, `false` otherwise. The function must adhere to the same logical rule as the reference snippet: a split is possible only when the weight is greater than 2 and even. For any other input (odd, 0, 1, or 2), the function must return `false`.
The problem reduces to a simple parity and size check. A weight `w` can be split into two positive even integers if and only if `w` is even and `w >= 4` (since the smallest positive even integer is 2, and `2 + 2 = 4`). The condition `w > 2` in the snippet effectively means `w >= 4` for integers, because even numbers greater than 2 start at 4. Therefore, the algorithm is: return `true` if `w % 2 == 0` and `w > 2`, otherwise return `false`. Edge cases: `w = 0` is not positive but still fails the `> 2` check; `w = 1` and `w = 2` are not even and/or not greater than 2; `w = 3` odd; `w = 4` is the first valid case. Time complexity is O(1) and space complexity is O(1).
#include <cstdint>

// Returns true if the given positive integer weight can be split
// into two positive even integer parts, false otherwise.
bool canSplitIntoTwoEvenParts(int weight) {
    // A split is possible if and only if the weight is even
    // and greater than 2 (i.e., at least 4, since 2+2=4 is the minimum).
    return (weight % 2 == 0) && (weight > 2);
}
#include <cassert>

// Forward declaration of the function under test.
bool canSplitIntoTwoEvenParts(int weight);

int main() {
    // Edge cases: 0, 1, 2, 3 are not splittable
    assert(canSplitIntoTwoEvenParts(0) == false);
    assert(canSplitIntoTwoEvenParts(1) == false);
    assert(canSplitIntoTwoEvenParts(2) == false);
    assert(canSplitIntoTwoEvenParts(3) == false);

    // First splittable case
    assert(canSplitIntoTwoEvenParts(4) == true);

    // Larger even numbers
    assert(canSplitIntoTwoEvenParts(6) == true);
    assert(canSplitIntoTwoEvenParts(10) == true);

    // Larger odd numbers
    assert(canSplitIntoTwoEvenParts(5) == false);
    assert(canSplitIntoTwoEvenParts(7) == false);

    // Negative even/odd (not positive in problem context, but check behavior)
    assert(canSplitIntoTwoEvenParts(-4) == false);
    assert(canSplitIntoTwoEvenParts(-3) == false);
}
