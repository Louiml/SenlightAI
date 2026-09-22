Given a sequence of positive integers representing daily cumulative production totals (the i-th input value is the total produced from day 1 through day i), and then a series of query values, for each query find the smallest day index `d` such that the cumulative total up to day `d` is at least the query value. Write a C++ function `int findDay(const vector<int>& cumulative, int query)` that returns that day index (1‑based). The cumulative vector is already built from the prefix sums of daily production, so it is strictly increasing because all daily productions are positive. If the query is larger than the last cumulative total, return the last day index (n). If the query is exactly equal to a cumulative total, return that day. The function must handle an empty cumulative vector gracefully (return 0). Do not include the main function in the solution; only provide the function.
The input already provides a strictly increasing prefix‑sum array `cumulative[1..n]`. For a query `b`, we want the smallest index `i` such that `cumulative[i] >= b`. This is a classic lower‑bound search, which can be performed with binary search. The original snippet uses a loop that narrows the interval `[low, high]` with the invariant that `cumulative[low-1] < b` and `cumulative[high] >= b`. The binary search takes `O(log n)` time per query. If the query is greater than the last cumulative value, the loop ends with `high = n` (since we never move past n), which correctly gives the last day. If the query is exactly equal to some cumulative value, the loop may find it early and return `mid`, but to keep a uniform implementation we can simply run the standard lower‑bound binary search that always returns `low` (or `high`) after the loop, because when `cumulative[mid] >= b` we set `high = mid`, else `low = mid+1`. This guarantees that after the loop, `low == high` and `cumulative[low] >= b`, or if `b > last cumulative`, `low` becomes `n+1`, so we clamp to `n`. Edge cases: empty vector (return 0), query smaller than first cumulative (return 1), query exactly equal to a value (return that index), and query larger than the max (return n). Time complexity: O(log n) per query, O(n) to build the prefix array beforehand (but not part of the function). Space: O(1) auxiliary.
#include <vector>
#include <algorithm>

// Given a strictly increasing cumulative array (prefix sums of positive daily values),
// return the smallest 1-based index i such that cumulative[i] >= query.
// If query > last cumulative, return the last index (n). If cumulative is empty, return 0.
int findDay(const std::vector<int>& cumulative, int query) {
    int n = static_cast<int>(cumulative.size());
    if (n == 0) return 0;

    // Binary search for the lower bound
    int low = 1;   // 1-based index
    int high = n;  // 1-based index
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (cumulative[mid - 1] >= query) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    // After loop, low == high, and cumulative[low-1] >= query (if query <= last cumulative)
    // If query > last cumulative, low will be n (since it never moved past n), and cumulative[n-1] < query.
    // To handle that case, check if cumulative[low-1] < query, then return n.
    if (cumulative[low - 1] < query) {
        return n;
    }
    return low;
}
#include <cassert>
#include <vector>

// Forward declaration of the solution function (if needed, include the header or just declare)
int findDay(const std::vector<int>& cumulative, int query);

int main() {
    std::vector<int> cum = {10, 25, 40, 70}; // represents daily production: 10,15,15,30

    // Query exactly a cumulative value
    assert(findDay(cum, 10) == 1);
    assert(findDay(cum, 25) == 2);
    assert(findDay(cum, 40) == 3);
    assert(findDay(cum, 70) == 4);

    // Query between cumulative values
    assert(findDay(cum, 11) == 2);
    assert(findDay(cum, 39) == 3);
    assert(findDay(cum, 69) == 4);

    // Query less than first
    assert(findDay(cum, 1) == 1);

    // Query larger than last cumulative
    assert(findDay(cum, 100) == 4);

    // Edge case: empty vector
    std::vector<int> empty;
    assert(findDay(empty, 5) == 0);

    // Edge case: single element
    std::vector<int> single = {7};
    assert(findDay(single, 7) == 1);
    assert(findDay(single, 6) == 1);
    assert(findDay(single, 8) == 1);

    // Multiple queries with same value
    assert(findDay(cum, 25) == 2);
    assert(findDay(cum, 25) == 2);
}
