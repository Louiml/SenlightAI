// Write a C++ function `vector<pair<int,int>> computeVisibleSpires(const vector<int>& heights)` that, given a 1-indexed conceptual array of tower heights (stored in a 0-indexed vector of size `n`), returns for each tower `i` (1 ≤ i ≤ n) a pair `(count, nearestIndex)` where `count` is the number of towers that are "visible" from tower `i` (including tower `i` itself and any tower to the left or right such that all towers between them and tower `i` are strictly shorter than the shorter of the two), and `nearestIndex` is the index (1-based) of the nearest visible tower among those to the left and right, choosing the one with the smaller absolute distance from `i`; if both distances are equal, choose the left one. If no other tower is visible, `nearestIndex` is `-1`. The output should be returned as a vector of pairs where the first element is the count and the second is the nearest index (or `-1` if none). Note: The original snippet uses 1-based indexing with sentinel `0` to indicate "none", but you must adapt to 0-based input and output using `-1` for "none".
// The core idea is to compute, for each index, how many towers are visible to the left and right under the condition that all intermediate towers are strictly shorter than the shorter of the two endpoints. This is a classic "next greater element" style monotonic stack problem, but with a twist: we count all towers that are visible, not just the immediate greater. For each tower, scanning left and right using a monotonic stack that maintains a decreasing sequence of heights allows us to find the nearest tower that is strictly taller than the current one (or any tower that blocks the view). The correct approach is to simulate the "visibility" by maintaining a stack of indices with strictly decreasing heights. When moving left-to-right, we pop from the stack while the current tower's height is at least as tall as the stack top, because those towers are blocked by the current taller tower from being visible to towers further right. The remaining stack size after popping (plus the current index itself? Actually careful: The original code counts `l.size()` after popping, which represents the number of towers to the left that are strictly taller than all intermediate towers? Let's reason: For tower `i`, a tower `j < i` is visible if all towers between `j` and `i` have height less than `min(heights[j], heights[i])`. In the original algorithm, they maintain a stack `l` that contains indices of towers that are "candidates" for visibility. When scanning left to right, before processing tower `i`, they add the previous tower `i-1` to the stack if it is strictly greater than `i`? Actually the original code does: 
// ```
// if (arr[i-1] > arr[i]) l.push_back(i-1);
// else while (!l.empty() && arr[i] >= arr[l.back()]) l.pop_back();
// ```
// This ensures that after this operation, the stack `l` contains indices of towers to the left that are strictly taller than any tower between them and `i` and also strictly taller than `arr[i]`? Not exactly. Let's analyze: For tower `i`, a tower `j` is visible if `arr[j] > arr[k]` for all `k` between `j` and `i`, and also `arr[j] > arr[i]`? Actually if `arr[j] <= arr[i]`, then tower `i` blocks the view of all towers to the right but for tower `j` visible from `i`, we require all intermediate towers' heights < min(arr[j], arr[i]). If `arr[j] <= arr[i]`, then the condition becomes all intermediate < arr[j]. So tall towers can block shorter ones. The monotonic stack of decreasing heights (from bottom to top) ensures that each element in the stack is strictly taller than all elements to its right that are still in the stack? Actually the stack stores indices in increasing order (left to right), but with strictly decreasing heights when read from bottom to top. When we pop while `arr[i] >= arr[top]`, we remove towers that are not taller than `arr[i]`, meaning they are not visible from any tower to the right of `i` because `i` blocks them. After popping, the remaining stack contains only towers that are strictly taller than `arr[i]` and also strictly taller than everything between them and `i`? Yes, because any tower that was not taller than something between it and `i` would have been popped earlier. So the size of the stack after popping gives the number of visible towers to the left of `i` (excluding `i` itself). And the back of the stack gives the nearest visible left tower. Similarly, scanning right to left builds a stack `r` for visible towers to the right. Sum counts and choose nearest based on distance. Edge cases: If no visible left, dist.first=0 (sentinel), if no visible right, dist.second=0. In the output, if only one side exists, choose that; if both exist, choose the one with smaller distance, tie goes to left. In the reference solution, we must adapt to 0-based indexing: return a vector of pairs where the pair's second is -1 if none. Time complexity is O(n) because each index is pushed and popped at most once per direction. Space complexity is O(n) for stacks and result.
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>
#include <cstdlib>

// Compute for each tower the number of visible towers and the nearest visible one.
// heights is 0-indexed; result[i] = {count, nearestIndex} where nearestIndex is 0-based, or -1 if none.
std::vector<std::pair<int, int>> computeVisibleSpires(const std::vector<int>& heights) {
    int n = static_cast<int>(heights.size());
    std::vector<int> leftCount(n, 0), rightCount(n, 0);
    std::vector<int> leftNearest(n, -1), rightNearest(n, -1);
    
    // Left pass: find visible towers to the left.
    std::stack<int> st;
    for (int i = 0; i < n; ++i) {
        // Remove towers from stack that are not strictly taller than heights[i]
        while (!st.empty() && heights[i] >= heights[st.top()]) {
            st.pop();
        }
        leftCount[i] = static_cast<int>(st.size()); // number of visible towers to the left
        if (!st.empty()) {
            leftNearest[i] = st.top();
        }
        // Now push current tower; it may block shorter towers to its right.
        st.push(i);
    }
    
    // Clear stack for right pass.
    while (!st.empty()) st.pop();
    
    // Right pass: find visible towers to the right.
    for (int i = n - 1; i >= 0; --i) {
        while (!st.empty() && heights[i] >= heights[st.top()]) {
            st.pop();
        }
        rightCount[i] = static_cast<int>(st.size());
        if (!st.empty()) {
            rightNearest[i] = st.top();
        }
        st.push(i);
    }
    
    std::vector<std::pair<int, int>> result(n);
    for (int i = 0; i < n; ++i) {
        int totalCount = leftCount[i] + rightCount[i] + 1; // +1 for itself? Actually original snippet counts s[i] = l.size() + r.size() (without +1 for itself? Check original: s[i] initialized 0, then s[i] += l.size(), then s[i] += r.size(). So count is number of other visible towers, not including itself. But the problem statement says "including tower i itself" - we'll follow the problem statement, so totalCount = leftCount+rightCount+1.
        // Let's re-read the task: "count is the number of towers that are visible from tower i (including tower i itself)". So yes, add 1.
        int nearest = -1;
        if (leftNearest[i] != -1 && rightNearest[i] != -1) {
            int distL = std::abs(i - leftNearest[i]);
            int distR = std::abs(i - rightNearest[i]);
            if (distL <= distR) {
                nearest = leftNearest[i];
            } else {
                nearest = rightNearest[i];
            }
        } else if (leftNearest[i] != -1) {
            nearest = leftNearest[i];
        } else if (rightNearest[i] != -1) {
            nearest = rightNearest[i];
        }
        result[i] = std::make_pair(totalCount, nearest);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function is declared above; include it here for test.
// (In a real environment, the solution code would be included, but for the test we repeat the declaration.)
std::vector<std::pair<int, int>> computeVisibleSpires(const std::vector<int>& heights);

int main() {
    // Test 1: Single tower
    {
        std::vector<int> h = {5};
        auto res = computeVisibleSpires(h);
        assert(res.size() == 1);
        assert(res[0].first == 1);
        assert(res[0].second == -1);
    }
    // Test 2: Two towers, left smaller than right
    {
        std::vector<int> h = {1, 3};
        auto res = computeVisibleSpires(h);
        // Tower 0: left none, right tower 1 visible (height 3 > 1), count=2, nearest=1
        assert(res[0].first == 2 && res[0].second == 1);
        // Tower 1: left tower 0 visible? All intermediate none, height 1 < 3, so yes visible. count=2, nearest=0
        assert(res[1].first == 2 && res[1].second == 0);
    }
    // Test 3: Three towers with a tall middle blocking view
    {
        std::vector<int> h = {1, 5, 3};
        auto res = computeVisibleSpires(h);
        // Tower 0 (height 1): left none, right visible? Tower 2 height 3 >1 but tower 1 height 5 in between, which is taller than 1, so tower 2 is NOT visible because intermediate tower is not shorter than min(1,3)=1. So only tower 1 visible. count=2, nearest=1
        assert(res[0].first == 2 && res[0].second == 1);
        // Tower 1 (height 5): left tower 0 visible, right tower 2 visible, count=3, nearest left or right? distances both 1, tie -> left (index 0)
        assert(res[1].first == 3 && res[1].second == 0);
        // Tower 2 (height 3): left tower 1 visible? Intermediate none, height 5>3, yes visible. tower 0 not visible because tower 1 taller. count=2, nearest=1
        assert(res[2].first == 2 && res[2].second == 1);
    }
    // Test 4: Decreasing sequence
    {
        std::vector<int> h = {6, 5, 4};
        auto res = computeVisibleSpires(h);
        // Tower 0: no left, right visible? tower 1 (5) visible, tower 2 (4) visible? For tower 2 from tower 0: intermediate tower 1 height 5 < min(6,4)=4? No, 5>4, so not visible. So count=2, nearest=1
        assert(res[0].first == 2 && res[0].second == 1);
        // Tower 1: left tower 0 visible, right tower 2 visible, count=3, nearest left? distances both 1, tie -> left=0
        assert(res[1].first == 3 && res[1].second == 0);
        // Tower 2: left visible? tower 1 visible, tower 0 not (intermediate 5 >4), count=2, nearest=1
        assert(res[2].first == 2 && res[2].second == 1);
    }
    // Test 5: Equal heights
    {
        std::vector<int> h = {2, 2, 2};
        auto res = computeVisibleSpires(h);
        // For each tower, only immediate neighbors are visible? Actually equal height: condition requires intermediate towers strictly shorter than the shorter of the two, but if equal, then shorter is that height, but intermediate must be strictly shorter than that, which they are not (since equal). So only adjacent equals are visible? Wait, for tower 0 and tower 1: heights 2 and 2, intermediate none, so condition holds (vacuous) because there are no intermediate towers. So they are visible. For tower 0 and tower 2: intermediate tower 1 has height 2, which is not < min(2,2)=2, so not visible. So each tower sees both its immediate neighbors, count=3 for middle, count=2 for ends. Nearest for middle: left distance1, right distance1, tie->left=0. For ends: only one side.
        assert(res[0].first == 2 && res[0].second == 1);
        assert(res[1].first == 3 && res[1].second == 0);
        assert(res[2].first == 2 && res[2].second == 1);
    }
    // Test 6: Mixed with non-equal
    {
        std::vector<int> h = {3, 1, 2};
        auto res = computeVisibleSpires(h);
        // Tower 0 (3): left none, right: tower 1 (1) visible, tower 2 (2) visible? intermediate tower 1 height 1 < min(3,2)=2? Yes 1<2, so visible. So both visible, count=3, nearest=1 (dist1) vs 2 (dist2) -> 1
        assert(res[0].first == 3 && res[0].second == 1);
        // Tower 1 (1): left tower 0 visible, right tower 2 visible, count=3, nearest left dist1, right dist1, tie->left=0
        assert(res[1].first == 3 && res[1].second == 0);
        // Tower 2 (2): left tower 1 (1) visible, left tower 0 (3) visible? intermediate tower 1 height 1 < min(2,3)=2? Yes 1<2, so visible. Both left, count=3, nearest=1 (dist1) vs 0 (dist2) -> 1
        assert(res[2].first == 3 && res[2].second == 1);
    }
    return 0;
}
