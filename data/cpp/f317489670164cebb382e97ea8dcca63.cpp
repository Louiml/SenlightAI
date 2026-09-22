/*
Write a C++ function that performs a recursive linear search on an integer array, returning a boolean indicating whether a given target value exists in the array. The function must not use loops or any standard library search algorithms. The array is passed by pointer along with its size, and the function must be const-correct, meaning it should not modify the array contents. Handle the empty-array case gracefully by returning false. You may assume the array size is non-negative; if it is zero, the function should immediately return false. The search should proceed from the last element toward the first, similar to the given code, but return a boolean instead of the value or zero.
*/

#include <cstddef> // for size_t (though we use int per spec)

// Recursive linear search: returns true if x is found in a[0..n-1].
// Searches from the last element toward the first. Does not modify the array.
bool recursiveLinearSearch(const int a[], int n, int x) {
    // Base case: no elements left to search
    if (n == 0) {
        return false;
    }
    // Check the last element; if match, return true
    if (a[n - 1] == x) {
        return true;
    }
    // Otherwise, search the remaining prefix (excluding the last element)
    return recursiveLinearSearch(a, n - 1, x);
}

int main() {
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {10, -5, 0, 7};
    int c[] = {42};
    int empty[] = {};

    // Basic search in a sorted array
    assert(recursiveLinearSearch(a, 5, 4) == true);
    assert(recursiveLinearSearch(a, 5, 6) == false);
    assert(recursiveLinearSearch(a, 5, 1) == true);
    assert(recursiveLinearSearch(a, 5, 5) == true);

    // Negative and zero values
    assert(recursiveLinearSearch(b, 4, -5) == true);
    assert(recursiveLinearSearch(b, 4, 0) == true);
    assert(recursiveLinearSearch(b, 4, 99) == false);

    // Single-element array
    assert(recursiveLinearSearch(c, 1, 42) == true);
    assert(recursiveLinearSearch(c, 1, 43) == false);

    // Empty array always returns false
    assert(recursiveLinearSearch(empty, 0, 1) == false);

    // Array with duplicates
    int d[] = {2, 2, 2};
    assert(recursiveLinearSearch(d, 3, 2) == true);

    return 0;
}

// The solution uses recursion to reduce the problem size by one at each step. The base case is when `n == 0`, meaning there are no elements left to examine, so we return `false`. If `n > 0`, we check the last element (`a[n-1]`); if it equals `x`, we return `true`; otherwise, we recursively call the function with `n-1` and the same array and target. This effectively searches from the end of the array to the beginning. Edge cases: empty array (`n == 0`) returns `false`; the target may be at the first element (index 0), which will be checked at the final recursion step when `n == 1`; duplicate values are irrelevant because we return `true` as soon as any match is found. Time complexity is `O(n)` in the worst case (target not present or at the beginning), and `O(1)` auxiliary space for each recursive call, but the call stack depth is `O(n)` in the worst case, giving `O(n)` space complexity overall. The function is const-correct by taking `const int a[]` (or `const int* a`) to guarantee the array is not modified.
