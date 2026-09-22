// Write a C++ function `removeOverlappingShows` that takes a non-negative integer `k` (the maximum number of shows that can be watched simultaneously) and a vector of `Show` structs, where each `Show` has integer fields `start`, `end`, and `id` (all within `[1, 200000]` for start/end, and `id` unique). The function must simulate a streaming platform that allows at most `k` concurrent shows. It processes time from `1` to `200000` inclusive. At each time `t`: first remove any shows that have ended before `t` (i.e., `end < t`), then add all shows whose `start <= t` (in order given, but only if they haven't been added yet), and finally, if the number of active shows exceeds `k`, repeatedly cancel (remove) the show with the latest `end` time among the active ones (ties broken by any, but since ends are unique per input? assume not necessarily, tie-break by larger `id`), until only `k` remain. Each canceled show records its `id` in the output vector. The function must return a vector of canceled `id`s in the order they were canceled.
//
// Input shows are given as a vector of `Show` with fields `start`, `end`, `id`. The output should be a vector of `int` containing the canceled `id`s in order.
//
// **Constraints**: `1 <= n <= 200000`, `0 <= k <= n`, all show times are positive and <= 200000.

#include <cassert>
#include <vector>

// Include your solution here or assume it's above
struct Show {
    int start;
    int end;
    int id;
};

// The function definition from solution goes here (omitted for brevity in this test section but assumed present)

int main() {
    // Test 1: Simple case k=1, shows overlap
    {
        std::vector<Show> shows = {{1,5,1}, {2,4,2}, {3,6,3}};
        std::vector<int> res = removeOverlappingShows(1, shows);
        // At t=1: add show1. t=2: add show2, active has show1(end5) and show2(end4) -> cancel latest end = show1 (id1). Then t=3: add show3, active has show2(end4) and show3(end6) -> cancel show3 (id3). So result [1,3]? Let's simulate properly:
        // t=1: add1 -> active {1}. size=1 ok.
        // t=2: add2 -> active {2(4),1(5)}. size=2>1 -> cancel last (show1 id1). active {2}.
        // t=3: add3 -> active {2(4),3(6)}. size=2>1 -> cancel last (show3 id3). active {2}.
        // No more cancellations. Result [1,3].
        assert(res == std::vector<int>({1,3}));
    }

    // Test 2: k=0, all canceled
    {
        std::vector<Show> shows = {{1,10,1}, {2,8,2}};
        std::vector<int> res = removeOverlappingShows(0, shows);
        // t=1: add show1, size>0 -> cancel show1 (id1). t=2: add show2, size>0 -> cancel show2 (id2). Result [1,2].
        assert(res == std::vector<int>({1,2}));
    }

    // Test 3: k large, no cancellations
    {
        std::vector<Show> shows = {{1,10,1}, {2,9,2}};
        std::vector<int> res = removeOverlappingShows(10, shows);
        assert(res.empty());
    }

    // Test 4: tie-breaking by id when same end
    {
        std::vector<Show> shows = {{1,5,10}, {2,5,20}, {3,5,30}};
        std::vector<int> res = removeOverlappingShows(2, shows);
        // At t=1: add 10. t=2: add 20, active {10,20} size=2 ok. t=3: add 30, active {10,20,30} size=3>2 -> cancel largest end (all end5) -> among those, largest id =30 (id30) canceled. Result [30].
        assert(res == std::vector<int>({30}));
    }

    // Test 5: Non-overlapping shows, k=1
    {
        std::vector<Show> shows = {{1,2,1}, {3,4,2}, {5,6,3}};
        std::vector<int> res = removeOverlappingShows(1, shows);
        // Since no overlap, never exceeds 1 active, so empty.
        assert(res.empty());
    }

    // Test 6: Ends before t removal works
    {
        std::vector<Show> shows = {{1,2,1}, {2,3,2}};
        std::vector<int> res = removeOverlappingShows(1, shows);
        // t=1: add1 (end2). t=2: first remove shows with end < 2? (end<2 none) then add show2 (start2), active has show1(end2) and show2(end3) size=2>1 -> cancel largest end (show2 id2). Result [2].
        assert(res == std::vector<int>({2}));
    }

    return 0;
}

#include <vector>
#include <algorithm>
#include <set>

struct Show {
    int start;
    int end;
    int id;
};

// Comparator for multiset: order by end ascending, and for equal ends by id descending (so largest id is last)
struct ShowComparator {
    bool operator()(const Show& a, const Show& b) const {
        if (a.end != b.end) return a.end < b.end;
        return a.id > b.id; // larger id comes first (so last in set is smaller id? Actually we want cancel largest end, tie by larger id: we need to define strict weak ordering for set. For cancel, we want the "largest" according to this order. Since we want to cancel the one with largest end, and if tie, largest id, we can sort by end ascending, then id ascending, and then the "largest" in the set is the last element. But to make last element be the one with largest end and largest id, we can define comparator: a.end < b.end, or if equal, a.id < b.id. Then the set's reverse iterator gives largest end, and for tie, largest id. Let's do that.
        // But the above line gave a.id > b.id, which would put larger id first, so last would be smaller id. Let's fix.
    }
};

std::vector<int> removeOverlappingShows(int k, const std::vector<Show>& shows) {
    int n = shows.size();
    std::vector<Show> sorted = shows;
    std::sort(sorted.begin(), sorted.end(), [](const Show& a, const Show& b) {
        if (a.start != b.start) return a.start < b.start;
        return a.end < b.end;
    });

    // Multiset sorted by end ascending, then id ascending, so the last element is the one to cancel
    std::multiset<Show, bool(*)(const Show&, const Show&)> active(
        [](const Show& a, const Show& b) {
            if (a.end != b.end) return a.end < b.end;
            return a.id < b.id; // for tie, smaller id comes first, so larger id is later (last)
        }
    );

    std::vector<int> ans;
    size_t j = 0;
    const int MAX_T = 200000;

    for (int t = 1; t <= MAX_T; ++t) {
        // Remove shows that have ended before t (i.e., end < t)
        while (!active.empty()) {
            auto it = active.begin();
            if (it->end >= t) break;
            active.erase(it);
        }

        // Add all shows with start <= t (but only the ones we haven't added yet)
        while (j < sorted.size() && sorted[j].start <= t) {
            active.insert(sorted[j]);
            ++j;
        }

        // Cancel excess shows
        while (active.size() > static_cast<size_t>(k)) {
            auto it = std::prev(active.end());
            ans.push_back(it->id);
            active.erase(it);
        }
    }

    return ans;
}

// The problem is a classic interval scheduling with a cap on concurrent active intervals. The algorithm processes time from 1 to maxEnd (which we can bound by 200000). We maintain a data structure of active shows sorted by end time (ascending). At each time step: first prune any shows whose end time is strictly less than current time (they are finished). Then insert all shows whose start time equals current time (or as we iterate from sorted starts, any with start <= t; but since we iterate t from 1 upward, we can pre-sort shows by start and use a pointer). Because we pre-sort by start ascending, we can add all shows with start <= t using a pointer. After adding, if active count > k, we must cancel the show with the latest end time (since that is the one occupying the longest time into the future). We repeatedly pop from the back of the multiset (which we can implement with a `std::multiset` sorted by end, but to get largest end we use `prev(s.end())`). Each canceled show's id is appended. Since we process each show exactly once (either inserted then possibly canceled, or inserted and never canceled), total operations are O(n log n) due to sorts and multiset operations. Time bound is O(200000 + n log n). Space is O(n) for the active set and output vector.
//
// Edge cases: k=0 → all shows get canceled immediately. k >= n → no cancellations. Shows with same end times – the problem says to tie-break by larger id. In the multiset comparator we can order by end ascending, and for equal ends, by id descending (so the largest id comes last). But the code snippet uses a comparator that only orders by end; we need to modify to include id to have deterministic tie-breaking. Also note that we only cancel when the count exceeds k, so even if there are shows with equal end, we cancel one at a time. Ensure the pointer `j` advances correctly and we don't re-add shows.
//
// Time complexity: O(T + n log n) where T = 200000 (or max end time). Space O(n).
