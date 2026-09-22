// Write a C++ function `minimumTaxis(int n, const std::vector<int>& groups)` that takes the number of groups `n` and a vector of group sizes (each group size is an integer from 1 to 4) and returns the minimum number of taxis needed to transport all groups. Each taxi can carry at most 4 passengers. Groups must not be split: a group of size 4 needs its own taxi, a group of size 3 can share only with a group of size 1, and groups of size 2 can pair with other size-2 groups or be accommodated with leftover space for up to two size-1 groups. The function should compute and return this minimal number of taxis as an `int`.

#include <cassert>
#include <vector>

// Function under test is declared above; this is the test main.
int main() {
    // Basic mixed groups
    assert(minimumTaxis(5, {1, 2, 4, 3, 3}) == 4); // 4 alone, 3+1, 3 alone, 2 alone => 1+1+1+1=4
    // All size-1 groups
    assert(minimumTaxis(4, {1, 1, 1, 1}) == 1);
    assert(minimumTaxis(5, {1, 1, 1, 1, 1}) == 2);
    // All size-2 groups
    assert(minimumTaxis(4, {2, 2, 2, 2}) == 2); // two pairs
    assert(minimumTaxis(2, {2, 2}) == 1);
    // Size-3 with size-1
    assert(minimumTaxis(2, {3, 1}) == 1);
    assert(minimumTaxis(3, {3, 1, 1}) == 2); // 3+1, 1 alone
    // Only size-4
    assert(minimumTaxis(3, {4, 4, 4}) == 3);
    // Edge: single group
    assert(minimumTaxis(1, {4}) == 1);
    assert(minimumTaxis(1, {1}) == 1);
    // Edge: empty input
    assert(minimumTaxis(0, {}) == 0);
    // Complex case: 3 twos and 2 ones
    assert(minimumTaxis(5, {2, 2, 2, 1, 1}) == 3); // (2+2), (2+1+1), no more
    return 0;
}

#include <vector>
#include <algorithm>

// Compute the minimum number of taxis needed to transport all groups.
// Each taxi carries at most 4 passengers; groups cannot be split.
// groups[i] is the size of the i-th group (1 to 4).
int minimumTaxis(int n, const std::vector<int>& groups) {
    // Count groups by size (indices 1..4; index 0 unused)
    int check[5] = {0};
    for (int i = 0; i < n; ++i) {
        check[groups[i]]++;
    }

    int result = 0;

    // Each group of size 4 needs its own taxi
    result += check[4];

    // Pair size-3 groups with size-1 groups
    int t = std::min(check[3], check[1]);
    result += t;
    check[3] -= t;
    check[1] -= t;

    // Remaining size-3 groups need individual taxis
    result += check[3];

    // Pair size-2 groups together
    result += check[2] / 2;
    check[2] %= 2;

    // One remaining size-2 group can share with up to two size-1 groups
    if (check[2] > 0) {
        result += 1;
        check[1] = std::max(0, check[1] - 2);
    }

    // Remaining size-1 groups: each taxi holds up to 4
    if (check[1] > 0) {
        result += check[1] / 4;
        if (check[1] % 4 != 0) {
            result += 1;
        }
    }

    return result;
}

// The problem is a classic greedy resource-allocation task. Since groups cannot be split, we process them by size:
// - Each group of size 4 requires exactly 1 taxi: add `check[4]` to the answer.
// - Each group of size 3 can pair with at most one group of size 1 (since 3+1=4). Pair as many as possible: `t = min(check[3], check[1])`, add `t` to answer, subtract `t` from both counters. Remaining size-3 groups must each take a separate taxi: add `check[3]` to answer.
// - Groups of size 2: two size-2 groups can share a taxi (2+2=4). Add `check[2] / 2` to answer, keep `check[2] %= 2`. If one size-2 group remains, it can share with up to two size-1 groups (2+1+1=4). So add 1 to answer and subtract 2 from `check[1]` (if `check[1]` becomes negative, cap it at 0 conceptually; we only care about remaining size-1 groups).
// - Finally, remaining size-1 groups: each taxi can hold up to 4 of them. Add `check[1] / 4`, and if there is any remainder (`check[1] % 4 != 0`), add one more taxi.
// Edge cases: if there are no groups, return 0. If `check[1]` becomes negative in the size-2 step (meaning not enough size-1 groups to fill the remaining size-2 taxi), treat it as 0 for subsequent calculations. Time complexity is O(n) for counting, plus O(1) for the greedy steps, so overall O(n) time and O(1) auxiliary space (excluding input storage). The solution is robust for any `n >= 0`.
