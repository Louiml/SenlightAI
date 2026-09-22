Write a C++ function `countEvenFrequencySubarrays(const std::vector<int>& volumes, int l, int r)` that, given a vector of integers and two 0-based indices `l` and `r` (inclusive), returns the number of distinct values that appear an **even** number of times in the subarray `volumes[l..r]`. For example, if the subarray is `{4, 2, 4, 2, 4}`, the value `4` appears 3 times (odd) and `2` appears 2 times (even), so the result is `1`. If no value appears an even number of times, return `0`. The function must be efficient for multiple queries, but since this is a standalone task, you only need to implement it once with no global state; each call processes a single query. Assume `l` and `r` are always valid (0 ≤ l ≤ r < size).
// The straightforward approach is to iterate through the subarray from index `l` to `r`, count the frequency of each value using a hash map (`unordered_map<int, int>`), then iterate over the map and count how many keys have an even frequency. This works correctly for any input size, including edge cases where the subarray length is 1 (then the single value has frequency 1, which is odd, so answer is 0) or where all values are identical (if the subarray length is even, that value has even count → answer 1; if odd → 0). Because each call processes only one subarray, the time complexity is O(k) where k = r – l + 1, and auxiliary space is O(k) in the worst case for the hash map (since there can be up to k distinct values). No special handling is needed for negative numbers or zero; they are just map keys. The function should be `const`‑correct, taking the vector by const reference to avoid copying. The implementation uses `unordered_map` for average O(1) insert and lookup. The main edge case is ensuring we count only distinct values with even frequency, not the total number of elements that appear even times.
#include <vector>
#include <unordered_map>

// Counts distinct values that appear an even number of times in the subarray volumes[l..r].
int countEvenFrequencySubarrays(const std::vector<int>& volumes, int l, int r) {
    std::unordered_map<int, int> freq;
    for (int i = l; i <= r; ++i) {
        ++freq[volumes[i]];
    }
    int evenCount = 0;
    for (const auto& entry : freq) {
        if (entry.second % 2 == 0) {
            ++evenCount;
        }
    }
    return evenCount;
}
#include <cassert>
#include <vector>

// Already included the solution function above here for completeness; in a real file, place it before main.

int main() {
    std::vector<int> v1 = {4, 2, 4, 2, 4};
    assert(countEvenFrequencySubarrays(v1, 0, 4) == 1);
    
    std::vector<int> v2 = {1, 1, 1, 1};
    assert(countEvenFrequencySubarrays(v2, 0, 3) == 1);
    
    std::vector<int> v3 = {1, 1, 1};
    assert(countEvenFrequencySubarrays(v3, 0, 2) == 0);
    
    std::vector<int> v4 = {5};
    assert(countEvenFrequencySubarrays(v4, 0, 0) == 0);
    
    std::vector<int> v5 = {1, 2, 3, 4, 5};
    assert(countEvenFrequencySubarrays(v5, 0, 4) == 0);
    
    std::vector<int> v6 = {7, 7, 8, 8, 8, 9, 9};
    assert(countEvenFrequencySubarrays(v6, 1, 5) == 1); // subarray {7,8,8,8,9} -> 7(1),8(3),9(1)
    
    std::vector<int> v7 = {2, 2, 2, 2, 2, 2};
    assert(countEvenFrequencySubarrays(v7, 2, 5) == 1); // {2,2,2,2} -> 4
    
    std::vector<int> v8 = {-1, -1, 0, 0, -1};
    assert(countEvenFrequencySubarrays(v8, 0, 4) == 1); // -1 appears 3, 0 appears 2
    
    std::vector<int> v9 = {100, 200, 100, 200, 300};
    assert(countEvenFrequencySubarrays(v9, 0, 3) == 2); // 100 twice, 200 twice
    
    std::vector<int> v10 = {3, 3, 4, 4, 3};
    assert(countEvenFrequencySubarrays(v10, 0, 4) == 1); // 3 appears 3, 4 appears 2
}
