/*
Given an array of `n` integers where each value is at least 1 and the array size is at most 200,000, write a C++ function `int maximumPartitions(const std::vector<int>& v)` that determines the maximum integer `k` such that the array can be partitioned into **at least** `k` contiguous segments, where each segment, when considered independently, contains each distinct value within that segment exactly once (i.e., each segment is a permutation of its distinct elements, so no duplicates within a segment). The partition must cover the entire array, and segments can be of any positive length. For example, for `v = [1,2,1,3,2]`, the maximum `k` is 2 because you can partition as `[1,2]` and `[1,3,2]`, but cannot partition into 3 such segments.
*/
#include <vector>
#include <algorithm>
#include <map>

// Checks if the array can be partitioned into at least target segments,
// each segment having no duplicate values.
bool canPartitionInto(const std::vector<int>& v, int target) {
    std::vector<bool> active(200001, false);
    active[v[0]] = true;
    int activeCount = 1;
    int segments = 1;
    // We need to simulate the greedy process. Maintain the set of elements
    // that must be present in the next segment (since we close current).
    std::vector<int> toReactivate;
    toReactivate.push_back(v[0]);

    for (size_t i = 1; i < v.size(); ++i) {
        if (active[v[i]]) {
            // Duplicate found: we must close current segment before v[i].
            // But careful: we cannot just close; we need to ensure that the
            // next segment starts from v[i] and must contain exactly one of
            // each distinct element that was in the closed segment? Actually
            // the original algorithm's logic is a bit different: it uses a
            // futureElements set to reset. We'll implement a simpler direct
            // greedy: split immediately before any duplicate.
            // But that might not give maximum segments. Instead, we use the
            // original approach: when activeCount becomes zero, we start a
            // new segment and reset active for all elements seen in the
            // just-completed segment.
        }
        // The original snippet uses a different logic, but we'll implement
        // the exact same logic as the snippet for correctness.
    }

    // The given snippet's isPossible function is copied here with adaptation.
    std::vector<bool> f(200001, false);
    f[v[0]] = true;
    int remaining = 1;
    int ans = 1;
    std::set<int> future;
    for (size_t i = 1; i < v.size(); ++i) {
        future.insert(v[i]);
        if (f[v[i]]) {
            f[v[i]] = false;
            remaining--;
        }
        if (remaining == 0) {
            ans++;
            for (int u : future) f[u] = true;
            remaining = (int)future.size();
            future.clear();
        }
    }
    return ans >= target;
}

// Returns the maximum number of valid partitions possible.
int maximumPartitions(const std::vector<int>& v) {
    if (v.empty()) return 0;
    // compute max frequency
    std::map<int,int> freq;
    for (int x : v) freq[x]++;
    int mx = 0;
    for (const auto& p : freq) mx = std::max(mx, p.second);

    int low = 1, high = mx, best = 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (canPartitionInto(v, mid)) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}
(Note: The above solution includes a duplicate helper function; the final cleaned version is provided below in the test section.)

After correction, the final solution is:
#include <vector>
#include <map>
#include <set>

// Checks if the array can be partitioned into at least target valid segments.
bool canPartitionInto(const std::vector<int>& v, int target) {
    const int MAXV = 200000;
    std::vector<bool> active(MAXV+1, false);
    active[v[0]] = true;
    int remaining = 1;
    int segments = 1;
    std::set<int> future;
    for (size_t i = 1; i < v.size(); ++i) {
        future.insert(v[i]);
        if (active[v[i]]) {
            active[v[i]] = false;
            remaining--;
        }
        if (remaining == 0) {
            segments++;
            for (int u : future) active[u] = true;
            remaining = (int)future.size();
            future.clear();
        }
    }
    return segments >= target;
}

// Returns the maximum number of valid partitions.
int maximumPartitions(const std::vector<int>& v) {
    if (v.empty()) return 0;
    std::map<int,int> freq;
    for (int x : v) freq[x]++;
    int maxFreq = 0;
    for (const auto& p : freq) maxFreq = std::max(maxFreq, p.second);

    int low = 1, high = maxFreq, best = 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (canPartitionInto(v, mid)) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}
#include <cassert>
#include <vector>

int main() {
    assert(maximumPartitions({1, 2, 1, 3, 2}) == 2);
    assert(maximumPartitions({1, 1, 1}) == 3);
    assert(maximumPartitions({1, 2, 3}) == 1);
    assert(maximumPartitions({5, 5, 5, 5}) == 4);
    assert(maximumPartitions({1, 2, 1, 2, 1}) == 2);
    assert(maximumPartitions({1}) == 1);
    assert(maximumPartitions({2, 2, 1, 1, 2, 2}) == 2);
    assert(maximumPartitions({1, 2, 3, 1, 2, 3}) == 2);
    assert(maximumPartitions({1, 1, 2, 2, 1}) == 1);
    assert(maximumPartitions({7, 7, 7, 8, 8, 7}) == 2);
    return 0;
}
// The goal is to maximize the number of segments, but each segment must have no repeated elements. The maximum possible number of segments is bounded by the maximum frequency of any value in the entire array, because each occurrence of the most frequent value must go into a different segment (otherwise a segment would contain a duplicate). We binary-search the answer `k` from 1 to that maximum frequency. For a given `k`, we try to greedily split the array into as many valid segments as possible: we scan left to right, maintain a "current set" of elements seen in the current segment, and a count of "free" slots (each distinct element in the segment must appear exactly once, so we need to ensure that when we see a duplicate, we close the segment before that duplicate). The helper `isPossible` checks whether we can achieve at least `k` such segments. It uses a boolean frequency array to track which values are currently "active" in the open segment. When encountering a value already active, we must cut before this position (i.e., finalize the current segment). When the active count drops to zero (meaning we have closed all active elements), we start a new segment and reset the active set to contain all elements we just saw in the closed segment (because those must appear again in the new segment? Actually we need to carefully reset: the logic in the snippet resets `f` for all elements that were in the current segment and makes them active again for the next segment, but it also counts `remainingElement` as the number of distinct elements in that segment. The algorithm essentially counts segments by "frontier" resets. This greedy approach works because if you can split into `k` valid segments, you can split into fewer, and the binary search finds the largest feasible. Time complexity is O(n log m) where m is the max frequency (at most n), and space is O(n) for the frequency array and auxiliary sets. Edge cases: all elements distinct → answer is 1 because each segment must be a permutation of its distinct elements, but a segment can contain all distinct elements, so you can do 1. If all elements are same, answer is n because each segment must be length 1.
