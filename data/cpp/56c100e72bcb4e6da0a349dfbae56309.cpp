// Write a C++ function `int countValidTriangles(const std::vector<int>& nums)` that, given a vector of positive integers, returns the number of triples `(i, j, k)` with `i < j < k` such that `nums[i]`, `nums[j]`, and `nums[k]` can form a non-degenerate triangle (i.e., the sum of any two side lengths is strictly greater than the third). The input vector may contain duplicate values and may have fewer than 3 elements, in which case the function must return 0. The function must handle empty vectors as well. Optimize for time efficiency beyond a simple triple nested loop.

#include <cassert>
#include <vector>

// Declaration provided by the solution; included for clarity.
int countValidTriangles(const std::vector<int>& nums);

int main() {
    // Basic valid cases
    assert(countValidTriangles({2, 2, 3, 4}) == 3); // (2,2,3), (2,3,4), (2,3,4) with different indices
    assert(countValidTriangles({1, 2, 3, 4}) == 1); // only (2,3,4)
    assert(countValidTriangles({4, 2, 3, 4}) == 4); // sorted: {2,3,4,4} -> (2,3,4), (2,3,4), (2,4,4), (3,4,4)
    
    // Duplicate values
    assert(countValidTriangles({5, 5, 5}) == 1); // equilateral
    
    // Edge cases: fewer than 3 elements
    assert(countValidTriangles({}) == 0);
    assert(countValidTriangles({1}) == 0);
    assert(countValidTriangles({1, 2}) == 0);
    
    // No valid triangles
    assert(countValidTriangles({1, 1, 3}) == 0); // 1+1=2 not > 3
    assert(countValidTriangles({1, 2, 3}) == 0); // degenerate
    
    // Larger test with all equal values
    std::vector<int> many(100, 7);
    assert(countValidTriangles(many) == 100*99*98/6); // C(100,3)
    
    return 0;
}

#include <vector>
#include <algorithm>

// Count the number of triples (i, j, k) with i < j < k that can form a triangle.
// The input vector is treated as read-only and is not modified.
int countValidTriangles(const std::vector<int>& nums) {
    if (nums.size() < 3) {
        return 0;
    }
    
    // Work on a copy to sort without mutating the caller's data.
    std::vector<int> sides = nums;
    std::sort(sides.begin(), sides.end());
    
    int ans = 0;
    const int n = static_cast<int>(sides.size());
    
    for (int i = 0; i < n - 2; ++i) {
        int k = i + 2; // k moves only forward within each i iteration
        for (int j = i + 1; j < n - 1; ++j) {
            // Increase k while the triangle inequality holds.
            while (k < n && sides[i] + sides[j] > sides[k]) {
                ++k;
            }
            // All indices from j+1 to k-1 (inclusive) are valid third sides.
            // Since k >= j+1, the difference is non-negative.
            ans += k - j - 1;
        }
    }
    
    return ans;
}

// The core idea is to sort the array first, which transforms the triangle condition into checking only one inequality. If `a <= b <= c` are three side lengths, then a triangle exists if and only if `a + b > c`; the other two inequalities (`b + c > a` and `a + c > b`) are automatically satisfied because `c` is the largest. After sorting, we can fix the two smaller sides `i` and `j` (with `i < j`) and use a moving pointer `k` (initially `j+1`) to find the largest index such that `nums[i] + nums[j] > nums[k]`. All indices from `j+1` to `k-1` form valid third sides with this pair, adding `k - j - 1` to the answer. As `j` increases for a fixed `i`, the value of `k` only moves forward (never backward), because `nums[i] + nums[j]` increases with `j`, so the same or more elements will satisfy the condition. This allows a linear scan inner loop per `i`, giving an overall time complexity of `O(n^2)` after the initial `O(n log n)` sort. The space complexity is `O(1)` auxiliary (ignoring the sort's internal stack usage). Edge cases: if the vector size is less than 3, return 0 immediately. Also, since `k` can reach `nums.size()`, the subtraction `k - j - 1` must be clamped to zero, but because `k` starts at `j+1` and only increases, `k - j - 1` is always non-negative. Duplicate values are handled naturally because they are sorted and the strict inequality `>` is correctly applied.
