/*
Write a standalone C++ function `rotatingTheArrayLeftByK` that takes a non-empty array of integers, its length `n`, and a non-negative integer `k`, and returns a new array (or modifies it in place per the specification below) representing the array rotated to the left by `k` positions. For example, rotating `[1, 2, 3, 4, 5]` left by 2 yields `[3, 4, 5, 1, 2]`. The function must handle cases where `k` is larger than `n` by normalizing it (i.e., `k = k % n`), and must handle `k = 0` returning the original array. The input array is guaranteed to have at least one element. The function should be const-correct where appropriate and should not use any global or static mutable state. The solution must be derived from the provided snippet’s rotation logic but improved to be efficient (avoid O(n*k) naive shifting) and robust.
*/

#include <vector>
#include <algorithm>
#include <cassert>

// Rotates the given array (vector) to the left by k positions in place.
// The array must be non-empty. k may be any non-negative integer.
void rotateLeftInPlace(std::vector<int>& arr, int k) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) return;

    // Normalize k to be within [0, n-1]
    k %= n;
    if (k == 0) return;

    // Reverse the entire array
    std::reverse(arr.begin(), arr.end());
    // Reverse the first n-k elements (those that come to the front)
    std::reverse(arr.begin(), arr.begin() + (n - k));
    // Reverse the last k elements (those that go to the back)
    std::reverse(arr.begin() + (n - k), arr.end());
}

#include <vector>
#include <cassert>

int main() {
    // Test 1: Basic rotation
    std::vector<int> a = {1, 2, 3, 4, 5};
    rotateLeftInPlace(a, 2);
    assert(a == std::vector<int>({3, 4, 5, 1, 2}));

    // Test 2: k = 0
    std::vector<int> b = {7, 8, 9};
    rotateLeftInPlace(b, 0);
    assert(b == std::vector<int>({7, 8, 9}));

    // Test 3: k > n
    std::vector<int> c = {1, 2, 3};
    rotateLeftInPlace(c, 5); // 5 % 3 = 2, so rotate left by 2
    assert(c == std::vector<int>({3, 1, 2}));

    // Test 4: k is multiple of n
    std::vector<int> d = {10, 20, 30, 40};
    rotateLeftInPlace(d, 8); // 8 % 4 = 0
    assert(d == std::vector<int>({10, 20, 30, 40}));

    // Test 5: single element with any k
    std::vector<int> e = {42};
    rotateLeftInPlace(e, 100);
    assert(e == std::vector<int>({42}));

    // Test 6: rotate by n-1 (rightmost goes to front)
    std::vector<int> f = {1, 2, 3, 4};
    rotateLeftInPlace(f, 3);
    assert(f == std::vector<int>({4, 1, 2, 3}));

    // Test 7: negative? k is non-negative by task spec, but ensure no crash if k=0 already handled
    // Additional test: large rotation
    std::vector<int> g = {5, 6, 7, 8, 9, 10};
    rotateLeftInPlace(g, 1000000);
    // 1000000 % 6 = 4, so rotate left by 4: result [9,10,5,6,7,8]
    assert(g == std::vector<int>({9, 10, 5, 6, 7, 8}));

    return 0;
}

// The naive approach in the snippet rotates the array left by one position `k` times, which takes O(n*k) time. A better approach uses reversal: to left-rotate by `k` positions, first normalize `k = k % n`. Then reverse the entire array, reverse the first `n-k` elements, and reverse the last `k` elements. Alternatively, a direct approach using modular indexing can build a new array, but the in-place reversal method is O(n) time and O(1) extra space. Edge cases: empty array (but task guarantees non-empty, still handle gracefully), `k=0`, `k` multiple of `n`, and `k` > `n` (by modulo). The function should be implemented to modify the array in place and return void (or return a vector, but the task specification says “returns a new array (or modifies it in place)”; we choose in-place modification for simplicity and to match the snippet’s style, but with an improved algorithm). Time complexity: O(n), space complexity: O(1) auxiliary.
