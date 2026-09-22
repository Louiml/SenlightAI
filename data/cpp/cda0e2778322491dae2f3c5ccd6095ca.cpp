Write a C++ function `findOriginalArray` that takes a vector of integers `changed` obtained by taking an original array of even length, doubling each element, and then shuffling the doubled array with the original array. The function must return the original array (before doubling) if such an array exists; otherwise, it should return an empty vector. The input may contain duplicates, zeros, and negative/positive values, and the elements are not guaranteed to be sorted. The original array is uniquely determined if it exists. Handle edge cases such as empty input, odd-length input, and cases where some elements cannot be matched to their doubles.
// The key observation is that if we sort the `changed` array, then for each element from smallest to largest, it must be either a "doubled" element or an "original" element. Since every original element `x` produces both `x` and `2*x` in `changed`, the smallest element in the sorted array must always be an original element (because its double would be larger). Therefore, we process elements in ascending order: for each element, if it hasn't been used up yet, we treat it as an original element, find its double `2*x`, and decrement the frequency count of that double. If the double is not available, the array is invalid and we return an empty vector. Because we process in sorted order and use a frequency map, we naturally match each original to its double. Important edge cases: odd length always returns empty; zeros have `2*0 == 0`, so the frequency count must be handled carefully—each zero must be paired with another zero, so we need to ensure the frequency of zero is even, and our algorithm automatically handles this by repeatedly matching a zero with another zero. Another subtlety: after processing all elements, we must verify that every frequency count is zero (i.e., all elements were paired); if any remain, the input is invalid. Time complexity is O(n log n) due to sorting, and space complexity is O(n) for the frequency map.
#include <vector>
#include <unordered_map>
#include <algorithm>

// Given a vector 'changed' containing original elements and their doubles,
// return the original array if it uniquely exists, otherwise return an empty vector.
std::vector<int> findOriginalArray(const std::vector<int>& changed) {
    std::vector<int> ans;
    if (changed.size() % 2 != 0) return ans; // Odd length impossible

    std::vector<int> sorted = changed;
    std::sort(sorted.begin(), sorted.end());

    std::unordered_map<int, int> freq;
    for (int x : sorted) freq[x]++;

    for (int x : sorted) {
        if (freq[x] == 0) continue; // Already used as a double

        // x must be an original element; check if its double exists
        if (freq[2 * x] == 0) return {}; // Missing double -> invalid

        freq[x]--;
        freq[2 * x]--;
        ans.push_back(x);
    }

    // Verify all counts are zero (all elements were matched)
    for (const auto& entry : freq) {
        if (entry.second != 0) return {};
    }
    return ans;
}
#include <cassert>
#include <vector>

int main() {
    // Basic valid case
    std::vector<int> res1 = findOriginalArray({1, 3, 4, 2, 6, 8});
    std::vector<int> expected1 = {1, 3, 4}; // sorted output
    assert(res1 == expected1);

    // Case with zeros: each zero pairs with another zero
    std::vector<int> res2 = findOriginalArray({0, 0, 2, 4});
    std::vector<int> expected2 = {0, 2};
    assert(res2 == expected2);

    // Empty input should return empty
    assert(findOriginalArray({}).empty());

    // Odd length always invalid
    assert(findOriginalArray({1, 2, 3}).empty());

    // Invalid because no double for the element 3
    assert(findOriginalArray({1, 2, 3, 4}).empty());

    // Duplicates handled correctly
    std::vector<int> res3 = findOriginalArray({2, 4, 2, 4});
    std::vector<int> expected3 = {2, 2};
    assert(res3 == expected3);

    // Negative numbers work
    std::vector<int> res4 = findOriginalArray({-4, -2, -2, -1});
    std::vector<int> expected4 = {-2, -1};
    assert(res4 == expected4);

    // All zeros: must have even count
    assert(findOriginalArray({0, 0, 0}).empty());
    std::vector<int> res5 = findOriginalArray({0, 0, 0, 0});
    std::vector<int> expected5 = {0, 0};
    assert(res5 == expected5);

    // Large single pair
    std::vector<int> res6 = findOriginalArray({5, 10});
    std::vector<int> expected6 = {5};
    assert(res6 == expected6);

    return 0;
}
