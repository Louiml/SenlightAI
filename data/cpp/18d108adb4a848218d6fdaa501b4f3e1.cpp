Write a C++ function `bool canChooseFromArrays` that takes two sorted vectors of integers `a` and `b`, along with two integers `k` and `m` (both 1-based counts), and returns `true` if it is possible to select exactly `k` elements from the front of array `a` and exactly `m` elements from the end of array `b` such that every selected element from `a` is strictly less than every selected element from `b`. The arrays are guaranteed to be non-empty, sorted in non-decreasing order, and `k` and `m` are valid (1 ≤ k ≤ a.size(), 1 ≤ m ≤ b.size()). The function should return `false` otherwise. You may assume that the arrays are given in ascending order and contain positive integers.

// The problem reduces to a single comparison. Since `a` is sorted ascending, the largest element among the first `k` elements of `a` is simply `a[k-1]` (0-indexed). Similarly, since `b` is sorted ascending, the smallest element among the last `m` elements of `b` is `b[b.size() - m]` (0-indexed). The condition "every selected element from `a` is strictly less than every selected element from `b`" is equivalent to requiring that the maximum of the chosen `a` elements is less than the minimum of the chosen `b` elements, i.e., `a[k-1] < b[b.size() - m]`. If this holds, return `true`; otherwise, return `false`. Edge cases: if `k` equals `a.size()` or `m` equals `b.size()`, the indexing still works because the largest of the whole array `a` is `a[a.size()-1]` and the smallest of the whole array `b` is `b[0]`, which is correctly given by `b[b.size()-m]` when `m == b.size()`. The solution runs in O(1) time and uses O(1) extra space, since we only do constant array indexing and comparison.

#include <vector>

// Returns true if we can select exactly k elements from the front of a
// and exactly m elements from the back of b, such that all selected
// elements from a are strictly less than all selected from b.
bool canChooseFromArrays(const std::vector<int>& a, const std::vector<int>& b, int k, int m) {
    // Largest among first k elements of a
    int max_a = a[k - 1];
    // Smallest among last m elements of b
    int min_b = b[b.size() - m];
    return max_a < min_b;
}

#include <cassert>
#include <vector>

// Declaration of the function (in real usage, include the header)
bool canChooseFromArrays(const std::vector<int>& a, const std::vector<int>& b, int k, int m);

int main() {
    // Basic positive case
    std::vector<int> a1 = {1, 2, 3, 4};
    std::vector<int> b1 = {5, 6, 7, 8};
    assert(canChooseFromArrays(a1, b1, 2, 2) == true);

    // Negative case: a's max is not less than b's min
    std::vector<int> a2 = {1, 5, 9};
    std::vector<int> b2 = {4, 6, 10};
    assert(canChooseFromArrays(a2, b2, 2, 2) == false);

    // Edge case: pick the entire arrays
    std::vector<int> a3 = {1, 2};
    std::vector<int> b3 = {3, 4};
    assert(canChooseFromArrays(a3, b3, 2, 2) == true);

    // Edge case: k=1 and m=1, boundary equal values -> false
    std::vector<int> a4 = {3};
    std::vector<int> b4 = {3, 4};
    assert(canChooseFromArrays(a4, b4, 1, 1) == false);

    // Edge case: k=1 and m=1, boundary strictly less -> true
    std::vector<int> a5 = {2};
    std::vector<int> b5 = {3, 4};
    assert(canChooseFromArrays(a5, b5, 1, 1) == true);

    // Larger arrays, values with duplicates
    std::vector<int> a6 = {1, 1, 2, 2};
    std::vector<int> b6 = {3, 3, 4, 5};
    assert(canChooseFromArrays(a6, b6, 4, 4) == true);

    // Larger arrays, duplicates that fail
    std::vector<int> a7 = {1, 2, 2, 3};
    std::vector<int> b7 = {2, 3, 4, 5};
    assert(canChooseFromArrays(a7, b7, 3, 2) == false);

    // All elements in a equal to all in b, any selection fails
    std::vector<int> a8 = {5, 5};
    std::vector<int> b8 = {5, 5};
    assert(canChooseFromArrays(a8, b8, 1, 1) == false);

    // where k and m are not the full sizes
    std::vector<int> a9 = {1, 10, 20};
    std::vector<int> b9 = {15, 30, 40};
    // first 1 from a (1) < last 1 from b (40) -> true
    assert(canChooseFromArrays(a9, b9, 1, 1) == true);
    // first 2 from a (1,10) < last 1 from b (40) -> true (max 10 < 40)
    assert(canChooseFromArrays(a9, b9, 2, 1) == true);
    // first 2 from a (1,10) < last 2 from b (30,40) -> min of last 2 is 30, 10<30 true
    assert(canChooseFromArrays(a9, b9, 2, 2) == true);
    // first 3 from a (1,10,20) < last 2 from b (30,40) -> max 20 < 30 true
    assert(canChooseFromArrays(a9, b9, 3, 2) == true);

    // Test where it fails because a's chosen max is too large
    std::vector<int> a10 = {1, 15, 20};
    std::vector<int> b10 = {10, 30, 40};
    // first 2 from a (1,15) vs last 2 from b (30,40) -> max 15 < 30 true
    assert(canChooseFromArrays(a10, b10, 2, 2) == true);
    // first 2 from a (1,15) vs last 1 from b (40) -> max 15 < 40 true
    assert(canChooseFromArrays(a10, b10, 2, 1) == true);
    // first 3 from a (1,15,20) vs last 1 from b (40) -> max 20 < 40 true
    assert(canChooseFromArrays(a10, b10, 3, 1) == true);
    // first 3 from a (1,15,20) vs last 2 from b (30,40) -> max 20 < 30 true
    assert(canChooseFromArrays(a10, b10, 3, 2) == true);

    return 0;
}
