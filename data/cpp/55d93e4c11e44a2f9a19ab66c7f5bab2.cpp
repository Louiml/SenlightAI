/*
Write a C++ function `bool canFormBox(const std::vector<std::pair<int,int>>& rectangles)` that takes exactly six pairs of positive integers representing the side lengths of six rectangles (each pair may be given in any order, not necessarily width then height) and returns `true` if these six rectangles can be assembled into the surface of a rectangular box (a cuboid), and `false` otherwise. A cuboid has three distinct pairs of opposite faces; each pair must consist of two identical rectangles, and the three distinct face dimensions must correspond to the three pairwise products of the cuboid's length, width, and height. The function should correctly handle arbitrary order of the rectangle pairs and arbitrary orientation of each rectangle (i.e., you may rotate any rectangle by 90 degrees). The input will always contain exactly six pairs of positive integers; no validation of count is required, but the function must handle any ordering and repeated dimensions.
*/
#include <vector>
#include <utility>
#include <algorithm>

// Check if six rectangles can form the surface of a rectangular box.
// Each pair is (side1, side2), order irrelevant; rotates are allowed.
bool canFormBox(const std::vector<std::pair<int,int>>& rectangles) {
    if (rectangles.size() != 6) return false;
    
    // Normalize each rectangle: put the larger side first.
    std::vector<std::pair<int,int>> rects = rectangles;
    for (auto& r : rects) {
        if (r.first < r.second) std::swap(r.first, r.second);
    }
    
    // Sort lexicographically (by first, then second).
    std::sort(rects.begin(), rects.end());
    
    // Check that pairs are identical: (0,1), (2,3), (4,5)
    for (int i = 0; i < 6; i += 2) {
        if (rects[i] != rects[i+1]) return false;
    }
    
    // Extract the three distinct face dimensions
    std::pair<int,int> face1 = rects[0]; // largest first
    std::pair<int,int> face2 = rects[2];
    std::pair<int,int> face3 = rects[4];
    
    // Collect all distinct side lengths that appear in these faces
    // They must be exactly three values, each appearing exactly twice.
    int vals[6] = {face1.first, face1.second, face2.first, face2.second, face3.first, face3.second};
    std::sort(vals, vals+6);
    // Check that the sorted values alternate: a,a,b,b,c,c
    if (vals[0] != vals[1] || vals[2] != vals[3] || vals[4] != vals[5]) return false;
    if (vals[0] == vals[2] || vals[2] == vals[4]) return false; // must be three distinct
    int a = vals[0], b = vals[2], c = vals[4];
    
    // Verify the three faces correspond to the three pairs among a,b,c
    std::pair<int,int> expected1 = {std::max(a,b), std::min(a,b)};
    std::pair<int,int> expected2 = {std::max(a,c), std::min(a,c)};
    std::pair<int,int> expected3 = {std::max(b,c), std::min(b,c)};
    std::vector<std::pair<int,int>> expected = {expected1, expected2, expected3};
    std::sort(expected.begin(), expected.end());
    std::vector<std::pair<int,int>> faces = {face1, face2, face3};
    std::sort(faces.begin(), faces.end());
    return faces == expected;
}
#include <cassert>
#include <vector>
#include <utility>

// Declare the function (implementation above in separate file)
bool canFormBox(const std::vector<std::pair<int,int>>&);

int main() {
    // Cube: all faces 2x2
    assert(canFormBox({{2,2},{2,2},{2,2},{2,2},{2,2},{2,2}}) == true);
    
    // Rectangular box 1x2x3: faces 1x2 (twice), 1x3 (twice), 2x3 (twice)
    std::vector<std::pair<int,int>> box = {{2,1},{1,3},{3,2},{1,2},{3,1},{2,3}};
    assert(canFormBox(box) == true);
    
    // Same but order shuffled
    std::vector<std::pair<int,int>> shuffled = {{3,2},{1,2},{2,1},{3,1},{2,3},{1,3}};
    assert(canFormBox(shuffled) == true);
    
    // Missing one face (e.g., only one 1x2)
    std::vector<std::pair<int,int>> missing = {{2,1},{1,3},{3,2},{1,2},{3,1},{3,2}}; // two 3x2, no second 1x3? Actually check: here we have {3,2} twice and {1,2} once, {1,3} once, {2,1} once, {3,1} once → invalid
    assert(canFormBox(missing) == false);
    
    // Box with a square base (e.g., 2x2x3): faces 2x2 twice, 2x3 twice, 2x3 twice (but note 2x2 appears twice, and 2x3 appears four times? Wait, that's wrong: for 2x2x3, faces are: 2x2 (two), 2x3 (two), 2x3 (two)?? Actually sides are 2,2,3 → faces: 2×2 (two), 2×3 (four? No: a=2,b=2,c=3 → faces ab=2×2 (two), ac=2×3 (two), bc=2×3 (two) → so 2×2 appears twice and 2×3 appears four times? That is impossible because only six faces total, so we have two 2×2 and four 2×3? But that would be 2+4=6, but the three distinct pairs are (2,2), (2,3), (2,3) – the same pair (2,3) appears twice in the three face types, meaning we need four copies of 2×3. That is valid! The box is a square prism. Check: a=2,b=2,c=3 → faces: 2×2 (two), 2×3 (two), 2×3 (two) → total four 2×3 and two 2×2. That is a valid box. Test:
    assert(canFormBox({{2,2},{2,2},{2,3},{2,3},{2,3},{2,3}}) == true);
    
    // Invalid: same dimensions but only one 2×3 and one 2×2 missing
    assert(canFormBox({{2,2},{2,2},{2,3},{2,3},{2,3},{1,1}}) == false);
    
    // Invalid: rectangles that cannot form a box (e.g., 1x1, 1x2, 1x3, but counts wrong)
    assert(canFormBox({{1,1},{1,2},{1,3},{1,2},{1,3},{1,4}}) == false);
    
    // Zero or negative not allowed but test degenerate: all sides 1 -> cube 1x1x1
    assert(canFormBox({{1,1},{1,1},{1,1},{1,1},{1,1},{1,1}}) == true);
    
    return 0;
}
// The key observation is that a cuboid has three pairs of identical faces, and each face is a rectangle with dimensions equal to two of the three edge lengths (say `a`, `b`, `c`). The six rectangles must form exactly two copies of each of the three face types: `a×b`, `a×c`, and `b×c`. A direct algorithmic approach is to normalize each rectangle by placing its two sides in a canonical order (e.g., larger side first) so that orientation is ignored. Then, for a valid box, after sorting these six canonical pairs lexicographically, the pairs must appear in three consecutive identical pairs. Additionally, if the three distinct dimensions are `(p,q)`, `(p,r)`, `(q,r)`, then they must share a common set of three edge values. A robust way to verify is: after sorting, group into three pairs; check that within each pair the two rectangles are identical; then extract the two distinct side lengths from each pair and confirm that the union of these six values has exactly three distinct integers, each appearing exactly twice, and that the pairs are consistent with a cuboid (i.e., the three pairs are `(x,y)`, `(x,z)`, `(y,z)` for some x,y,z). This can be checked by sorting the three pairs and verifying the first coordinate of the second pair equals the second coordinate of the first pair, etc., but simpler: collect all distinct side lengths; if there are exactly three values `u,v,w`, then the pairs must be exactly `{u,v}`, `{u,w}`, `{v,w}`. Since we've already normalized each pair to (larger, smaller), after sorting the six rectangles, we can check pairs directly. Edge cases: all three dimensions distinct (common), exactly two dimensions equal (then one face is a square and the box is a rectangular prism with a square base), and all three equal (cube). Time complexity is O(1) (constant operations on six elements) but sorting six elements is effectively constant; space O(1). The solution sorts six pairs and does a few comparisons.
