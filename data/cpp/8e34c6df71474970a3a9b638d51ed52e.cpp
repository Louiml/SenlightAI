// Given a vector of integers, write a C++ function `isBidiMixed` that returns true if the sequence contains at least one left-to-right character and at least one right-to-left character, treating all nonzero integers as either LTR (if positive) or RTL (if negative), and zero as neutral. The function should iterate through the input once and track whether any positive and any negative values were seen, returning true only if both categories appear. Empty input or input with only neutrals should return false.
#include <cassert>
#include <vector>

// Declaration of the function under test (assumes it is in the same translation unit).
bool isBidiMixed(const std::vector<int>& input);

int main() {
    // Basic mixed case
    assert(isBidiMixed({1, -1}) == true);
    assert(isBidiMixed({-1, 1}) == true);

    // Single direction only
    assert(isBidiMixed({1, 2, 3}) == false);
    assert(isBidiMixed({-1, -2, -3}) == false);

    // All neutrals
    assert(isBidiMixed({0, 0, 0}) == false);

    // Empty vector
    assert(isBidiMixed({}) == false);

    // Single element
    assert(isBidiMixed({1}) == false);
    assert(isBidiMixed({-1}) == false);

    // Mixed with neutrals interspersed
    assert(isBidiMixed({0, 1, 0, -2, 0}) == true);

    // Larger mixed cases
    assert(isBidiMixed({10, 20, -30, 40}) == true);
    assert(isBidiMixed({-5, -10, 15}) == true);

    // Only one direction plus neutrals
    assert(isBidiMixed({1, 0, 2, 0}) == false);
    assert(isBidiMixed({-1, 0, -2}) == false);
}
#include <vector>

/**
 * Returns true if the input contains at least one positive (LTR) and at least one negative (RTL) value.
 * Zero values are treated as neutral and ignored.
 */
bool isBidiMixed(const std::vector<int>& input) {
    bool hasLTR = false;
    bool hasRTL = false;

    for (int value : input) {
        if (value > 0) {
            hasLTR = true;
        } else if (value < 0) {
            hasRTL = true;
        }
        // Early exit if both have been found.
        if (hasLTR && hasRTL) {
            return true;
        }
    }

    return hasLTR && hasRTL;
}
// The solution is a straightforward single-pass scan. We initialize two boolean flags, `hasLTR` and `hasRTL`, to false. For each integer in the vector, if the value is greater than zero we set `hasLTR = true`; if it is less than zero we set `hasRTL = true`. After scanning all elements, we return `hasLTR && hasRTL`. This directly mimics the BiDi concept of mixed-directionality: a paragraph is mixed if it contains both LTR and RTL strong types, otherwise it is single-directional. Edge cases include empty vectors (return false), vectors with only zeros (return false), and vectors containing a single nonzero value (return false because both flags are never set). Time complexity is \(O(n)\) for \(n\) elements, and auxiliary space is \(O(1)\).
