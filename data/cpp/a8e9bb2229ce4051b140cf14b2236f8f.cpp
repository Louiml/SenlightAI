You are given `m` exam groups, each with a non-empty list of candidate identifiers (positive integers from 1 to `n`). For every group, you must choose exactly one candidate as the group’s representative, and the chosen representative must appear somewhere in that group’s list. Your goal is to decide whether there exists a valid assignment such that no candidate is chosen as representative for more than `ceil(m/2)` groups (i.e., the floor of `(m+1)/2`). If such an assignment exists, output `YES` followed by the chosen representative for each group in order from group 1 to group m. Otherwise, output `NO`. Write a C++ function named `assignRepresentatives` that takes `n`, `m`, and the vector of group lists (each list is a vector of candidate IDs, at least one element) and returns a `std::pair<bool, std::vector<int>>`: the first component is `true` if a valid assignment exists, the second component is the assigned representative per group (size `m`) if the first is `true`, otherwise the second component can be empty or arbitrary. Note that the total number of candidates across all groups can be up to 200,000, but `n` and `m` individually can be up to 100,000. The input may contain duplicate candidate IDs within a group, but the first element of each group is always present and can be used as a default. Also, there is no guarantee that the same candidate appears in multiple groups unless they do so naturally. Ensure your solution handles the case where multiple candidates exceed the limit simultaneously — but in practice, due to the pigeonhole principle, at most one candidate can exceed `(m+1)/2` when all groups have at least one element, so you only need to fix that single candidate if it exists.

// The problem is essentially a frequency‑limiting assignment. For each group, the safest default is to pick its first candidate. After counting how many times each candidate appears as the first element, we check if any candidate `p` appears more than `(m+1)/2` times. If no such candidate exists, we simply assign every group’s first element — the constraint is satisfied because no candidate’s count exceeds the threshold. If a candidate `p` does exceed the threshold, it must be reduced. The only way to reduce `p`’s count is to replace some groups that currently have `p` as first element with another candidate from that same group (preferably the second element, if available). Since each such replacement decreases `p`’s count by exactly one, and we need to reduce it to at most `(m+1)/2`, we compute `excess = cnt[p] - (m+1)/2`. We then iterate through all groups; whenever we encounter a group whose first element is `p` and whose list has length > 1, we replace the assignment with its second element and decrement `excess`. If after this process `excess` becomes zero, we have a valid assignment; otherwise, it is impossible. Edge cases: a group may have only one element, so it cannot be changed; if the same candidate appears as second element, that’s fine because we only care about the final counts. Also, if multiple candidates exceed the limit — which is impossible because that would sum to more than `m` — but we can still guard by checking all candidates with count > `(m+1)/2` and if any remain unfixed, output NO. Time complexity is O(n + total_group_size) because we count first elements in O(m) and scan all groups once to fix, and space is O(n + total_group_size) for storing the vectors and counts.

#include <vector>
#include <utility>
#include <algorithm>

// Returns {true, assignment} if a valid assignment exists (no candidate chosen > (m+1)/2 times),
// otherwise {false, {}}.
std::pair<bool, std::vector<int>> assignRepresentatives(
    int n,
    int m,
    const std::vector<std::vector<int>>& groups) {
    // Count how many groups have each candidate as their first element.
    std::vector<int> cnt(n + 1, 0);
    for (int i = 0; i < m; ++i) {
        ++cnt[groups[i][0]];
    }
    
    int limit = (m + 1) / 2;
    int bad_candidate = 0;
    for (int c = 1; c <= n; ++c) {
        if (cnt[c] > limit) {
            bad_candidate = c;
            break;
        }
    }
    
    std::vector<int> ans(m);
    if (bad_candidate == 0) {
        // No candidate exceeds the limit; just take the first element of each group.
        for (int i = 0; i < m; ++i) {
            ans[i] = groups[i][0];
        }
        return {true, ans};
    }
    
    // We need to replace some groups that have bad_candidate as first element.
    int excess = cnt[bad_candidate] - limit;
    for (int i = 0; i < m && excess > 0; ++i) {
        if (groups[i][0] == bad_candidate && groups[i].size() > 1) {
            ans[i] = groups[i][1];   // substitute with second candidate
            --excess;
        } else {
            ans[i] = groups[i][0];
        }
    }
    
    // For groups not processed yet (when excess reached 0 early), just assign first element.
    for (int i = 0; i < m; ++i) {
        if (ans[i] == 0) {
            ans[i] = groups[i][0];
        }
    }
    
    if (excess > 0) {
        return {false, {}};
    }
    return {true, ans};
}

#include <cassert>
#include <vector>
#include <utility>

// Function declaration (would be in the same translation unit in practice)
std::pair<bool, std::vector<int>> assignRepresentatives(
    int n, int m, const std::vector<std::vector<int>>& groups);

int main() {
    // Test 1: Simple case, no candidate exceeds limit.
    {
        std::vector<std::vector<int>> g = {{1, 2}, {2, 3}, {1, 3}};
        auto res = assignRepresentatives(3, 3, g);
        assert(res.first == true);
        assert(res.second == std::vector<int>({1, 2, 1}));
    }
    // Test 2: One candidate (1) appears as first in all 3 groups, but m=3 limit=2.
    {
        std::vector<std::vector<int>> g = {{1, 2}, {1, 3}, {1, 4}};
        auto res = assignRepresentatives(4, 3, g);
        assert(res.first == true);
        assert(res.second == std::vector<int>({1, 3, 4}));
    }
    // Test 3: Impossible because the only candidate in each group is the same and appears too often.
    {
        std::vector<std::vector<int>> g = {{1}, {1}, {1}};
        auto res = assignRepresentatives(1, 3, g);
        assert(res.first == false);
    }
    // Test 4: Edge case with m=1, always possible.
    {
        std::vector<std::vector<int>> g = {{5, 7}};
        auto res = assignRepresentatives(7, 1, g);
        assert(res.first == true);
        assert(res.second == std::vector<int>({5}));
    }
    // Test 5: Candidate 2 appears 4 times as first, m=4 limit=2, but only one group can be changed.
    {
        std::vector<std::vector<int>> g = {{2, 1}, {2}, {2, 3}, {2, 4}};
        auto res = assignRepresentatives(4, 4, g);
        assert(res.first == false);
    }
    // Test 6: Multiple groups with same second candidate, but only one needs change.
    {
        std::vector<std::vector<int>> g = {{2, 1}, {2, 1}, {2, 1}, {2, 3}};
        auto res = assignRepresentatives(3, 4, g);
        assert(res.first == true);
        assert(res.second == std::vector<int>({1, 1, 1, 2}));
        // Verify no candidate appears >2 times in the assignment.
        std::vector<int> freq(4, 0);
        for (int x : res.second) ++freq[x];
        for (int x : freq) assert(x <= 2);
    }
    // Test 7: Large n, no overuse.
    {
        std::vector<std::vector<int>> g = {{10}, {20}, {30}};
        auto res = assignRepresentatives(30, 3, g);
        assert(res.first == true);
        assert(res.second == std::vector<int>({10, 20, 30}));
    }
    // Test 8: Candidate appearing exactly at the limit is fine.
    {
        std::vector<std::vector<int>> g = {{1, 2}, {1, 3}, {1, 4}, {2, 5}};
        auto res = assignRepresentatives(5, 4, g);
        assert(res.first == true);
        assert(res.second == std::vector<int>({1, 1, 1, 2}));
    }
    return 0;
}
