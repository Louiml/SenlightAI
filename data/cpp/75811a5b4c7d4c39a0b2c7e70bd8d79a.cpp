/*
Write a C++ function `int maxSteps(int n, int b, int a, const std::vector<int>& arr)` that simulates a journey along `n` segments. Initially, you have a battery level `b` and an accumulator level `a` (the accumulator cannot exceed its initial value `a`). For each segment `i` (0-indexed), `arr[i]` is either 0 (no sun) or 1 (sunny). To traverse a segment, you must consume exactly one unit from either the accumulator or the battery. If you use the battery, the battery decreases by 1. If you use the accumulator, the accumulator decreases by 1. Additionally, on a sunny segment (`arr[i]==1`), if you choose to use the battery while the accumulator is not full (below its initial value), then the accumulator increases by 1 (but never above its initial value). You stop when you cannot traverse the next segment (both levels are 0) or after all segments are processed. Return the maximum number of segments you can traverse by choosing optimally at each step. The accumulator can never exceed its initial value, and you must consume exactly one unit per segment (either from battery or accumulator). The battery can be used anytime, even when accumulator is full.
*/

#include <vector>
#include <algorithm>

// Simulate the journey and return the maximum number of segments traversed.
// n: number of segments, b: initial battery, a: initial accumulator (also max accumulator).
// arr[i] = 1 if sunny, 0 otherwise.
int maxSteps(int n, int b, int a, const std::vector<int>& arr) {
    const int maxAccumulator = a;
    int battery = b;
    int accumulator = a;
    int steps = 0;

    for (int i = 0; i < n; ++i) {
        if (battery == 0 && accumulator == 0) {
            break;
        }

        if (arr[i] == 1) {
            // Sunny segment: prefer using battery to recharge accumulator if possible.
            if (battery > 0 && accumulator < maxAccumulator) {
                --battery;
                ++accumulator;
            } else if (accumulator > 0) {
                --accumulator;
            } else if (battery > 0) {
                --battery;
            } else {
                break;
            }
        } else {
            // Not sunny: prefer battery to save accumulator for future sunny segments.
            if (battery > 0) {
                --battery;
            } else if (accumulator > 0) {
                --accumulator;
            } else {
                break;
            }
        }
        ++steps;
    }

    return steps;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(maxSteps(5, 1, 1, {0, 0, 0, 0, 0}) == 2);
    assert(maxSteps(5, 2, 1, {1, 1, 1, 1, 1}) == 5);
    assert(maxSteps(5, 0, 2, {1, 0, 1, 0, 1}) == 5);
    assert(maxSteps(5, 1, 0, {1, 1, 1, 1, 1}) == 1);
    assert(maxSteps(3, 0, 0, {0, 0, 0}) == 0);

    // Optimal use: recharge accumulator on sunny days
    assert(maxSteps(4, 2, 1, {1, 0, 1, 0}) == 4);
    // When accumulator is full, use battery anyway
    assert(maxSteps(5, 3, 1, {1, 1, 1, 1, 1}) == 5);
    // Mixed case with banking
    assert(maxSteps(6, 2, 1, {1, 0, 0, 1, 1, 0}) == 6);
    // Edge: stops when run out
    assert(maxSteps(10, 1, 1, {0, 0, 1, 1, 1, 0, 0, 0, 0, 0}) == 4);

    return 0;
}

// The problem is a greedy simulation. At each segment, you must choose which resource to consume. The key insight: you should preserve the accumulator (since it can be recharged on sunny segments) and use the battery whenever possible, especially on sunny segments when the accumulator is not full, because that both consumes battery and recharges the accumulator. However, if the battery is 0 and the accumulator is positive, use the accumulator. If both are positive and the segment is sunny, using the battery recharges the accumulator (up to its initial max), which is always beneficial because it increases total future capacity (since battery doesn't recharge). If sunny and accumulator is already full, using battery is fine too, it just doesn't recharge. If not sunny, either resource works; prefer using the battery to preserve the accumulator, because the accumulator is more valuable when sunny segments may come later (since it can be recharged). But careful: if not sunny, using battery vs accumulator doesn't change future recharge potential, but still using battery first is a safe greedy (since battery has no recharge ability, you want to keep the rechargeable resource). So algorithm: iterate each segment, if both resources are 0, break. If accumulator > 0 and battery > 0 and segment is sunny and accumulator is not full, then use battery (b--, a++), else if accumulator > 0 and battery == 0, use accumulator (a--), else if battery > 0, use battery (b--), else use accumulator (a--). This greedy works because on sunny segments, using battery to recharge accumulator never hurts (it increases or maintains accumulator while decreasing battery). On non-sunny, you want to save the accumulator for future sunny segments, so use battery first. Edge cases: accumulator initial value could be 0? The problem says you have an accumulator level `a` (likely positive). But handle zero. Time O(n), space O(1) aside from input array.
