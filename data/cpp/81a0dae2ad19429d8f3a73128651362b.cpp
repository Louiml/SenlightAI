// Write a C++ function named `minimumPresses` that takes an integer `targetChannel` (0 ≤ targetChannel ≤ 1,000,000), a vector of integers `brokenButtons` (each value is a digit 0-9, and the vector may be empty), and returns the minimum number of button presses needed to reach the target channel starting from channel 100. A remote control has number buttons 0-9, plus `+` and `-` buttons that increment/decrement the current channel by 1. If a digit button is broken, it cannot be pressed. The `+` and `-` buttons are always functional and can be used from any channel, including from 100 directly. The cost is 1 press per digit for direct channel entry, or 1 press per +/- operation. You may enter any channel directly (including one with broken digits only if all its digits are functional), then use +/- from there. Return the smallest total number of presses.

The problem is a classic brute-force search over possible channels. Since the target is limited to 1,000,000, the maximum useful channel to consider is 1,000,000 (because going beyond that never helps—the +/- distance increases, and entering more digits costs more). However, in some edge cases, it might be beneficial to go slightly above 1,000,000? No—since the +/- button can move from 100 directly, the worst-case is pressing +/- 1,000,000 times, which is already considered. Also, the largest direct channel we might need is 999,999 but we set the upper bound to 1,000,000 to include the target itself and nearby numbers. The algorithm: 
1. Compute `from100 = abs(targetChannel - 100)`.
2. For every possible channel `ch` from 0 to 1,000,000 inclusive, check if `ch` can be typed using only non-broken digits. If yes, compute `presses = digits(ch) + abs(ch - targetChannel)`. Keep the minimum.
3. The answer is `min(from100, minPresses)`.
Edge cases: target=100 (answer 0). All buttons broken (from100 only, because no digit can be typed). Target=0 (digit '0' might be broken; then must use +/-). The check function must handle number 0 specially because the while loop would skip it. Complexity: O(1,000,001 * number of digits) ≈ O(7 million) operations, which is trivial. Time complexity O(N * d) where N=1,000,001 and d≤7, so effectively O(N). Space O(1) excluding input.

#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// Returns the minimum number of button presses to reach targetChannel from 100.
// brokenButtons contains digits 0-9 that are non-functional.
int minimumPresses(int targetChannel, const std::vector<int>& brokenButtons) {
    // Mark broken digits as true.
    bool broken[10] = {false};
    for (int digit : brokenButtons) {
        broken[digit] = true;
    }

    // Check if a number can be typed directly using only functional digit buttons.
    auto isTypable = [&](int num) -> bool {
        if (num == 0) {
            return !broken[0];
        }
        while (num > 0) {
            if (broken[num % 10]) {
                return false;
            }
            num /= 10;
        }
        return true;
    };

    // Option 1: only use + and - from channel 100.
    int best = std::abs(targetChannel - 100);

    // Option 2: try every possible direct channel from 0 to 1,000,000.
    const int LIMIT = 1000000;
    for (int ch = 0; ch <= LIMIT; ++ch) {
        if (isTypable(ch)) {
            int presses = std::to_string(ch).size() + std::abs(ch - targetChannel);
            best = std::min(best, presses);
        }
    }

    return best;
}

#include <cassert>
#include <vector>

int minimumPresses(int, const std::vector<int>&); // declaration

int main() {
    // Target already at 100.
    assert(minimumPresses(100, {}) == 0);

    // No broken buttons: direct entry.
    assert(minimumPresses(123, {}) == 3);
    assert(minimumPresses(1000, {}) == 4);

    // Use +/- because digit 1 is broken.
    assert(minimumPresses(100, {1}) == 0); // from 100, no presses needed, even though 1 is broken.

    // All digits broken: must use +/- from 100.
    assert(minimumPresses(50, {0,1,2,3,4,5,6,7,8,9}) == 50);

    // Mixed: digit 5 is broken, target 500. Best is +/- from 100 (400 presses) vs enter 500 (impossible) but 499? 500 not typable, but 499 typable? 499 has 4,9,9 all functional? If broken is 5 only, 499 is typable. Then presses = 3 + |499-500| = 4. So answer 4.
    assert(minimumPresses(500, {5}) == 4);

    // Target 0, button 0 broken. Must use +/- from 100: 100 presses.
    assert(minimumPresses(0, {0}) == 100);

    // Target 999999, button 9 broken. Direct impossible, use 1000000? But 1000000 has digit 1 and zeros all functional, so enter 1000000 (7 presses) + 1 = 8. But also from 100, +999899 = 999899 presses. So answer 8.
    assert(minimumPresses(999999, {9}) == 8);

    // Target 1, button 1 broken, button 0 broken. Can't type 0 either. From 100 => 99 presses. Or type 2 (1 press) + |2-1| = 2. So answer 2.
    assert(minimumPresses(1, {0,1}) == 2);

    // Large target with no broken: direct digits count.
    assert(minimumPresses(1000000, {}) == 7);

    // Edge: broken button 0, target 10. 10 typable? digits 1 and 0 — 0 broken so no. Use 11: 2 presses +1 = 3. Or 9: 1 +1 =2. So answer 2.
    assert(minimumPresses(10, {0}) == 2);

    // Multiple broken, target near 100.
    assert(minimumPresses(101, {1,2,3}) == 1); // 101 not typable? 1 broken, so cannot. But 102? 2 broken. 103? 3 broken. Use 100? But 100 has 1 broken? Wait 1 broken, so 100 not typable. Use 99: 2 +2 =4. Or from 100: 1. So answer 1.

    return 0;
}
