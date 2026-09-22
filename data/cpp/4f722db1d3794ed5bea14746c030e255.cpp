/*
Write a C++ function that, given a positive integer `n`, recursively calculates the sum of all integers from 1 to `n` (i.e., 1 + 2 + ... + n). The function must use recursion (not a loop) and must handle the base case correctly. Assume the input is always a positive integer (n ≥ 1). The function should return an `int` result. Do not include any input/output logic inside the function; it should only compute and return the sum.
*/
// Recursively compute the sum of integers from 1 to n.
// Precondition: n >= 1.
int recursiveSum(int n) {
    if (n == 1) {
        return 1;
    }
    return n + recursiveSum(n - 1);
}
int main() {
    assert(recursiveSum(1) == 1);
    assert(recursiveSum(2) == 3);
    assert(recursiveSum(3) == 6);
    assert(recursiveSum(4) == 10);
    assert(recursiveSum(5) == 15);
    assert(recursiveSum(10) == 55);
    assert(recursiveSum(100) == 5050);
    assert(recursiveSum(1000) == 500500);
    assert(recursiveSum(5000) == 12502500);
    return 0;
}
// The solution follows a standard recursive summation pattern. For any `n > 1`, the sum of 1 through `n` equals `n` plus the sum of 1 through `n-1`. The base case occurs when `n == 1`, in which case the sum is trivially 1. This recursion reduces the problem size by 1 each call, so it makes exactly `n` recursive calls.  
//
// Edge cases: The function must handle `n = 1` correctly (return 1) and should not be called with `n = 0` or negative numbers, as specified. For large `n` (close to INT_MAX), the recursion depth would be too large and could cause a stack overflow, but for reasonable test values (e.g., ≤ 10,000) it is safe.  
//
// Time complexity: O(n) — each recursive call does constant work.  
// Space complexity: O(n) — due to the call stack depth (each call holds a frame until the base case returns). For a sum that could be computed in O(1) with a formula, recursion is intentionally used per the task requirement.
