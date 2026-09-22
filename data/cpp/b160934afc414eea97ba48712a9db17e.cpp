// Write a C++ function that, given a vector of integers (each either 1 or 2), returns the number of unordered pairs (i, j) with i < j such that the sum of the two elements at those positions is not equal to 3. That is, count pairs where the two numbers are not one 1 and one 2. The input is guaranteed to contain only 1s and 2s, but may be empty (in which case the answer is 0). The function must be efficient for up to 10^5 elements.
The key insight is that the only "bad" pairs are those consisting of one 1 and one 2 (since 1+2=3). All other pairs—(1,1) and (2,2)—have sums 2 and 4, respectively, which are acceptable. Therefore, we can count the number of 1s (`cnt1`) and 2s (`cnt2`) in the array. The number of valid pairs is the total number of distinct unordered pairs minus the number of mixed pairs. Total pairs = n*(n-1)/2 (where n is the array size). Mixed pairs = cnt1 * cnt2 (each 1 paired with each 2). So answer = n*(n-1)/2 - cnt1*cnt2. Alternatively, directly compute valid pairs as cnt1*(cnt1-1)/2 + cnt2*(cnt2-1)/2, which counts pairs within each group. Both formulas are equivalent. Edge cases: empty array (return 0), all elements the same, and very large n (use 64-bit integers to avoid overflow). Time complexity is O(n) for a single pass, and space complexity is O(1) auxiliary (excluding the input vector).
#include <vector>

// Count unordered pairs (i,j) with i<j whose sum is not equal to 3.
// Input vector contains only 1s and 2s.
long long countValidPairs(const std::vector<int>& values) {
    long long cnt1 = 0;
    long long cnt2 = 0;
    for (int v : values) {
        if (v == 1) {
            ++cnt1;
        } else if (v == 2) {
            ++cnt2;
        }
        // No other values expected.
    }
    // Valid pairs: all pairs where both are 1, or both are 2.
    return cnt1 * (cnt1 - 1) / 2 + cnt2 * (cnt2 - 1) / 2;
}
#include <cassert>
#include <vector>

int main() {
    // Test empty
    assert(countValidPairs({}) == 0);
    
    // Test single element
    assert(countValidPairs({1}) == 0);
    assert(countValidPairs({2}) == 0);
    
    // Test all same
    assert(countValidPairs({1, 1, 1}) == 3); // C(3,2)=3 pairs, all valid
    assert(countValidPairs({2, 2}) == 1);
    
    // Test mixed
    assert(countValidPairs({1, 2}) == 0); // only pair is (1,2) sum=3 invalid
    assert(countValidPairs({1, 2, 1}) == 1); // pairs: (1,2) invalid, (1,1) valid, (2,1) invalid -> 1
    assert(countValidPairs({2, 2, 1, 1, 1}) == 4); // 2s: C(2,2)=1, 1s: C(3,2)=3, total=4
    
    // Test larger case with many elements (use 64-bit result)
    std::vector<int> large(100000, 1);
    assert(countValidPairs(large) == 100000LL * 99999 / 2);
    
    return 0;
}
