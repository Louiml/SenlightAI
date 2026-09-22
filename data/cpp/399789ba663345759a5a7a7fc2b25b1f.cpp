// Write a C++ function `long long countCowGroups(int n, long long t, const std::vector<std::pair<long long, long long>>& cows)` that processes `n` cows, where each cow starts at position `x` and runs at constant speed `y`. All cows run for exactly `t` units of time. Cows are considered to be in the same group if their final positions are equal; however, groups are formed dynamically from left to right: a cow joins the current rightmost group if its final position is strictly greater than the last cow's final position in that group; otherwise it starts a new group. The task is to return the total number of such groups formed. The input pairs represent `{x, y}` (starting position, speed). The final position of cow `i` is `x_i + t * y_i`. The function must handle up to 200,000 cows, with positions and speeds that may be negative or large (up to 1e9), and `t` may be up to 1e9. The order of cows in the input is their natural left-to-right order. The function should compute the minimum number of non-overlapping “chains” of strictly increasing final positions when processing cows in given order, which corresponds to the size of the longest decreasing subsequence in terms of final positions (or equivalently, the patience sorting pile count). Return that count.
// The problem is a classic “minimum number of piles” (patience sorting) where we need to partition a sequence into the minimum number of strictly increasing subsequences. By Dilworth’s theorem, the minimum number of increasing subsequences needed to cover a sequence equals the length of the longest decreasing subsequence. However, we can simulate directly using a multiset (or a set of endpoints) of currently active groups. Process cows in their input order. Compute each cow’s final position `pos = x + t * y`. We maintain a sorted container of the rightmost (largest) final position of each current group. For a new cow, we want to place it into the group whose current rightmost position is the largest among those that are strictly less than the new cow’s position (because placing it there keeps the group’s sequence increasing). If no such group exists (i.e., all group endpoints are >= new position), we start a new group. If such a group exists, we update that group’s endpoint to the new position. This is exactly the standard LIS-O(n log n) algorithm but here we count groups instead of LIS length. The use of `std::set` with `lower_bound` finds the first endpoint that is >= new position; the predecessor (before that) is the largest endpoint strictly less than new position. If there is no predecessor, start new group; otherwise remove that predecessor and insert new position. The size of the set is the answer. Edge cases: negative positions and speeds, duplicate positions: if two cows have the same final position, they cannot be in the same group (since group requires strictly increasing), so we treat them as needing a new group or replacement accordingly (the algorithm handles duplicates correctly because `lower_bound` finds the first >=, and if there is an equal endpoint, we cannot use that group, but we can use the group before it if its endpoint is smaller). Time complexity: O(n log n), space O(n). The given code uses `-a[i].ss` trick to store the negative index, but we can avoid that by using `std::set<pair<long long, long long>>` or storing position and a tie-breaker. Since we only need the endpoint, duplicates are allowed in the set? Actually we need multiset because multiple groups can have the same endpoint? Actually no, if two groups have the same endpoint, they could be merged? But careful: the algorithm must handle equal final positions: if two cows have identical final position, they cannot be in the same group (needs strictly increasing), but we could potentially have two groups both ending at that position? That would be wasteful; the optimal is to never have two groups with the same endpoint because you could merge them. The standard patience sorting uses a multiset or a vector where we replace the first element >= current. For duplicates, we replace the first >= (which might be an equal value) which is correct. Using `std::multiset<long long>` with `lower_bound` and then erasing that iterator (if it exists) and inserting the new value works: if there is an equal value, `lower_bound` points to it, we replace it with the same value, so set size doesn't change. If there is no >=, we insert (new group). So we can just use `std::multiset<long long>` and always do: `auto it = s.lower_bound(pos); if (it != s.end()) s.erase(it); s.insert(pos);`. That is simpler and handles duplicates correctly. But the original snippet uses a set and stores negative index as tie-breaker to avoid duplicates; we can avoid that by using multiset. Let’s use `std::multiset<long long>` for clarity. Then final answer is `s.size()`. This is O(n log n). Edge case: n=0? Not needed but we can return 0. Also ensure `long long` for arithmetic overflow; `t` up to 1e9, `y` up to 1e9, so `t*y` up to 1e18 fits in `long long` (signed 64-bit, max ~9e18). Positions up to 1e9, so sum fits.
#include <vector>
#include <set>
#include <cstdint>

// Count the minimum number of groups formed by cows running for time t.
// Each cow is given as {starting position, speed}.
// Cows are processed in input order; a cow joins the rightmost group 
// whose current final position is strictly less than its own final position.
long long countCowGroups(int n, long long t, const std::vector<std::pair<long long, long long>>& cows) {
    std::multiset<long long> endpoints; // current rightmost position for each group

    for (const auto& cow : cows) {
        long long finalPos = cow.first + cow.second * t;
        auto it = endpoints.lower_bound(finalPos);
        // Replace the smallest endpoint that is >= finalPos.
        // If none exists, this cow starts a new group.
        if (it != endpoints.end()) {
            endpoints.erase(it);
        }
        endpoints.insert(finalPos);
    }

    return static_cast<long long>(endpoints.size());
}
#include <cassert>
#include <vector>

// Include the solution function here (or link it).
long long countCowGroups(int n, long long t, const std::vector<std::pair<long long, long long>>& cows);

int main() {
    // Basic test: three cows at positions 0,1,2 with speed 0, t=0 => final positions 0,1,2 (already increasing) -> 1 group
    {
        std::vector<std::pair<long long, long long>> cows = {{0,0},{1,0},{2,0}};
        assert(countCowGroups(3, 0, cows) == 1);
    }
    // Decreasing final positions: each forms its own group
    {
        std::vector<std::pair<long long, long long>> cows = {{10,0},{5,0},{0,0}};
        assert(countCowGroups(3, 0, cows) == 3);
    }
    // t and speed cause equal final positions: duplicates can't be in same group
    {
        std::vector<std::pair<long long, long long>> cows = {{0,1},{1,0}, {2,-1}}; // t=1 => final: 1,1,1 => three groups
        assert(countCowGroups(3, 1, cows) == 3);
    }
    // Mixed: final positions: 1, 3, 2, 4 -> groups: [1,3,4] and [2] => 2
    {
        std::vector<std::pair<long long, long long>> cows = {{0,1},{0,3},{0,2},{0,4}}; // t=1
        assert(countCowGroups(4, 1, cows) == 2);
    }
    // Negative positions and speeds
    {
        std::vector<std::pair<long long, long long>> cows = {{-5, -1}, {-2, -2}, {0, 1}}; // t=2 => final: -7, -6, 2 => increasing => 1
        assert(countCowGroups(3, 2, cows) == 1);
    }
    // Large values to ensure no overflow
    {
        std::vector<std::pair<long long, long long>> cows = {{1000000000LL, 1000000000LL}, {0, 0}, {-1000000000LL, -1000000000LL}};
        // t=1000000000LL => final: 1000000000+1e18 = 1e18, 0, -1e18 => decreasing -> 3
        assert(countCowGroups(3, 1000000000LL, cows) == 3);
    }
    // Edge: single cow
    {
        std::vector<std::pair<long long, long long>> cows = {{42, 7}};
        assert(countCowGroups(1, 3, cows) == 1);
    }
    // Edge: empty (n=0) – function should return 0 (though not required by problem, test for safety)
    {
        std::vector<std::pair<long long, long long>> cows = {};
        assert(countCowGroups(0, 5, cows) == 0);
    }
    return 0;
}
