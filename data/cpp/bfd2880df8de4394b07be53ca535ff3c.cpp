Given `n` rectangles, each with height `h[i]` and width `w[i]`, write a C++ function `std::vector<int> findCorrectPlacement(const std::vector<int>& h, const std::vector<int>& w)` that returns, for each rectangle `i`, the index of any rectangle `j` (0-based) such that rectangle `j` can be placed strictly inside rectangle `i` (i.e., `h[j] < h[i] && w[j] < w[i]` OR `h[j] < w[i] && w[j] < h[i]`), and returns `-1` if no such rectangle exists. You may rotate rectangles by 90 degrees (swap height and width) before comparing. If multiple valid indices exist, return any one of them. The input may contain duplicates (identical dimensions) and the rectangles are not necessarily sorted. The function must handle up to `n = 2*10^5` rectangles with dimensions up to `10^9`, and must run efficiently.
#include <cassert>
#include <vector>

// The function is declared above; include the implementation or link it.

int main() {
    // Basic case: rectangle 0 (1x2) fits inside rectangle 1 (3x4), and rectangle 1 has no fit.
    {
        std::vector<int> h = {1, 3};
        std::vector<int> w = {2, 4};
        auto ans = findCorrectPlacement(h, w);
        assert(ans[0] == -1);
        assert(ans[1] == 0);
    }

    // Rotation case: rectangle 0 is 5x3, normalized to 3x5; rectangle 1 is 4x4 normalized to 4x4.
    // Rectangle 0 (3x5) cannot fit inside rectangle 1 (4x4) because width 5 > 4.
    // Rectangle 1 (4x4) fits inside rectangle 0 (3x5)? height 4 > 3, no. So both -1.
    {
        std::vector<int> h = {5, 4};
        std::vector<int> w = {3, 4};
        auto ans = findCorrectPlacement(h, w);
        assert(ans[0] == -1);
        assert(ans[1] == -1);
    }

    // Set with duplicates: (2,2), (2,3), (3,3)
    // Index 0: (2,2) normalized (2,2) – can fit inside index 1 (2,3)? height equal 2, no.
    // Index 1: (2,3) – can fit inside index 2 (3,3)? height 2<3, width 3<3? no, equal, so no.
    // Index 2: (3,3) – can fit? no smaller rectangle with both dimensions strictly smaller.
    {
        std::vector<int> h = {2, 2, 3};
        std::vector<int> w = {2, 3, 3};
        auto ans = findCorrectPlacement(h, w);
        assert(ans[0] == -1);
        assert(ans[1] == -1);
        assert(ans[2] == -1);
    }

    // Multiple possible, pick any: (1,10), (2,3), (4,5)
    // Index 0 (1,10) fits inside both 1 and 2? For 1: height 1<2, width 10<3? no.
    // Index 1 (2,3) fits inside 2: height 2<4, width 3<5, yes.
    // Index 2 (4,5) has no fit.
    {
        std::vector<int> h = {1, 2, 4};
        std::vector<int> w = {10, 3, 5};
        auto ans = findCorrectPlacement(h, w);
        assert(ans[0] == -1);
        assert(ans[1] == 2); // because index 2 is the only larger? Actually index 2 is (4,5), so index1 fits inside it.
        assert(ans[2] == -1);
    }

    // All same size: no one fits.
    {
        std::vector<int> h = {2, 2, 2};
        std::vector<int> w = {3, 3, 3};
        auto ans = findCorrectPlacement(h, w);
        for (int a : ans) assert(a == -1);
    }

    // Large test: verify one correct assignment for a simple chain.
    {
        std::vector<int> h = {5, 4, 3, 2};
        std::vector<int> w = {5, 4, 3, 2};
        // Normalized all are squares. Index 3 (2) fits inside 2 (3), 2 fits inside 1, 1 fits inside 0.
        auto ans = findCorrectPlacement(h, w);
        // Index 0 has no smaller, index 1 can be 0? height 4<5, width 4<5, yes.
        // But index1 itself has smaller (2,3) etc. We only check that each ans is valid or -1.
        for (int i = 0; i < 4; ++i) {
            if (ans[i] == -1) {
                // If -1, there should be no valid j
                for (int j = 0; j < 4; ++j) {
                    if (j == i) continue;
                    bool ok = (std::min(h[j], w[j]) < std::min(h[i], w[i]) && std::max(h[j], w[j]) < std::max(h[i], w[i]));
                    assert(!ok);
                }
            } else {
                int j = ans[i];
                bool ok = (std::min(h[j], w[j]) < std::min(h[i], w[i]) && std::max(h[j], w[j]) < std::max(h[i], w[i]));
                assert(ok);
            }
        }
    }

    return 0;
}
#include <vector>
#include <numeric>
#include <algorithm>

// Returns for each rectangle i the index of a rectangle j (0-based) that can fit inside i,
// or -1 if none exists. Rectangles can be rotated (swap height/width) before comparing.
// The condition is: strictly smaller height and strictly smaller width after rotation.
std::vector<int> findCorrectPlacement(const std::vector<int>& h, const std::vector<int>& w) {
    int n = static_cast<int>(h.size());
    std::vector<int> norm_h = h, norm_w = w;
    for (int i = 0; i < n; ++i) {
        if (norm_h[i] > norm_w[i]) {
            std::swap(norm_h[i], norm_w[i]);
        }
    }

    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), 0);
    std::sort(p.begin(), p.end(), [&](int a, int b) {
        return norm_h[a] < norm_h[b];
    });

    std::vector<int> ans(n, -1);
    int best_index = -1; // index with minimal width among all processed groups with smaller height

    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && norm_h[p[i]] == norm_h[p[j]]) {
            ++j;
        }

        // For each rectangle in current height group, check if best_index fits
        for (int k = i; k < j; ++k) {
            int idx = p[k];
            if (best_index != -1 && norm_w[best_index] < norm_w[idx]) {
                ans[idx] = best_index;
            }
        }

        // Update best_index with minimal width among current group
        for (int k = i; k < j; ++k) {
            int idx = p[k];
            if (best_index == -1 || norm_w[idx] < norm_w[best_index]) {
                best_index = idx;
            }
        }

        i = j;
    }

    return ans;
}
// The key is to normalize each rectangle so that height `h[i]` is always the smaller dimension and width `w[i]` is the larger (swap if needed). After normalization, a rectangle `j` can fit inside `i` if and only if `h[j] < h[i]` and `w[j] < w[i]` (since both are sorted, the condition is symmetric under rotation). Sort the rectangles by height ascending. Process them in groups of equal height. Maintain a variable `u` that stores the index of the rectangle with the smallest width among all previously processed groups (i.e., rectangles with strictly smaller height). For each rectangle in the current group, if `u` exists and `w[u] < w[current]`, then `u` is a valid answer because its height is strictly smaller and its width is smaller too. Assign `ans[current] = u`. After processing the group, update `u` to the rectangle with minimal width among the current group (and previous groups) for future groups. Edge cases: same-height rectangles cannot contain each other (since height must be strictly less), so they are processed before updating `u`. If no such `u` exists, answer is `-1`. Time complexity is O(n log n) due to sorting, and O(n) auxiliary space. This is efficient for large `n`.
