Write a C++ function that takes a vector of integers (which may contain duplicates) and returns a vector of all possible subsets (the power set), where each subset is itself a vector of integers. The result must contain no duplicate subsets, meaning that if two subsets have the same elements regardless of order, only one of them should appear. The output subsets can be in any order, but the function must handle an input vector of any size from 0 up to, say, 20 elements (though performance may degrade for larger sizes due to exponential output). The input vector may contain negative numbers, zero, and repeated values. The function should be implemented without using any standard library container besides `std::vector` and must work efficiently by first sorting the input to group duplicates together.
// The core idea is to generate subsets iteratively while avoiding duplicates caused by repeated elements. Start with an empty subset (already in the result). Sort the input array so equal elements are adjacent. For each distinct value in the sorted array, determine its frequency `count`. Before adding subsets that include this value, note the current size `previousN` of the result vector. For each existing subset (the ones present before processing this value), create `count` new subsets by successively appending one copy, two copies, ..., up to `count` copies of the current value. Because we only expand from the subsets that existed before the current distinct value, and we never expand from subsets that already contain the current value (since those were just added during this iteration), we avoid generating duplicate subsets. For example, if the input is `[1,2,2]`, the sorted array has `1` (count=1) and `2` (count=2). Start with `[[]]`. Process `1`: previousN=1, for the empty subset, add `[1]` (since count=1). Result: `[[], [1]]`. Process `2` (count=2): previousN=2. For the empty subset: add `[2]` and `[2,2]`. For subset `[1]`: add `[1,2]` and `[1,2,2]`. Final result: `[[], [1], [2], [2,2], [1,2], [1,2,2]]` – no duplicates. Edge cases: empty input returns a vector containing just the empty subset. Input with all identical elements produces only `n+1` subsets (empty plus one subset for each count from 1 to n). Time complexity is O(n log n) for sorting plus O(2^n) for generating all subsets (since there are at most 2^n distinct subsets and each is copied). Space complexity is O(2^n) for the output and O(n) for temporary copies.
#include <vector>
#include <algorithm>

using Subset = std::vector<int>;
using Subsets = std::vector<Subset>;

// Generate all unique subsets of the input vector, handling duplicates.
Subsets uniqueSubsets(std::vector<int> nums) {
    Subsets result;
    result.push_back(Subset{}); // Start with the empty subset

    std::sort(nums.begin(), nums.end());

    std::size_t i = 0;
    while (i < nums.size()) {
        // Count frequency of the current distinct value
        std::size_t count = 0;
        while (i + count < nums.size() && nums[i + count] == nums[i]) {
            ++count;
        }

        // Process only subsets that existed before this value
        std::size_t previousSize = result.size();
        for (std::size_t k = 0; k < previousSize; ++k) {
            Subset current = result[k];
            for (std::size_t j = 0; j < count; ++j) {
                current.push_back(nums[i]);
                result.push_back(current);
            }
        }

        i += count;
    }

    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

// Include the solution function here (or link it)

bool subsetsEqual(const std::vector<std::vector<int>>& a,
                  const std::vector<std::vector<int>>& b) {
    if (a.size() != b.size()) return false;
    auto sortedA = a;
    auto sortedB = b;
    for (auto& s : sortedA) std::sort(s.begin(), s.end());
    for (auto& s : sortedB) std::sort(s.begin(), s.end());
    std::sort(sortedA.begin(), sortedA.end());
    std::sort(sortedB.begin(), sortedB.end());
    return sortedA == sortedB;
}

int main() {
    // Test 1: Basic with duplicates
    {
        std::vector<int> input = {1, 2, 2};
        auto res = uniqueSubsets(input);
        std::vector<std::vector<int>> expected = {{}, {1}, {2}, {1,2}, {2,2}, {1,2,2}};
        assert(subsetsEqual(res, expected));
    }

    // Test 2: All identical elements – only n+1 subsets
    {
        std::vector<int> input = {3, 3, 3};
        auto res = uniqueSubsets(input);
        assert(res.size() == 4); // empty, [3], [3,3], [3,3,3]
        // Verify no duplicates by checking size
        std::vector<std::vector<int>> expected = {{}, {3}, {3,3}, {3,3,3}};
        assert(subsetsEqual(res, expected));
    }

    // Test 3: Empty input
    {
        std::vector<int> input = {};
        auto res = uniqueSubsets(input);
        std::vector<std::vector<int>> expected = {{}};
        assert(subsetsEqual(res, expected));
    }

    // Test 4: All unique elements – classic power set size 2^n
    {
        std::vector<int> input = {1, 2, 3};
        auto res = uniqueSubsets(input);
        assert(res.size() == 8);
        // Check that each subset element appears exactly once (no duplicate subsets)
        std::vector<std::vector<int>> expected = {{}, {1}, {2}, {3}, {1,2}, {1,3}, {2,3}, {1,2,3}};
        assert(subsetsEqual(res, expected));
    }

    // Test 5: Negative numbers and zero
    {
        std::vector<int> input = {-1, 0, -1};
        auto res = uniqueSubsets(input);
        std::vector<std::vector<int>> expected = {{}, {-1}, {0}, {-1,-1}, {-1,0}, {-1,-1,0}};
        assert(subsetsEqual(res, expected));
    }

    // Test 6: Large duplicates – check count
    {
        std::vector<int> input(10, 7);
        auto res = uniqueSubsets(input);
        assert(res.size() == 11); // empty + 1 to 10 copies
    }

    return 0;
}
