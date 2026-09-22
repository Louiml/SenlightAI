/*
Write a C++ function that, given a vector of integers `A` of length `N`, simulates a process where the array represents a sequence of digits (0-9). Starting from the last element, if it is not equal to 9, return the original `N`. If the last element is 9, scan backward from the second-to-last element toward the front. For each `9` encountered, decrement a counter that initially equals `N`. If a non-9 digit is found, return the counter minus 1. If the entire array consists of 9s, return 0. The function must handle edge cases such as empty vectors (return 0) and vectors with a single element and the last element being 9 (return 0). Ensure the function is `const` correct and does not modify the input vector.
*/
#include <vector>

// Given a vector of digits (0-9), return the number of leading elements before the trailing run of 9s.
// If the last element is not 9, return N. If all are 9s, return 0.
int countNonTrailingNines(const std::vector<int>& A) {
    const int n = static_cast<int>(A.size());
    if (n == 0) {
        return 0;
    }
    int count = n;
    // Scan from the end while encountering 9s.
    for (int i = n - 1; i >= 0 && A[i] == 9; --i) {
        --count;
    }
    return count;
}
#include <cassert>
#include <vector>

int countNonTrailingNines(const std::vector<int>& A);

int main() {
    // Non-9 at the end: returns N
    assert(countNonTrailingNines({0, 1, 2}) == 3);
    assert(countNonTrailingNines({7}) == 1);
    
    // Last digit is 9, but not all are 9s
    assert(countNonTrailingNines({1, 9}) == 1);
    assert(countNonTrailingNines({1, 2, 9}) == 2);
    assert(countNonTrailingNines({1, 9, 9, 9}) == 1);
    
    // All 9s
    assert(countNonTrailingNines({9}) == 0);
    assert(countNonTrailingNines({9, 9, 9, 9}) == 0);
    
    // Edge case: empty vector
    std::vector<int> empty;
    assert(countNonTrailingNines(empty) == 0);
    
    // Mixed with multiple trailing 9s
    assert(countNonTrailingNines({0, 9, 9}) == 1);
    assert(countNonTrailingNines({8, 5, 9, 9, 9}) == 2);
    
    // Single non-9 with leading 9s
    assert(countNonTrailingNines({9, 9, 5}) == 3);
    
    // All non-9
    assert(countNonTrailingNines({3, 4, 5}) == 3);
    
    return 0;
}
// The problem reduces to finding how many trailing 9s exist in the array. If the last digit is not 9, the answer is `N`. If the last digit is 9, we need to count how many consecutive 9s are at the end. If the whole array is 9s, the answer is 0; otherwise, the answer is `N` minus the number of trailing 9s. The provided snippet has a subtle off-by-one error in its loop logic; a clean approach is to use a simple traversal from the end. We initialize `count = N`, then loop backward from `i = N-1` down to 0 while `A[i] == 9`; for each such element decrement `count`. Stop when we find a non-9 or reach the beginning. The final `count` is the answer. This avoids the messy multiple returns in the original snippet. Edge cases: empty vector returns 0; single element 9 returns 0; single element non-9 returns 1. Time complexity is O(N) in the worst case (all 9s) and O(1) average if the last digit is not 9 because we check immediately. Space complexity is O(1) extra. The solution should be self-contained and properly const-correct.
