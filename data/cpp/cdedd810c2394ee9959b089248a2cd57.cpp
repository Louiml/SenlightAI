/*
Given a vector `light` where `light[i]` is the position (1-indexed) of a bulb that is turned on at minute `i+1` (i.e., the first element corresponds to minute 1), write a C++ function `int numTimesAllBlue(const std::vector<int>& light)` that returns the number of minutes at which all currently turned‑on bulbs are blue. Initially all bulbs are off and uncolored. When a bulb is turned on, if it is the leftmost uncolored bulb (i.e., all bulbs to its left are already blue) then it becomes blue and any consecutive already‑on bulbs immediately to its right also become blue, otherwise it remains uncolored (yellow). At each minute, a “moment” is counted if **every** bulb that has been turned on up to that minute is blue. The vector size `n` equals the number of minutes, and bulb positions range from `1` to `n`. The function must be efficient for `n` up to 10^5.
*/

#include <vector>
#include <algorithm>

// Returns the number of minutes when all turned-on bulbs are blue.
// light[i] is the 1-indexed bulb turned on at minute i+1.
int numTimesAllBlue(const std::vector<int>& light) {
    int count = 0;
    int max_on = 0;  // largest bulb index turned on so far
    const int n = static_cast<int>(light.size());

    for (int minute = 1; minute <= n; ++minute) {
        // Update max_on with the bulb turned on this minute (1-indexed)
        max_on = std::max(max_on, light[minute - 1]);
        // If all turned-on bulbs are exactly the prefix 1..minute,
        // then max_on must equal minute. In that case they are all blue.
        if (max_on == minute) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Assume the function declaration is above.
int main() {
    // Example from typical LeetCode problem: [2,1,3,5,4] -> 3
    std::vector<int> v1 = {2, 1, 3, 5, 4};
    assert(numTimesAllBlue(v1) == 3);

    // Already sorted: every minute is a blue moment
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    assert(numTimesAllBlue(v2) == 5);

    // Reverse order: only the last minute
    std::vector<int> v3 = {5, 4, 3, 2, 1};
    assert(numTimesAllBlue(v3) == 1);

    // Single bulb
    std::vector<int> v4 = {1};
    assert(numTimesAllBlue(v4) == 1);

    // Two bulbs, order [2,1] -> only second minute
    std::vector<int> v5 = {2, 1};
    assert(numTimesAllBlue(v5) == 1);

    // Two bulbs, order [1,2] -> both minutes
    std::vector<int> v6 = {1, 2};
    assert(numTimesAllBlue(v6) == 2);

    // Larger random permutation: n=6, [3,2,4,1,6,5] -> count manually:
    // minute1: max=3 !=1 -> no
    // minute2: max=3 !=2 -> no
    // minute3: max=4 !=3 -> no
    // minute4: max=4 ==4 -> yes (1)
    // minute5: max=6 !=5 -> no
    // minute6: max=6 ==6 -> yes (2)
    std::vector<int> v7 = {3, 2, 4, 1, 6, 5};
    assert(numTimesAllBlue(v7) == 2);

    // All same? Not possible because positions are distinct 1..n.
    // But test a case with n=1 already done.

    // Edge: n=2, [1,2] tested. n=3, [2,3,1] -> 
    // minute1: max=2 !=1 -> no
    // minute2: max=3 !=2 -> no
    // minute3: max=3 ==3 -> yes (1)
    std::vector<int> v8 = {2, 3, 1};
    assert(numTimesAllBlue(v8) == 1);

    return 0;
}

// The problem is essentially tracking a prefix of consecutively turned‑on bulbs that must all be blue. The condition for a “moment” at minute `k` (1‑indexed) is that the maximum bulb index turned on so far equals `k`, because then all positions `1..k` have been turned on, and since each new bulb either extends the blue prefix or remains yellow, the only way all `k` are blue is if they form a contiguous prefix. To verify this, we can maintain a boolean array `state` where `state[i]` is `true` if bulb `i` is blue, `false` if yellow/off. But a simpler observation: at minute `k`, after turning on `light[k-1]`, the number of turned‑on bulbs is exactly `k`. If `maxOnSoFar == k`, then the turned‑on set is exactly `{1,2,...,k}`. Because bulbs are turned on in some order, the only way that all `k` bulbs are blue is if the prefix `1..k` is fully turned on and the newest bulb completes this prefix. In the original snippet, the algorithm marks bulbs as blue when the left neighbor is already blue (state `-1`), extending the blue run; but a direct simplification is to just track `maxOn` and count when `maxOn == minute`. Edge cases: single bulb (always 1), lights already sorted (all moments), reverse order (only last minute). Time complexity O(n) with O(1) auxiliary space. Space O(1) if we avoid storing states.
