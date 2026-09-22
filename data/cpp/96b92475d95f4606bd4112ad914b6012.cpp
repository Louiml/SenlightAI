// Write a C++ function `vector<int> incrementLargeInteger(vector<int>& digits)` that takes a non-empty vector of single decimal digits representing a non-negative integer (most significant digit first, no leading zeros except for the number 0 itself) and returns a new vector representing the result of adding 1 to that integer. The function must modify the input vector in place and return a reference to it (i.e., return `vector<int>&`), preserving the same digit‑per‑element format. Handle carries correctly, including the special case where adding 1 causes the number length to increase (e.g., `[9,9,9]` → `[1,0,0,0]`). You may assume the input is always valid (non‑empty, each element 0–9, no leading zeros unless the entire number is 0). Do not convert the vector to a built‑in integer type (e.g., `int` or `long long`), as the number may be arbitrarily large.
// The solution processes the digits from the least significant (rightmost) to the most significant (leftmost), simulating the manual addition of 1. Start by adding 1 to the last digit. If that digit becomes 10, set it to 0 (i.e., subtract 10) and carry 1 to the next digit to the left; continue this carry propagation while the current digit was 9. If a carry survives past the most significant digit (i.e., all digits were 9), insert a `1` at the beginning of the vector and set the rest to 0 (which already happened via the loop). Key edge cases: a single digit `9` becomes `[1,0]`; the number `0` becomes `[1]`; no carry when the last digit is <9. The algorithm runs in O(n) time in the worst case (all 9s) and O(1) auxiliary space, since only a possible insertion at the front requires shifting elements (which is still O(n) time). The in‑place modification avoids extra copies.
#include <vector>

// Adds 1 to a non-negative integer represented as a vector of decimal digits.
// The vector is modified in place and returned by reference.
std::vector<int>& incrementLargeInteger(std::vector<int>& digits) {
    int i = static_cast<int>(digits.size()) - 1;

    // Add 1 to the least significant digit.
    digits[i] += 1;

    // Propagate carry while the digit exceeds 9.
    while (i >= 0 && digits[i] > 9) {
        digits[i] -= 10;
        --i;
        if (i >= 0) {
            digits[i] += 1;
        }
    }

    // If carry overflowed past the most significant digit, prepend a 1.
    if (digits.front() == 0) {
        digits.insert(digits.begin(), 1);
    }

    return digits;
}
#include <cassert>
#include <vector>

// The solution function is expected to be defined above.
std::vector<int>& incrementLargeInteger(std::vector<int>& digits);

int main() {
    // Basic increment without carry.
    std::vector<int> a = {1, 2, 3};
    assert(incrementLargeInteger(a) == std::vector<int>({1, 2, 4}));

    // Increment with carry in the middle.
    std::vector<int> b = {1, 9, 9};
    assert(incrementLargeInteger(b) == std::vector<int>({2, 0, 0}));

    // Increment with carry causing length increase.
    std::vector<int> c = {9, 9, 9};
    assert(incrementLargeInteger(c) == std::vector<int>({1, 0, 0, 0}));

    // Increment of zero.
    std::vector<int> d = {0};
    assert(incrementLargeInteger(d) == std::vector<int>({1}));

    // Increment of a single non‑nine digit.
    std::vector<int> e = {5};
    assert(incrementLargeInteger(e) == std::vector<int>({6}));

    // Increment of a single nine.
    std::vector<int> f = {9};
    assert(incrementLargeInteger(f) == std::vector<int>({1, 0}));

    // Large number with carries in multiple positions.
    std::vector<int> g = {8, 9, 9, 9};
    assert(incrementLargeInteger(g) == std::vector<int>({9, 0, 0, 0}));

    // Already a power of ten minus one.
    std::vector<int> h = {1, 0, 0, 0};
    assert(incrementLargeInteger(h) == std::vector<int>({1, 0, 0, 1}));

    // Edge case: all zeros except first zero (valid for 0 only).
    std::vector<int> i = {0, 0}; // Not valid per spec, but test robustness.
    // The spec says non-empty and no leading zeros except for 0 itself,
    // so {0,0} is invalid. Skip invalid inputs.

    return 0;
}
