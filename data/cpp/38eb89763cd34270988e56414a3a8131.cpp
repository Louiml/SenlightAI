Write a C++ function named `swapTimes` that takes two `Time` struct references (where `Time` contains `int hours, mins, secs;`) and interchanges all corresponding fields of the two structs. The function must swap the `hours`, `mins`, and `secs` values between the two `Time` objects, so that after the call, the first struct holds the original values of the second, and the second holds the original values of the first. The function should not return anything, and it must handle any valid integer values, including negative values or values that may overflow when using arithmetic-based swapping. Use a safe approach (e.g., temporary variables or `std::swap` per field) rather than the addition/subtraction trick to avoid undefined behavior from integer overflow. The task is to implement only the function and not a full program; the caller will read input and print the results.

The problem is straightforward: given two `Time` structs, we need to exchange each corresponding field (`hours`, `mins`, `secs`). The main algorithm is to perform three separate swaps—one for each field—using a temporary variable (or `std::swap` from `<utility>`). The edge case is integer overflow: the original snippet uses addition/subtraction (e.g., `a = a + b; b = a - b; a = a - b;`), which is undefined behavior in C++ if the intermediate values exceed the `int` range. Since the input values are arbitrary integers, we must avoid that. Using a temporary variable guarantees correctness for any representable `int` value. Time complexity is O(1) since only a constant number of assignments is performed. Space complexity is O(1) auxiliary space (temporary variable per field, or one temporary `Time` if swapping whole structs). The solution is idempotent and works for negative values, duplicates, and extreme values like `INT_MIN` and `INT_MAX` because no arithmetic is done.

#include <utility>  // for std::swap

// Time structure to hold hours, minutes, seconds.
struct Time {
    int hours;
    int mins;
    int secs;
};

// Interchange all fields of two Time objects.
// Uses std::swap for each field to avoid overflow.
void swapTimes(Time& t1, Time& t2) {
    std::swap(t1.hours, t2.hours);
    std::swap(t1.mins, t2.mins);
    std::swap(t1.secs, t2.secs);
}

#include <cassert>
#include <climits>  // for INT_MIN, INT_MAX
#include "solution.h"  // or paste the swapTimes definition here

int main() {
    // Basic swap
    Time a{1, 2, 3};
    Time b{4, 5, 6};
    swapTimes(a, b);
    assert(a.hours == 4 && a.mins == 5 && a.secs == 6);
    assert(b.hours == 1 && b.mins == 2 && b.secs == 3);

    // Swapping equal values leaves unchanged
    Time c{10, 20, 30};
    Time d{10, 20, 30};
    swapTimes(c, d);
    assert(c.hours == 10 && c.mins == 20 && c.secs == 30);
    assert(d.hours == 10 && d.mins == 20 && d.secs == 30);

    // Negative values
    Time e{-5, -100, -8};
    Time f{7, -9, 11};
    swapTimes(e, f);
    assert(e.hours == 7 && e.mins == -9 && e.secs == 11);
    assert(f.hours == -5 && f.mins == -100 && f.secs == -8);

    // Extreme values to ensure no overflow
    Time g{INT_MIN, INT_MAX, -1};
    Time h{INT_MAX, INT_MIN, 0};
    swapTimes(g, h);
    assert(g.hours == INT_MAX && g.mins == INT_MIN && g.secs == 0);
    assert(h.hours == INT_MIN && h.mins == INT_MAX && h.secs == -1);

    // Swap with a zero-valued struct
    Time i{0, 0, 0};
    Time j{12, 59, 59};
    swapTimes(i, j);
    assert(i.hours == 12 && i.mins == 59 && i.secs == 59);
    assert(j.hours == 0 && j.mins == 0 && j.secs == 0);

    // Swap with negative zeros (just normal ints)
    Time k{-1, -1, -1};
    Time l{1, 1, 1};
    swapTimes(k, l);
    assert(k.hours == 1 && k.mins == 1 && k.secs == 1);
    assert(l.hours == -1 && l.mins == -1 && l.secs == -1);

    // Re-swapping returns to original
    swapTimes(k, l);
    assert(k.hours == -1 && k.mins == -1 && k.secs == -1);
    assert(l.hours == 1 && l.mins == 1 && l.secs == 1);

    // Swap large positive numbers
    Time m{123456789, 987654321, 111111111};
    Time n{999999999, 123456, 777};
    swapTimes(m, n);
    assert(m.hours == 999999999 && m.mins == 123456 && m.secs == 777);
    assert(n.hours == 123456789 && n.mins == 987654321 && n.secs == 111111111);

    return 0;
}
