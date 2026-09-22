// You are given a list of \(N\) runners, each with a constant positive integer speed \(v_i\) (in units per second). All runners start at the starting line at time \(t=0\), and their distance from the start at integer time \(t\) is \(i + v_i \cdot t\), where \(i\) is their unique starting lane offset (1-indexed). Over the time interval from \(t=0\) up to \(t=4 \times \max(v)\) (inclusive), the number of distinct distances achieved at any integer time \(t\) is recorded. You are to compute, for each integer time \(t\) in that interval, the value \(\text{diff}(t) = 1 + (N - \text{number of distinct distances at time } t)\). Then, after collecting all \(\text{diff}(t)\) values, find the minimum and maximum among them. Write a function `std::pair<int,int> minMaxDiff(const std::vector<int>& speeds)` that returns the minimum and maximum diff values as a pair, and if all runners have speed 0, return `{1,1}`. The input vector contains the actual speeds of the N runners (size N). The function must handle up to \(N=10^4\) and speeds up to \(10^3\). Note: The original code had a bug (missing semicolon and incorrect indexing), but your task is to implement the corrected version as described.
// The core idea is to simulate the process directly. For each integer time \(t\) from 0 to \(4 \times \max(\text{speeds})\), compute the position \(i + v_i \cdot t\) for each runner using 1-based indexing. Use a `std::set` (or `std::unordered_set` if you want, but due to small range a set suffices) to count distinct positions. Let `distinct` be the size of the set. Then `diff = 1 + (N - distinct)`. Append this to a vector. After finishing all time steps, the answer is the minimum and maximum of that vector. If the maximum speed is 0, all runners are stationary, so at every time step the distinct count is 1 (since all positions are just the offsets 1..N), so diff = 1 + (N-1) = N, thus min=max=N. But the problem statement says to return {1,1} for that case, which is peculiar; we'll follow the original code's behavior: if max speed is 0, output size twice (i.e., return {size,size}). However the task description says "if all runners have speed 0, return `{1,1}`". We must follow the task specification, not the original buggy code. So we'll return {1,1} when all speeds are zero. For non-zero speeds, the time range is up to `4 * maxSpeed` inclusive (so loop `for t=0; t <= 4*maxSpeed; t++`). Complexity: Let \(M = \max(v)\). There are \(4M+1\) time steps, each doing an O(N) insertion into a set that has at most N distinct elements, so O(N log N) per step, total O(N M log N). With N up to 1e4 and M up to 1e3, that's about 4e7 log operations, which is acceptable in C++ with optimization. Space is O(N) for the set and O(4M) for the diff vector. Edge cases: speeds can be zero individually, but if at least one is non-zero, we simulate normally; if all are zero, we return {1,1} as per spec.
#include <vector>
#include <set>
#include <algorithm>
#include <utility>

// Computes the minimum and maximum of diff(t) = 1 + (N - distinct_positions) over t = 0..4*max_speed.
// If all speeds are zero, returns {1,1} as specified.
std::pair<int,int> minMaxDiff(const std::vector<int>& speeds) {
    const int n = static_cast<int>(speeds.size());
    if (n == 0) return {0,0}; // Edge: empty input, though not expected.

    int maxSpeed = 0;
    for (int s : speeds) maxSpeed = std::max(maxSpeed, s);

    if (maxSpeed == 0) {
        return {1,1};
    }

    std::vector<int> diffs;
    diffs.reserve(4 * maxSpeed + 1);

    for (int t = 0; t <= 4 * maxSpeed; ++t) {
        std::set<int> positions;
        for (int i = 0; i < n; ++i) {
            // 1-based index: offset = i+1
            int pos = (i + 1) + speeds[i] * t;
            positions.insert(pos);
        }
        int distinct = static_cast<int>(positions.size());
        int diff = 1 + (n - distinct);
        diffs.push_back(diff);
    }

    int minDiff = *std::min_element(diffs.begin(), diffs.end());
    int maxDiff = *std::max_element(diffs.begin(), diffs.end());
    return {minDiff, maxDiff};
}
#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above.
int main() {
    // Trivial all-zero speeds
    std::pair<int,int> r1 = minMaxDiff({0,0,0});
    assert(r1.first == 1 && r1.second == 1);

    // Single runner non-zero
    std::pair<int,int> r2 = minMaxDiff({5});
    // N=1, at any t, distinct=1, diff=1+0=1. So {1,1}
    assert(r2.first == 1 && r2.second == 1);

    // Two runners same speed -> always distinct positions 1 and 2 (diff=2)
    std::pair<int,int> r3 = minMaxDiff({3,3});
    assert(r3.first == 2 && r3.second == 2);

    // Two runners with speeds 1 and 2, max=2, t=0..8
    // t=0: positions 1,2 -> distinct=2 diff=1
    // t=1: 2,4 -> distinct=2 diff=1
    // t=2: 3,6 -> distinct=2 diff=1
    // ... always distinct since v differ -> diff=1 always; min=max=1
    std::pair<int,int> r4 = minMaxDiff({1,2});
    assert(r4.first == 1 && r4.second == 1);

    // N=3 speeds {1,1,2}, max=2, t=0..8
    // t=0: 1,2,3 -> distinct=3 diff=1
    // t=1: 2,3,5 -> distinct=3 diff=1
    // t=2: 3,4,7 -> distinct=3 diff=1
    // t=3: 4,5,9 -> distinct=3 diff=1
    // ... always diff=1? Check t where collisions: only if two speeds equal and same offset offset different? No collisions since offsets differ always? Actually position = offset + v*t, if two runners have same v but different offsets, they never collide. So diff stays 1 for all t. So {1,1}
    std::pair<int,int> r5 = minMaxDiff({1,1,2});
    assert(r5.first == 1 && r5.second == 1);

    // N=2 speeds {2,2} same as r3 -> {2,2}
    std::pair<int,int> r6 = minMaxDiff({2,2});
    assert(r6.first == 2 && r6.second == 2);

    // N=3 speeds {2,2,3} max=3, t=0..12
    // t=0: 1,2,3 -> distinct=3 diff=1
    // t=1: 3,4,6 -> distinct=3 diff=1
    // t=2: 5,6,9 -> distinct=3 diff=1
    // t=3: 7,8,12 -> distinct=3 diff=1
    // t=4: 9,10,15 -> distinct=3 diff=1
    // t=5: 11,12,18 -> distinct=3 diff=1
    // t=6: 13,14,21 -> distinct=3 diff=1
    // t=7: 15,16,24 -> distinct=3 diff=1
    // t=8: 17,18,27 -> distinct=3 diff=1
    // t=9: 19,20,30 -> distinct=3 diff=1
    // t=10:21,22,33 -> distinct=3 diff=1
    // t=11:23,24,36 -> distinct=3 diff=1
    // t=12:25,26,39 -> distinct=3 diff=1
    // Since speeds 2 and 3 never collide with any other because offsets differ? Actually collision occurs when offset1+v1*t = offset2+v2*t -> (v1-v2)*t = offset2-offset1. For v1=2,v2=3, difference -1, offset diff -1, so t=1 gives 1+2*1=3 and 2+3*1=5? Wait compute: runner1 offset1=1, v=2; runner2 offset2=2, v=3. At t=1: runner1=1+2=3, runner2=2+3=5, no. For two with same v=2: offsets 1,2 never collide. So diff=1 always. So {1,1}
    std::pair<int,int> r7 = minMaxDiff({2,2,3});
    assert(r7.first == 1 && r7.second == 1);

    // N=3 speeds {1,1,1} same speed -> all offsets distinct always -> distinct=3 diff=1
    std::pair<int,int> r8 = minMaxDiff({1,1,1});
    assert(r8.first == 1 && r8.second == 1);

    // Edge: maxSpeed large but small N – just ensure no crash, result finite
    std::pair<int,int> r9 = minMaxDiff({1000});
    assert(r9.first == 1 && r9.second == 1);

    // Empty test not needed but can assert if provided
    // std::pair<int,int> r10 = minMaxDiff({});
    // assert(r10.first == 0 && r10.second == 0);
}
