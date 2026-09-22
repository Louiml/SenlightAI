// Write a C++ function named `addTimes` that takes two `Time` objects (each representing a time of day with integer hours, minutes, and seconds, where hours are in [0,23], minutes and seconds are in [0,59]) and returns a new `Time` object representing the sum of the two times, normalized to a 24-hour clock. The addition must carry overflow minutes into hours and overflow seconds into minutes, and the resulting hours must wrap modulo 24. For example, adding 20:40:40 and 20:40:40 should yield 17:21:20 (since 40+40=80 seconds → 1 minute and 20 seconds; 40+40+1=81 minutes → 1 hour and 21 minutes; 20+20+1=41 hours → 17 hours after modulo 24). The function must be `const`‑correct (the two input parameters should be passed by `const` reference) and must not modify its inputs. The `Time` class must have a public constructor `Time(int h, int m, int s)` and a public method `void display()` that prints the time in the format `H : M : S` (with spaces around the colons as shown). The function should be self-contained (include necessary headers) and placed in a header/implementation style suitable for separate compilation, but for this task just provide the function alone without `main`.
// The solution involves a straightforward arithmetic summation with carry propagation. For two times, we first compute the total seconds as `sec1 + sec2`, then the carry into minutes is the integer division of that sum by 60, and the resulting seconds are the remainder modulo 60. Next, we compute the total minutes as `min1 + min2 + carrySeconds`, where `carrySeconds` is the floor division from the seconds step. The carry into hours is the integer division of this minutes sum by 60, and the resulting minutes are modulo 60. Finally, we compute total hours as `hr1 + hr2 + carryMinutes`, then wrap modulo 24 using the modulus operator, being careful with negative numbers (though inputs are guaranteed non-negative, so no issue). All arithmetic uses integer types. The time complexity is O(1) (constant time, only a few arithmetic operations). Space complexity is O(1) auxiliary, plus the returned object. Edge cases include: when sums are exactly multiples of 60 or 24 (e.g., adding 23:59:59 and 0:0:1 yields 0:0:0); when both are zero; and when carry values are zero. The display format must match exactly `"H : M : S"` with a single space around each colon.
#include <iostream>

class Time {
public:
    int hr;
    int min;
    int sec;

    Time(int h, int m, int s) : hr(h), min(m), sec(s) {}

    void display() const {
        std::cout << hr << " : " << min << " : " << sec;
    }
};

// Add two times and return a new Time normalized to 24-hour clock.
Time addTimes(const Time& t1, const Time& t2) {
    int totalSec = t1.sec + t2.sec;
    int carryMin = totalSec / 60;
    int newSec = totalSec % 60;

    int totalMin = t1.min + t2.min + carryMin;
    int carryHr = totalMin / 60;
    int newMin = totalMin % 60;

    int totalHr = t1.hr + t2.hr + carryHr;
    int newHr = totalHr % 24;

    return Time(newHr, newMin, newSec);
}
#include <cassert>
#include <sstream>

// Helper to capture display output as string for testing
std::string timeToString(const Time& t) {
    std::ostringstream oss;
    t.display();
    return oss.str();
}

int main() {
    // Basic test from the snippet
    Time t1(20, 40, 40);
    Time t2(20, 40, 40);
    Time result = addTimes(t1, t2);
    assert(timeToString(result) == "17 : 21 : 20");

    // Exact rollover to next day
    Time a(23, 59, 59);
    Time b(0, 0, 1);
    result = addTimes(a, b);
    assert(timeToString(result) == "0 : 0 : 0");

    // No carry
    Time c(1, 2, 3);
    Time d(4, 5, 6);
    result = addTimes(c, d);
    assert(timeToString(result) == "5 : 7 : 9");

    // Carry seconds only
    Time e(0, 0, 59);
    Time f(0, 0, 2);
    result = addTimes(e, f);
    assert(timeToString(result) == "0 : 1 : 1");

    // Carry minutes only
    Time g(0, 59, 30);
    Time h(0, 1, 30);
    result = addTimes(g, h);
    assert(timeToString(result) == "1 : 1 : 0");

    // Both inputs zero
    Time z1(0, 0, 0);
    Time z2(0, 0, 0);
    result = addTimes(z1, z2);
    assert(timeToString(result) == "0 : 0 : 0");

    return 0;
}
