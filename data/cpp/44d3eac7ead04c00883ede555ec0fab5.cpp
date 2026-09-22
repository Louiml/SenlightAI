Write a C++ function named `maximumRecursive` that takes a non-empty array of integers and its length `n`, and returns the maximum value in the array. The function must be implemented recursively using the divide-and-conquer style shown in the snippet: for `n == 1`, return the single element; for `n > 1`, recursively compute the maximum of the first `n-1` elements and compare it with the last element. The function must be `const`-correct, meaning the array parameter should be a pointer to `const int` or `const int*` to indicate that the array is not modified. Do not use loops or built-in `std::max_element`. The solution should be self-contained with necessary headers, and must not include a `main` function in the solution section.
The recursive algorithm follows the recurrence: `maximumRecursive(A, n) = max( maximumRecursive(A, n-1), A[n-1] )`, with the base case returning `A[0]` when `n == 1`. This directly mirrors the provided snippet but with a more descriptive name and `const` correctness. The recursion depth is exactly `n` (from `n` down to `1`), so each call uses O(1) additional space on the call stack, yielding O(n) time and O(n) auxiliary space due to the recursion stack. Edge cases: the array must be non-empty (`n >= 1`) — the function assumes this precondition, so no check is needed. For `n == 1`, the function returns the sole element. For larger `n`, the recursion eventually reaches the base case, then unwinds, comparing each element once. No special handling is needed for duplicates or negative numbers; the comparison using `std::max` (or a ternary) handles all integer types.
#include <algorithm> // for std::max

// Recursively find the maximum value in the array A of length n.
// Precondition: A is non-empty, i.e., n >= 1.
// The array is not modified, hence const qualification.
int maximumRecursive(const int A[], int n) {
    // Base case: single element is the maximum of itself.
    if (n == 1) {
        return A[0];
    }
    // Recursive step: compare max of first n-1 elements with the last element.
    return std::max(maximumRecursive(A, n - 1), A[n - 1]);
}
#include <cassert>

int main() {
    int a1[] = {5};
    assert(maximumRecursive(a1, 1) == 5);

    int a2[] = {1, 2, 3, 4};
    assert(maximumRecursive(a2, 4) == 4);

    int a3[] = {4, 3, 2, 1};
    assert(maximumRecursive(a3, 4) == 4);

    int a4[] = {-1, -5, -3, -10};
    assert(maximumRecursive(a4, 4) == -1);

    int a5[] = {7, 7, 7, 7};
    assert(maximumRecursive(a5, 4) == 7);

    int a6[] = {100, -100, 0, 50, 99};
    assert(maximumRecursive(a6, 5) == 100);

    int a7[] = {0, -1, -2};
    assert(maximumRecursive(a7, 3) == 0);

    int a8[] = {42};
    assert(maximumRecursive(a8, 1) == 42);

    int a9[] = {1, 2};
    assert(maximumRecursive(a9, 2) == 2);

    int a10[] = {2, 1};
    assert(maximumRecursive(a10, 2) == 2);
}
