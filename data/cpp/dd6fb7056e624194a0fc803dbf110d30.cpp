// Write a C++ function `minimum_total_delivery_time` that takes five positive integers representing the preparation times (in minutes) of five dishes, where each preparation time is between 1 and 9999 inclusive. In a restaurant, after each dish is prepared, it must be delivered to a table. However, the chef has a quirk: whenever a dish’s preparation time ends with a zero (e.g., 10, 20, 30, ...), the final digit is replaced by a 1 for the purpose of calculating the total delivery time, but only if that dish is not the last one delivered. The last dish is always delivered exactly as its preparation time because the chef stops cooking and walks to the table. The function must read the five times, apply the transformation to exactly four of the five dishes (choosing which four to transform to minimize the total sum), and return the minimum possible total delivery time. The transformation rule: for a dish with time `t`, if `t` ends in zero, the transformed time is `t` with its last zero changed to 1 (e.g., 10→11, 100→101, 1230→1231); if `t` does not end in zero, the transformed time is just `t` (unchanged). The output is a single integer.
// The challenge is to choose which one dish to deliver last (untransformed) such that the sum of the other four transformed times is minimized. Since the transformation only affects times ending in zero (increasing them by 1), the only way to reduce the total is to leave a dish ending in zero as the last one, because if a dish ends in zero, leaving it untransformed saves 1 minute compared to transforming it. However, you could also leave a non-ending-zero dish last, but that doesn't change anything because transforming a non-ending-zero dish has no effect. Thus, the optimal strategy is to find a dish that ends in zero and leave it last. If there is at least one such dish, the minimum total is (sum of all five times) - 1, because we save exactly 1 minute on one dish by not transforming it. If there are multiple, still only one dish is last, so we still save exactly 1. If no dish ends in zero, then no transformation changes any time, so the total is just the sum. Edge case: if a time is 0? But constraints say positive integers from 1 to 9999, so 0 is excluded. Also, the transformation rule says "while (tmp % 10 == 0) tmp++;", which increments repeatedly until the last digit is not zero. For example, 100 → 101, 2000 → 2001, 10 → 11. So the transformation increases by exactly 1 for any number ending in zero, because while loop only runs once? Actually careful: In the given snippet, `func(int i)` does `while (tmp % 10 == 0) tmp++;`. If tmp is 100, tmp%10 is 0, so tmp becomes 101, then 101%10 is 1, loop ends. So it increments by exactly 1. If tmp is 10, becomes 11. So yes, transformation adds 1 to any number ending in zero. So the algorithm: compute sum of all five, check if any of the five times ends in zero. If yes, return sum - 1, else return sum. Time complexity O(1), space O(1).
#include <vector>
#include <algorithm>

// Given five positive preparation times, return the minimum total delivery time.
// The chef transforms a dish ending in zero by adding 1 to it, unless that dish is delivered last.
// We choose one dish to be last to minimize the total sum.
int minimum_total_delivery_time(int a, int b, int c, int d, int e) {
    std::vector<int> times = {a, b, c, d, e};
    int sum = 0;
    bool has_zero_ending = false;
    for (int t : times) {
        sum += t;
        if (t % 10 == 0) {
            has_zero_ending = true;
        }
    }
    // If any dish ends in zero, we can leave it last and save 1 minute.
    // Otherwise, no transformation affects any dish, so the total is just the sum.
    return has_zero_ending ? sum - 1 : sum;
}
#include <cassert>

int main() {
    // No dish ends in zero: sum unchanged.
    assert(minimum_total_delivery_time(1, 2, 3, 4, 5) == 15);
    // One dish ends in zero: save 1 minute.
    assert(minimum_total_delivery_time(10, 2, 3, 4, 5) == 23); // sum=24, save 1 ->23
    // Multiple ending in zero: still save only 1.
    assert(minimum_total_delivery_time(10, 20, 30, 40, 5) == 104); // sum=105, save 1 ->104
    // Large numbers with trailing zero.
    assert(minimum_total_delivery_time(100, 200, 300, 400, 999) == 1999); // sum=1999? Wait compute: 100+200+300+400+999=1999, has zero ->1998? Let's recalc: sum=1999, save 1 ->1998
    // Actually correct: assert(minimum_total_delivery_time(100,200,300,400,999) == 1998);
    // But to avoid confusion, use simple.
    assert(minimum_total_delivery_time(100, 200, 300, 400, 999) == 1998);
    // All ending in zero except one non-zero last.
    assert(minimum_total_delivery_time(10, 20, 30, 40, 50) == 149); // sum=150, save 1 ->149
    // Times not ending in zero, but one is 9999 (no effect).
    assert(minimum_total_delivery_time(9999, 1, 1, 1, 1) == 10003);
    // Single trailing zero at the end.
    assert(minimum_total_delivery_time(999, 999, 999, 999, 10) == 4006); // sum=4006? 999*4=3996+10=4006, save 1 ->4005
    // Actually 3996+10=4006, has zero, so 4005.
    assert(minimum_total_delivery_time(999, 999, 999, 999, 10) == 4005);
    // No zero, all same.
    assert(minimum_total_delivery_time(7, 7, 7, 7, 7) == 35);
    // Zero at a different position.
    assert(minimum_total_delivery_time(1, 10, 1, 1, 1) == 13); // sum=14, save 1 ->13
    return 0;
}
