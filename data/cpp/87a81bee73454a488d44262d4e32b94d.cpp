// Write a standalone C++ function that takes a `std::vector<int>` and returns a `std::vector<int>` containing only the elements that appear in both the first and last half of the input vector (where "half" is defined by integer division of the vector's size by 2, so for size `n`, the first half is indices `[0, n/2 - 1]` and the second half is `[n/2, n-1]`). The returned vector must be sorted in ascending order and contain each common element exactly once (duplicates are removed). If the vector size is odd, the middle element belongs to the second half. If there are no common elements, return an empty vector. The input vector may be empty, and it should be handled safely. Your function should not modify the input vector.
#include <cassert>
#include <vector>

int main() {
    // Basic case with common and unique elements
    std::vector<int> v1 = {1, 2, 3, 4, 2, 5};
    assert(commonHalves(v1) == std::vector<int>({2}));

    // No common elements
    std::vector<int> v2 = {1, 2, 3, 4, 5, 6};
    assert(commonHalves(v2).empty());

    // Odd size, middle element belongs to second half
    std::vector<int> v3 = {7, 8, 9, 7, 8};
    assert(commonHalves(v3) == std::vector<int>({7, 8}));

    // Duplicates within halves are removed
    std::vector<int> v4 = {5, 5, 5, 5, 5, 5};
    assert(commonHalves(v4) == std::vector<int>({5}));

    // Empty input
    std::vector<int> v5;
    assert(commonHalves(v5).empty());

    // Single element: first half empty, second half has the element
    std::vector<int> v6 = {42};
    assert(commonHalves(v6).empty());

    // Two elements: halves are {a} and {b}
    std::vector<int> v7 = {3, 3};
    assert(commonHalves(v7) == std::vector<int>({3}));

    // Larger test: first half {1,2,3}, second half {3,4,5}
    std::vector<int> v8 = {1, 2, 3, 3, 4, 5};
    assert(commonHalves(v8) == std::vector<int>({3}));

    // All distinct but common values repeated across halves
    std::vector<int> v9 = {10, 20, 30, 40, 10, 20, 50};
    assert(commonHalves(v9) == std::vector<int>({10, 20}));

    return 0;
}
#include <vector>
#include <set>
#include <algorithm>

// Returns sorted unique elements that appear in both the first and second half of the input vector.
std::vector<int> commonHalves(const std::vector<int>& input) {
    if (input.empty()) {
        return {};
    }

    const size_t n = input.size();
    const size_t half = n / 2; // first half: [0, half), second half: [half, n)

    std::set<int> firstHalf(input.begin(), input.begin() + half);
    std::set<int> secondHalf(input.begin() + half, input.end());

    std::vector<int> result;

    // Iterate over the smaller set for efficiency
    const std::set<int>& smaller = (firstHalf.size() <= secondHalf.size()) ? firstHalf : secondHalf;
    const std::set<int>& larger = (firstHalf.size() <= secondHalf.size()) ? secondHalf : firstHalf;

    for (int value : smaller) {
        if (larger.find(value) != larger.end()) {
            result.push_back(value);
        }
    }

    return result;
}
// The solution separates the input into two halves by index: `firstHalf = [begin, begin + size/2)` and `secondHalf = [begin + size/2, end)`. For each half, we build a `std::set<int>` to automatically remove duplicates and sort the unique elements. Then we iterate over the smaller set (or either) and check membership in the other set using `find()`. Every element found in both sets is inserted into a result vector, which remains sorted because sets iterate in ascending order. Edge cases: empty input → both sets empty → empty result; size 1 → first half empty, second half has one element → empty result; duplicates within a half are handled by the set; the middle element (when size is odd) is always in the second half. Time complexity is O(n log n) due to set insertion for each half, and the intersection check is O(min(k1, k2) * log(max(k1,k2))). Space complexity is O(n) for the sets. The input vector is not modified.
