// Write a standalone C++ function that, given a vector of integers `nums` which may contain duplicates, returns a vector of vectors containing all possible subsets (the power set), with no duplicate subsets in the final result. The order of subsets and the order of elements within each subset do not matter. For example, if the input is `{1, 2, 2}`, the output should be `{{}, {1}, {1,2}, {1,2,2}, {2}, {2,2}}` (in any order), and importantly, the subset `{2}` should appear only once even though the input has two copies of `2`. The function must be `const`‑correct (i.e., it must not modify its input parameter) and should not rely on any global state or external libraries beyond the standard library.

The classic approach is to first sort the input vector so that all duplicate values are adjacent. Then, we use a depth‑first search (backtracking) that, at each step, decides whether to include the current element in the subset. However, to avoid duplicate subsets, we must skip over consecutive duplicate elements when we choose *not* to include a particular duplicate value. Specifically, we sort the array, and in the recursion, for each index `i`, we first include `nums[i]` and recurse to `i+1`. After that, we skip all elements that are equal to `nums[i]` (i.e., while `i+1 < n` and `nums[i+1] == nums[i]`, increment `i`) before moving to the next distinct value. This ensures that the subset that *skips* that value is explored only once. The base case is when we reach the end of the array, at which point we add the current subset to the answer. Edge cases include an empty input (which yields a single empty subset), all elements identical (only subsets of different sizes), and a mix of duplicates and unique elements. The time complexity is \(O(2^n \cdot n)\) in the worst case because there are at most \(2^n\) subsets, and copying each subset to the answer takes linear time. The space complexity is \(O(n)\) for the recursion stack and the current subset, not counting the output storage.

#include <vector>
#include <algorithm>

// Recursive helper: builds all unique subsets starting from index 'idx'.
// 'current' holds the subset being built, 'result' collects all unique subsets.
void generateUniqueSubsets(const std::vector<int>& nums, size_t idx,
                           std::vector<int>& current,
                           std::vector<std::vector<int>>& result) {
    // Every partial subset is a valid subset; add it to the result.
    result.push_back(current);

    // Explore choices for the next element.
    for (size_t i = idx; i < nums.size(); ++i) {
        // Skip duplicate values: if this is not the first occurrence
        // in this recursion level, then choosing 'not include' was already
        // covered by the previous identical value, so we skip it.
        if (i > idx && nums[i] == nums[i - 1]) {
            continue;
        }

        // Include nums[i] and recurse.
        current.push_back(nums[i]);
        generateUniqueSubsets(nums, i + 1, current, result);
        current.pop_back(); // backtrack
    }
}

// Returns a vector of all unique subsets of the input 'nums'.
// The input vector is not modified; a sorted copy is made internally.
std::vector<std::vector<int>> uniqueSubsets(const std::vector<int>& nums) {
    std::vector<int> sortedNums = nums;
    std::sort(sortedNums.begin(), sortedNums.end());

    std::vector<std::vector<int>> result;
    std::vector<int> current;
    generateUniqueSubsets(sortedNums, 0, current, result);
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Forward declaration of the function under test (already defined above,
// but here for clarity in this standalone test block).
std::vector<std::vector<int>> uniqueSubsets(const std::vector<int>& nums);

// Helper to check if two vectors of subsets are equal as sets (order‑independent).
bool sameSubsets(const std::vector<std::vector<int>>& a,
                 const std::vector<std::vector<int>>& b) {
    if (a.size() != b.size()) return false;
    // Deep copy and sort each subset, then sort the outer vector.
    auto canonical = [](const std::vector<std::vector<int>>& v) {
        std::vector<std::vector<int>> temp = v;
        for (auto& subset : temp) {
            std::sort(subset.begin(), subset.end());
        }
        std::sort(temp.begin(), temp.end());
        return temp;
    };
    return canonical(a) == canonical(b);
}

int main() {
    // Test 1: Empty input -> one empty subset.
    std::vector<int> empty;
    std::vector<std::vector<int>> result1 = uniqueSubsets(empty);
    assert(result1.size() == 1);
    assert(result1[0].empty());

    // Test 2: All distinct elements.
    std::vector<int> distinct = {1, 2, 3};
    std::vector<std::vector<int>> expected2 = {{}, {1}, {2}, {3}, {1,2}, {1,3}, {2,3}, {1,2,3}};
    assert(sameSubsets(uniqueSubsets(distinct), expected2));

    // Test 3: Duplicate elements (example from problem).
    std::vector<int> dup = {1, 2, 2};
    std::vector<std::vector<int>> expected3 = {{}, {1}, {1,2}, {1,2,2}, {2}, {2,2}};
    assert(sameSubsets(uniqueSubsets(dup), expected3));

    // Test 4: All duplicate elements.
    std::vector<int> allDup = {5, 5, 5};
    std::vector<std::vector<int>> result4 = uniqueSubsets(allDup);
    // Subsets: size 0, 1, 2, 3 each with value 5.
    assert(result4.size() == 4);
    for (const auto& s : result4) {
        assert(std::all_of(s.begin(), s.end(), [](int x){ return x == 5; }));
    }

    // Test 5: Negative and duplicate values.
    std::vector<int> mixed = {-1, 0, -1, 2};
    std::vector<std::vector<int>> result5 = uniqueSubsets(mixed);
    // Expected unique subsets: all combinations of {-1, 0, 2} with at most one -1? Actually -1 appears twice, but duplicate subsets must be removed. So:
    std::vector<std::vector<int>> expected5 = {
        {}, {-1}, {0}, {2}, {-1,0}, {-1,2}, {0,2}, {-1,0,2}, {-1,-1}, {-1,-1,0}, {-1,-1,2}, {-1,-1,0,2}
    };
    assert(sameSubsets(result5, expected5));

    // Test 6: Input is not modified (const correctness).
    std::vector<int> original = {3, 1, 3};
    std::vector<int> copy = original;
    uniqueSubsets(original);
    assert(original == copy);

    return 0;
}
