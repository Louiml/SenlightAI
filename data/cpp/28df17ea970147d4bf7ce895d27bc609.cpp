Write a C++ function that simulates a simple alarm clock timing scenario. The function takes four integer arguments: `h1`, `m1`, `h2`, `m2`, and `k`, where the first time is given in hours `h1` and minutes `m1`, and the second time is given in hours `h2` and minutes `m2`. Both times are on the same day and use a 24-hour clock format (0 <= h < 24, 0 <= m < 60). The function should return the number of minutes from the first time to the second time after shifting the second time earlier by `k` minutes. If the result is negative, return 0. For example, if the first time is 10:00, the second is 12:30, and `k` is 60, then the second time becomes 11:30, and the result is 90 minutes. If the second time after shifting is before the first time, the result is 0.
// The solution converts both given times to total minutes from midnight by computing `total1 = h1 * 60 + m1` and `total2 = h2 * 60 + m2`. Then subtract `k` from `total2` to represent the earlier shifted time. The difference between the shifted second time and the first time is `total2 - k - total1`. Since the problem requires returning 0 if this difference is negative (i.e., the shifted second time is earlier than or equal to the first time), we use `max(0, difference)`. Edge cases include when `k` is larger than the second time's total minutes (which could even make `total2 - k` negative), when both times are equal or very close, and when the difference is exactly 0. The time complexity is O(1) constant time, and space complexity is O(1) constant auxiliary space.
#include <algorithm>

// Return the number of minutes from time1 (h1:m1) to time2 (h2:m2)
// after shifting time2 earlier by k minutes, with a minimum result of 0.
long long minutesBetween(long long h1, long long m1, long long h2, long long m2, long long k) {
    const long long total1 = h1 * 60 + m1;
    const long long total2 = h2 * 60 + m2;
    const long long shiftedTotal2 = total2 - k;
    const long long difference = shiftedTotal2 - total1;
    return std::max(0LL, difference);
}
#include <cassert>
#include <iostream>

// Forward declaration or include the solution here
long long minutesBetween(long long h1, long long m1, long long h2, long long m2, long long k);

int main() {
    // Basic example: 10:00 -> 12:30 minus 60 min = 11:30, difference = 90
    assert(minutesBetween(10, 0, 12, 30, 60) == 90);

    // Exact equality after shift
    assert(minutesBetween(9, 0, 10, 0, 60) == 0);

    // Negative difference returns 0
    assert(minutesBetween(12, 0, 12, 30, 60) == 0);

    // Shift makes second time before first completely
    assert(minutesBetween(8, 0, 9, 0, 120) == 0);

    // Zero-minute shift
    assert(minutesBetween(6, 15, 6, 15, 0) == 0);

    // Large k that goes negative in total minutes
    assert(minutesBetween(0, 0, 1, 0, 300) == 0);

    // Normal positive difference with minutes
    assert(minutesBetween(23, 50, 23, 55, 2) == 3);

    // Exactly one hour difference after 10-minute shift
    assert(minutesBetween(1, 0, 2, 10, 10) == 60);

    // Shift by zero, different times
    assert(minutesBetween(14, 20, 14, 21, 0) == 1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
