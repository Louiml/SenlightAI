Given a sequence of positive integers and sentinel values `-1` (representing unknown elements), write a C++ function `int minimalSegments(const std::vector<int>& a)` that returns the minimum number of contiguous segments into which the whole array can be partitioned, such that within each segment, all known values lie on a single arithmetic progression (constant difference), and for every position in the segment the computed value (from that progression) is strictly positive. If a single segment cannot cover the whole array under these rules, you must split it into the smallest possible number of valid segments. The function receives a vector whose first element (index 0) is unused; valid indices are 1..n, where n = a.size() - 1. The array always starts at index 1, and `-1` values may appear anywhere. You may assume that for any segment that contains at least two known values, those values are compatible with some arithmetic progression; your function must still correctly determine whether the progression yields positive values at all positions. For segments with zero or one known value, the segment is always valid (since you can choose a positive progression). Edge case: if the entire array cannot be partitioned into valid segments (though constraints prevent this), return 0. The function must be self-contained and not rely on global variables.

// The problem is a greedy partitioning problem. We traverse the array from left to right, trying to extend the current segment as far as possible. For the current segment starting at `start`, we maintain:
// - The first and second known (non-`-1`) indices, `first` and `second`, along with the common difference `d = (a[second] - a[first]) / (second - first)` once we have two known values.
// - A flag `hasTwo` to know if we have fixed the progression.
//
// While scanning `i` from `start` to `n`:
// - If we see a `-1`, we ignore it unless we already have two known values; in that case, we check that the computed value at position `i` is > 0. If not, the segment cannot be extended, so we cut before `i` (i.e., start the next segment at `i`).
// - If we see a known value:
//   - If we have not yet seen any known value, set `first = i`.
//   - Else if we have seen one known value (`second` not set), set `second = i`, compute `d`, and also check that the value at position `first` (which is also `-1` possibly? Actually if first is known, it's the first known; if the segment starts with a `-1`, first might be later. When we set `second`, we must also verify that if there was a leading `-1` before `first`, the value at that leading position is positive. However, in the code the check is done only when `first == start` (i.e., the segment's first element is known). If the segment starts with `-1`, then that position's value is unknown, but we can choose the progression to make it positive, so no issue.
//   - Else (we already have two known values), check that `a[i] == d*(i - second) + a[second]`. If not, cut before `i`.
// - Additionally, if we have two known values, for every later position (known or `-1`), we must ensure the computed value > 0. For known values, that positivity is implied if they are positive (given input constraints). But for `-1` positions, we must check.
//
// When we find a violation, we increment segment count, and set `start = i` (the current index that caused the failure), reset the tracking variables, and continue. If we reach the end without a violation, we finish with one more segment.
//
// Time complexity: O(n) because each index is processed once per segment attempt, but since we reset and move `start` forward, each index is visited at most twice (once in a failed attempt, once as part of a new segment). Space: O(1) auxiliary.
//
// Important edge cases:
// - Empty array? The problem states the array has at least one element (index 1..n). But handle n=0 gracefully.
// - Segment with only one known value: valid regardless of position.
// - Segment with two known values where the progression yields non-positive values at some earlier `-1` positions: must cut.
// - If the first element is `-1`, the first known value might be later; when we set the second known, we need to check if the progression yields positive values for all earlier `-1`s? Actually for `-1`s before the first known, we can choose the progression to make them positive? Wait, the progression is fixed by the two known values. So if the first known is at index `first` and there is a `-1` before it, the value at that preceding position is determined by `d*(pos - first) + a[first]`. That could be non-positive. But in the original code, they only check that if `fr==head` (i.e., the segment starts with a known value) then they check `a[first] - (first - head)*d` which is the value at the segment start. If the segment starts with `-1`, the value at that start is not checked because it can be chosen? Actually no, the progression is fixed once two knowns are found; the value at the start (if `-1`) is also determined. So we must also check that for all positions from `start` to `first-1`, the computed values are positive. The original code only checks the immediate start when `fr==head`. But for a robust solution, we should check all positions in the segment. However, for simplicity, the problem statement says "for every position in the segment (from the computed progression) is strictly positive." So we must enforce that. Since the progression is linear, if the value at the segment start (position `start`) is positive and the common difference is non-negative, all are positive; but if d is negative, later values might become non-positive. So we must check the farthest position (the last index of the segment). Actually, the most stringent is the last position. But we don't know the end yet. So we can check for each `-1` or known when we encounter it. For known values, they are positive by input. For `-1`, we compute and check. For the leading `-1`s before the first known, we must check them after we have two knowns. So in our solution, we can, once we have two knowns, go back and check all positions from `start` to `first-1`. But that would make it O(n^2). Instead, we can maintain a flag that we haven't yet validated leading unknowns, and when we get the second known, we check the segment start. Since the progression is linear, checking the segment start is sufficient because if the value at the start is positive and d is positive, all are positive; if d is negative, then later values decrease, so the minimum is at the end, but the end is not fixed. So we need to check the farthest point, which is the current `i`. So we can check at each step. For leading unknowns, we can check the farthest unknown position, which is the current `i`. So we can do: when we have two knowns, for any index `j` from `start` to `i`, we must ensure `d*(j - first) + a[first] > 0`. The worst is either the smallest (start) or the largest (current i) depending on sign of d. So we can check both endpoints. But to keep it simple, we can check the value at the current position `i` (which is either known or -1) and also check the value at the segment start when we first get two knowns. Actually, after we get two knowns, we know `d`; we can check the value at `start` (if `start` is -1) because that is the smallest index in the segment. That is sufficient for the leftmost, but the rightmost we check as we iterate. So we'll implement that.
//
// Thus, the algorithm: For each segment, keep `first` and `second` (indices) and `hasTwo`. When we get the second known, compute `d`, and if `start` is less than `first` (i.e., there are leading -1s), check that computed value at `start` is > 0. Then as we scan forward, for each position `i` (whether known or -1), if `hasTwo`, compute value and if <=0, cut. For known values, we also check they match the progression. When a cut occurs, start new segment at `i` (the current position that caused the violation), and reset.
//
// Edge: If the array has only one element, the answer is 1. If the array has two known values that yield non-positive at some point, we split.
//
// Let's test with a few examples. We'll implement `minimalSegments`.

#include <vector>
#include <algorithm>

// Returns the minimum number of valid arithmetic-progression segments.
// The input vector a has valid data from index 1 to a.size()-1.
// -1 represents an unknown value; known values are positive integers.
int minimalSegments(const std::vector<int>& a) {
    int n = static_cast<int>(a.size()) - 1;
    if (n <= 0) return 0;

    int segments = 0;
    int start = 1;

    while (start <= n) {
        int first = 0;      // index of first known value in this segment
        int second = 0;     // index of second known value
        long long d = 0;    // common difference
        bool hasTwo = false;
        bool broken = false;
        int i = start;

        while (i <= n) {
            if (a[i] != -1) {
                if (first == 0) {
                    first = i;
                } else if (second == 0) {
                    second = i;
                    if ((a[second] - a[first]) % (second - first) != 0) {
                        // Not an integer difference, but problem guarantees compatibility.
                        // Still handle defensively: cut before i.
                        broken = true;
                        break;
                    }
                    d = (a[second] - a[first]) / (second - first);
                    hasTwo = true;
                    // Check leading -1 positions from start to first-1.
                    // The most critical is the segment start.
                    if (start < first) {
                        long long valAtStart = a[first] + d * (start - first);
                        if (valAtStart <= 0) {
                            broken = true;
                            break;
                        }
                    }
                } else {
                    // Already have two knowns: check consistency.
                    long long expected = a[second] + d * (i - second);
                    if (expected != a[i]) {
                        broken = true;
                        break;
                    }
                }
            } else {
                // a[i] == -1
                if (hasTwo) {
                    long long val = a[second] + d * (i - second);
                    if (val <= 0) {
                        broken = true;
                        break;
                    }
                }
            }
            ++i;
        }

        ++segments; // count this segment
        if (broken) {
            start = i; // i is the position that caused the break
            // But careful: if the loop broke because of i, that i belongs to the next segment.
        } else {
            break; // finished whole array
        }
        if (start > n) break;
    }

    return segments;
}

#include <cassert>
#include <vector>

int minimalSegments(const std::vector<int>& a);

int main() {
    // ["-1", "2", "4", "6"] all fit one arithmetic progression with d=2.
    std::vector<int> t1 = {0, -1, 2, 4, 6};
    assert(minimalSegments(t1) == 1);

    // ["-1", "2", "4", "-1", "10"]: first segment [1,4]? Actually check: 
    // first known=2 at index2, second=4 at index3, d=2, then at index4 (-1) value=6 (>0), at index5 known=10 -> expected 8, not match. So cut at index5. Then segment [5,5] valid. So ans=2.
    std::vector<int> t2 = {0, -1, 2, 4, -1, 10};
    assert(minimalSegments(t2) == 2);

    // Single element
    std::vector<int> t3 = {0, 5};
    assert(minimalSegments(t3) == 1);

    // All -1, any segment valid, so one segment.
    std::vector<int> t4 = {0, -1, -1, -1};
    assert(minimalSegments(t4) == 1);

    // Two knowns that force negative at start: e.g., [6, 3] with d=-3, starting at -1 before first known.
    // Segment [1,3]? Let's construct: index1=-1, index2=6, index3=3. d=-3. Value at index1 = 6 + (-3)*(1-2)=9>0, so fine. But if index1=-1, index2=2, index3=1? d=-1, value at index1=3>0. Hard to get negative with positive knowns and small indices. But test case: start at index2, known 2 and 1, d=-1, leading -1 at index1 gives 3>0. So not an issue.
    // However, test with long negative difference: a = {0, 100, -1, 90}? But known values are positive, but 90 positive. d=-10, at index2 value=100, at index3=-1 -> value=90>0. So fine.
    // To test the negative start check, we need a case where the progression yields negative at the start. Since known values are positive and indices increase, if d is negative, the start (smaller index) will have larger value, so positive. If d is positive, the start is smaller, but knowns are positive, so start is positive if the known at `first` is positive and d*(start-first) may be negative if start<first and d positive? Wait start<first, start-first negative, so d*(negative) negative, so start value = a[first] + d*(start-first) could be negative if d positive large and first far from start. Example: start=1, first=10 (index 10 known=100), d=1, then value at start = 100 + 1*(-9)=91>0. Not negative. To get negative, need a[first] small and d positive but first far? e.g., a[10]=1, d=1 => value at 1 = 1 + 1*(-9)=-8 <0. So that should cause a break. Let's build: indices 1..10, a[10]=1, a[9]? must be second known to determine d. If a[9]=0? but knowns must be positive, so a[9]=? Actually both knowns must be positive. If a[9]=? Let's choose a[9]=2, a[10]=1, then d = (1-2)/(10-9) = -1, not positive. To get d positive, need increasing. Let's do a[5]=2, a[10]=7 (d=1), then value at start=1 is 2 + 1*(1-5)=-2 <0. So input: a[1]=-1, a[2..4]=-1, a[5]=2, a[6..9]=-1, a[10]=7. That should break because leading -1 yields negative. So expected segments: first segment can't include these two knowns, so break at index? When we get second known at 10, we check start=1, get -2 <=0, we cut. So the segment breaks, we increment segments, and start becomes i=10? Actually we break because we detect the leading negative when we set second. At that point, we haven't processed index 10 yet? We are processing i=10 (since second becomes 10). The break sets broken=true, and then after loop, segments++ and start = i (which is 10). Then next segment starts at 10, which is a known 7, valid. So total segments = 2? But wait, we also have known at 5, which was in the first segment attempt but got discarded. Actually the first segment attempt starts at 1, scans 1..10, finds first known at 5, second at 10, computes d=1, checks start=1 gets -2, so breaks at i=10. Then segments++ (1), start=10. Next segment starts at 10, only one known (7), valid, end, segments++ (2). So answer 2. Is that minimal? Could we split differently? Maybe segment [1,4] (all -1) and [5,10] with progression from 2 to7? But [5,10] includes -1 at 6..9, and the progression gives 3,4,5,6 at those, all positive, so valid. That would be 2 segments as well. So answer 2. Let's test.
    std::vector<int> t5 = {0, -1, -1, -1, -1, 2, -1, -1, -1, -1, 7};
    assert(minimalSegments(t5) == 2);

    // Test where two knowns incompatible (shouldn't happen per constraints but we handle).
    // Not needed.

    return 0;
}
