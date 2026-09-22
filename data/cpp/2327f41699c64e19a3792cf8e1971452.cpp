// Given an array `A` of `N` distinct positive integers (where `1 ≤ N ≤ 100` and each integer is between 1 and 100) and a starting index `K` (1-based), write a C++ function `int resolveCycle(const int A[], int N, int K)` that repeatedly replaces `K` with the value `A[K-1]` (since `A` is 0-indexed) until the current `K` equals the value stored at that position (i.e., the condition `A[currentK-1] == currentK` is true). The function must return that final value of `K`. The input will always be such that this process terminates, and you must not modify the input array.
The problem is a functional graph traversal where each index `i` (1-based) has an outgoing edge to the value `A[i-1]`. Since the process is defined as `while(A[K-1] != K) K = A[K-1]`, we are essentially following the chain starting from `K` until we reach a fixed point where the value at the position equals the position itself (a self-loop in the mapping). Because the numbers are distinct and bounded, the sequence cannot get stuck in a non-repeating cycle without entering a fixed point; indeed, the termination condition is precisely hitting a self-loop. The naive loop runs at most `N` times because each step moves to a new index, and the process is guaranteed to end when we reach the fixed point. The algorithm uses no extra space besides a few variables, and time complexity is `O(N)` in the worst case (since the length of the chain is at most the number of elements). Edge cases include starting at a fixed point (returns immediately), and the fact that the index `K` is 1-based while the array is 0-based. The input is guaranteed to be valid, so no out-of-bounds or infinite loops occur.
#include <vector>

// Given an array A of distinct positive integers (1..100) and a 1-based starting index K,
// follow the mapping K -> A[K-1] until the mapping is fixed (A[K-1] == K),
// and return that final index.
int resolveCycle(const int A[], int N, int K) {
    // Follow the chain until we reach a fixed point.
    while (A[K - 1] != K) {
        K = A[K - 1];
    }
    return K;
}
Note: The function uses `const int A[]` to enforce read-only access, and does not need `N` except for potential validation, but it is kept for interface compatibility. For safety, we could assert `1 <= K && K <= N` and that all A values are in range, but the problem guarantees valid input.
#include <cassert>

int resolveCycle(const int A[], int N, int K);

int main() {
    // Case 1: chain to fixed point.
    int A1[] = {2, 3, 4, 4};
    assert(resolveCycle(A1, 4, 1) == 4);

    // Case 2: start already at fixed point.
    int A2[] = {2, 1, 3};
    assert(resolveCycle(A2, 3, 3) == 3);

    // Case 3: longer chain.
    int A3[] = {2, 3, 4, 5, 5};
    assert(resolveCycle(A3, 5, 2) == 5);

    // Case 4: single element.
    int A4[] = {1};
    assert(resolveCycle(A4, 1, 1) == 1);

    // Case 5: non-trivial path.
    int A5[] = {3, 4, 2, 4};
    assert(resolveCycle(A5, 4, 1) == 4);

    // Case 6: start at fixed point with others pointing to it.
    int A6[] = {3, 1, 3, 4};
    // A[0]=3, A[1]=1, A[2]=3, A[3]=4. Index 1 maps to 1? Actually A[1]=1, so index 1 is fixed? Check: index 1 (1-based) corresponds to A[0]=3, not 1. Let's do: A={3,1,3,4}. Index 1: A[0]=3 → K=3. Index 3: A[2]=3, so fixed at 3. Index 2: A[1]=1 → K=1 → again to 3. So from K=2: 2->1->3->3 → result 3.
    assert(resolveCycle(A6, 4, 2) == 3);

    // Case 7: multiple steps from K=4 gives fixed at 4.
    int A7[] = {2, 2, 4, 4};
    // Index 1 ->2 (A[0]=2), index 2 fixed (A[1]=2) → result 2.
    assert(resolveCycle(A7, 4, 1) == 2);

    return 0;
}
