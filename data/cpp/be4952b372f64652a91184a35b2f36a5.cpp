// Write a C++ function that takes two integer arrays (and their sizes) and returns a vector containing all elements that appear in both arrays, with each common element appearing as many times as the minimum of its frequencies in the two input arrays. The output vector must be sorted in non-decreasing order. For example, if the first array is {3,4,2,2,4} and the second is {3,2,2,7}, the result should be {2,2,3}. Handle cases where one or both arrays are empty, where there are no common elements (return an empty vector), and where elements repeat with different frequencies. The function should not modify the input arrays.

#include <cassert>
#include <vector>
#include <initializer_list>

int main() {
    // Example from prompt
    std::vector<int> a1 = {3,4,2,2,4};
    std::vector<int> b1 = {3,2,2,7};
    assert(commonElements(a1, b1) == std::vector<int>({2,2,3}));

    // No common elements
    std::vector<int> a2 = {1,2,3};
    std::vector<int> b2 = {4,5,6};
    assert(commonElements(a2, b2).empty());

    // Empty arrays
    std::vector<int> a3;
    std::vector<int> b3 = {1,2,3};
    assert(commonElements(a3, b3).empty());
    assert(commonElements(b3, a3).empty());

    // All elements common with different frequencies
    std::vector<int> a4 = {1,1,2,2,2};
    std::vector<int> b4 = {1,2,2,3};
    // Freq: 1->1, 2->2, 3->0
    assert(commonElements(a4, b4) == std::vector<int>({1,2,2}));

    // Single common element repeated
    std::vector<int> a5 = {5,5,5};
    std::vector<int> b5 = {5,5};
    assert(commonElements(a5, b5) == std::vector<int>({5,5}));

    // Duplicate in both but one has more
    std::vector<int> a6 = {7,7,7,8};
    std::vector<int> b6 = {7,8,8};
    assert(commonElements(a6, b6) == std::vector<int>({7,8}));

    // Large single array
    std::vector<int> a7 = {10};
    std::vector<int> b7 = {10};
    assert(commonElements(a7, b7) == std::vector<int>({10}));

    // Unsorted inputs
    std::vector<int> a8 = {2,1,2};
    std::vector<int> b8 = {3,2,1};
    assert(commonElements(a8, b8) == std::vector<int>({1,2}));

    // Negative numbers
    std::vector<int> a9 = {-1,-2,-1};
    std::vector<int> b9 = {-1,-1,-2};
    assert(commonElements(a9, b9) == std::vector<int>({-2,-1,-1}));

    return 0;
}

#include <vector>
#include <unordered_map>
#include <algorithm>

// Return a sorted vector of elements common to both arrays, with duplicates as per minimum frequency.
std::vector<int> commonElements(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::unordered_map<int, int> freq;
    for (int x : arr1) {
        ++freq[x];
    }

    std::vector<int> result;
    for (int x : arr2) {
        auto it = freq.find(x);
        if (it != freq.end() && it->second > 0) {
            result.push_back(x);
            --(it->second);
        }
    }

    std::sort(result.begin(), result.end());
    return result;
}

// The solution uses a frequency map (e.g., `std::unordered_map` or `std::map`) to count occurrences in the first array. Then it iterates through the second array, and for each element that exists in the map with a positive remaining count, it adds the element to a result vector and decrements the count. This ensures each common element appears exactly the minimum of its frequencies in both arrays. After building the result, sort it in non-decreasing order using `std::sort`. Edge cases: empty input arrays—just return an empty vector. No common elements—the result remains empty. Duplicates are handled naturally by the counting mechanism. Time complexity is O(n + m + k log k) where k is the size of the result (worst-case O(n log n) if all elements are common), and space complexity is O(n) for the frequency map plus O(k) for the result vector.
