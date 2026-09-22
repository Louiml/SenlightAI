// Write a standalone C++ function named `countEnumDays` that accepts an integer `startingValue`, and returns an `std::array<int, 7>` containing the integer values of the seven days of the week (`mon`, `tue`, `wed`, `thu`, `fri`, `sat`, `sun`) in that exact order, but with the `mon` value set to the provided `startingValue` and all subsequent day values computed using the same pattern as the original snippet: `tue = start + 2`, `wed = tue + 1`, `thu = wed + 1`, `fri = thu + 1`, `sat = 1` (fixed regardless of start), and `sun = start + 6`. The function must ensure the values are stored in a `const`-correct manner (return const array). The purpose is to demonstrate understanding of how enumeration values are derived when explicitly assigned and how they follow from the previous enumerator in C++.
#include <array>
#include <cassert>

int main() {
    std::array<int, 7> r0 = countEnumDays(0);
    assert(r0[0] == 0 && r0[1] == 2 && r0[2] == 3 && r0[3] == 4 && r0[4] == 5 && r0[5] == 1 && r0[6] == 6);

    std::array<int, 7> r5 = countEnumDays(5);
    assert(r5[0] == 5 && r5[1] == 7 && r5[2] == 8 && r5[3] == 9 && r5[4] == 10 && r5[5] == 1 && r5[6] == 11);

    std::array<int, 7> rNeg = countEnumDays(-3);
    assert(rNeg[0] == -3 && rNeg[1] == -1 && rNeg[2] == 0 && rNeg[3] == 1 && rNeg[4] == 2 && rNeg[5] == 1 && rNeg[6] == 3);
}
#include <array>

// Returns the seven day-of-week values with mon set to start,
// tue = start + 2, wed = tue + 1, thu = wed + 1, fri = thu + 1,
// sat = 1 (fixed), sun = start + 6.
const std::array<int, 7> countEnumDays(int start) {
    std::array<int, 7> days;
    days[0] = start;                    // mon
    days[1] = start + 2;                // tue
    days[2] = days[1] + 1;              // wed
    days[3] = days[2] + 1;              // thu
    days[4] = days[3] + 1;              // fri
    days[5] = 1;                        // sat (explicit)
    days[6] = start + 6;                // sun
    return days;
}
// The original snippet defines an enum: `enum day { mon, tue=3, wed, thu, fri, sat=1, sun=mon+6 };`. Values: `mon=0` (default), `tue=3` (explicit), `wed=4`, `thu=5`, `fri=6`, `sat=1` (explicit), `sun=6` (since `mon+6=6`). The task generalizes: we want `mon` to be a user-provided start. Then each subsequent enumerator without an explicit initializer automatically takes the previous enumerator's value plus 1, and enumerators with explicit initializers (like `sat`) ignore that rule. However, the pattern in the original is: `tue` is explicitly set to `3`, but in the generalized function we want to preserve the *relative offsets*: `tue = start + 2` (since in original, `mon=0` and `tue=3`, so offset +2), `wed` is `tue+1`, `thu` is `wed+1`, `fri` is `thu+1`, `sat` is fixed as 1 (independent of start), and `sun` is `start + 6` (original `mon+6`). The main algorithm is straightforward: build an `std::array<int,7>` and assign each element. The only edge case is when `start` is negative or large; the function still works as integers support any range. Also, there is a potential collision between the computed values (e.g., if start=1, `tue=3` and `sat=1` collide with `mon`), but that is allowed in C++ enums, and the function just returns the values, not enforcing uniqueness. Time complexity is O(1) and space is O(1) as we only return a fixed-size array.
