You are given an array of \( n \) integers and a repeated transformation process. In each operation, you first determine the length of the suffix of the array that already consists entirely of equal values (specifically, the value equal to the last element). Then you double that suffix length (capped by the array size) and overwrite every element from the new suffix start to the end of the array with the last element’s value. After this overwrite, you recompute the suffix length of equal last-value elements from the updated array. The process stops when the entire array consists of the same value as the original last element. Write a C++ function `int minimum_operations(vector<int>& arr)` that returns the number of operations required to make all elements equal to the last element of the initial array. The input array can have length 1 (then 0 operations), and all elements are integers. Assume the array is non-empty and may contain duplicates and negative numbers. The operations must be performed exactly as described, even if no change occurs in some steps (though in practice the overwrite always changes at least one element until termination).
// The key observation is that the only value that matters is the last element of the initial array, call it `target`. The process repeatedly extends a contiguous suffix of `target` values backward. Initially, we count how many elements at the end are equal to `target` (call this `len`). Each operation doubles `len` (but not exceeding `n`), and then all positions from `n-len+1` to `n` are set to `target`. Since we only ever overwrite with `target`, any element not equal to `target` must be overwritten at some point. The process terminates when `len >= n`. We repeat counting the current suffix length after each overwrite; note that after overwriting, the suffix length may increase beyond 2×previous because the newly overwritten portion might connect with previously equal elements, but since we only double the original `len` in that operation, the next count could be larger than that doubled value if the newly overwritten region is contiguous to already equal ones. To simulate correctly, we simply follow the description: each operation doubles the current `len` (after the previous count), overwrite, then recount the suffix length. The worst-case number of operations is logarithmic in `n` because `len` doubles each time (and if it exceeds `n` we stop). The time complexity is \( O(n \log n) \) in the worst case because each operation scans the array to recount suffix length (at most `n` per operation, and at most `\log n` operations). Space complexity is \( O(1) \) beyond the input array. Edge case: if the array is already all equal to the last element, the initial suffix length is `n`, so the loop breaks immediately and returns 0. If the array has length 1, it returns 0.
#include <vector>
#include <algorithm>

// Returns the number of operations needed to make all elements equal to the last element.
int minimum_operations(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) return 0; // not expected per spec, but safe
    const int target = arr[n - 1];

    // Count initial suffix of equal target values.
    int len = 1;
    for (int i = n - 2; i >= 0; --i) {
        if (arr[i] == target) ++len;
        else break;
    }
    if (len == n) return 0;

    int operations = 0;
    while (true) {
        // Check if all are equal to target.
        bool all_equal = true;
        for (int i = n - 1; i >= 0; --i) {
            if (arr[i] != target) {
                all_equal = false;
                break;
            }
        }
        if (all_equal) break;

        ++operations;
        len = std::min(n, len * 2); // double the suffix length, capped at n
        // Overwrite the suffix of length len with target.
        for (int i = n - len; i < n; ++i) {
            arr[i] = target;
        }
        // Recount the actual suffix length after overwrite.
        len = 1;
        for (int i = n - 2; i >= 0; --i) {
            if (arr[i] == target) ++len;
            else break;
        }
        // If len already equals n, extra iteration is unnecessary but loop will break.
    }
    return operations;
}
#include <vector>
#include <cassert>

// Declare the solution function (for testing, include the definition above).
int minimum_operations(std::vector<int>& arr);

int main() {
    std::vector<int> a1 = {1, 2, 3, 4};
    assert(minimum_operations(a1) == 3); // 3 operations needed

    std::vector<int> a2 = {5, 5, 5, 5};
    assert(minimum_operations(a2) == 0);

    std::vector<int> a3 = {1};
    assert(minimum_operations(a3) == 0);

    std::vector<int> a4 = {2, 1, 1, 1};
    assert(minimum_operations(a4) == 1);

    std::vector<int> a5 = {1, 2, 2, 1, 1};
    assert(minimum_operations(a5) == 2);

    std::vector<int> a6 = {7, 7, 3, 7, 7};
    assert(minimum_operations(a6) == 2);

    std::vector<int> a7 = {0, -1, -1, -1, -1};
    assert(minimum_operations(a7) == 1);

    std::vector<int> a8 = {1, 2, 3, 2, 2};
    assert(minimum_operations(a8) == 2);

    std::vector<int> a9 = {1, 2, 3, 4, 5, 5};
    assert(minimum_operations(a9) == 3);

    std::vector<int> a10 = {9, 9, 8, 9, 9, 9};
    assert(minimum_operations(a10) == 1);
    return 0;
}
