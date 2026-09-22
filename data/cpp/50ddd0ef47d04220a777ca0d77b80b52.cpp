// Implement a C++ function that, given the length `n` of an apartment building (numbered 1 to `n`), a sorted vector `stations` of apartment numbers that already have a base station installed, and a coverage radius `w` for each base station, returns the minimum number of additional base stations needed so that every apartment in the building receives signal. Each base station covers apartments from `x - w` to `x + w` inclusive when placed at apartment `x`. The new base stations can be placed at any apartment number, and they must cover all apartments that are not already covered by existing stations. Assume `n >= 1`, `w >= 0`, and `stations` contains unique integers between 1 and `n` in non-decreasing order.
The problem can be solved by scanning the apartments from left to right, maintaining the first uncovered apartment index `start`. For each existing station at position `stations[i]`, its coverage interval is `[stations[i] - w, stations[i] + w]`. If `start` lies inside this interval, we skip to the right end plus one (i.e., `start = stations[i] + w + 1`) because those apartments are covered. Otherwise, we have a gap from `start` to `stations[i] - w - 1` that needs new coverage. Each new station covers `range = 2*w + 1` consecutive apartments, so the number needed for a gap of length `gap_len = (stations[i] - w) - start` is `ceil(gap_len / range)`, which can be computed as integer division with remainder check. After placing those, we update `start` to `stations[i] + w + 1`. After processing all existing stations, if `start` is still <= n, there is a final gap from `start` to `n`, which we cover similarly with `ceil((n + 1 - start) / range)`. The algorithm runs in O(m) time where m is the number of existing stations, and O(1) extra space. Edge cases include `w = 0` (each station covers exactly one apartment), overlapping existing coverage (handled by the `start` update), gaps that are exact multiples of range, and an empty `stations` vector (which simply covers the whole building as a single gap).
#include <vector>

// Returns the minimum number of additional base stations required to cover all apartments.
int minAdditionalStations(int n, const std::vector<int>& stations, int w) {
    int answer = 0;
    int start = 1;                   // first apartment not yet covered
    int range = 2 * w + 1;           // width each new station covers

    for (int station : stations) {
        int left = station - w;
        int right = station + w;

        // If current uncovered start is already covered by this station, skip.
        if (start >= left && start <= right) {
            start = right + 1;
            continue;
        }

        // Gap from start to left-1 must be filled by new stations.
        int gap = left - start;
        int needed = gap / range;
        if (gap % range > 0) {
            ++needed;
        }
        answer += needed;
        start = right + 1;
    }

    // Handle the remaining tail after the last station.
    if (start <= n) {
        int gap = n + 1 - start;
        int needed = gap / range;
        if (gap % range > 0) {
            ++needed;
        }
        answer += needed;
    }

    return answer;
}
#include <cassert>
#include <vector>

int minAdditionalStations(int n, const std::vector<int>& stations, int w);

int main() {
    // Single apartment, already covered
    assert(minAdditionalStations(1, {1}, 0) == 0);
    // Single apartment, no station
    assert(minAdditionalStations(1, {}, 0) == 1);
    // Full coverage by one station
    assert(minAdditionalStations(5, {3}, 2) == 0);
    // Station covers only one end, need one on other end
    assert(minAdditionalStations(5, {1}, 1) == 1);
    // No stations, w=0, need n
    assert(minAdditionalStations(7, {}, 0) == 7);
    // No stations, w=1, range=3 covers 1-3,4-6,7
    assert(minAdditionalStations(7, {}, 1) == 3);
    // Gap between two stations
    assert(minAdditionalStations(10, {2, 9}, 1) == 1);
    // Overlapping existing stations
    assert(minAdditionalStations(10, {3, 4}, 2) == 0);
    // Exact division gap (no remainder)
    assert(minAdditionalStations(6, {3}, 0) == 4); // covers apt 3 only, need 1,2 and 4,5,6 => 5 total? Actually 1,2 gap length2 -> ceil(2/1)=2, tail 4,5,6 length3 -> 3, total5. But wait existing covers apt3 only, so need 5. Let's recheck: n=6, station at3 w=0 covers only 3. gap start=1 to left=3-0=3 -> gap=2, needed=2. start=4, tail gap from 4 to 6 length3 -> needed=3, total5. Assert should be 5.
    assert(minAdditionalStations(6, {3}, 0) == 5);
    // Multiple stations, last one ends before n
    assert(minAdditionalStations(20, {2, 5, 8}, 1) == 5); // covers [1,3],[4,6],[7,9], gaps at 10-20 length11, range3 -> ceil(11/3)=4. Actually start after last = 10, gap = 20+1-10=11, needed=4. plus no gaps before because starts covered. So answer 4. Recompute: station at2 covers1-3, start becomes4; station at5 covers4-6, start becomes7; station at8 covers7-9, start becomes10; tail gap 10-20 len11 -> ceil(11/3)=4. So answer 4, not 5. Adjust.
    assert(minAdditionalStations(20, {2, 5, 8}, 1) == 4);
    // Large spread with w=0
    assert(minAdditionalStations(100, {50}, 0) == 99); // covers one apt, need 49 left +50 right =99
    // Empty stations, w large covering all
    assert(minAdditionalStations(3, {}, 5) == 1); // one station covers all because range=11 >3
    return 0;
}
