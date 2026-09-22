// Write a C++ function `vector<int> findIntersection(const vector<int>& arr1, const vector<int>& arr2)` that returns a vector containing the unique elements that appear in both input arrays. The function must preserve the relative order of elements as they first appear in `arr1`, must not include duplicates, and should handle cases where one or both arrays are empty, where there is no intersection (return an empty vector), and where elements appear multiple times in either array. The result should contain each common element exactly once, even if it appears multiple times in either input.

The solution approach uses an `unordered_set` to store all unique elements from `arr1` for O(1) average lookup. Then iterate through `arr2`, and for each element that exists in the set from `arr1` and hasn't already been added to the result set, add it to the result vector. To preserve order by `arr1`'s first occurrence, it's better to instead iterate through `arr1` and check membership in a set built from `arr2`, then add to the result vector only if the element hasn't already been added. This ensures the order follows `arr1` and no duplicates. Edge cases: empty inputs return empty vector; no common elements return empty vector; duplicates in either input are handled naturally by the set and the result check. Time complexity is O(n + m) average, where n and m are sizes of arr1 and arr2, due to O(1) set operations; space complexity is O(n + min(n,m)) for the set and result vector. The function is `const` correct by taking const references to vectors.

#include <vector>
#include <unordered_set>

// Return unique elements present in both input vectors, ordered by first appearance in arr1.
std::vector<int> findIntersection(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    std::unordered_set<int> set2(arr2.begin(), arr2.end());
    std::unordered_set<int> seen;
    std::vector<int> result;

    for (const int& value : arr1) {
        if (set2.find(value) != set2.end() && seen.insert(value).second) {
            result.push_back(value);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// free function declaration
std::vector<int> findIntersection(const std::vector<int>& arr1, const std::vector<int>& arr2);

int main() {
    // Basic case with duplicates
    assert(findIntersection({1,2,2,1}, {2,2}) == std::vector<int>({2}));
    // Order preserved by arr1
    assert(findIntersection({3,1,4,1,5}, {5,1,9}) == std::vector<int>({1,5}));
    // No intersection
    assert(findIntersection({1,2}, {3,4}) == std::vector<int>());
    // One empty array
    assert(findIntersection({}, {1,2}) == std::vector<int>());
    assert(findIntersection({1,2}, {}) == std::vector<int>());
    // All common
    assert(findIntersection({5,6,7}, {7,6,5}) == std::vector<int>({5,6,7}));
    // Multiple duplicates in both
    assert(findIntersection({1,1,1}, {1,1}) == std::vector<int>({1}));
    // Single element match
    assert(findIntersection({42}, {42}) == std::vector<int>({42}));
    // Negative numbers
    assert(findIntersection({-1,-2,-3}, {-3,0}) == std::vector<int>({-3}));
    // Large array
    std::vector<int> big1(1000, 7);
    std::vector<int> big2(1000, 7);
    assert(findIntersection(big1, big2) == std::vector<int>({7}));
    return 0;
}
