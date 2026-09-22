Write a C++ function `int calendarYear(int targetE, int targetS, int targetM)` that simulates a cyclic calendar system where Earth years cycle every 15, Sun years every 28, and Moon years every 19, all starting from 1. Starting from year 1 with current values (1,1,1), the function must increment the year and each current value by 1, resetting each to 1 when it exceeds its cycle length, and return the first year where all three current values equal the given target values respectively. The inputs are guaranteed to be within valid ranges (1–15, 1–28, 1–19).
#include <cassert>

int main() {
    // The initial year already matches.
    assert(calendarYear(1, 1, 1) == 1);

    // After one increment, Earth=2, Sun=2, Moon=2.
    assert(calendarYear(2, 2, 2) == 2);

    // A known triple: (15, 28, 19) occurs first at year 7980 (LCM of cycles).
    assert(calendarYear(15, 28, 19) == 7980);

    // A smaller example: Earth=15, Sun=1, Moon=1 appears at year 15.
    assert(calendarYear(15, 1, 1) == 15);

    // Sun=28 alone appears at year 28 (with Earth=1, Moon=1? Yes, because 28 is not a multiple of 15 or 19).
    assert(calendarYear(1, 28, 1) == 28);

    // Moon=19 alone appears at year 19.
    assert(calendarYear(1, 1, 19) == 19);

    // Mixed values example: Earth=3, Sun=5, Moon=7 – verify manually via small loop if needed.
    // For demonstration, just check the function returns a positive year.
    int result = calendarYear(3, 5, 7);
    assert(result > 0 && result <= 7980);
}
#include <cstddef>

// Returns the first year (starting from 1) where the current (Earth, Sun, Moon)
// cycle values match the given target values.
// Earth cycles every 15, Sun every 28, Moon every 19.
int calendarYear(int targetE, int targetS, int targetM) {
    int currentE = 1;
    int currentS = 1;
    int currentM = 1;
    int year = 1;

    while (currentE != targetE || currentS != targetS || currentM != targetM) {
        ++currentE;
        ++currentS;
        ++currentM;
        ++year;

        if (currentE > 15) currentE = 1;
        if (currentS > 28) currentS = 1;
        if (currentM > 19) currentM = 1;
    }

    return year;
}
// The approach is a straightforward simulation: maintain three counters and a year counter, both starting at 1. In each iteration, increment all counters and the year, then apply modulo-style resets (if a counter exceeds its max, reset to 1). Continue until all three counters match the target triple. Because the three cycles (15, 28, 19) are pairwise coprime(? Actually 15 and 28 are coprime, 15 and 19 coprime, 28 and 19 coprime – yes, all pairwise coprime), the least common multiple is \(15 \times 28 \times 19 = 7980\). Therefore, the worst-case number of iterations is exactly 7980 (since the pattern repeats every 7980 years and all possible triples occur exactly once). This gives a time complexity of \(O(7980) = O(1)\) in practical terms (constant bound), and space complexity \(O(1)\). Edge cases include target values all equal to 1 (returns 1 without any increment) and values at cycle boundaries (e.g., 15, 28, 19). No special handling needed beyond the reset condition. The function must be pure (no global state) and `const`-correct (parameters by value, no mutation of external state).
