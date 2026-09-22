Write a C++ function named `commonElements` that takes two vectors of integers, `nums1` and `nums2`, and returns a new vector containing the unique elements that appear in both input vectors. The order of the returned elements should be the order in which they first appear in `nums2`. Each common element must appear only once in the result, even if it occurs multiple times in either input. The function should handle empty inputs and should treat the vectors as read-only data.

#include <cassert>
#include <vector>

// The commonElements function is expected to be declared above.

int main() {
    // Basic case with overlapping elements.
    std::vector<int> a1 = {1, 2, 2, 3};
    std::vector<int> a2 = {2, 3, 4};
    assert(commonElements(a1, a2) == std::vector<int>({2, 3}));

    // Empty first vector.
    std::vector<int> b1;
    std::vector<int> b2 = {1, 2, 3};
    assert(commonElements(b1, b2).empty());

    // Empty second vector.
    std::vector<int> c1 = {5, 6, 7};
    std::vector<int> c2;
    assert(commonElements(c1, c2).empty());

    // No common elements.
    std::vector<int> d1 = {1, 2, 3};
    std::vector<int> d2 = {4, 5, 6};
    assert(commonElements(d1, d2).empty());

    // All elements common but with duplicates in both.
    std::vector<int> e1 = {1, 1, 2, 2};
    std::vector<int> e2 = {2, 1, 2, 1};
    // Order follows nums2: first 2, then 1.
    assert(commonElements(e1, e2) == std::vector<int>({2, 1}));

    // Large values and negative numbers.
    std::vector<int> f1 = {-10, -5, 0, 100};
    std::vector<int> f2 = {100, -5, -10, 200};
    assert(commonElements(f1, f2) == std::vector<int>({100, -5, -10}));

    // Single common element repeated multiple times in both.
    std::vector<int> g1 = {7, 7, 7};
    std::vector<int> g2 = {7};
    assert(commonElements(g1, g2) == std::vector<int>({7}));

    // Both vectors identical and already unique.
    std::vector<int> h1 = {9, 8, 7};
    std::vector<int> h2 = {9, 8, 7};
    assert(commonElements(h1, h2) == std::vector<int>({9, 8, 7}));

    return 0;
}

#include <vector>
#include <unordered_set>

// Return unique elements that appear in both input vectors, ordered by their first appearance in nums2.
std::vector<int> commonElements(const std::vector<int>& nums1, const std::vector<int>& nums2) {
    // Store all unique elements of nums1 for fast lookup.
    std::unordered_set<int> seen(nums1.begin(), nums1.end());
    std::vector<int> result;
    
    // Iterate through nums2, adding elements that exist in the set and then removing them to avoid duplicates.
    for (int value : nums2) {
        if (seen.find(value) != seen.end()) {
            result.push_back(value);
            seen.erase(value);
        }
    }
    return result;
}

// The solution uses a hash set to store all unique integers from the first vector, enabling O(1) average lookup. Then it iterates through the second vector, checking whether each element exists in the set. If it does, that element is appended to the result and immediately removed from the set to prevent duplicates in the output. This ensures that each common element is added only once, and because we scan `nums2` from start to finish, the result order matches the first occurrence in `nums2`. Edge cases include empty input vectors, vectors with no common elements, duplicate values within either vector, and cases where all elements are common. The time complexity is O(n1 + n2) for building the set and iterating through the second vector. The space complexity is O(n1) for the hash set in the worst case, plus the space for the output vector which is at most min(n1, n2) distinct common elements.
