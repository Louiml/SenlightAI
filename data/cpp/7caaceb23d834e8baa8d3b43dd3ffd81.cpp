Write a C++ function `insertAndPrint` that takes a dynamic-size integer array (represented by a fixed-capacity C-style array of size 100, its current logical length `n`, an insertion position `k` (1-indexed, where 1 means inserting at the beginning), and an integer value `x` to insert). The function must insert `x` at position `k`, shifting all existing elements from index `k-1` onward to the right by one position, and then return the resulting sequence as a `std::vector<int>`. The function must assume that the input array is valid (i.e., `0 <= k <= n+1` and `n < 100`), but it must handle the edge case where `k` equals `n+1` (appending at the end) and `k` equals 1 (prepending). The function must not modify the original array content in a way that affects external callers—it should work on a copy of the input array. Also, if `n` is 0, the function should return a vector containing just `{x}` regardless of `k` (since any valid `k` is 1). The function signature must be `std::vector<int> insertAndPrint(const int arr[], int n, int k, int x)`, and it should use `const` correctness for the input array. Note: The function name is descriptive and you must not include a `main` function in the solution section; instead, the test section will provide `assert` checks.
#include <cassert>
#include <vector>

// The solution function is declared here (assume it's included from above).
std::vector<int> insertAndPrint(const int arr[], int n, int k, int x);

int main() {
    // Test case 1: Insert in the middle
    int a1[] = {1, 2, 3, 4};
    std::vector<int> r1 = insertAndPrint(a1, 4, 3, 99);
    assert(r1 == std::vector<int>({1, 2, 99, 3, 4}));

    // Test case 2: Insert at the beginning (k=1)
    int a2[] = {10, 20, 30};
    std::vector<int> r2 = insertAndPrint(a2, 3, 1, 5);
    assert(r2 == std::vector<int>({5, 10, 20, 30}));

    // Test case 3: Insert at the end (k=n+1)
    int a3[] = {4, 5, 6};
    std::vector<int> r3 = insertAndPrint(a3, 3, 4, 7);
    assert(r3 == std::vector<int>({4, 5, 6, 7}));

    // Test case 4: Empty array
    int a4[] = {};
    std::vector<int> r4 = insertAndPrint(a4, 0, 1, 42);
    assert(r4 == std::vector<int>({42}));

    // Test case 5: Single element, insert at beginning
    int a5[] = {9};
    std::vector<int> r5 = insertAndPrint(a5, 1, 1, 0);
    assert(r5 == std::vector<int>({0, 9}));

    // Test case 6: Single element, insert at end
    int a6[] = {9};
    std::vector<int> r6 = insertAndPrint(a6, 1, 2, 11);
    assert(r6 == std::vector<int>({9, 11}));

    // Test case 7: Negative numbers and duplicates
    int a7[] = {-3, -1, -1, 0};
    std::vector<int> r7 = insertAndPrint(a7, 4, 2, -2);
    assert(r7 == std::vector<int>({-3, -2, -1, -1, 0}));

    // Test case 8: Large n, insert at position 1
    int a8[100];
    for (int i = 0; i < 100; ++i) a8[i] = i; // 0 to 99
    std::vector<int> r8 = insertAndPrint(a8, 100, 1, -1);
    assert(r8.size() == 101);
    assert(r8[0] == -1);
    assert(r8[1] == 0);
    assert(r8[100] == 99);
    return 0;
}
#include <vector>

// Inserts value x at position k (1-indexed) in a copy of arr, shifting elements right.
// Precondition: 0 <= n < 100, 0 <= k <= n+1.
// Returns a vector of size n+1 with the inserted element.
std::vector<int> insertAndPrint(const int arr[], int n, int k, int x) {
    if (n == 0) {
        // Empty array: result is just the inserted element
        return std::vector<int>{x};
    }
    // Start with a copy of the input array
    std::vector<int> result(arr, arr + n);
    // Resize to make room for the new element
    result.push_back(0); // or resize(n+1)
    
    // Shift elements from the end down to position k-1 one step right
    for (int i = n - 1; i >= k - 1; --i) {
        result[i + 1] = result[i];
    }
    // Insert the new value
    result[k - 1] = x;
    return result;
}
// The solution approach is straightforward: copy the input array into a new vector (or directly build the result vector) of size `n+1`. To insert at position `k` (1-indexed), we need to shift all elements from the end of the array down to index `k-1` one position to the right. This is done by iterating backward from `i = n-1` down to `i = k-1`, and assigning `result[i+1] = result[i]`. After the shift, assign `result[k-1] = x`. Edge cases include: (1) `k = 1` means shifting all elements one step right, which the loop handles because `i` goes from `n-1` down to `0`, and then `result[0] = x`. (2) `k = n+1` means no shifting needed because the loop condition `i >= (k-1)` becomes `i >= n`, which is false for all valid `i` (since `i` starts at `n-1`), so we just set `result[n] = x`. (3) If `n` is 0, then the input array is empty, and the function should return `{x}`; we can handle this by simply creating a vector of size 1 with `x`. Time complexity is \(O(n)\) because we shift up to `n` elements and copy the array; space complexity is \(O(n)\) for the result vector. The original array is not modified because we copy it into the result vector at the start.
