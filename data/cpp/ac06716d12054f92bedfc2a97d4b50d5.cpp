Given an array `A` of `N` positive integers and a target value `M`, write a C++ function `countSubarraysWithSum` that returns the number of contiguous subarrays whose elements sum exactly to `M`. The function must operate in-place (no extra array for prefix sums) and must stop early during inner-loop summation once the running sum exceeds `M` (since all values are positive). The input array is guaranteed to contain only positive integers, and `N` may be as large as 10,000. Handle edge cases where no subarray sums to `M` (return 0), where a single element equals `M` (count it once), and where the entire array sums to `M` (count once). The function must accept the array by pointer/reference, its size, and the target, and return an integer count.
// The solution uses a brute-force enumeration of all possible starting indices `i` from 0 to `N-1`. For each starting index, we reset a running `sum` to 0 and then iterate `j` from `i` to `N-1`, accumulating `A[j]`. If the sum equals `M`, we increment the answer and break out of the inner loop because any further addition (all positive) will only increase the sum beyond `M`. If the sum exceeds `M`, we also break. If the sum stays below `M` for all `j`, the loop naturally ends. This early termination is correct only because all elements are positive—once the sum passes `M`, it can never decrease, so no later `j` from the same `i` can produce an exact match. Edge cases include: a single element equal to `M` (counted when `i` is that index and `j` starts there), repeated matches at different intervals (each counted separately), and no match at all. Time complexity is worst-case \(O(N^2)\) when `M` is large and most sums remain below it (e.g., all elements are 1 and `M` > N), but with the given constraints (N ≤ 10,000) this is acceptable; average case benefits from early breaks. Space complexity is \(O(1)\) beyond the input array.
#include <cstddef> // for size_t

// Count contiguous subarrays whose sum equals target M.
// Assumes all elements in A are positive integers.
int countSubarraysWithSum(const int* A, size_t N, int M) {
    int answer = 0;
    for (size_t i = 0; i < N; ++i) {
        int sum = 0;
        for (size_t j = i; j < N; ++j) {
            sum += A[j];
            if (sum == M) {
                ++answer;
                break; // all positive, further sums only increase
            }
            if (sum > M) {
                break; // no chance for exact match from this i
            }
        }
    }
    return answer;
}
#include <cassert>

int main() {
    // Basic cases
    int A1[] = {1, 1, 1};
    assert(countSubarraysWithSum(A1, 3, 3) == 1); // [1,1,1]
    assert(countSubarraysWithSum(A1, 3, 2) == 2); // [1,1] at start and end

    // Single element equals M
    int A2[] = {5};
    assert(countSubarraysWithSum(A2, 1, 5) == 1);
    assert(countSubarraysWithSum(A2, 1, 4) == 0);

    // No match
    int A3[] = {2, 4, 6};
    assert(countSubarraysWithSum(A3, 3, 10) == 0);
    assert(countSubarraysWithSum(A3, 3, 12) == 1); // [2,4,6]

    // Larger test with multiple matches
    int A4[] = {1, 2, 3, 1, 2};
    assert(countSubarraysWithSum(A4, 5, 3) == 3); // [1,2], [3], [1,2]

    // All ones, M = 1
    int A5[] = {1, 1, 1, 1};
    assert(countSubarraysWithSum(A5, 4, 1) == 4);

    // Sum exceeds M early break
    int A6[] = {10, 1, 1};
    assert(countSubarraysWithSum(A6, 3, 2) == 1); // [1,1] only

    // Empty array (N=0) returns 0
    assert(countSubarraysWithSum(nullptr, 0, 0) == 0);

    // Large values, M = 100
    int A7[] = {50, 50, 1};
    assert(countSubarraysWithSum(A7, 3, 100) == 1); // [50,50]
    assert(countSubarraysWithSum(A7, 3, 101) == 1); // [50,50,1]
}
