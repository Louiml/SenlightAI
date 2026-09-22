/*
Write a C++ function `bool canDivideIntoThreeEqualSums(std::array<int, 6> a)` that takes an array of exactly 6 integers, where each integer is between 1 and 10^9 inclusive, and returns `true` if there exist three distinct indices `i`, `j`, `k` such that the sum of the elements at those indices is exactly half of the total sum of all 6 elements. Equivalently, the six values can be partitioned into two groups of three elements each, where both groups have the same sum. The function must return `false` if no such triplet exists. The input may contain duplicated values, and the three chosen indices must be pairwise distinct. Note that the total sum of all 6 elements is always even in valid test cases, but your function must handle both even and odd totals gracefully (i.e., if the total is odd, no equal split is possible — return `false`). Provide a complete solution with proper `#include` directives, and ensure the function is reusable (no main function in the solution section).
*/

#include <array>
#include <numeric>

// Returns true if there exist three distinct indices i, j, k such that
// a[i] + a[j] + a[k] == total_sum / 2.
// Returns false otherwise. Handles odd total sums automatically.
bool canDivideIntoThreeEqualSums(const std::array<int, 6>& a) {
    const int total = std::accumulate(a.begin(), a.end(), 0);
    
    // If the total is odd, it's impossible to get exactly half as an integer sum.
    if (total % 2 != 0) {
        return false;
    }
    
    const int half = total / 2;
    
    // Enumerate all triplets of distinct indices (i < j < k).
    for (int i = 0; i < 6; ++i) {
        for (int j = i + 1; j < 6; ++j) {
            for (int k = j + 1; k < 6; ++k) {
                if (a[i] + a[j] + a[k] == half) {
                    return true;
                }
            }
        }
    }
    
    return false;
}

#include <array>
#include <cassert>

// The solution function is assumed to be available from the Solution section.
bool canDivideIntoThreeEqualSums(const std::array<int, 6>& a);

int main() {
    // Test 1: same sum split (1+2+3 = 6, 4+5+6 = 15? Not equal, so false)
    std::array<int, 6> a1 = {1, 2, 3, 4, 5, 6};
    assert(canDivideIntoThreeEqualSums(a1) == false);
    
    // Test 2: total = 12, half = 6, a triplet sums to 6 (1,2,3)
    std::array<int, 6> a2 = {1, 2, 3, 4, 5, 9};
    assert(canDivideIntoThreeEqualSums(a2) == true);
    
    // Test 3: all equal, total = 30, half = 15, any 3 values sum to 15
    std::array<int, 6> a3 = {5, 5, 5, 5, 5, 5};
    assert(canDivideIntoThreeEqualSums(a3) == true);
    
    // Test 4: odd total (sum = 21), immediately false
    std::array<int, 6> a4 = {3, 3, 3, 3, 3, 6};
    assert(canDivideIntoThreeEqualSums(a4) == false);
    
    // Test 5: total = 14, half = 7, need triplet summing to 7, but values are 1,2,3,4,5,6? Actually 1+2+4=7 exists
    std::array<int, 6> a5 = {1, 2, 3, 4, 5, 6};
    // 1+2+4=7, so true
    assert(canDivideIntoThreeEqualSums(a5) == true);
    
    // Test 6: total = 14, but no triplet sums to 7? Let's use 1,1,1,1,1,9 -> total 14, half 7, no triplet sums to 7 (since max sum of three 1's is 3, and 9 alone can't be with two others to make 7)
    std::array<int, 6> a6 = {1, 1, 1, 1, 1, 9};
    assert(canDivideIntoThreeEqualSums(a6) == false);
    
    // Test 7: large values, total even, but no split possible
    std::array<int, 6> a7 = {1000000000, 1, 2, 3, 4, 5};
    // total = 1000000015, which is odd -> false
    assert(canDivideIntoThreeEqualSums(a7) == false);
    
    // Test 8: perfect split with duplicate values
    std::array<int, 6> a8 = {2, 2, 2, 2, 2, 2};
    // total = 12, half = 6, any triplet of 2's sums to 6 -> true
    assert(canDivideIntoThreeEqualSums(a8) == true);
    
    // Test 9: split where the half equals a single large value plus two zeros? But no zeros, so just checking extreme.
    std::array<int, 6> a9 = {10, 10, 20, 20, 30, 30};
    // total = 120, half = 60, 10+20+30 = 60 -> true
    assert(canDivideIntoThreeEqualSums(a9) == true);
    
    // Test 10: total even, but no triplet hits half: e.g., {1,1,1,1,2,2} total=8 half=4, triplets: 1+1+1=3, 1+1+2=4 actually yes -> true, so change to {1,1,1,1,1,3} total=8 half=4, triplet max sum=1+1+3=5? Actually 1+1+3=5, 1+1+1=3, no 4 -> false
    std::array<int, 6> a10 = {1, 1, 1, 1, 1, 3};
    assert(canDivideIntoThreeEqualSums(a10) == false);
    
    return 0;
}

// The problem reduces to checking whether there exists a subset of exactly 3 elements whose sum equals exactly half of the total sum. Since there are only 6 elements, we can brute-force all combinations of 3 distinct indices. There are C(6,3) = 20 such combinations, which is trivially small. For each combination, compute the sum of the selected three values. If twice that sum equals the total sum, then the remaining three values must automatically sum to the other half, so the answer is `true`. If the total sum is odd, then half of it is not an integer, and no integer sum of three elements can equal it, so we can immediately return `false` without iterating. Edge cases include: all six values being equal (any triplet works), values with zeros (if zeros are allowed, but the problem states positive integers, so zero won't appear), duplicate values (the triplets by index still work), and the case where the total sum is less than twice any possible triplet sum (but since all are positive, the total is finite, and we just check equality). Time complexity is O(1) as there are only 20 fixed iterations, and space complexity is O(1) aside from the input array.
