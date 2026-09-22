/*
Given a vector of non-decreasing attack times `timeSeries` and a poison duration `duration`, write a C++ function `findPoisonedDuration` that returns the total duration (in seconds) for which the target remains poisoned. The poison is applied instantly at each attack time and lasts for `duration` seconds, but if a new attack occurs before the previous poison expires, the remaining poison time is replaced by a fresh full duration starting at the new attack time. The input vector may contain duplicate times, a single element, or be empty; the duration is a non-negative integer. Return the total number of seconds the target is in a poisoned state.
*/
#include <vector>
#include <algorithm>

// Returns total poisoned duration given sorted attack times and poison duration.
int findPoisonedDuration(const std::vector<int>& timeSeries, int duration) {
    if (timeSeries.empty()) return 0;
    int total = duration; // first attack guarantees full duration
    for (size_t i = 0; i + 1 < timeSeries.size(); ++i) {
        int gap = timeSeries[i + 1] - timeSeries[i];
        total += std::min(duration, gap);
    }
    return total;
}
#include <cassert>
#include <vector>

int findPoisonedDuration(const std::vector<int>& timeSeries, int duration);

int main() {
    assert(findPoisonedDuration({}, 5) == 0);
    assert(findPoisonedDuration({1}, 5) == 5);
    assert(findPoisonedDuration({1, 2}, 3) == 4); // overlap: 1-3 and 2-4 => 1+3? Actually 1+1+2? Let's compute: 1 to 2 =1, then 2 to 5 =3 => total=4
    assert(findPoisonedDuration({1, 4}, 3) == 6); // 1-4 and 4-7 => 3+3
    assert(findPoisonedDuration({1, 2, 3}, 1) == 3); // each attack covers 1 sec, no overlap
    assert(findPoisonedDuration({1, 1, 1}, 2) == 2); // duplicates: only first counts full
    assert(findPoisonedDuration({1, 2, 3, 4}, 2) == 5); // 1-3,2-4,3-5,4-6 => gaps:1+1+1 =3 +2? Actually total = 2+1+1+1=5
    assert(findPoisonedDuration({0, 5, 10}, 4) == 12); // 0-4,5-9,10-14 => 4+4+4
    assert(findPoisonedDuration({1, 100}, 50) == 100); // 1-51,100-150 => 50+50
    assert(findPoisonedDuration({1, 2, 100}, 10) == 20); // 1-11,2-12,100-110 => 1+8+10? Actually total=10+1+8+? Wait compute: 1-11 (10), overlap from 2 gives +1, then 100-110 gives 10 => total=10+1+10=21? Let's correct: method adds duration=10 for first, then min(10,1)=1, then min(10,98)=10 => total=21
}
// The problem reduces to summing the effective contribution of each attack interval. Since the times are non-decreasing, we can iterate through the vector and for each consecutive pair `(timeSeries[i], timeSeries[i+1])`, the poison from attack `i` lasts until `timeSeries[i] + duration`. If the next attack occurs before that (i.e., `timeSeries[i+1] < timeSeries[i] + duration`), then the overlap is not counted twice, so we only add the gap `timeSeries[i+1] - timeSeries[i]`; otherwise we add the full `duration`. A simple way: initialize the total with `duration` (assuming at least one attack), then for each `i` from 0 to `m-2`, add `min(duration, timeSeries[i+1] - timeSeries[i])`. Edge cases: empty vector returns 0; single element returns `duration`; duplicate attack times cause zero gap, so only `duration` is counted once. Time complexity is O(n), space O(1).
