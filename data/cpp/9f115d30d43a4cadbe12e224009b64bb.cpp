/*
Implement a C++ function `maxEarnings(vector<pair<int,int>> lectures)` where each pair is `{day, pay}` representing a lecture that pays `pay` dollars and must be completed by the end of `day` (day numbering starts at 1). You may complete at most one lecture per day, and you can only do a lecture on or before its deadline. The function should return the maximum total pay you can earn by selecting a valid subset of lectures. The input vector may contain duplicate days, unsorted pairs, and up to 100,000 lectures. The function should handle cases where no lectures are possible (return 0) and where deadlines are large (up to 1,000,000). The function must not modify the input vector; it should work on a copy or read-only reference.
*/
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>

// Returns maximum total pay from lectures with {deadline, pay}.
int maxEarnings(const std::vector<std::pair<int, int>>& lectures) {
    // Copy to allow sorting without modifying input
    std::vector<std::pair<int, int>> sorted = lectures;

    // Sort by deadline (day) ascending
    std::sort(sorted.begin(), sorted.end());

    // Min-heap to keep selected pays; smallest at top
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;

    for (const auto& [day, pay] : sorted) {
        heap.push(pay);
        // If we have more selected than days available, drop lowest pay
        if (static_cast<int>(heap.size()) > day) {
            heap.pop();
        }
    }

    int total = 0;
    while (!heap.empty()) {
        total += heap.top();
        heap.pop();
    }
    return total;
}
#include <cassert>
#include <vector>
#include <utility>

// Include the solution function here (assuming it's above or in same file)

int main() {
    // Empty input
    assert(maxEarnings({}) == 0);

    // Single lecture, deadline large enough
    assert(maxEarnings({{1, 5}}) == 5);

    // Single lecture, deadline zero => impossible
    assert(maxEarnings({{0, 10}}) == 0);

    // Basic case: two lectures, same deadline, pick higher pay
    assert(maxEarnings({{1, 3}, {1, 7}}) == 7);

    // Example from typical problem: deadlines [1,2,3], pays [5,1,2] => pick all => 8
    assert(maxEarnings({{1, 5}, {2, 1}, {3, 2}}) == 8);

    // More lectures than days: must drop lowest
    assert(maxEarnings({{1, 3}, {1, 4}, {2, 1}}) == 5); // pick 3 and 4? day1=3, day2=4 => total 7? Wait: day1 can do 4, day2 can do 3? both deadline <=2 => pick two highest => 3+4=7, but 1 is dropped. Wait check: day1=3, day2=4 => both fit => total 7. Actually correct.

    // Let's test a case where dropping is needed: day1 pay 1, day1 pay 2, day1 pay 3 => pick top 1 only => 3
    assert(maxEarnings({{1,1},{1,2},{1,3}}) == 3);

    // Larger case: deadlines [2,2,2], pays [10,20,30] => pick 20+30=50
    assert(maxEarnings({{2,10},{2,20},{2,30}}) == 50);

    // Unsorted input
    assert(maxEarnings({{3,4},{1,2},{2,3}}) == 9); // all fit, sum=9

    // Duplicate days with mixed pays
    assert(maxEarnings({{2,5},{2,5},{2,5}}) == 10); // pick two highest

    // Edge: deadlines huge, many lectures
    assert(maxEarnings({{100, 1}, {100, 2}, {100, 3}}) == 6); // pick top 3? actually all fit => 6

    return 0;
}
// The optimal strategy is a greedy algorithm using a min-heap (priority queue). Sort all lectures by their deadline in ascending order. Iterate through the sorted list, and for each lecture, push its pay into the min-heap. If after pushing, the size of the heap exceeds the current deadline (`day`), it means we have selected more lectures than days available up to that deadline, so we must drop the lecture with the smallest pay (the top of the min-heap). This ensures we always keep the highest-paying lectures within the available time slots. At the end, the sum of all elements in the heap is the maximum earnings. Edge cases: if a lecture has day = 0, it cannot be scheduled; but since the heap size check is `pq.size() > day`, if day=0, we push then immediately pop it, effectively ignoring it. Also, if multiple lectures share the same deadline, the heap ensures we only keep the best payers. Time complexity is O(N log N) for sorting plus O(N log N) for heap operations, total O(N log N). Space complexity is O(N) for the heap and sort storage.
