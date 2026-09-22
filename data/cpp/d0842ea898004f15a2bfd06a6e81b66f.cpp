Write a C++ function that takes a vector of integers and a target integer, and returns a vector of two indices whose corresponding values sum to the target. The input vector is non-empty and contains exactly one valid solution; each element may be used only once. The function should return the two indices in ascending order (smaller index first). If the input vector is empty, return an empty vector. You must implement the solution without modifying the input vector.

#include <cassert>
#include <vector>

int main() {
    // Basic case
    std::vector<int> v1{2, 7, 11, 15};
    assert(twoSum(v1, 9) == std::vector<int>({0, 1}));

    // Unsorted input
    std::vector<int> v2{3, 2, 4};
    assert(twoSum(v2, 6) == std::vector<int>({1, 2}));

    // Duplicate values
    std::vector<int> v3{3, 3};
    assert(twoSum(v3, 6) == std::vector<int>({0, 1}));

    // Negative numbers
    std::vector<int> v4{-1, -2, -3, -4, -5};
    assert(twoSum(v4, -8) == std::vector<int>({2, 4}));

    // Zero and positive
    std::vector<int> v5{0, 4, 3, 0};
    assert(twoSum(v5, 0) == std::vector<int>({0, 3}));

    // Large value at end
    std::vector<int> v6{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(twoSum(v6, 19) == std::vector<int>({8, 9}));

    // Empty input returns empty
    std::vector<int> v7;
    assert(twoSum(v7, 5) == std::vector<int>());

    // Single element cannot have a solution, but if it did? We test safety (returns empty)
    std::vector<int> v8{5};
    assert(twoSum(v8, 5) == std::vector<int>());

    // Test const correctness (input not modified)
    std::vector<int> original{1, 2, 3};
    std::vector<int> copy = original;
    twoSum(original, 3);
    assert(original == copy);

    return 0;
}

#include <vector>
#include <unordered_map>

// Return indices of the two numbers that sum to target.
// The returned indices are in ascending order. Empty input yields empty output.
std::vector<int> twoSum(const std::vector<int>& nums, int target) {
    if (nums.empty()) {
        return {};
    }

    std::unordered_map<int, int> seen; // maps value to index+1 (0 means absent)
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int complement = target - nums[i];
        if (seen.find(complement) != seen.end()) {
            int first = seen[complement] - 1;
            int second = i;
            if (first > second) {
                std::swap(first, second);
            }
            return {first, second};
        }
        seen[nums[i]] = i + 1;
    }

    return {}; // Should never reach here per problem constraints, but for safety.
}

// The optimal approach uses a hash map to store previously seen numbers and their indices. For each element `nums[i]`, we compute the complement `target - nums[i]`. If that complement already exists in the map, we have found the pair: the stored index of the complement and `i`. To avoid confusion with the sentinel value 0, we store indices as `index + 1` in the map (so 0 means "not present"). When a match is found, return `{smaller_index, larger_index}`. If the vector is empty, return an empty vector immediately. The algorithm makes a single pass through the array, so time complexity is O(n) and space complexity is O(n) for the hash map. Edge cases include duplicate values (handled naturally because we return as soon as a match is found before overwriting) and the possibility that the complement is the same value as the current element (which is fine because each element is used once; we only store each index after processing the current element, so we never match an element with itself).
