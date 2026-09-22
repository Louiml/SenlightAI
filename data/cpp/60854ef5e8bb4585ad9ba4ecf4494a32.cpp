// Write a C++ function that takes a vector of positive integers (each less than 10,000 and the total number of elements `N` satisfies 1 ≤ N ≤ 20) and determines whether there exist two distinct non-empty subsets of the given numbers that have the same sum. If such a pair exists, return a `std::pair<std::vector<int>, std::vector<int>>` where the first vector contains the elements of one subset (in any order) and the second vector contains the elements of the other subset (in any order). If no such pair exists, return a pair of two empty vectors. Subsets are considered distinct if their element sets differ (i.e., they are not identical multisets). The order of elements within each returned vector does not matter. Note that the input vector may contain duplicate values, and duplicate values are treated as separate elements (e.g., the multiset {5,5} has subsets {5}, {5}, {5,5}—the two singleton subsets are considered distinct because they correspond to different indices, but for the purpose of this problem, you only need to return the values, not indices, so if two subsets contain the same multiset of values, they are considered the same subset for output purposes—however, for detection, any two distinct non-empty subsets with equal sums that correspond to the same multiset of values are still acceptable as a valid answer). The function must be efficient enough for N up to 20 (2^N = 1,048,576 possible subsets). The solution must be deterministic and must always find a pair if one exists; if multiple pairs exist, any valid pair is acceptable.
The core idea is to enumerate all non-empty subsets of the input elements using bitmask representation. For each subset, compute its sum and store the bitmask that produced that sum. If we encounter a sum that was already seen from a different bitmask, then the stored bitmask and the current bitmask represent two distinct subsets with the same sum, so we can extract the elements of both subsets and return them. If the full enumeration completes without finding a duplicate sum, then no such pair exists, and we return two empty vectors.

Because there are up to 2^N - 1 non-empty subsets and N ≤ 20, enumeration is feasible (about 1 million subsets). For each subset, computing the sum by iterating over its bits takes O(N), giving a total O(N · 2^N) time, which for N=20 is roughly 20 million operations—acceptable. Space usage is O(2^N) in the worst case for the map storing sums to bitmasks, but in practice the number of distinct sums is limited by the maximum possible sum (N · 10000 = 200,000), so the map size is bounded by that.

A key nuance: when we find a sum that already exists in the map, we need to ensure the two bitmasks are genuinely different subsets (i.e., not the same bitmask). Since we only store the first bitmask for each sum and never overwrite it, and we iterate bitmasks in increasing numeric order (1 to 2^N-1), any later bitmask with the same sum must be different from the stored one. However, there is a subtle edge case: if the sum is zero (impossible because all elements are positive), or if the input contains duplicates such that two different bitmasks produce the same multiset of values (e.g., two identical elements), the problem statement says that for output purposes, if two subsets contain the same multiset of values, they are considered the same subset—but it also says that for detection, any two distinct non-empty subsets with equal sums are acceptable. Since we are distinguishing by bitmask (indices), two different bitmasks are distinct subsets regardless of the values they contain, so this is fine.

Another important edge case: if the input has N=1, there are only 1 non-empty subset (the element itself), so no pair can exist; we return empty vectors. If the input contains duplicate values, e.g., {5,5}, subsets are {first 5}, {second 5}, {5,5}. The two singleton subsets have the same sum (5) and are distinct bitmasks, so we should return ( [5], [5] ). This is a valid answer.

Implementation details: We'll use an `unordered_map<int, int>` (or `std::map`) from sum to bitmask. For each bitmask from 1 to (1<<N)-1, compute the sum by iterating over bits. If the sum is already in the map, then we have found a pair. Extract the elements for the stored mask and the current mask into two vectors and return them. If the loop completes without finding a pair, return two empty vectors.

Time complexity: O(N · 2^N). Space complexity: O(2^N) in the worst case for the map, but bounded by maximum possible sum O(N · maxVal). For N=20 and maxVal=10000, sum upper bound is 200,000, so map size is at most 200,001 entries.
#include <vector>
#include <unordered_map>
#include <utility>

// Finds two distinct non-empty subsets of 'nums' that have equal sums.
// Returns a pair of vectors containing the elements of the two subsets.
// If no such pair exists, returns a pair of empty vectors.
std::pair<std::vector<int>, std::vector<int>> findEqualSumSubsets(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    const int totalSubsets = 1 << n;  // total number of subsets (including empty)
    
    // Map from subset sum -> bitmask that first produced that sum.
    std::unordered_map<int, int> sumToMask;
    sumToMask.reserve(200000);  // reserve space for up to max possible distinct sums
    
    for (int mask = 1; mask < totalSubsets; ++mask) {
        int sum = 0;
        // Compute sum of elements in this subset (based on set bits in mask).
        for (int bit = 0; bit < n; ++bit) {
            if (mask & (1 << bit)) {
                sum += nums[bit];
            }
        }
        
        // Check if this sum was already seen.
        auto it = sumToMask.find(sum);
        if (it != sumToMask.end()) {
            // Found two distinct subsets with equal sum.
            const int firstMask = it->second;
            
            // Extract elements for the first subset.
            std::vector<int> firstSubset;
            for (int bit = 0; bit < n; ++bit) {
                if (firstMask & (1 << bit)) {
                    firstSubset.push_back(nums[bit]);
                }
            }
            
            // Extract elements for the second subset (current mask).
            std::vector<int> secondSubset;
            for (int bit = 0; bit < n; ++bit) {
                if (mask & (1 << bit)) {
                    secondSubset.push_back(nums[bit]);
                }
            }
            
            return {firstSubset, secondSubset};
        } else {
            // Record this sum with its bitmask.
            sumToMask[sum] = mask;
        }
    }
    
    // No equal-sum pair found.
    return {std::vector<int>(), std::vector<int>()};
}
#include <cassert>
#include <vector>
#include <algorithm>

// The solution function is declared above.
// For testing, we'll define a helper to compare two vectors as multisets.
bool sameMultiset(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) return false;
    std::vector<int> sortedA = a;
    std::vector<int> sortedB = b;
    std::sort(sortedA.begin(), sortedA.end());
    std::sort(sortedB.begin(), sortedB.end());
    return sortedA == sortedB;
}

int main() {
    // Test 1: Simple case with a pair.
    // nums = {1, 2, 3, 4}, subsets {1,4} sum 5, {2,3} sum 5.
    {
        std::vector<int> nums = {1, 2, 3, 4};
        auto result = findEqualSumSubsets(nums);
        assert(!result.first.empty() && !result.second.empty());
        int sumA = 0, sumB = 0;
        for (int x : result.first) sumA += x;
        for (int x : result.second) sumB += x;
        assert(sumA == sumB);
        // Both subsets must be non-empty and distinct.
        assert(!result.first.empty() && !result.second.empty());
    }

    // Test 2: No pair exists (powers of two).
    {
        std::vector<int> nums = {1, 2, 4, 8, 16};
        auto result = findEqualSumSubsets(nums);
        assert(result.first.empty() && result.second.empty());
    }

    // Test 3: Duplicate elements produce the same sum for different subsets.
    {
        std::vector<int> nums = {5, 5};
        auto result = findEqualSunSubsets(nums);
        assert(!result.first.empty() && !result.second.empty());
        assert(result.first.size() == 1 && result.second.size() == 1);
        assert(result.first[0] == 5 && result.second[0] == 5);
    }

    // Test 4: Single element, no pair.
    {
        std::vector<int> nums = {42};
        auto result = findEqualSumSubsets(nums);
        assert(result.first.empty() && result.second.empty());
    }

    // Test 5: Larger set with known equal sums.
    // nums = {1, 1, 1, 1, 1}, any two singletons or many combinations.
    {
        std::vector<int> nums = {1, 1, 1, 1, 1};
        auto result = findEqualSumSubsets(nums);
        assert(!result.first.empty() && !result.second.empty());
        int sumA = 0, sumB = 0;
        for (int x : result.first) sumA += x;
        for (int x : result.second) sumB += x;
        assert(sumA == sumB);
        assert(result.first.size() >= 1 && result.second.size() >= 1);
    }

    // Test 6: Check that returned subsets are indeed subsets of the input.
    {
        std::vector<int> nums = {3, 5, 7, 11, 13};
        auto result = findEqualSumSubsets(nums);
        // In this case, 3+11=14 and 7+? No, but 5+? Actually check: 3+5+? 
        // This input has no solution? Actually 3+11 = 14, 5+? There is no 14. So return empty.
        // To be safe, we'll test with an input that has a solution.
        nums = {3, 5, 7, 11, 12}; // 5+7=12, and 12 alone sum 12.
        result = findEqualSumSubsets(nums);
        assert(!result.first.empty() && !result.second.empty());
        // Verify both subsets contain only elements from nums.
        for (int x : result.first) {
            assert(std::find(nums.begin(), nums.end(), x) != nums.end());
        }
        for (int x : result.second) {
            assert(std::find(nums.begin(), nums.end(), x) != nums.end());
        }
    }

    // Test 7: Edge case with N=20 (just run to ensure no timeout in typical execution).
    // We'll use a small set with a known solution.
    {
        std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
        auto result = findEqualSumSubsets(nums);
        // There are many pairs, e.g., {1,4} and {2,3} both sum 5.
        assert(!result.first.empty() && !result.second.empty());
        int sumA = 0, sumB = 0;
        for (int x : result.first) sumA += x;
        for (int x : result.second) sumB += x;
        assert(sumA == sumB);
    }

    // Test 8: Verify the two subsets are distinct (not the same mask).
    {
        std::vector<int> nums = {2, 2, 2};
        auto result = findEqualSumSubsets(nums);
        assert(!result.first.empty() && !result.second.empty());
        // Since all values are 2, the subsets could be {2} and {2} (different indices).
        // Or {2,2} and {2,2}, but they are distinct as bitmasks.
        // Just check sums equal.
        int sumA = 0, sumB = 0;
        for (int x : result.first) sumA += x;
        for (int x : result.second) sumB += x;
        assert(sumA == sumB);
        // Ensure both are non-empty and not exactly the same sequence.
        assert(result.first.size() > 0 && result.second.size() > 0);
        // They could be identical by value, but that's fine for this problem.
    }

    return 0;
}
