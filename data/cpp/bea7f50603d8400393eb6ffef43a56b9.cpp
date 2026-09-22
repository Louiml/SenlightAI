// Given a positive integer `n`, write a C++ function named `countWorkingMinutes` that simulates a simplified bank over exactly `n` full weeks (7 days each), starting on day 1 (Monday) at time 0 minutes (9:00 AM). Each day has a maximum open minutes: Sunday (day 7) is closed (0 minutes), Saturday (day 6) is open for 360 minutes, and all other weekdays are open for 480 minutes. At the start of each day, time resets to 0. The function should return the total number of “working minutes” across those `n` weeks, meaning the sum over all days of the number of minutes the bank is open (i.e., the maxTime for that day). You may assume `n >= 1`. The function does not need to model customers or queues—only the daily schedule and cumulative count.
// The problem reduces to computing, for each day in an `n`-week cycle, the number of open minutes based on the day of the week. Since the week repeats every 7 days, the pattern is: Monday (day 1) = 480, Tuesday (2) = 480, Wednesday (3) = 480, Thursday (4) = 480, Friday (5) = 480, Saturday (6) = 360, Sunday (0) = 0. For each full week, the total is 480*5 + 360 + 0 = 2400 + 360 = 2760 minutes. Since `n` full weeks always start on Monday and end on Sunday, the total is simply `2760 * n`. Edge cases: `n=1` gives 2760; if the function were to accept partial weeks, we’d need to be careful with day indexing, but the task explicitly states full weeks. Time complexity is O(1) as it’s a single multiplication; space complexity is O(1). I’ll implement it as a simple constant-based calculation, but for clarity I’ll also show a loop-based version that could be extended to partial weeks, though the final output uses the direct formula.
#include <cassert>

/**
 * @brief Returns the total number of minutes the bank is open over exactly n full weeks.
 * 
 * Each week has 5 weekdays (480 min each), Saturday (360 min), and Sunday (0 min).
 * 
 * @param n Number of full weeks (n >= 1).
 * @return Total open minutes = 2760 * n.
 */
int countWorkingMinutes(int n) {
    // One full week: 5 * 480 (Mon-Fri) + 360 (Sat) + 0 (Sun) = 2400 + 360 = 2760.
    const int minutesPerWeek = 2760;
    return minutesPerWeek * n;
}
int main() {
    // One full week: Mon-Fri (480*5=2400) + Sat (360) + Sun (0) = 2760.
    assert(countWorkingMinutes(1) == 2760);
    // Two full weeks: double the single-week total.
    assert(countWorkingMinutes(2) == 5520);
    // Three weeks: 2760 * 3.
    assert(countWorkingMinutes(3) == 8280);
    // Ten weeks: 27600.
    assert(countWorkingMinutes(10) == 27600);
    // Check a larger value to ensure no overflow for typical int range.
    assert(countWorkingMinutes(100) == 276000);
    // Ensure the function works for the minimum input.
    assert(countWorkingMinutes(1) == 2760);
    // Sanity check for a non-trivial multiple.
    assert(countWorkingMinutes(7) == 19320);
    // Another check: 5 weeks = 13800.
    assert(countWorkingMinutes(5) == 13800);
    // Edge: n=1 again (already done, but keep as separate assert).
    assert(countWorkingMinutes(1) == 2760);
    // Final check: 52 weeks (a year) = 143520.
    assert(countWorkingMinutes(52) == 143520);
    return 0;
}
