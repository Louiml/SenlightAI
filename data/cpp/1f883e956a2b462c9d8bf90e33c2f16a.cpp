Write a C++ function that solves the classic "subset sum counting" problem using the Meet-in-the-Middle technique, but with a twist: the function must return the count of subsets whose sum equals a target value `x`, given an array of positive integers `t` (size up to 40). The function must use the two-pointer optimization instead of binary search, and must correctly handle the case where `n` is odd (with the split point being `n/2` for the first half and `n/2 + 1` for the second half). Your function should take the array size `n`, the target `x`, and a `std::vector<int>` of the array values (1-indexed) as parameters, and return a `long long` count. The subset sum can be large, so use appropriate integer types.
#include <cassert>
#include <vector>

// Include the solution function here (or place in header).
// For brevity, the function is assumed to be defined above.

int main() {
    // Test 1: Basic example from the snippet
    {
        int n = 5;
        int x = 10;
        std::vector<int> t = {0, 1, 2, 3, 4, 5}; // 1-indexed, indices 1..5
        // Subsets summing to 10: {1,4,5}, {2,3,5}, {1,2,3,4} -> 3
        assert(countSubsetSumMeetInMiddle(n, x, t) == 3);
    }

    // Test 2: All subsets sum to 0 (empty subset only)
    {
        int n = 1;
        int x = 0;
        std::vector<int> t = {0, 5}; // t[1] = 5
        assert(countSubsetSumMeetInMiddle(n, x, t) == 1); // empty subset
    }

    // Test 3: No subset matches
    {
        int n = 3;
        int x = 100;
        std::vector<int> t = {0, 1, 2, 3};
        assert(countSubsetSumMeetInMiddle(n, x, t) == 0);
    }

    // Test 4: Odd n, all elements equal to 1, x = n
    {
        int n = 7;
        int x = 7;
        std::vector<int> t(8, 1); // t[1..7] = 1
        // Only one subset: all elements included
        assert(countSubsetSumMeetInMiddle(n, x, t) == 1);
    }

    // Test 5: Duplicate subset sums (two identical elements)
    {
        int n = 2;
        int x = 2;
        std::vector<int> t = {0, 1, 1};
        // Subsets summing to 2: {1,1} (both elements) and also {1} from first alone? No, sum=1. 
        // Actually subsets summing to 2: only the full set {1,1} -> 1
        assert(countSubsetSumMeetInMiddle(n, x, t) == 1);
    }

    // Test 6: x = 0, all non-zero elements
    {
        int n = 4;
        int x = 0;
        std::vector<int> t = {0, 2, 3, 5, 7};
        assert(countSubsetSumMeetInMiddle(n, x, t) == 1); // empty subset only
    }

    // Test 7: Larger example, n=4, all ones, x=2
    {
        int n = 4;
        int x = 2;
        std::vector<int> t = {0, 1, 1, 1, 1};
        // Choose 2 out of 4 elements: combinations = 6
        assert(countSubsetSumMeetInMiddle(n, x, t) == 6);
    }

    // Test 8: Extreme: n=0 (no elements) but the function handles it specially? 
    // The problem specification says array size up to 40, but we test boundary.
    {
        int n = 0;
        int x = 0;
        std::vector<int> t = {0}; // t[1] doesn't exist, but we don't access it
        assert(countSubsetSumMeetInMiddle(n, x, t) == 1);
    }

    // Test 9: All subsets sum exceeds x; no count
    {
        int n = 2;
        int x = 3;
        std::vector<int> t = {0, 5, 6};
        assert(countSubsetSumMeetInMiddle(n, x, t) == 0);
    }

    // Test 10: Mixed values with multiple combinations
    {
        int n = 6;
        int x = 10;
        std::vector<int> t = {0, 1, 2, 3, 4, 5, 6};
        // Manually count subsets summing to 10:
        // single numbers: 4+6, 5+5? no duplicates, 1+3+6, 1+4+5, 2+3+5, 2+4+4? no, 3+7? no, 1+2+3+4? sum=10 -> yes
        // Let's systematically: subsets:
        // {1,2,3,4}, {1,3,6}, {1,4,5}, {2,3,5}, {4,6} -> total 5
        assert(countSubsetSumMeetInMiddle(n, x, t) == 5);
    }

    return 0;
}
#include <vector>
#include <algorithm>

// Count subsets of t[1..n] whose sum equals x using Meet-in-the-Middle and two pointers.
long long countSubsetSumMeetInMiddle(int n, int x, const std::vector<int>& t) {
    if (n == 0) {
        return (x == 0) ? 1 : 0;
    }

    int half = n / 2;
    std::vector<int> A, B;

    // Generate all subset sums for first half (indices 1..half)
    auto generateFirst = [&](auto&& self, int i, int sum) -> void {
        if (sum > x) return;
        if (i > half) {
            A.push_back(sum);
            return;
        }
        self(self, i + 1, sum);
        self(self, i + 1, sum + t[i]);
    };
    generateFirst(generateFirst, 1, 0);

    // Generate all subset sums for second half (indices half+1..n)
    auto generateSecond = [&](auto&& self, int i, int sum) -> void {
        if (sum > x) return;
        if (i > n) {
            B.push_back(sum);
            return;
        }
        self(self, i + 1, sum);
        self(self, i + 1, sum + t[i]);
    };
    generateSecond(generateSecond, half + 1, 0);

    // Sort A descending so that x - A[i] is non-decreasing (target for B)
    std::sort(A.begin(), A.end(), std::greater<int>());
    std::sort(B.begin(), B.end());

    long long count = 0;
    size_t j1 = 0, j2 = 0;
    for (int s : A) {
        int target = x - s;
        while (j1 < B.size() && B[j1] < target) ++j1;
        while (j2 < B.size() && B[j2] <= target) ++j2;
        count += static_cast<long long>(j2 - j1);
    }

    return count;
}
// The brute-force backtracking over all subsets has complexity \(O(2^n)\), which is too slow for \(n\) up to 40. The Meet-in-the-Middle approach splits the array into two halves of roughly equal size. We generate all possible subset sums for the first half (indices 1..n/2) and for the second half (indices n/2+1..n). For every subset sum `sA` from the first half, we need to count how many subset sums `sB` from the second half satisfy `sA + sB == x`, i.e., `sB == x - sA`. To count these efficiently, we sort the second-half sums. We then use a two-pointer technique: for each `sA` in the first half, we maintain two indices `j1` and `j2` in the sorted second-half array that point to the first element greater than or equal to the target and the first element strictly greater than the target, respectively. The difference `j2 - j1` gives the number of matching sums. Since `j1` and `j2` only move forward as `sA` changes (because `x - sA` is non-increasing if we sort the first-half sums in non-increasing order), the total time is linear in the number of generated sums after sorting. Edge cases include: `x` being larger than the sum of all elements (count = 0), empty subsets (sum = 0) being valid, and the array size being exactly 1 (the split still works). The number of subsets in each half is at most \(2^{20}\) (since \(n \le 40\)), so storing them requires about a million integers per half, which is fine. Time complexity is \(O(2^{n/2} \log 2^{n/2} + 2^{n/2})\) for sorting and two-pointer, overall \(O(2^{n/2} \log 2^{n/2})\). Space complexity is \(O(2^{n/2})\).
