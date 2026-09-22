/*
You are given an initial power strip with `N` outlets and a sequence of `K` plug-in requests, each identifying a device type (an integer from 1 to 100). When a device is requested, if it is already plugged into one of the `N` outlets, nothing changes; otherwise, you must plug it in. If all `N` outlets are already occupied by different devices, you must unplug one currently plugged device before plugging in the new one (the device you unplug is then removed from the outlet, and a new one is plugged in). The goal is to minimize the total number of unplugging actions over the entire sequence. Write a C++ function that takes integers `N`, `K`, and a vector of length `K` (the sequence of device types) and returns the minimum number of unplugs required. The function must be self-contained, handle `1 ≤ N ≤ 100`, `1 ≤ K ≤ 1000`, device types in `[1,100]`, and assume no invalid input. If the sequence can be processed without ever unplugging (because the number of distinct concurrently used devices never exceeds `N`), the answer is 0.
*/
#include <vector>
#include <algorithm>

// Compute the minimum number of unplugs for an outlet strip with N outlets
// and a sequence of device requests given in `requests`.
int min_unplugs(int N, const std::vector<int>& requests) {
    const int K = static_cast<int>(requests.size());
    std::vector<bool> plugged(101, false); // device types are 1..100
    int plugged_count = 0;
    int unplugs = 0;

    for (int i = 0; i < K; ++i) {
        const int device = requests[i];
        if (plugged[device]) {
            continue; // already plugged, no action needed
        }

        if (plugged_count < N) {
            // free outlet available
            plugged[device] = true;
            ++plugged_count;
        } else {
            // need to unplug one existing device
            int to_unplug = -1;
            int farthest_next = -1; // largest next index; -1 means never again

            for (int cur = 1; cur <= 100; ++cur) {
                if (!plugged[cur]) continue;
                int next_idx = -1;
                for (int j = i + 1; j < K; ++j) {
                    if (requests[j] == cur) {
                        next_idx = j;
                        break;
                    }
                }
                // If next_idx == -1, that device never appears again; unplug it immediately.
                // Otherwise, choose the one with the largest next index.
                if (next_idx == -1 || next_idx > farthest_next) {
                    farthest_next = next_idx;
                    to_unplug = cur;
                }
            }
            // Unplug the chosen device
            plugged[to_unplug] = false;
            // Plug the new device
            plugged[device] = true;
            ++unplugs;
            // plugged_count remains the same (N)
        }
    }
    return unplugs;
}
#include <cassert>
#include <vector>

// Declare the function from the solution
int min_unplugs(int N, const std::vector<int>& requests);

int main() {
    // Test 1: Basic example where unplugging is needed
    // N=2, sequence: 1,2,3,1,2 → 
    // Plug 1, plug 2, request 3 unplug 1 (farthest next is at pos 3 for 2 vs pos 3 for 1? Actually 2 appears at index 4, 1 at index 3, so unplug 1), plug 3,
    // request 1 unplug 3 (never appears again), plug 1, request 2 already plugged → total 2 unplugs.
    assert(min_unplugs(2, {1, 2, 3, 1, 2}) == 2);

    // Test 2: No unplug needed because N is large enough
    // N=3, sequence: 1,2,3,1,2,3 → each device fits, no unplugs
    assert(min_unplugs(3, {1, 2, 3, 1, 2, 3}) == 0);

    // Test 3: Single device, always plugged
    assert(min_unplugs(1, {5, 5, 5}) == 0);

    // Test 4: N=1, sequence of distinct devices → each new device forces an unplug
    // 1,2,3,4 → unplug 1, unplug 2, unplug 3 → 3 unplugs
    assert(min_unplugs(1, {1, 2, 3, 4}) == 3);

    // Test 5: Repeated device that never gets unplugged if it appears soon
    // N=2, sequence: 1,2,3,2,1 → 
    // Plug 1, plug 2, request 3: unplug 1 (next at index 4) vs 2 (next at index 3) → unplug 1, plug 3,
    // request 2 already plugged, request 1 unplug 3 (never again) → total 2
    assert(min_unplugs(2, {1, 2, 3, 2, 1}) == 2);

    // Test 6: Edge with device never appearing again
    // N=1, sequence: 1,2,1 → Plug 1, request 2 unplug 1 (never again? 1 appears at index 2 later? Actually sequence: 1,2,1 → after plugging 1, next request 2, unplug 1 because 1 appears at index 2 (next at 2) vs 2 never appears → unplug 1, plug 2, request 1 unplug 2 → total 2
    assert(min_unplugs(1, {1, 2, 1}) == 2);

    // Test 7: All same devices, N=1 → no unplugs
    assert(min_unplugs(1, {7, 7, 7, 7}) == 0);

    // Test 8: Larger N but still unplugs needed
    // N=3, sequence: 1,2,3,4,2,3,4,1 → 
    // Plug 1,2,3; request 4 unplug 1 (next at index 7) vs 2 (next at 4) vs 3 (next at 5) → unplug 1, plug 4,
    // request 2 already, request 3 already, request 4 already, request 1 unplug 2 (next at -1) → unplug 2, plug 1 → total 2
    assert(min_unplugs(3, {1, 2, 3, 4, 2, 3, 4, 1}) == 2);

    // Test 9: N=100, K small, all fit, no unplugs
    assert(min_unplugs(100, {1, 2, 3, 4, 5}) == 0);

    return 0;
}
// This is the classic "Optimal Page Replacement" problem, where outlets are cache slots and device requests are page references. The greedy strategy is optimal: when a new device must be plugged and all outlets are occupied, unplug the device whose next use is the farthest in the future (or never used again). To implement this, maintain a boolean or visited array tracking which devices are currently plugged. For each request in order:
// - If the device is already plugged, skip (no unplug).
// - If there is a free outlet (current plugged count < N), plug it in.
// - Otherwise, among all currently plugged devices, find the one whose next occurrence in the remaining sequence is the latest (largest index), or if never occurs again, treat that as infinite distance. Unplug that device, increment answer, and plug the new one.
// Edge cases: the very first request always plugs with no unplug; if a device appears multiple times, it may be unplugged and later re-plugged; if a device never appears again after a full outlet state, unplug it immediately. Complexity: For K requests and at most N plugged devices, each unplug decision scans the remaining sequence for each plugged device, giving worst-case O(K * N * K) = O(K²N). With K ≤ 1000 and N ≤ 100, this is at most 100 million operations, acceptable. Space is O(N) for the set of plugged devices plus O(1) for the visited array.
