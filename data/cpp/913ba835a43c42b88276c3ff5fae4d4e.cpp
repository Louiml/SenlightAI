// Write a C++ function `long long countValidTriangles(const std::vector<int>& arr)` that, given a non-empty vector of positive integers, returns the number of triples `(i, j, k)` with `i < j < k` such that the three values can form a non-degenerate triangle (i.e., the sum of any two sides is strictly greater than the third). The function must count all such triples efficiently. The input vector may contain duplicate values, and all values are guaranteed to be positive. If no valid triple exists, return 0.
// The classic approach is to sort the array first. After sorting, for each possible largest side (iterating from the end toward the beginning), use a two-pointer technique to count how many pairs of smaller elements have a sum strictly greater than that largest side. For a fixed index `i` (the largest side), set `left = 0` and `right = i-1`. While `left < right`, if `arr[left] + arr[right] > arr[i]`, then for all indices between `left` and `right-1` (inclusive) the sum with `arr[right]` also exceeds `arr[i]` because the array is sorted, so we can add `(right - left)` to the answer and decrement `right`. Otherwise, we need a larger pair sum, so increment `left`. This counts each valid triple exactly once. Edge cases: arrays with fewer than 3 elements, duplicates (they are handled naturally), and very large counts (hence the `long long` return). Time complexity is O(n log n) for sorting plus O(n^2) for the two-pointer loop, so overall O(n^2). Space complexity is O(1) auxiliary, excluding the input vector.
#include <vector>
#include <algorithm>

// Count triples (i<j<k) that can form a non-degenerate triangle.
long long countValidTriangles(std::vector<int> arr) {
    const std::size_t n = arr.size();
    if (n < 3) return 0;

    std::sort(arr.begin(), arr.end());
    long long answer = 0;

    for (std::size_t i = n - 1; i >= 2; --i) {
        std::size_t left = 0;
        std::size_t right = i - 1;
        while (left < right) {
            if (arr[left] + arr[right] > arr[i]) {
                // All pairs (left..right-1, right) are valid.
                answer += static_cast<long long>(right - left);
                --right;
            } else {
                ++left;
            }
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    // Simple valid triples.
    assert(countValidTriangles({2, 3, 4}) == 1);
    // Multiple triples.
    assert(countValidTriangles({4, 6, 3, 7}) == 3); // (3,4,6), (3,4,7), (4,6,7)
    // Duplicates: (2,2,3) and (2,3,3) and (3,3,3) → count? All combos.
    assert(countValidTriangles({2, 2, 3, 3, 3}) == 7);
    // Extra check: list all: indices (0,1,2), (0,1,3), (0,1,4), (0,2,3), (0,2,4), (1,2,3), (1,2,4) → 7.
    // Degenerate (sum equals) not counted.
    assert(countValidTriangles({1, 2, 3}) == 0);
    // Very small array.
    assert(countValidTriangles({1, 2}) == 0);
    // All equal large values: C(4,3)=4 triples.
    assert(countValidTriangles({5, 5, 5, 5}) == 4);
    // Mixed large numbers.
    assert(countValidTriangles({10, 21, 22, 100, 101, 200, 300}) == 8);
    return 0;
}
