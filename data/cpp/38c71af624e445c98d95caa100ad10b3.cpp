// Write a C++ function that takes a vector of integers of even length `2*N` and returns the count of indices `i` from `1` to `2*N-2` (inclusive) where the element immediately before and immediately after index `i` are equal (i.e., `A[i-1] == A[i+1]`). The vector length is guaranteed to be even and at least 2. Your function should not modify the input vector, and it should handle any integer values including negatives and duplicates. For example, given `[1, 2, 1, 3, 2, 2]`, the indices satisfying the condition are `i=1` (because `A[0]==A[2]` both = 1) and `i=4` (because `A[3]==A[5]` both = 3? actually check: A[3]=3, A[5]=2, no, so only i=1 and i=3? Let's compute properly: i=1: A[0]==A[2] → 1==1 true; i=2: A[1]==A[3] → 2==3 false; i=3: A[2]==A[4] → 1==2 false; i=4: A[3]==A[5] → 3==2 false. So answer = 1). Return the total count.

#include <cassert>
#include <vector>

// Function declaration from the solution (assume it's defined above).
int countMiddleEqualNeighbors(const std::vector<int>& A);

int main() {
    // Basic case: [1,2,1,3,2,2] → only i=1 (1==1) => 1
    assert(countMiddleEqualNeighbors({1, 2, 1, 3, 2, 2}) == 1);
    // All equal: [5,5,5,5] → i=1 (5==5) and i=2 (5==5) => 2
    assert(countMiddleEqualNeighbors({5, 5, 5, 5}) == 2);
    // No matches: [1,2,3,4] → i=1 (1!=3) and i=2 (2!=4) => 0
    assert(countMiddleEqualNeighbors({1, 2, 3, 4}) == 0);
    // Minimum size (N=1): length 2, no middle indices => 0
    assert(countMiddleEqualNeighbors({7, 8}) == 0);
    // Negative and duplicates: [-3,0,-3,0,-3] → i=1 (-3==-3) and i=3 (0==0?) check i=1: A[0]==A[2] -3==-3 true; i=2: A[1]==A[3] 0==0 true; i=3: A[2]==A[4] -3==-3 true => 3
    assert(countMiddleEqualNeighbors({-3, 0, -3, 0, -3}) == 3);
    // Larger vector with pattern: [1,2,1,2,1] → i=1,2,3 all true => 3
    assert(countMiddleEqualNeighbors({1, 2, 1, 2, 1}) == 3);
    return 0;
}

#include <vector>

// Count indices i (1 <= i <= size-2) where A[i-1] == A[i+1].
int countMiddleEqualNeighbors(const std::vector<int>& A) {
    const int size = static_cast<int>(A.size());
    int count = 0;
    for (int i = 1; i < size - 1; ++i) {
        if (A[i - 1] == A[i + 1]) {
            ++count;
        }
    }
    return count;
}

// The solution is a simple linear scan over the vector from index 1 to `2*N-2` (since we need `i-1` and `i+1` to be valid). For each such index, compare `A[i-1]` and `A[i+1]`. If equal, increment a counter. The edge cases: if `N <= 1`, then `2*N <= 2`, so there are no indices to check; the loop won't run and the answer is 0. Also, the input may contain any integers, but we only compare equality, so no arithmetic overflow concerns. Time complexity is O(N) (specifically O(2*N) iterations), and space complexity is O(1) beyond the input vector itself. The function should be `const`-correct by taking a `const std::vector<int>&` parameter.
