// Write a C++ function `int findIndexAfterWait(int totalEvents, const std::vector<int>& waitCounts, double tolerance)` that simulates the core waiting behavior from the SearchSpace code: given a sequence of "events" (each event has a waiting period), you are to process them by accumulating wait counts. The function must return the index (0-based) of the first event where the cumulative number of waiting steps (sum of all `waitCounts[0..i]`) is at least `totalEvents * tolerance`. If no event reaches that threshold, return `-1`. The input `totalEvents` is the total number of events in the search space (always positive), `waitCounts` is a non-empty vector of non-negative integers representing how many waiting steps each event requires, and `tolerance` is a positive double. The function must be const-correct and work correctly when `tolerance` is less than 1 (since it multiplies `totalEvents` as in the code `val.getTolerance()`), with edge cases like exactly reaching the threshold, zero wait counts, and large cumulative sums that might overflow `int`.
// The solution is straightforward: iterate through the vector while maintaining a cumulative sum of wait counts. For each index `i`, after adding `waitCounts[i]`, check if the cumulative sum `cum` is greater than or equal to `threshold = totalEvents * tolerance`. Since `tolerance` is a positive double and `totalEvents` is an int, compute the threshold as a double to avoid integer truncation. Use a `long long` for the cumulative sum to prevent overflow when `waitCounts` values are large (e.g., up to 1e9 each). The first index satisfying the condition is returned. Edge cases: if `tolerance` is very small (e.g., 0.001), the threshold might be less than the first event's wait count, so return 0 immediately; if `tolerance` is >=1 (though typical less than 1), the threshold could exceed the total sum, leading to `-1`. Also handle empty vector (though specified non-empty) by returning -1 defensively. Time complexity is O(n) where n is the size of `waitCounts`; space complexity is O(1) auxiliary.
#include <vector>
#include <cstddef>

// Given a total number of events and a vector of wait counts per event,
// find the index of the first event where the cumulative wait count
// reaches at least totalEvents * tolerance. Returns -1 if never reached.
int findIndexAfterWait(int totalEvents, const std::vector<int>& waitCounts, double tolerance) {
    if (waitCounts.empty()) return -1;
    const double threshold = static_cast<double>(totalEvents) * tolerance;
    long long cumulative = 0;
    for (std::size_t i = 0; i < waitCounts.size(); ++i) {
        cumulative += waitCounts[i];
        if (static_cast<double>(cumulative) >= threshold) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

// Declare the function (in real code, include the header)
int findIndexAfterWait(int totalEvents, const std::vector<int>& waitCounts, double tolerance);

int main() {
    // Basic case: threshold reached exactly at third event
    assert(findIndexAfterWait(10, {1, 2, 3, 4}, 0.5) == 2); // cumulative: 1,3,6 >=5

    // Zero wait counts: never reaches threshold if all zero
    assert(findIndexAfterWait(10, {0, 0, 0}, 0.5) == -1);

    // First event already exceeds threshold
    assert(findIndexAfterWait(10, {100, 1}, 0.1) == 0); // threshold=1, cum=100>=1

    // Edge: tolerance exactly 1.0, total sum equals threshold
    assert(findIndexAfterWait(5, {2, 3}, 1.0) == 1); // threshold=5, cum=2,5 >=5

    // Edge: large wait counts (no overflow)
    std::vector<int> large = {1000000000, 1000000000, 1000000000};
    assert(findIndexAfterWait(3, large, 0.5) == 1); // threshold=1.5, cum=1e9,2e9 >=1.5

    // Single event, threshold less than wait
    assert(findIndexAfterWait(1, {7}, 0.3) == 0); // threshold=0.3, cum=7>=0.3

    // Single event, threshold greater than wait (tolerance > wait/total)
    assert(findIndexAfterWait(10, {2}, 0.3) == -1); // threshold=3, cum=2<3

    return 0;
}
