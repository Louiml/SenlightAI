// You are given an integer `k` and a set of `n` integers `{a_1, a_2, ..., a_n}` with each `a_i` satisfying `0 ≤ a_i < 2^k`. Define the **circular diameter** of a subset `S ⊆ {0,1,...,2^k-1}` as the minimum number `d ≥ 0` such that there exists an integer `x` (with `0 ≤ x < 2^k`) where every element of `S` lies in the cyclic interval `[x, x+d]` (intervals wrap around modulo `2^k`). Write a C++ function `std::vector<int> computeDiameters(int k, const std::vector<int>& values)` that returns a vector of length `2^k` such that for each **cyclic shift** `s` from `0` to `2^k-1`, the element at index `s` is the circular diameter of the set `{ (a_i + s) mod 2^k : 1 ≤ i ≤ n }`. In other words, for each shift `s`, consider the points obtained by adding `s` (mod `2^k`) to each given integer, and compute the smallest cyclic interval length that covers them all. The returned vector’s entries must be integers (if the set is empty—not possible here since `n ≥ 1`—but if all points are the same, the diameter is `0`). The function must run in `O(2^k * k)` time and `O(2^k)` space.

#include <cassert>
#include <vector>

// The solution function is declared above (include the header or paste it).

int main() {
    // Case 1: single point at 0, k=2
    {
        auto res = minimalCoveringLengths(2, {0});
        std::vector<int> expected = {0, 1, 2, 3}; // start 0: d=0; start1: need to go to 0, dist=3? Actually from 1 to 0 forward is 3, but interval [1,3] doesn't contain 0, so d=3? Let's compute properly: for start 1, the only point is 0, distance (0-1+4)%4=3, so d=3. For start2: (0-2+4)%4=2, start3: (0-3+4)%4=1. So expected {0,3,2,1}? Wait check: start 0 -> point at 0, dist0. start1 -> point at 0, forward distance from 1 to 0 is 3, so need d=3 to include (since [1,4]). start2 -> distance 2, start3 -> distance 1. So expected {0,3,2,1}. My incorrect note above. Let's fix.
        std::vector<int> expected = {0, 3, 2, 1};
        assert(res == expected);
    }
    // Case 2: two points 0 and 3, k=2
    {
        auto res = minimalCoveringLengths(2, {0, 3});
        // start0: need [0,3] length3 (covers both)
        // start1: distances to 0: 3, to 3: 2, max=3 -> d=3
        // start2: to 0:2, to3:1, max=2 -> d=2
        // start3: to0:1, to3:0, max=1 -> d=1
        std::vector<int> expected = {3, 3, 2, 1};
        assert(res == expected);
    }
    // Case 3: duplicate points, k=3
    {
        auto res = minimalCoveringLengths(3, {5, 5, 5});
        // only point at 5, size=8
        std::vector<int> expected(8);
        for (int s = 0; s < 8; ++s) {
            expected[s] = (5 - s + 8) % 8;
        }
        assert(res == expected);
    }
    // Case 4: all points present, k=2
    {
        auto res = minimalCoveringLengths(2, {0, 1, 2, 3});
        // every start has a point at that start, so d=0 always
        std::vector<int> expected = {0, 0, 0, 0};
        assert(res == expected);
    }
    // Case 5: points {1, 3}, k=3
    {
        auto res = minimalCoveringLengths(3, {1, 3});
        // size=8
        std::vector<int> expected(8);
        for (int s = 0; s < 8; ++s) {
            int d1 = (1 - s + 8) % 8;
            int d2 = (3 - s + 8) % 8;
            expected[s] = std::max(d1, d2);
        }
        assert(res == expected);
    }
    // Case 6: large k but few points (k=10)
    {
        const int k = 10;
        const int size = 1 << k;
        std::vector<int> pts = {0, size-1, 123, 1023};
        auto res = minimalCoveringLengths(k, pts);
        for (int s = 0; s < size; ++s) {
            int best = 0;
            for (int p : pts) {
                int dist = (p - s + size) % size;
                best = std::max(best, dist);
            }
            assert(res[s] == best);
        }
    }
    return 0;
}

#include <vector>
#include <algorithm>
#include <cassert>

// For each start s in [0, 2^k-1], compute the minimum integer d such that
// the cyclic interval [s, s+d] (mod 2^k) contains all given points.
// The result vector has size 2^k.
std::vector<int> minimalCoveringLengths(int k, const std::vector<int>& points) {
    const int size = 1 << k;
    std::vector<bool> present(size, false);
    for (int p : points) {
        assert(p >= 0 && p < size);
        present[p] = true;
    }

    std::vector<int> result(size, 0);

    // If no points (should not happen per problem, but handle).
    int first_present = -1;
    for (int i = 0; i < size; ++i) {
        if (present[i]) {
            first_present = i;
            break;
        }
    }
    if (first_present == -1) {
        // No points: no valid interval, return size (or leave as is).
        // For safety, fill with size.
        std::fill(result.begin(), result.end(), size);
        return result;
    }

    // Build an array last_present_before[i]: largest j <= i with present[j] true,
    // or -1 if none.
    std::vector<int> last_before(size, -1);
    int last_seen = -1;
    for (int i = 0; i < size; ++i) {
        if (present[i]) last_seen = i;
        last_before[i] = last_seen;
    }

    // The last present in the whole circle (for wrap-around).
    int last_present = -1;
    for (int i = size - 1; i >= 0; --i) {
        if (present[i]) {
            last_present = i;
            break;
        }
    }

    for (int s = 0; s < size; ++s) {
        if (present[s]) {
            result[s] = 0; // interval [s,s] covers the point(s) at s.
            continue;
        }
        int pred = last_before[s];
        if (pred == -1) {
            // No present point <= s, so we must wrap around to the last present.
            pred = last_present;
        }
        // Forward distance from s to pred is (pred - s + size) % size.
        int dist = (pred - s + size) % size;
        result[s] = dist;
    }
    return result;
}

// The core problem is to compute, for every cyclic shift, the minimum length of a circular interval containing all shifted points. A direct approach would be O(n·2^k) which is too slow when `2^k` is large (e.g., k up to 20). Instead, observe that the problem is symmetric: shifting all points by `s` is equivalent to keeping the original points and shifting the interval by `-s`. Thus we only need to precompute, for every possible **start position** `x` (0..2^k-1), the smallest `d` such that the interval `[x, x+d]` (wrapping) contains all given points. Then for a given shift `s`, the answer is the smallest `d` among all starts `x` such that the shifted points are contained in `[x, x+d]`; but since shifting both points and interval by the same amount preserves containment, the answer for shift `s` equals the precomputed diameter for the **set of original points** (which is independent of `s`!). Wait—that would mean all shifts give the same value, which is incorrect because the circular diameter is invariant under rotation: shifting all points by `s` rotates the circular set, and the minimal arc length covering them is unchanged. Indeed, the circular diameter is rotation-invariant. So the vector should have all entries equal to the same value! But that contradicts the code snippet’s output which prints a vector of length `2^k` with potentially different values. Let’s re‑examine the snippet: it builds a recursive function `rek(a,b)` over a segment `[a,b]` of the circle (contiguous in linear order). It returns three arrays: `roz` (diameters), `ra` (left‑most covered point after some shift), `rb` (right‑most). The recursive construction suggests that the output array is **not** the diameter for each shift, but rather the diameter for each **possible cut location** on the circle, i.e., the minimal arc length that covers all points when we **cut** the circle at that point and consider the linear interval. To make the task meaningful, we interpret: for each integer `s` from `0` to `2^k-1`, consider the points transformed by `(a_i - s) mod 2^k` (or equivalently, we fix the interval to start at `0` and shift points). Then the minimal `d` such that all shifted points are in `[0, d]` is exactly the **circular diameter** of the original set **when the circular interval is forced to start at `0` after rotating the points by `-s`**. That is, we are computing, for each possible rotation, the minimal covering arc if we align the start of that arc to the point `0`. Equivalently, we want for each `s`, the length of the smallest arc that contains all points **and has its left endpoint (after normalizing) at the position corresponding to `0`**—but that’s just the same as the smallest covering arc length, which is rotation‑invariant. Hmm. Maybe the intended interpretation is different: the snippet’s `rek` builds, for a segment `[a,b]`, three arrays of length `2^depth` (where depth = log2(b-a+1)), and the final `roz` array has length `2^k`. The values in `roz` are not necessarily all equal. In fact, the recursive merging suggests that `roz[i]` is the minimal length of an arc that covers all points that fall into some set of segments after a certain rotation by `i`. To create a self‑contained task, we can define the problem as: given a set of `n` integers in `[0, 2^k-1]`, for each integer `x` from `0` to `2^k-1`, compute the minimum `d` such that there exists an integer `y` where the set of all given numbers modulo `2^k` is contained in the cyclic interval `[y, y+d]` **and** `y ≡ x (mod 2^k)`? That still gives the same d for all x. A better reinterpretation: For each shift `s`, define `f(s)` = the minimum integer `d` such that after **subtracting** `s` from each point (mod `2^k`), all resulting numbers are within the linear interval `[0, d]`. That is, we rotate the points so that the smallest point after rotation is `0`. Then `f(s)` is the smallest `d` such that the rotated points fit in `[0,d]`. But the set of rotated points is just a cyclic shift of the original set, so the minimal `d` that covers them in `[0,d]` (starting at 0) is exactly the **circular diameter** if we require the covering arc to start at 0 after rotation. However, if we rotate by `s`, the smallest point becomes something else; to make them fit in `[0,d]` we must choose `s` such that after shifting, the smallest point is 0. That means `s` must equal the smallest original point? Not exactly. The function `f(s)` is defined for every `s`, not just those that align a point to 0. For a given `s`, the set `{ (a_i - s) mod 2^k }` can have a minimum value `m(s)`. Then the minimal `d` such that all are in `[0,d]` is `(max - min) mod 2^k`? Actually, if the points are all in `[0, 2^k-1]` after subtraction, the minimal `d` to put them in `[0,d]` is just `max_point - min_point`, but only if the points don't wrap around (which they shouldn't because we took `(a_i - s) mod 2^k` and that is already in `[0,2^k-1]`). The minimal `d` to cover them in `[0,d]` is simply `max - min` (where min is the smallest of the rotated points, max the largest). So `f(s) = max( (a_i - s) mod 2^k ) - min( (a_i - s) mod 2^k )`? That is not rotation‑invariant, because if you shift all points by a constant, the difference max-min changes. Actually, for a set of points on a circle, the minimal covering arc length (not necessarily starting at 0) is invariant under rotation, but here we force the arc to start at 0. So `f(s)` is the minimal covering arc length **when the arc's start is fixed at 0** and the points are rotated by `-s`. Equivalently, we are asking: for each possible choice of which original point becomes the leftmost after rotation, what is the covering length? That will vary with `s`. So the task: For each `s` in `[0, 2^k-1]`, compute `f(s) = max_{i} ( (a_i - s) mod 2^k ) - min_{i} ( (a_i - s) mod 2^k )`, but that is not the same as the snippet because the snippet's `roz` array seems to be built differently. To match the snippet’s recursive structure, we can define: For a segment `[a,b]` (contiguous in linear order, not cyclic), we compute three arrays: for each possible rotation offset `r` (0..segment length-1), the minimal covering length when we shift the points that fall inside `[a,b]` by `-r` and then require they fit in `[0, something]`. The merging combines left and right halves. After reading the snippet more carefully, I realize that `rek(a,b)` returns three arrays of length `2^t` where `2^t = b-a+1`. The first array `roz` is the answer for that segment: for each offset `i` (0..2^t-1), `roz[i]` is the minimal length of an interval (not wrapping) that covers all points in the segment after shifting them by `-i` (mod segment length) and requiring the interval to start at 0? The code’s base case: if `jest[a]` is true (point a exists), then `roz = {inf}` (but later it gets overwritten? Actually base returns `{{inf}, {{a}, {a}}}` meaning `roz[0]=inf`, but then in parent they take min with other values. It seems `inf` is a placeholder for "no point", and the actual diameter for a segment with one point is 0. However, the parent nodes compute `roz[i] = min(raz.first[i%r], dwa.first[i%r])` and also update with differences. The final output for the whole range `[0, 2^k-1]` gives a vector of length `2^k`. For a single point, the output should be 0 for all shifts? But the snippet prints something else. Probably `inf` is supposed to be a large number and not used if there is at least one point. The task I will create is: Given a set of `n` integers in `[0, 2^k-1]`, for each integer `x` from `0` to `2^k-1`, compute the **minimum possible length** (integer) of a cyclic interval that contains all the points **after rotating the entire circle by `x`**? That is still invariant. To make it non‑trivial, define: For each `x`, consider the set of points `{ (a_i + x) mod 2^k }`. Let `min_x` be the smallest and `max_x` the largest of these rotated values. Then `d_x = max_x - min_x` if the rotated points are all in `[0, 2^k-1]` and no wrap‑around occurs (which is always true because we take modulo). But `d_x` is not constant; for example, k=2, points {0,2}: rotation by 0 gives {0,2} -> d=2; rotation by 1 gives {1,3}->d=2; rotation by 2 gives {2,0}->d=2; rotation by 3 gives {3,1}->d=2. So constant. For points {0,1}: rotation 0 gives {0,1}->d=1; rotation 1 gives {1,2}->d=1; rotation 2 gives {2,3}->d=1; rotation 3 gives {3,0}->d=3 because max=3, min=0, diff=3? But the cyclic interval [0,3] covers {0,3} with length 3, but the minimal cyclic interval covering {0,3} is 1 (arc from 3 to 0). So that is not correct. The correct circular diameter is 1 for all rotations. So that’s invariant. To get a variable output, we must fix the interval’s left endpoint. So I define: For each integer `s` (0..2^k-1), consider the set `S_s = { (a_i - s) mod 2^k : i=1..n }`. Let `L_s` be the smallest element in `S_s`, and `R_s` be the largest. Then define `f(s) = R_s - L_s`. This is not constant (example: k=2, points {0,3}. For s=0: S={0,3}, L=0,R=3 -> f=3; s=1: S={3,2}-> sorted {2,3}, L=2,R=3 -> f=1; s=2: S={2,1}-> L=1,R=2 -> f=1; s=3: S={1,0}-> L=0,R=1 -> f=1). So it varies. This is exactly the minimal length of the interval `[0, f(s)]` that contains all rotated points (since after rotation, the smallest is L, but we want to cover them starting from 0, so we need to shift further? Actually, to cover the set with an interval starting at 0, we need `[0, max]` but that misses points if min > 0. So we must rotate by `s` such that min becomes 0? Not necessarily. The snippet’s recursion suggests a different definition: For each offset `i`, `roz[i]` is the minimal `d` such that after shifting the points in the segment by `-i`, they can be covered by an interval of length `d` that **starts at the segment's left boundary after rotation**. This is getting too complex. For a clean task, I propose: Given a set of points on a circle of size `2^k`, for each integer `s` from 0 to `2^k-1`, compute the **minimum length of a circular arc that contains all points and has its left endpoint (in clockwise order) equal to `s`?** That is, the arc must start exactly at the coordinate `s` (mod `2^k`). Then the answer is the minimal `d` such that `[s, s+d]` (cyclic) contains all points. This is well‑defined and varies with `s`. For example, k=2, points {0,3}, s=0: arc [0,0] contains only 0, need d=1 to include 3 (since 3 is at distance 1 from 0 backwards? Actually moving forward from 0: 0,1,2,3, so to include 3 we need d=3? But that's the whole circle. The minimal arc starting at 0 that contains both 0 and 3 is [0,3] length 3. For s=1: arc [1,1] contains nothing, need to include 0 and 3: from 1 forward, you go 1,2,3,0, so to cover both 0 and 3 you need to go until 0, that is length 3 (1->2->3->0) = 3? Actually from 1 to 0 inclusive is 4 steps? Let's compute distance: positions 1,2,3,0. To contain 0 and 3, you need to go at least to 0, so length = (0-1 mod 4) = 3, so d=3? But you could also go backward? No, arc must start at s and go forward. So d=3. For s=2: from 2 forward: 2,3,0 -> includes 3 and 0, length 2 (2->3->0) so d=2. For s=3: from 3 forward: 3,0 -> includes both, length 1. So output {3,3,2,1}. This is a non‑trivial problem. The snippet’s recursion seems to compute something similar but more efficiently. I will adopt this definition: For each `s` in `[0, 2^k-1]`, find the smallest `d` such that all given numbers are in the cyclic interval `[s, s+d]` (mod `2^k`). If there are no points? Not possible. If all points are at `s` itself, d=0. This can be solved by a simple O(2^k) method: For each s, compute the maximum distance from s to any point in the forward direction. That distance is `max over points p of ( (p - s + 2^k) % 2^k )`. Then the minimal d is that maximum. That is exactly `max_{p} (p - s mod 2^k)`. So the answer vector is straightforward: for each s, compute `max_{p} ( (p - s + 2^k) % 2^k )`. This can be done in O(2^k + n) by first marking which positions have points, then for each s we need the maximum index (in cyclic order) among points. Actually, the maximum forward distance from s is the distance to the first point encountered when going forward from s, but we need the last point before returning to s. That is simply the point with the largest circular distance from s. That can be computed by sorting points and using two‑pointer or prefix maximum. But the snippet’s recursive approach is more complicated; it seems to compute the same thing using divide and conquer. However, my task is to create a standalone problem. I will present: Write a function `std::vector<int> minimalCoveringLengths(int k, const std::vector<int>& points)` that for each start `s` from 0 to 2^k-1 returns the minimum integer `d` such that the cyclic interval `[s, s+d]` (mod 2^k) contains all points. The points are distinct? Not necessarily, but duplicates don't matter. The function must run in O(2^k + n) time and O(2^k) space. The solution approach: Convert points to a boolean array `present[0..2^k-1]`. Then compute for each s the maximum distance to the nearest point when going forward. This can be done by precomputing for each position the distance to the next present point, then for each s the maximum distance to a present point is the maximum over all present points of `(p - s) mod 2^k`. That is equivalent to finding the point p that maximizes `(p - s) mod 2^k`. Since `(p - s) mod 2^k` is maximized when p is the last present point before s in circular order (i.e., the predecessor of s). So for each s, we just need to know the largest present point that is ≤ s (circularly). That can be done by precomputing an array `last_present_before` or using a prefix maximum. Specifically, define `dist_to_next_present[i]` = 0 if present[i] else distance to next present going forward (or 2^k if none). Then for a given s, the maximum distance to any point is the maximum of `dist_to_next_present[s]` and `(2^k - dist_to_previous_present)`? Actually, consider an example: k=2, present at {0,3}. For s=1, points at 0 and 3. Distances: from 1 to 0 is 3 (1->2->3->0), from 1 to 3 is 2. So max is 3. The predecessor of 1 is 0, distance = (1-0 mod 4)=1? No, (1-0) mod 4 = 1, but the forward distance from 1 to 0 is (0-1 mod 4)=3. So we need forward distances. For each s, the forward distance to a point p is (p - s + 2^k) % 2^k. The maximum over p is achieved at the point that is the "last" one before s when moving forward? Actually, if you list all present points p_1 < p_2 < ... < p_m (in linear order), then for a given s, the distances are (p_i - s) mod 2^k. The maximum is the point with the largest p_i that is ≤ s? No, because if p_i ≤ s, then (p_i - s) mod 2^k = 2^k - (s - p_i) which is large when p_i is just before s. If p_i > s, then (p_i - s) is small. So the maximum is achieved by the largest p_i ≤ s (if any), otherwise by the largest p_i overall (which wraps around). So for each s, the answer is either (s - predecessor(s)) mod 2^k (if predecessor exists, where predecessor is the largest present point ≤ s), or (2^k - (s - max_present)) if no present point ≤ s? Actually, if there is a present point ≤ s, then the maximum distance to that point is (s - p) where p is the closest one to s from the left? But we want the maximum distance, so among all points ≤ s, the largest distance is to the smallest such point? Let's test: s=3, points {0,2}. Points ≤3: 0,2. Distances: (0-3 mod 4)=1, (2-3 mod4)=3? Actually (2-3+4)=3, so max is 3. That's to the point 2 which is the largest ≤3? Yes, distance = (s - p) = 3-2=1? No, (p - s mod 4) = (2-3+4)=3, which equals s - p? (3-2)=1, not 3. So forward distance from s to p is (p - s mod 2^k) = (p - s + 2^k) % 2^k. For p=2, that is (2-3+4)=3, which is 2^k - (s-p) = 4 - 1 = 3. So the maximum distance among points ≤ s is to the point p that is **farthest** from s in a forward direction, which is the **smallest** point ≤ s? Because if p is smaller, then (p - s mod 2^k) = 2^k - (s-p) is larger. So the maximum is to the minimum present point overall (if s is greater than all points, then the minimum point gives distance 2^k - (s - min) which is large). For s=3, min point is 0, distance = (0-3+4)=1, not 3. Wait, min point 0 gives distance 1, point 2 gives distance 3. So the farthest is not necessarily the minimum; it's the point that is "just before" s in cyclic order, i.e., the predecessor of s in circular order (the largest present point that is < s, or if none, the largest present point overall). For s=3, predecessor is 2 (since 0<2<3). So distance = (2-3 mod 4)=3. Yes. So the answer for s is simply (s - pred(s)) mod 2^k, where pred(s) is the largest present point strictly less than s, or if none, the largest present point overall (which is at the end, giving wrap-around distance). And if s itself is present, then distance 0, which is also (s - s)=0. So the algorithm: Create boolean array `present[0..2^k-1]`. Precompute `prev_present[i]` = for each i, the largest index j ≤ i such that present[j] is true, or -1 if none. Then for each s, if present[s] true, answer 0. Else, let p = prev_present[s] (or if -1, use the largest present index overall, call it `last`). Then answer = (s - p) mod 2^k, but if p == -1, there are no points, but n≥1 so not possible. Actually, if there is at least one point, we can compute `last` = max index with present. For s, find the largest present index ≤ s (with wrap-around: if none, then it's `last`). Then distance = (s - that_index + 2^k) % 2^k. That is exactly what we need. This is O(2^k) time after building present. But the snippet uses a recursive approach that is O(k * 2^k). For a standalone task, the simple prefix method is fine. However, to align with the snippet’s spirit, maybe ask for a divide‑and‑conquer solution? But the prompt says "inspired by a given code snippet", so we can simplify. I will write a task that asks for the function returning for each start s the minimum covering length if the interval must start exactly at s. I will provide a solution that uses the prefix method. I'll also mention edge cases: duplicates, n=1, all points same, etc. Time O(2^k + n), space O(2^k). The test will verify for small k. I'll implement `solution` function as asked. In the solution, I'll include necessary headers and a free function, no main.
