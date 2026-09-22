// You are given an array of integers, initially of size N. You must process Q range-assignment queries: each query gives a position P (1-indexed) and a value V, and sets the element at position P to V. After each update, output the number of maximal contiguous segments (runs) of equal values in the entire array. The initial array and all queries are guaranteed to have N and Q up to 10^5, and there can be up to T test cases, with the sum of N+Q over all test cases also at most 10^5. Write a C++ function `std::vector<int> processQueries(const std::vector<int>& initial, const std::vector<std::pair<int,int>>& queries)` that returns a vector of integers, where the i-th element is the number of segments after applying the first i+1 queries. The function should be efficient enough for large inputs.

// We maintain the array as a set of disjoint segments, each represented by a tuple `(l, r, value)` where the segment covers indices `[l, r)` (0-indexed). Initially, we compress consecutive equal values into one segment. For each query `(p, v)`, we first convert to 0-indexed `qi = p-1`. We locate the segment containing `qi` using `lower_bound` on the start index. If the new value equals the segment's current value, no change is needed. Otherwise, we must split the segment at `qi`: remove the old segment, and insert up to three new segments: the part before `qi` (if non-empty), the single position `qi` with the new value, and the part after `qi` (if non-empty). After this insertion, we must merge any adjacent segments that now have equal values. Since only the position `qi` changed, only the segment newly inserted and its immediate neighbors can become mergeable. We check and merge those, possibly cascading if merging creates new equal neighbors (though at most two merges are possible because the inserted segment's value is new). This can be done with a helper that checks the three consecutive segments around a given position. The total number of segments is O(N), and each query changes at most O(1) segments, so each operation is O(log N) for set operations plus O(1) checks. Thus the total time is O((N+Q) log N) per test case, and space O(N). Edge cases include the position being at the very beginning or end of the array, the new value matching the left or right neighbor, and the inserted segment being the only segment.

#include <bits/stdc++.h>
using namespace std;

// Returns the number of segments after each query.
std::vector<int> processQueries(const std::vector<int>& initial,
                                const std::vector<std::pair<int,int>>& queries) {
    int n = (int)initial.size();

    // Build initial segments: (l, r, value) covering [l, r)
    std::vector<std::array<int,3>> segList;
    for (int i = 0; i < n; ++i) {
        if (i == 0 || initial[i] != initial[i-1]) {
            segList.push_back({i, -1, initial[i]});
        }
        segList.back()[1] = i + 1;
    }

    std::set<std::array<int,3>> segs(segList.begin(), segList.end());

    // Merge helper: after changing at position qi, try to merge seg at qi with its
    // immediate neighbors.  The iterator it points to the segment containing qi.
    auto fixAround = [&](int qi) {
        auto it = segs.lower_bound({qi, 0, 0});
        if (it == segs.end()) return;

        // Check left neighbor (it-1) and right neighbor (it+1) if they exist.
        // We'll handle all cases by repeatedly attempting to merge adjacent nodes
        // that have the same value.  Since only one insertion happened, at most
        // two merges are possible.
        while (true) {
            auto cur = it;
            bool merged = false;

            // Merge with previous if same value
            if (cur != segs.begin()) {
                auto prevIt = std::prev(cur);
                if ((*prevIt)[2] == (*cur)[2]) {
                    auto [l0, r0, v0] = *prevIt;
                    auto [l1, r1, v1] = *cur;
                    segs.erase(prevIt);
                    segs.erase(cur);
                    auto inserted = segs.insert({l0, r1, v0});
                    it = inserted.first;
                    merged = true;
                }
            }

            if (!merged) {
                // Merge with next if same value
                auto nextIt = std::next(cur);
                if (nextIt != segs.end()) {
                    if ((*cur)[2] == (*nextIt)[2]) {
                        auto [l1, r1, v1] = *cur;
                        auto [l2, r2, v2] = *nextIt;
                        segs.erase(cur);
                        segs.erase(nextIt);
                        auto inserted = segs.insert({l1, r2, v1});
                        it = inserted.first;
                        merged = true;
                    }
                }
            }

            if (!merged) break;
        }
    };

    std::vector<int> result;
    result.reserve(queries.size());

    for (const auto& [p, v] : queries) {
        int qi = p - 1;

        // Locate segment containing qi
        auto it = segs.lower_bound({qi + 1, 0, 0});
        --it;
        auto [l, r, curVal] = *it;

        if (curVal == v) {
            // No change, just record current segment count
            result.push_back((int)segs.size());
            continue;
        }

        // Remove old segment
        segs.erase(it);

        // Insert up to three new segments
        if (l != qi) {
            segs.insert({l, qi, curVal});
        }
        if (qi + 1 != r) {
            segs.insert({qi + 1, r, curVal});
        }
        auto insertedIt = segs.insert({qi, qi + 1, v}).first;

        // Merge possible neighbors
        fixAround(qi);

        result.push_back((int)segs.size());
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// The solution function is provided above.
// Include the solution code here or link appropriately.

int main() {
    // Example from the problem statement
    {
        std::vector<int> init = {1, 1, 2, 5, 2};
        std::vector<std::pair<int,int>> queries = {{1,3}, {4,2}};
        std::vector<int> expected = {5, 3};
        assert(processQueries(init, queries) == expected);
    }

    // Single element, single query
    {
        std::vector<int> init = {7};
        std::vector<std::pair<int,int>> queries = {{1, 9}};
        std::vector<int> expected = {1};
        assert(processQueries(init, queries) == expected);
    }

    // Change to same value should not change count
    {
        std::vector<int> init = {1, 2, 3};
        std::vector<std::pair<int,int>> queries = {{2, 2}};
        std::vector<int> expected = {3};
        assert(processQueries(init, queries) == expected);
    }

    // Change in the middle merging both neighbors
    {
        std::vector<int> init = {1, 2, 2, 1};
        // Change position 2 (index 1) to 1 -> becomes [1,1,2,1]? Actually index1 (value2) -> 1 gives [1,1,2,1], segments: [0,2) value1, [2,3) value2, [3,4) value1 => 3 segments
        std::vector<std::pair<int,int>> queries = {{2, 1}};
        std::vector<int> expected = {3};
        assert(processQueries(init, queries) == expected);
    }

    // Change at left edge
    {
        std::vector<int> init = {2, 2, 3, 3};
        std::vector<std::pair<int,int>> queries = {{1, 3}};
        // After: [3,2,3,3]? Actually set pos1 to 3 => [3,2,3,3] => segments: [0,1) 3, [1,2) 2, [2,4) 3 => 3 segments
        std::vector<int> expected = {3};
        assert(processQueries(init, queries) == expected);
    }

    // Change at right edge
    {
        std::vector<int> init = {1, 1, 2, 2};
        std::vector<std::pair<int,int>> queries = {{4, 1}};
        // [1,1,2,1] => segments: [0,2) 1, [2,3) 2, [3,4) 1 => 3 segments
        std::vector<int> expected = {3};
        assert(processQueries(init, queries) == expected);
    }

    // Multiple queries with cascading merges
    {
        std::vector<int> init = {1, 2, 3, 4};
        std::vector<std::pair<int,int>> queries = {{2, 1}, {3, 1}, {4, 1}};
        // After first: [1,1,3,4] => segments: [0,2)1, [2,3)3, [3,4)4 => 3
        // After second: [1,1,1,4] => segments: [0,3)1, [3,4)4 => 2
        // After third: [1,1,1,1] => segments: [0,4)1 => 1
        std::vector<int> expected = {3, 2, 1};
        assert(processQueries(init, queries) == expected);
    }

    // All same initially, change one to different
    {
        std::vector<int> init = {5, 5, 5};
        std::vector<std::pair<int,int>> queries = {{2, 7}};
        // [5,7,5] => segments: [0,1)5, [1,2)7, [2,3)5 => 3
        std::vector<int> expected = {3};
        assert(processQueries(init, queries) == expected);
    }

    // Query changes to value matching left neighbor only
    {
        std::vector<int> init = {3, 1, 2};
        std::vector<std::pair<int,int>> queries = {{2, 3}};
        // [3,3,2] => segments: [0,2)3, [2,3)2 => 2
        std::vector<int> expected = {2};
        assert(processQueries(init, queries) == expected);
    }

    // Query changes to value matching right neighbor only
    {
        std::vector<int> init = {1, 2, 3};
        std::vector<std::pair<int,int>> queries = {{2, 3}};
        // [1,3,3] => segments: [0,1)1, [1,3)3 => 2
        std::vector<int> expected = {2};
        assert(processQueries(init, queries) == expected);
    }

    // Large stress test (basic)
    {
        int n = 100000;
        std::vector<int> init(n, 0);
        std::vector<std::pair<int,int>> queries;
        for (int i = 0; i < 100; ++i) {
            queries.push_back({i * 1000 + 1, i % 3 + 1});
        }
        std::vector<int> res = processQueries(init, queries);
        // Just verify size, and that each result is >=1 and <= n
        assert((int)res.size() == 100);
        for (int x : res) assert(x >= 1 && x <= n);
    }

    return 0;
}
