Given the dimensions `L`, `W`, `H` of a rectangular prism (all positive integers) and a list of `N` cube sizes (each size is a power of two, specified by an exponent `a` giving side length `2^a`, and a count `b` of available cubes of that size), write a C++ function `countCubesNeeded` that returns the minimum number of cubes required to exactly fill the prism, or `-1` if it is impossible. Cubes may only be placed axis-aligned, and you may use at most `b` cubes of each size. The function should try to use the largest possible cubes first, and if a placement fails (cannot fit any remaining cube into some sub‑space), it should return `-1`. Assume `N >= 1`, all counts `b` are non‑negative, and the prism dimensions are up to `10^5`.

The problem is a classic greedy filling of a rectangular box with cubes of sizes that are powers of two. The key insight is that because all side lengths are powers of two, a greedy strategy of always using the largest cube that fits into the current sub‑box is optimal. The algorithm recursively partitions the space: after placing a cube of side `s` at the corner of a sub‑box of dimensions `(l, w, h)`, the remaining volume can be decomposed into three non‑overlapping rectangular blocks: `(l-s, w, h)`, `(s, w-s, h)`, and `(s, s, h-s)`. The recursion proceeds on each block in order, using the same cube index (or lower) because larger cubes are attempted first. If at any point no cube (from the current index down to size 1) fits into the current sub‑box, the filling is impossible, and the function returns `-1`. Edge cases: if any dimension is zero, that sub‑box contributes nothing; if the cube count for a size is exhausted, it is skipped; if the exponent `a` gives a side length larger than the smallest dimension, it cannot fit. The total number of cubes used is accumulated. Time complexity is O(number of cubes placed * N) in the worst case (since each recursion scans down the cube list), but because sizes are powers of two and we always pick the largest, the number of recursive calls is proportional to the number of cubes placed, which is at most O(L*W*H / minCubeSize) but practically limited by the input counts. Space complexity is O(N) for the cube list plus recursion depth proportional to the number of different cube sizes used (at most N).

#include <vector>
#include <algorithm>

using Cube = std::pair<int, int>; // first = side length, second = count available

// Recursive helper: attempt to fill a box of dimensions (l, w, h) using cubes from index 'idx' down to 0.
// Returns true and updates 'ans' if successful, otherwise returns false.
bool fillBox(int l, int w, int h, int idx, std::vector<Cube>& cubes, long long& ans) {
    if (l == 0 || w == 0 || h == 0) return true;

    for (int i = idx; i >= 0; --i) {
        int s = cubes[i].first;
        if (cubes[i].second > 0 && s <= l && s <= w && s <= h) {
            cubes[i].second--;  // use one cube
            ans++;              // count it
            // Partition remaining space into three non-overlapping boxes
            bool ok = true;
            // Box 1: (l - s, w, h) – after removing the cube from the front
            if (!fillBox(l - s, w, h, i, cubes, ans)) ok = false;
            // Box 2: (s, w - s, h) – side strip
            if (ok && !fillBox(s, w - s, h, i, cubes, ans)) ok = false;
            // Box 3: (s, s, h - s) – top strip
            if (ok && !fillBox(s, s, h - s, i, cubes, ans)) ok = false;
            if (ok) return true;
            // If any sub-box failed, backtrack: we cannot place this cube here.
            cube[i].second++;  // restore count
            ans--;              // undo count
        }
    }
    return false; // no cube fits
}

// Given box dimensions L, W, H and a list of (exponent, count) pairs,
// return the minimum number of cubes needed or -1 if impossible.
long long countCubesNeeded(int L, int W, int H, const std::vector<std::pair<int, int>>& input) {
    // Build cube list: side length = 2^a, count = b
    std::vector<Cube> cubes;
    for (const auto& p : input) {
        int side = 1 << p.first; // 2^a
        cubes.emplace_back(side, p.second);
    }
    // Sort by side length ascending? Our recursive function expects descending index.
    // So sort descending by side length for easier scanning.
    std::sort(cubes.begin(), cubes.end(), [](const Cube& a, const Cube& b) {
        return a.first > b.first; // largest first
    });

    long long ans = 0;
    bool success = fillBox(L, W, H, static_cast<int>(cubes.size()) - 1, cubes, ans);
    return success ? ans : -1;
}

#include <cassert>
#include <vector>
#include <utility>

// Include the solution function declaration here (or paste the code above)
// This test harness directly calls countCubesNeeded.

int main() {
    // Case 1: Simple 2x2x2 box, one cube of size 2 (exponent 1) count 1.
    assert(countCubesNeeded(2, 2, 2, {{1, 1}}) == 1);

    // Case 2: 4x4x4 box, only size 2 cubes (exponent 1) – need 8 cubes.
    assert(countCubesNeeded(4, 4, 4, {{1, 100}}) == 8);

    // Case 3: 4x4x4 box, only size 4 cube (exponent 2) count 1 – impossible.
    assert(countCubesNeeded(4, 4, 4, {{2, 1}}) == 1);

    // Case 4: 5x5x5 box, only size 2 cubes – impossible (odd dimension).
    assert(countCubesNeeded(5, 5, 5, {{1, 100}}) == -1);

    // Case 5: 8x8x8 box with mixed sizes: 8 (exp 3) count 1, 4 (exp 2) count 100.
    // Should use one 8 and then the rest 4s? Actually 8x8x8 fits exactly one 8, no leftover.
    assert(countCubesNeeded(8, 8, 8, {{3, 1}, {2, 100}}) == 1);

    // Case 6: 8x8x8 box, only size 4 cubes – need 8 cubes.
    assert(countCubesNeeded(8, 8, 8, {{2, 100}}) == 8);

    // Case 7: 1x1x1 box, need a size 1 cube (exponent 0) count 1.
    assert(countCubesNeeded(1, 1, 1, {{0, 1}}) == 1);

    // Case 8: 3x3x3 box, only size 1 cubes – need 27.
    assert(countCubesNeeded(3, 3, 3, {{0, 100}}) == 27);

    // Case 9: 6x6x6 box, available size 4 (exp 2) count 1, size 2 (exp 1) count 100.
    // Place one 4 in corner, leftover is 2x6x6? Actually careful: 6x6x6 with a 4 cube leaves
    // partitions that can be filled with 2s? Let's just test impossible because 6 not divisible by 2? Actually 6 is divisible by 2, but 4 doesn't divide 6 evenly. Let's see: one 4 in corner leaves (2,6,6), (4,2,6), (4,4,2). Each of those can be filled with 2s? (2,6,6) → 2x6x6 can be filled with 2s? yes, 3*3*3=27 cubes? Wait 2x6x6 → 6*6/4 = 9 cubes of size 2? Actually 2x6x6 is 72 volume, each 2-cube volume 8 → 9 cubes. They fit because 6 is multiple of 2. So total cubes = 1 + 9 + (4,2,6) → 4*2*6=48 volume /8=6 cubes, (4,4,2) → 32/8=4 cubes. Total 1+9+6+4=20. But is it optimal? Greedy works because powers of two. We'll test that answer is 20.
    assert(countCubesNeeded(6, 6, 6, {{2, 1}, {1, 100}}) == 20);

    // Case 10: Empty input? Not allowed per spec (N>=1), but test with zero count for all.
    assert(countCubesNeeded(2, 2, 2, {{1, 0}}) == -1);

    return 0;
}
