/*
Write a C++ function `generateAllSubsets` that takes a `std::vector<int>` (which may be empty, contain duplicates, negative numbers, and zeros) and returns a `std::vector<std::vector<int>>` containing all possible subsets of the input vector. The order of subsets in the result and the order of elements within each subset do not matter. The function must handle the empty input by returning a vector containing exactly one empty vector. Do not modify the input vector; ensure your implementation uses recursion with backtracking.
*/

#include <vector>

// Generate all subsets of the input vector using recursive backtracking.
// Returns a vector containing every subset, including the empty subset.
std::vector<std::vector<int>> generateAllSubsets(const std::vector<int>& nums) {
    std::vector<std::vector<int>> result;
    std::vector<int> current;
    
    // Helper lambda for recursive subset generation.
    // index: current position in nums, current: subset being built.
    std::function<void(int)> backtrack = [&](int index) {
        // Base case: processed all elements, save current subset.
        if (index == static_cast<int>(nums.size())) {
            result.push_back(current);
            return;
        }
        
        // Option 1: include nums[index] in the subset.
        current.push_back(nums[index]);
        backtrack(index + 1);
        current.pop_back();
        
        // Option 2: exclude nums[index] from the subset.
        backtrack(index + 1);
    };
    
    backtrack(0);
    return result;
}

#include <cassert>
#include <vector>
#include <algorithm>

// Helper to check if two sets of subsets are equivalent (ignoring order).
bool sameSubsets(const std::vector<std::vector<int>>& a,
                 const std::vector<std::vector<int>>& b) {
    if (a.size() != b.size()) return false;
    // Make copies to sort each subset and the outer vector.
    std::vector<std::vector<int>> a_copy = a, b_copy = b;
    for (auto& sub : a_copy) std::sort(sub.begin(), sub.end());
    for (auto& sub : b_copy) std::sort(sub.begin(), sub.end());
    std::sort(a_copy.begin(), a_copy.end());
    std::sort(b_copy.begin(), b_copy.end());
    return a_copy == b_copy;
}

int main() {
    // Test 1: Empty input
    std::vector<int> empty;
    std::vector<std::vector<int>> r1 = generateAllSubsets(empty);
    assert(r1.size() == 1 && r1[0].empty());

    // Test 2: Single element
    std::vector<int> single = {5};
    auto r2 = generateAllSubsets(single);
    assert(r2.size() == 2);
    assert(sameSubsets(r2, {{}, {5}}));

    // Test 3: Two distinct elements
    std::vector<int> two = {1, 2};
    auto r3 = generateAllSubsets(two);
    assert(r3.size() == 4);
    assert(sameSubsets(r3, {{}, {1}, {2}, {1,2}}));

    // Test 4: Three elements including duplicates (treated as distinct)
    std::vector<int> dup = {1, 1};
    auto r4 = generateAllSubsets(dup);
    assert(r4.size() == 4); // because positions are distinct
    // Subsets: {}, {1}, {1}, {1,1} - count and elements check
    assert(sameSubsets(r4, {{}, {1}, {1}, {1,1}}));

    // Test 5: Negative numbers and zero
    std::vector<int> mixed = {0, -1};
    auto r5 = generateAllSubsets(mixed);
    assert(r5.size() == 4);
    assert(sameSubsets(r5, {{}, {0}, {-1}, {0,-1}}));

    // Test 6: Larger set, verify count = 2^n
    std::vector<int> four = {1, 2, 3, 4};
    auto r6 = generateAllSubsets(four);
    assert(r6.size() == 16);
    // Check that one specific subset exists: {2,4}
    bool found = false;
    for (const auto& sub : r6) {
        if (sub.size() == 2 && sub[0] == 2 && sub[1] == 4) found = true;
    }
    assert(found);

    // Test 7: Ensure input is not modified
    std::vector<int> orig = {1, 2};
    std::vector<int> copy_orig = orig;
    generateAllSubsets(orig);
    assert(orig == copy_orig);

    return 0;
}

// The problem is the classic "subsets" generation problem. The main algorithm uses recursion and backtracking: at each index in the input array, we make a binary choice — either include the current element in the current subset or exclude it. Once we reach the end of the array (index equals the vector size), we save a copy of the current subset. Recursive calls naturally explore all 2^n combinations, where n is the size of the input. Important edge cases: empty input must produce one empty subset (n=0 gives 2^0=1 subset); duplicates are treated as distinct elements (so subsets may contain equal values multiple times, but the algorithm does not deduplicate); negative numbers and zeros are handled naturally since we only copy integers. Time complexity is O(n * 2^n) because there are 2^n subsets and each subset requires copying up to n elements into the answer. Space complexity is O(n) for recursion depth and the currently built subset, not counting the output storage which is necessary for the result.
