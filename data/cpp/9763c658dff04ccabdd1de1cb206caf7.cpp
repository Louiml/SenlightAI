/*
Write a C++ function named `canDistribute` that takes two non-negative integers `a` and `b` representing the counts of two types of items. The function should return `true` if it is possible to distribute all items into groups such that each group contains exactly 3 items, with the total number of items (a + b) divisible by 3, and the size of the smaller pile is at least one-third of the total (i.e., `min(a, b) >= (a + b) / 3`). Otherwise, return `false`. The function must not read from or write to standard input/output; it should only compute and return the boolean result. Handle edge cases like zeros and very large values (up to 1e9) without overflow.
*/
#include <algorithm>

// Returns true if items of counts a and b can be distributed into groups of exactly 3 items
// such that each group contains at least one of each type.
bool canDistribute(int a, int b) {
    long long total = static_cast<long long>(a) + b; // avoid overflow
    if (total % 3 != 0) {
        return false;
    }
    long long minCount = std::min(a, b);
    long long maxGroups = total / 3;
    return minCount >= maxGroups;
}
#include <cassert>

int main() {
    // Basic valid cases
    assert(canDistribute(3, 3) == true);   // total 6, min 3 >= 2
    assert(canDistribute(1, 2) == true);   // total 3, min 1 >= 1
    assert(canDistribute(2, 4) == true);   // total 6, min 2 >= 2
    assert(canDistribute(0, 0) == true);   // total 0, min 0 >= 0

    // Invalid cases
    assert(canDistribute(0, 3) == false);  // total 3, min 0 < 1
    assert(canDistribute(2, 2) == false);  // total 4 not divisible by 3
    assert(canDistribute(1, 3) == false);  // total 4 not divisible by 3
    assert(canDistribute(5, 1) == false);  // total 6, min 1 < 2

    // Large values without overflow
    assert(canDistribute(1000000000, 1000000000) == true);  // total 2e9 divisible by 3? 2e9%3=2 → false
    assert(canDistribute(1000000000, 1000000000) == false);
    assert(canDistribute(999999999, 999999999) == true);    // total 1999999998 %3=0, min>=333333333

    return 0;
}
// This problem is a known Codeforces-style check. For a valid distribution, every group must have exactly 3 items. Since each group needs at least one of each type (otherwise you could have all groups of one type, but then the other type would remain unused), the maximum number of groups is limited by the smaller pile. Also, the total items must be divisible by 3, otherwise you cannot form whole groups. The condition `min(a,b) >= (a+b)/3` ensures that the smaller pile can supply at least one item to every group (since there are `(a+b)/3` groups). If both conditions hold, return true. Edge cases: (0,0) → total 0 divisible by 3, min=0 >=0 → true (empty distribution allowed). (0,3) → total 3, min=0 >=1? false. (1,2) → total 3, min=1 >=1 true. (2,2) → total 4 not divisible → false. Use integer division carefully. Time O(1), space O(1).
