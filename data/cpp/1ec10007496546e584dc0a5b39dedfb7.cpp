You are given a positive integer `n` representing the number of consecutive apartments arranged in a line, numbered from 1 to `n`. Some apartments already have a base station installed, given as a sorted vector of distinct integers `stations` (each between 1 and `n`). Each base station has a coverage radius `w` (a non-negative integer), meaning it covers all apartments from `x - w` to `x + w` inclusive, clamped to the range [1, n]. However, no base station can be installed outside the range [1, n]. Write a C++ function `int minAdditionalStations(int n, const std::vector<int>& stations, int w)` that returns the **minimum** number of additional base stations (each with the same coverage radius `w`) that must be installed so that every apartment from 1 to `n` is covered by at least one base station. The input `stations` will be sorted, may be empty, and will contain no duplicates. The function must be efficient for large `n` (up to 200,000,000) and large `stations` size (up to 100,000). You may assume `n >= 1` and `w >= 0`.

#include <cassert>
#include <vector>

int main() {
    // Example 1: n=11, stations=[4,11], w=1 -> cover: 4 covers 3-5, 11 covers 10-11 => missing 1,2,6,7,8,9 (6 apartments) => need ceil(6/3)=2 stations (e.g., at 1 and 7)
    assert(minAdditionalStations(11, {4, 11}, 1) == 2);

    // Example 2: No stations, n=5, w=0 -> each station covers 1 apartment -> need 5
    assert(minAdditionalStations(5, {}, 0) == 5);

    // Example 3: All apartments covered already: n=7, stations=[2,5], w=2 -> covers 1-4 and 3-7 => all covered
    assert(minAdditionalStations(7, {2, 5}, 2) == 0);

    // Example 4: Large w, small gap: n=10, stations=[5], w=10 -> covers all even beyond boundaries
    assert(minAdditionalStations(10, {5}, 10) == 0);

    // Example 5: Single apartment, no station: n=1, stations=[], w=0 -> need 1
    assert(minAdditionalStations(1, {}, 0) == 1);

    // Example 6: Single apartment, no station, w=1 -> need 1 (but coverage extends beyond, still need to place a station)
    assert(minAdditionalStations(1, {}, 1) == 1);

    // Example 7: Stations adjacent, no gap: n=5, stations=[1,2], w=0 -> covers 1 and 2, missing 3,4,5 -> need 3
    assert(minAdditionalStations(5, {1, 2}, 0) == 3);

    // Example 8: Stations at ends, large gap: n=20, stations=[1,20], w=1 -> covers 1-2 and 19-20, missing 3-18 (16 apartments) -> need ceil(16/3)=6
    assert(minAdditionalStations(20, {1, 20}, 1) == 6);

    // Example 9: w=0, stations cover some, then need exact count of missing apartments
    assert(minAdditionalStations(10, {2, 5, 8}, 0) == 7); // missing 1,3,4,6,7,9,10

    // Example 10: Large n but few stations, w=0
    assert(minAdditionalStations(1000000, {500000}, 0) == 999999);

    return 0;
}

#include <vector>
#include <cmath>  // for std::ceil, but we use integer arithmetic to avoid floating-point

int minAdditionalStations(int n, const std::vector<int>& stations, int w) {
    int answer = 0;
    const int coverage = 2 * w + 1;  // maximum apartments one new station can cover

    // Build virtual boundaries to handle the start and end of the line.
    // prev represents the rightmost covered boundary of the previous station.
    // We initialize prev to be the coverage end of a virtual station at position 0 - w,
    // which makes the first uncovered segment start at apartment 1.
    int prev = (0 - w) + w;  // = 0, but conceptually the virtual station covers up to 0.
    // Actually, the first uncovered segment starts at prev + w + 1 = 0 + w + 1 = w+1? Wait, need careful derivation.
    // Let's use the same formula as the original code: for each gap, we compute length = (b - w - 1) - (a + w + 1) + 1.
    // We set a as the previous station's position (or virtual), b as current station's position.
    // For the first gap, we set a = 0 - w, so that a + w + 1 = 1, which is the first apartment.
    // For the last gap, we set b = n + w + 1, so that b - w - 1 = n, the last apartment.
    // So we can just loop over a vector that includes these virtual boundaries.

    // But to avoid copying, we can process on the fly.
    // We'll keep previous position (virtual or real) in `prevPos`.
    int prevPos = 0 - w;  // virtual station before the start

    for (int station : stations) {
        // Gap between prevPos and current station
        int leftUncovered = prevPos + w + 1;      // first uncovered apartment in this gap
        int rightUncovered = station - w - 1;     // last uncovered apartment in this gap
        int uncoveredCount = rightUncovered - leftUncovered + 1;
        if (uncoveredCount > 0) {
            // Number of needed stations = ceil(uncoveredCount / coverage)
            answer += (uncoveredCount + coverage - 1) / coverage;
        }
        prevPos = station;
    }

    // Gap after the last station
    int leftUncovered = prevPos + w + 1;
    int rightUncovered = n;  // since b = n + w + 1, b - w - 1 = n
    int uncoveredCount = rightUncovered - leftUncovered + 1;
    if (uncoveredCount > 0) {
        answer += (uncoveredCount + coverage - 1) / coverage;
    }

    return answer;
}

// The key observation is that between any two existing base stations (including the virtual boundaries before the first and after the last apartment), there is a contiguous segment of apartments that are not covered by any existing station. For two consecutive existing stations at positions `a` (previous, or virtual lower bound) and `b` (current, or virtual upper bound), the uncovered segment starts at `(a + w + 1)` and ends at `(b - w - 1)`. The length of this segment is `L = (b - w - 1) - (a + w + 1) + 1 = b - a - 2*w - 1`. If `L <= 0`, no additional stations are needed for that segment. Otherwise, each new station covers at most `2*w + 1` apartments (since it must be placed within [1,n] but we only care about the uncovered segment, and placing it optimally inside that segment covers exactly that many). Therefore, the minimum number of new stations for that segment is `ceil(L / (2*w + 1))`. We initialize virtual boundaries: before the first station, treat `a = 0 - w` (so that the first uncovered segment starts at `1` because `a + w + 1 = 1`); after the last station, treat `b = n + w + 1` (so that the last uncovered segment ends at `n` because `b - w - 1 = n`). We iterate over the expanded list of virtual and real stations, compute `L` for each gap, and add `ceil(L / (2*w + 1))`. Edge cases: if `stations` is empty, we compute the single segment from 1 to n. If `w` is very large, `L` may be negative, which we skip. Time complexity is O(m) where m is the number of stations, and space is O(1) extra (excluding input storage). The algorithm is robust for large `n` because it never iterates over individual apartments.
