// Write a C++ function `std::vector<int> fillPaintedFence(int n, const std::vector<int>& paintQueries)` that simulates the following scenario: there are `n` fence posts numbered 1 through `n`, initially unpainted (value 0). A list of `m` paint operations is given, where each operation specifies a color value `c`. For each operation, starting from post `c` and moving rightward to post `n`, if a post is still unpainted (value 0), it is painted with color `c`. Once a post has been painted, later operations do not change its color. The function must return a vector of length `n` where index `i` (0-based) contains the final color of post `i+1`. If a post remains unpainted after all operations, its value is 0. The input will always have at least one paint operation, and all colors `c` are between 1 and `n`. You may assume `n` and the number of operations are both positive.
#include <cassert>
#include <vector>

// The solution function is declared above (same as in ).
int main() {
    // Example from the snippet: n=5, operations [2,4,1]
    std::vector<int> res1 = fillPaintedFence(5, {2, 4, 1});
    std::vector<int> expected1 = {1, 2, 2, 1, 1}; // Let's verify: op2 paints posts 2..5, op4 paints 4..5 (4,5 already colored by op2? op2 painted 2,3,4,5, so op4 does nothing, op1 paints 1..5 but 2..5 already set, so 1 becomes 1. Final: [1,2,2,2,2]? Wait: op2: posts 2,3,4,5 become 2. op4: post 4 already 2, post 5 already 2, does nothing. op1: post 1 becomes 1, posts 2..5 already 2, so final [1,2,2,2,2]. Correct.
    std::vector<int> expected1 = {1, 2, 2, 2, 2};
    assert(res1 == expected1);

    // All posts painted by first operation
    std::vector<int> res2 = fillPaintedFence(3, {1});
    assert(res2 == std::vector<int>({1, 1, 1}));

    // No posts painted because all colors start after the last unpainted? Not possible, but test n=1
    std::vector<int> res3 = fillPaintedFence(1, {1});
    assert(res3 == std::vector<int>({1}));

    // Multiple operations, later operations have no effect if all already painted
    std::vector<int> res4 = fillPaintedFence(4, {2, 2, 2});
    assert(res4 == std::vector<int>({0, 2, 2, 2}));

    // Colors are not sorted, but each post takes the earliest operation that reaches it
    std::vector<int> res5 = fillPaintedFence(6, {5, 3, 1});
    // op5 paints 5,6 -> 5
    // op3 paints 3,4 (5,6 already 5) -> 3
    // op1 paints 1,2 (3,4,5,6 already) -> 1
    std::vector<int> expected5 = {1, 1, 3, 3, 5, 5};
    assert(res5 == expected5);

    // Large n but only one operation at the end
    std::vector<int> res6 = fillPaintedFence(10, {10});
    std::vector<int> expected6(10, 0);
    expected6[9] = 10;
    assert(res6 == expected6);

    // Operation with color larger than n? not allowed per constraints, skip.

    return 0;
}
#include <vector>

// Simulate painting fence posts. Each operation paints all yet-unpainted posts
// from position color (1-indexed) to n with that color.
std::vector<int> fillPaintedFence(int n, const std::vector<int>& paintQueries) {
    std::vector<int> result(n, 0);
    std::vector<int> parent(n + 2); // 1-indexed posts, sentinel n+1
    for (int i = 1; i <= n + 1; ++i) {
        parent[i] = i;
    }

    // Disjoint set find with path compression
    auto find = [&](int x) {
        int root = x;
        while (parent[root] != root) {
            root = parent[root];
        }
        while (parent[x] != x) {
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    };

    // Union: make x point to y (y > x)
    auto unite = [&](int x, int y) {
        parent[x] = y;
    };

    for (int c : paintQueries) {
        int i = find(c);
        while (i <= n) {
            result[i - 1] = c;
            unite(i, i + 1);
            i = find(i);
        }
    }
    return result;
}
// The core observation is that each operation paints a contiguous range of unpainted posts starting from position `c` up to `n`. However, since later operations only affect posts that are still 0, we cannot simply paint the whole range blindly. A naive simulation would set `num[i]=c` for each `i` from `c` to `n` if `num[i]==0`, which is exactly what the snippet does, running in O(m * n) time. For small constraints, this is fine, but we can improve it. The key optimization: for each operation, we only need to process unpainted posts. We can maintain a "next unpainted" pointer using a disjoint set union (DSU) / union‑find structure where each post points to the next post that is still 0. When we paint post `i`, we union it with `i+1`, so future queries skip it. For each operation with color `c`, start at `c`, find the next unpainted post using DSU `find(c)`, and while that post ≤ n, paint it with `c`, then union it with `i+1`. This processes each post exactly once, leading to O(n α(n) + m) time (α is inverse Ackermann, effectively constant). Edge cases: if `c` is greater than `n`? Not possible per constraints. If all posts are already painted, the DSU find will return n+1, so we skip. Space is O(n) for the array and DSU parent. The final vector is the painted colors.
