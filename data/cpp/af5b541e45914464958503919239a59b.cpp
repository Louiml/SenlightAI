// Write a C++ function that, given a vector of integers, returns the minimum size of a set of numbers that must be removed from the array so that the remaining array contains at most half the original length. Removal means selecting a subset of distinct values from the array and deleting every occurrence of those values. The function should determine the smallest number of distinct values to delete such that the remaining count is less than or equal to `arr.size() / 2`. The input vector may contain duplicate values. For example, given `[3,3,3,3,5,5,5,2,2,7]` of size 10, removing all occurrences of `3` (4 elements) and `5` (3 elements) leaves 3 elements, which is less than 5, so the answer is 2. If removing a single most frequent value already reduces the length to at most half, return 1. If the array has only one unique value, the answer is 1 (unless the array is already empty or size 1, where the answer is 1 because removing that value reduces to 0 or at most half).
The core idea is to group elements by their frequency using an associative container like `std::map` (or `std::unordered_map` for average O(1) access). After counting frequencies, we collect all pairs `(frequency, value)` into a vector, then sort that vector in descending order by frequency. The goal is to find the smallest prefix of this sorted list whose cumulative sum of frequencies is at least `arr.size() / 2`. Since the frequencies are sorted descending, the cumulative sums are monotonic non-decreasing, allowing a binary search on the prefix length. We compute prefix sums in-place (overwrite each frequency with the cumulative sum) to avoid extra space. The answer is `mid + 1` when the cumulative sum at index `mid` first reaches the required threshold. Edge cases: if the array is empty, the required is 0 and the answer should be 0 (but the problem likely expects at least 1 for non-empty; here we handle empty by returning 0). If all frequencies are 1 (all distinct), then the maximum frequency is 1, and we must delete enough distinct values to cover at least half the length, which is `ceil(n/2)` distinct numbers (since each deletion removes one element). The binary search handles this naturally because cumulative sums increase by 1 each step. Time complexity: O(n log n) due to sorting, and O(n) for counting and prefix sums; space O(n).
#include <vector>
#include <map>
#include <algorithm>

// Returns the minimum number of distinct values to delete from arr
// so that the remaining elements are at most arr.size()/2.
int minSetSize(std::vector<int>& arr) {
    if (arr.empty()) return 0;
    
    const int target = static_cast<int>(arr.size() / 2);
    if (target == 0) return 1; // size 1: remove the single element
    
    // Count frequencies of each value.
    std::map<int, int> freq;
    for (int value : arr) {
        freq[value]++;
    }
    
    // Collect (frequency, value) pairs.
    std::vector<std::pair<int, int>> frequencies;
    frequencies.reserve(freq.size());
    for (const auto& entry : freq) {
        frequencies.emplace_back(entry.second, entry.first);
    }
    
    // Sort descending by frequency (first element of pair).
    std::sort(frequencies.rbegin(), frequencies.rend());
    
    // Convert to prefix sums in-place: frequencies[i].first = sum of first i+1 frequencies.
    for (size_t i = 1; i < frequencies.size(); ++i) {
        frequencies[i].first += frequencies[i - 1].first;
    }
    
    // Binary search smallest prefix whose cumulative sum >= target.
    int low = 0;
    int high = static_cast<int>(frequencies.size()) - 1;
    int answer = static_cast<int>(frequencies.size());
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (frequencies[mid].first >= target) {
            answer = mid + 1; // mid is 0-indexed, so prefix length = mid+1
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    
    return answer;
}
#include <cassert>
#include <vector>

// Function declaration (include the solution above)
int minSetSize(std::vector<int>& arr);

int main() {
    // Basic cases
    std::vector<int> arr1 = {3,3,3,3,5,5,5,2,2,7};
    assert(minSetSize(arr1) == 2);
    
    // Single most frequent already covers half
    std::vector<int> arr2 = {1,2,2,2,3,4};
    // size 6, target 3; value 2 appears 3 times -> remove 1 distinct value
    assert(minSetSize(arr2) == 1);
    
    // All distinct: need ceil(n/2) distinct values
    std::vector<int> arr3 = {1,2,3,4,5,6,7,8};
    // size 8, target 4, each frequency 1 -> need 4 distinct
    assert(minSetSize(arr3) == 4);
    
    // All same
    std::vector<int> arr4 = {5,5,5,5,5};
    assert(minSetSize(arr4) == 1);
    
    // Size 1
    std::vector<int> arr5 = {42};
    assert(minSetSize(arr5) == 1);
    
    // Size 2 with same values
    std::vector<int> arr6 = {7,7};
    assert(minSetSize(arr6) == 1);
    
    // Size 2 with different values
    std::vector<int> arr7 = {7,8};
    // target = 1, need 1 distinct value
    assert(minSetSize(arr7) == 1);
    
    // Larger case with many duplicates
    std::vector<int> arr8 = {1,1,1,1,1,2,2,2,2,3,3,3,4,4,5};
    // size 15, target 7; frequencies: 1->5, 2->4, 3->3, 4->2, 5->1
    // remove 1 (5) and 2 (4) = 9 >= 7, so 2
    assert(minSetSize(arr8) == 2);
    
    // Edge: empty (though problem may not call it, but function handles)
    std::vector<int> arr9 = {};
    assert(minSetSize(arr9) == 0);
    
    // Case where removing two values is just enough
    std::vector<int> arr10 = {1,1,2,2,3,3,4,4};
    // target = 4; each freq 2 -> need 2 distinct (2+2=4)
    assert(minSetSize(arr10) == 2);
    
    return 0;
}
