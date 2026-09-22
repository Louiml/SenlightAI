Write a C++ function that, given a vector of integers, returns a new vector containing only the numbers that appear an odd number of times, preserving the relative order of their first occurrence in the input. If no number appears an odd number of times, return an empty vector. The function must handle duplicates, negatives, and large values. Example: input `{4, 2, 4, 3, 2, 2}` → output `{4, 3, 2}` (because 4 appears twice (even), 3 once (odd), 2 three times (odd)). The signature should be `std::vector<int> oddOccurrences(const std::vector<int>& nums)`.

// We need to count frequencies of each integer while preserving the order of first occurrence for elements that will be included. The main algorithm: iterate through the input vector once, using an `std::unordered_map<int, int>` to count occurrences. Simultaneously, maintain a separate vector `order` that stores each distinct integer when first encountered. After the counting pass, iterate over `order` and check each element’s count; if the count is odd, append it to the result vector. Edge cases: empty input → empty result; all counts even → empty result; duplicates where the first occurrence has an even count but later occurrences would also be even (the order vector only stores each distinct value once, so there is no issue). Time complexity: O(n) for counting and O(d) for building result, where d ≤ n, so overall O(n). Space complexity: O(d) for the map and the order vector, in the worst case O(n) when all elements are distinct.

#include <vector>
#include <unordered_map>

// Return a vector of integers that appear an odd number of times in nums,
// preserving the order of their first occurrence in the original vector.
std::vector<int> oddOccurrences(const std::vector<int>& nums) {
    std::unordered_map<int, int> counts;
    std::vector<int> order;
    
    // Count occurrences and record first-occurrence order.
    for (int value : nums) {
        if (counts.find(value) == counts.end()) {
            order.push_back(value);
        }
        ++counts[value];
    }
    
    std::vector<int> result;
    for (int value : order) {
        if (counts[value] % 2 != 0) {
            result.push_back(value);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Standard case with mixed parity.
    std::vector<int> input1 = {4, 2, 4, 3, 2, 2};
    assert(oddOccurrences(input1) == std::vector<int>({4, 3, 2}));

    // All even counts > result empty.
    std::vector<int> input2 = {1, 1, 2, 2, 3, 3};
    assert(oddOccurrences(input2).empty());

    // All odd counts > result equals distinct values in order.
    std::vector<int> input3 = {5, 1, 5, 2}; // 5 odd, 1 odd, 2 odd
    assert(oddOccurrences(input3) == std::vector<int>({5, 1, 2}));

    // Single element odd count.
    std::vector<int> input4 = {42};
    assert(oddOccurrences(input4) == std::vector<int>({42}));

    // Empty input.
    std::vector<int> input5;
    assert(oddOccurrences(input5).empty());

    // Negatives and zeros.
    std::vector<int> input6 = {0, -1, 0, -1, -1, 0};
    // counts: 0 appears 3 (odd), -1 appears 3 (odd) → order: 0, -1
    assert(oddOccurrences(input6) == std::vector<int>({0, -1}));

    // Large duplicate set where only one appears odd times.
    std::vector<int> input7 = {7, 7, 7, 7, 9, 7};
    // 7 appears 5 (odd), 9 appears 1 (odd) → order: 7, 9
    assert(oddOccurrences(input7) == std::vector<int>({7, 9}));

    // Values that appear once but among many duplicates.
    std::vector<int> input8 = {2, 2, 3, 2, 2, 5, 5};
    // 2 count 4 (even), 3 count 1 (odd), 5 count 2 (even) → {3}
    assert(oddOccurrences(input8) == std::vector<int>({3}));
}
