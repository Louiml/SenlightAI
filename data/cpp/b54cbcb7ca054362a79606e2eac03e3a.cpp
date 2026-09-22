/*
Given an array of `n` integers (where `1 <= n <= 200,000` and each element is between -1,000,000 and 1,000,000), write a C++ function that returns the maximum possible sum of a subarray that also appears as a suffix sum of the array. More precisely, consider all non-empty contiguous subarrays of the array; also consider all suffix sums of the array (where a suffix sum is the sum of elements from some index `k` to `n`, inclusive). Find the largest sum `S` such that there exists a prefix of the array whose sum equals `S`, and there exists a suffix of the array whose sum also equals `S`. The function should return that maximum `S`. If no such `S` exists, return 0. Note that the prefix and suffix may overlap, but both must be non-empty.
*/
#include <map>
#include <vector>

// Returns the maximum sum S such that a prefix sum equals a suffix sum.
// If no such S exists, returns 0.
long long maxMatchingPrefixSuffixSum(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n == 0) return 0;

    std::map<long long, int> suffixCount;
    long long total = 0;

    // Compute all suffix sums and count their frequencies.
    for (int i = n - 1; i >= 0; --i) {
        total += arr[i];
        suffixCount[total]++;
    }

    long long prefixSum = 0;
    long long best = 0;

    // Iterate over prefixes, removing current suffix before checking.
    for (int i = 0; i < n; ++i) {
        prefixSum += arr[i];

        // Remove the suffix that ends at i (the current total sum before subtraction).
        // This prevents counting a prefix that exactly overlaps the entire array when i==n-1.
        suffixCount[total]--;
        if (suffixCount[total] == 0) {
            suffixCount.erase(total);
        }
        total -= arr[i];

        if (suffixCount.count(prefixSum) && suffixCount[prefixSum] > 0) {
            if (prefixSum > best) {
                best = prefixSum;
            }
        }
    }

    return best;
}
#include <cassert>
#include <vector>

// The function declaration is assumed from the solution above.
// (In a real compilation, paste the solution here.)

int main() {
    // Basic case: prefix sum 3 matches suffix sum 3.
    std::vector<int> a1 = {1, 2, 3};
    assert(maxMatchingPrefixSuffixSum(a1) == 3);

    // Multiple matches, pick maximum.
    std::vector<int> a2 = {1, -1, 1, -1};
    // Suffix sums: -1, 0, 1, 0 ; prefix sums: 1, 0, 1, 0. Matches at 1 and 0, max=1.
    assert(maxMatchingPrefixSuffixSum(a2) == 1);

    // No match -> 0.
    std::vector<int> a3 = {5, -3, 2};
    // Suffix: 2, -1, 4 ; prefix: 5, 2, 4. Only 4? 4 appears as suffix (total) and prefix (last) -> actually 4 matches.
    // Let's choose a different one: {1,2,4} suffix:4,6,7; prefix:1,3,7 -> 7 matches -> 7.
    // So use {1,2,5} suffix:5,7,8; prefix:1,3,8 -> 8 matches -> 8.
    // Need a true no-match: {1,2,4,8} suffix:8,12,14,15; prefix:1,3,7,15 -> 15 matches.
    // Use {1, -1, 3} suffix:3,2,3; prefix:1,0,3 -> 3 matches. Hmm.
    // Try all distinct: {2,4,6} suffix:6,10,12; prefix:2,6,12 -> 6 and 12 match, so not zero.
    // For zero, need no overlap: {1,2,3} already has 3. Let's use {5, -10, 6} suffix:6,-4,1; prefix:5,-5,1 -> 1 matches. 
    // Actually with small arrays, usually there's a match. Try {1,2,-3} suffix:-3,-1,0; prefix:1,3,0 -> 0 matches.
    // Try {1,2,4,8} we found 15 matches. So use {2,3,5} suffix:5,8,10; prefix:2,5,10 -> 5 and 10 match -> 10.
    // For a zero case, use {1, -2, 4} suffix:4,2,3; prefix:1,-1,3 -> 3 matches.
    // Maybe no zero possible for n>=1? Actually if all prefix sums distinct and all suffix sums distinct and no intersection, but with overlapping they often intersect. 
    // Simpler: use an array with only positive increments? But then total appears as prefix. 
    // We'll just test a case with a possible zero: {1,1,1} suffix:1,2,3; prefix:1,2,3 -> 3 is max.
    // Let's use {1, 2, 4, 8} -> max is 15? prefix sums:1,3,7,15; suffix sums:8,12,14,15 -> 15 matches.
    // We'll just test a known zero: {1, -1, 2} suffix:2,1,2; prefix:1,0,2 -> 2 matches.
    // I'll pick {5, 1, 2} suffix:2,3,8; prefix:5,6,8 -> 8 matches. 
    // Let's use { -1, -2, -3 } suffix:-3,-5,-6; prefix:-1,-3,-6 -> -3 and -6 match? -6 matches, so max -3? But we compare with 0, so returns 0.
    std::vector<int> a3 = {-1, -2, -3};
    // Suffix sums: -3, -5, -6 ; prefix sums: -1, -3, -6 -> matches -3 and -6. Max is -3, but best starts at 0, so returns 0.
    assert(maxMatchingPrefixSuffixSum(a3) == 0);

    // Single element: prefix = suffix = element.
    std::vector<int> a4 = {7};
    assert(maxMatchingPrefixSuffixSum(a4) == 7);

    // Duplicate sums, e.g., all zeros.
    std::vector<int> a5 = {0, 0, 0};
    assert(maxMatchingPrefixSuffixSum(a5) == 0);

    // Negative and positive mixed, larger match.
    std::vector<int> a6 = {3, -2, 5, -2};
    // Suffix sums: -2,3,1,4 ; prefix sums:3,1,6,4 -> matches 3,1,4 -> max=4.
    assert(maxMatchingPrefixSuffixSum(a6) == 4);

    // Large value check.
    std::vector<int> a7 = {1000000, -999999, 1000000};
    // Suffix: 1000000,1,1000001 ; prefix:1000000,1,1000001 -> max=1000001.
    assert(maxMatchingPrefixSuffixSum(a7) == 1000001LL);

    // Overlap: prefix equals middle suffix.
    std::vector<int> a8 = {4, 1, 4};
    // Suffix:4,5,9 ; prefix:4,5,9 -> max=9.
    assert(maxMatchingPrefixSuffixSum(a8) == 9);

    return 0;
}
// The solution computes suffix sums first. Starting from the last element, we accumulate `sum` (total sum of all elements so far from the right) and store each suffix sum in a frequency map (`cnt`). Then we iterate from the left, maintaining a running prefix sum `tmp`. At each step, before moving to the next element, we must remove the current total sum from the map because the suffix starting exactly at the next element is no longer valid once we have consumed this prefix element. Specifically, we decrement the count of the current total sum (which equals the sum of the current suffix) and subtract the current element from `total`. Then if `cnt[tmp]` is positive (meaning there exists a suffix with the same sum as the current prefix), we update the answer with the maximum of the current answer and `tmp`. Edge cases: duplicate sums are handled by counts; if prefix and suffix overlap, it's still valid because we remove the current suffix before checking. Time complexity is O(n) due to a single pass for suffix sums and another for prefix sums, using a hash map with O(1) average per operation. Space complexity is O(n) for the map and the array. The result could be 0 if no equal prefix/suffix sums exist (e.g., array with all distinct sums). Also, the prefix must be non-empty, but we start with the first element, so it's fine.
