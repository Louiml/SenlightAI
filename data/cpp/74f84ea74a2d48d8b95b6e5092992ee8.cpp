Write a C++ function `int minimum_rounding_time(const std::array<int, 5>& delivery_times)` that takes exactly five positive integer delivery times (each between 1 and 1000) and returns the minimum total time needed to complete all deliveries, where you may choose one delivery to be "express" (its full time is counted exactly as is), while the other four deliveries are rounded **up** to the nearest multiple of 10 (e.g., 23 becomes 30, 10 stays 10, 5 becomes 10). The goal is to minimize the sum of the rounded times for four deliveries plus the exact time of the chosen express delivery. For example, if the times are `{29, 20, 7, 35, 120}`, the best choice is to make `7` express (since it has the smallest non-zero last digit), yielding rounded times for the others: 30, 20, 40, 120 → sum = 30+20+40+120+7 = 217. Note that if a time already ends in zero, it requires no rounding and is never a good choice for express. If all times end in zero, the answer is simply the sum of the original times (no rounding needed).

// The key insight is that rounding a time up to the next multiple of 10 adds `(10 - (time % 10)) % 10` minutes, except when the last digit is already 0, where the increase is 0. To minimize the total, we want to avoid the largest possible rounding penalty on one item. Since we can pick exactly one delivery to be unrounded, we should choose the one with the **largest** rounding penalty (i.e., the largest last digit among those that are non-zero, because a digit of 1 adds 9, 2 adds 8, ..., 9 adds 1; the highest penalty is 9 when last digit is 1). Wait: careful—the penalty is higher when the last digit is smaller (1 → +9, 9 → +1). The snippet in the prompt actually selects the item with the **smallest non-zero last digit**, because that is the one that would otherwise incur the **largest rounding increase**. So the algorithm: first compute the sum of all times rounded up to the next multiple of 10 (including those already multiples of 10, unchanged). Then find the smallest non-zero last digit among the five input times; let its original value be `min_digit_value` (the full original number, not just the digit). Subtract from the rounded sum the rounding penalty for that item: penalty = (10 - (min_digit_value % 10)) (which is between 1 and 9). The final answer is `rounded_sum - penalty`. Edge cases: if all last digits are 0, there is no rounding penalty, and the answer is the plain sum; but the formula also works because penalty would be 0 for any chosen item (but we treat "no non-zero digit" as no express benefit). Time complexity O(1), space O(1) since we only store constants and the array.

#include <array>
#include <algorithm>
#include <climits>

// Given five positive delivery times, returns the minimum total time
// where exactly one delivery is unrounded and the other four are rounded
// up to the nearest multiple of 10.
int minimum_rounding_time(const std::array<int, 5>& delivery_times) {
    int total_rounded = 0;
    int smallest_nonzero_digit = 10; // larger than any single digit
    int express_original_value = 0;

    for (int time : delivery_times) {
        int last_digit = time % 10;
        // Round up to next multiple of 10 (if already multiple, unchanged)
        int rounded = (last_digit == 0) ? time : (time / 10 * 10 + 10);
        total_rounded += rounded;

        // Track the smallest NON-ZERO last digit (because that incurs the largest penalty)
        if (last_digit != 0 && last_digit < smallest_nonzero_digit) {
            smallest_nonzero_digit = last_digit;
            express_original_value = time;
        }
    }

    // If no non-zero last digit exists, all times already multiples of 10, no express benefit.
    if (smallest_nonzero_digit == 10) {
        return total_rounded;
    }

    // The rounding penalty for the express item was added to total_rounded.
    // Subtract it to get the exact time for that item.
    int penalty = 10 - (express_original_value % 10);
    return total_rounded - penalty;
}

#include <cassert>
#include <array>

int main() {
    // Example from prompt
    std::array<int, 5> t1 = {29, 20, 7, 35, 120};
    assert(minimum_rounding_time(t1) == 217);

    // All multiples of 10: no rounding, no express benefit
    std::array<int, 5> t2 = {10, 20, 30, 40, 50};
    assert(minimum_rounding_time(t2) == 150);

    // One small non-zero digit (1) has highest penalty
    std::array<int, 5> t3 = {11, 20, 30, 40, 50};
    // Rounded sum: 20+20+30+40+50=160, penalty for 11 is 9 -> 151
    assert(minimum_rounding_time(t3) == 151);

    // Last digit 9 has small penalty (1), so rounding others is cheap
    std::array<int, 5> t4 = {19, 20, 30, 40, 50};
    // Rounded sum: 20+20+30+40+50=160, penalty for 19 is 1 -> 159
    assert(minimum_rounding_time(t4) == 159);

    // Mixed digits, choose the smallest non-zero (digit=2, penalty=8)
    std::array<int, 5> t5 = {22, 35, 41, 58, 63};
    // Rounded: 30,40,50,60,70 -> 250, smallest non-zero digit=1? Actually 22->2, 35->5, 41->1, 58->8, 63->3. Smallest is 1 (41). Penalty=9, answer=241
    assert(minimum_rounding_time(t5) == 241);

    // Duplicate smallest non-zero digit
    std::array<int, 5> t6 = {15, 25, 35, 45, 55};
    // Rounded: 20,30,40,50,60 -> 200. Smallest digit=5, penalty=5, answer=195
    assert(minimum_rounding_time(t6) == 195);

    // All same last digit
    std::array<int, 5> t7 = {101, 201, 301, 401, 501};
    // Rounded: 110,210,310,410,510 -> 1550, penalty for any = 9, answer=1541
    assert(minimum_rounding_time(t7) == 1541);

    // Edge case: minimum value 1
    std::array<int, 5> t8 = {1, 2, 3, 4, 5};
    // Rounded: 10,10,10,10,10 -> 50, smallest digit=1 (time=1), penalty=9 -> 41
    assert(minimum_rounding_time(t8) == 41);

    // Edge case: maximum value 1000 and others
    std::array<int, 5> t9 = {1000, 999, 998, 997, 996};
    // Rounded: 1000,1000,1000,1000,1000 -> 5000, smallest digit=6 (time=996), penalty=4 -> 4996
    assert(minimum_rounding_time(t9) == 4996);

    // All numbers end in zero except one with digit 9
    std::array<int, 5> t10 = {10, 20, 30, 40, 49};
    // Rounded: 10,20,30,40,50 -> 150, smallest digit=9 (time=49), penalty=1 -> 149
    assert(minimum_rounding_time(t10) == 149);

    return 0;
}
