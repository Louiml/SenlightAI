/*
Write a C++ function `int maximumGroups(int people)` that, given a non-negative integer `people` representing the number of people attending an event, returns the maximum number of complete groups of exactly 3 people that can be formed. Only complete groups of 3 are counted; any leftover people (1 or 2) are ignored. For example, with 7 people, two complete groups can be formed, and 1 person is left out. The function must handle `people = 0` (return 0) and large values up to at least 2,000,000,000 (fits in a 32‑bit signed integer).
*/
#include <cstdint>

// Return the maximum number of complete groups of 3 that can be formed.
// Non-negative integer input; leftover people are discarded.
int maximumGroups(int people) {
    // Integer division truncates toward zero, which is correct for non-negative inputs.
    return people / 3;
}
#include <cassert>

int maximumGroups(int people); // declaration for testing

int main() {
    assert(maximumGroups(0) == 0);
    assert(maximumGroups(1) == 0);
    assert(maximumGroups(2) == 0);
    assert(maximumGroups(3) == 1);
    assert(maximumGroups(4) == 1);
    assert(maximumGroups(5) == 1);
    assert(maximumGroups(6) == 2);
    assert(maximumGroups(7) == 2);
    assert(maximumGroups(8) == 2);
    assert(maximumGroups(2000000000) == 666666666);
}
// The problem reduces to integer division by 3 with truncation toward zero, which in C++ for non‑negative integers is simply `people / 3`. This directly counts how many full groups of 3 are possible. Edge cases include `people` being 0 (result 0), `people` being 1 or 2 (result 0 because no full group exists), and `people` being an exact multiple of 3 (result is `people/3` without a remainder). Since the input is non‑negative, no sign handling is needed. The solution runs in constant time (`O(1)`) and uses constant extra space (`O(1)`). The only possible pitfall is overflow when directly computing a multiplication, but we do not multiply; we only divide, and the division result fits within the range of the input, so it is safe for all valid inputs.
