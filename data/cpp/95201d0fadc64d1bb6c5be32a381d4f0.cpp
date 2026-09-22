Write a C++ function named `thirdMaximum` that takes a non-empty `std::vector<int>` (passed by `const` reference) and returns the third distinct maximum number in the array. If the third distinct maximum does not exist (i.e., there are fewer than three distinct values), return the maximum value. The function must handle negative numbers, duplicate values, and values that may be as low as `INT_MIN`. The result should be returned as an `int`.

The algorithm maintains the top three distinct maximum values seen so far using three `long` variables (`max1`, `max2`, `max3`) initialized to `LONG_MIN` to safely handle edge cases where actual values equal `INT_MIN`. Iterate through each number once, applying a cascading update: if the current number exceeds `max1`, shift `max1` to `max2`, `max2` to `max3`, and set `max1` to the number. Otherwise, if the number is strictly between `max1` and `max2`, shift `max2` to `max3` and set `max2`; similarly, if strictly between `max2` and `max3`, set `max3`. Duplicates are naturally ignored because conditions require strict inequality. After the loop, if `max3` still equals `LONG_MIN`, it means fewer than three distinct values exist, so return `max1` (the overall maximum). Otherwise, return `max3`. Time complexity is \(O(n)\) with a single pass, and space complexity is \(O(1)\) extra space.

#include <vector>
#include <climits>

// Return the third distinct maximum value, or the maximum if it doesn't exist.
int thirdMaximum(const std::vector<int>& nums) {
    long max1 = LONG_MIN;
    long max2 = LONG_MIN;
    long max3 = LONG_MIN;

    for (const int num : nums) {
        if (num > max1) {
            max3 = max2;
            max2 = max1;
            max1 = num;
        } else if (max1 > num && num > max2) {
            max3 = max2;
            max2 = num;
        } else if (max2 > num && num > max3) {
            max3 = num;
        }
    }

    // If max3 was never updated, there are fewer than 3 distinct numbers.
    return (max3 == LONG_MIN) ? static_cast<int>(max1) : static_cast<int>(max3);
}

#include <cassert>
#include <vector>

int main() {
    assert(thirdMaximum({3, 2, 1}) == 1);
    assert(thirdMaximum({1, 2}) == 2);
    assert(thirdMaximum({2, 2, 3, 1}) == 1);
    assert(thirdMaximum({1, 1, 1}) == 1);
    assert(thirdMaximum({5, 5, 4, 4, 3, 3}) == 3);
    assert(thirdMaximum({-10, -20, -30}) == -30);
    assert(thirdMaximum({-2147483648, 1, 2}) == -2147483648);
    assert(thirdMaximum({0, -1, 1}) == -1);
    assert(thirdMaximum({2, 1}) == 2);
    assert(thirdMaximum({1, 2, 2, 5, 3, 5}) == 2);
    return 0;
}
