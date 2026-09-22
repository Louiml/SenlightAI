/*
Write a C++ function that takes a sequence of unsigned 32-bit integers (as a `std::vector<uint32_t>`) and returns a new vector containing only the elements that appear an odd number of times, in the exact order of their first occurrence in the input. For example, given `{1,2,3,1,2,1}`, the output should be `{3,1}` because `3` appears once (odd), `1` appears three times (odd), and `2` appears twice (even). The input vector may be empty, may contain duplicates, and may contain the value `0`. The result must preserve the original first-appearance order. Do not use any external libraries beyond standard headers, and ensure the function is `const` Correct (i.e., it does not modify its input and returns by value).
*/
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Return a vector of elements that appear an odd number of times,
// preserving the order of their first occurrence in the input.
std::vector<uint32_t> extractOddFrequencyElements(const std::vector<uint32_t>& input) {
    // First pass: count frequency of each element.
    std::unordered_map<uint32_t, size_t> frequency;
    for (uint32_t value : input) {
        ++frequency[value];
    }

    // Second pass: collect elements with odd counts, in first-occurrence order.
    std::unordered_set<uint32_t> added;
    std::vector<uint32_t> result;
    for (uint32_t value : input) {
        // If the frequency is odd and not already added, append and mark.
        if (frequency[value] % 2 == 1 && added.insert(value).second) {
            result.push_back(value);
        }
    }
    return result;
}
#include <cassert>
#include <cstdint>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // Empty input yields empty result.
    std::vector<uint32_t> empty;
    assert(extractOddFrequencyElements(empty).empty());

    // All elements appear even number of times -> empty.
    std::vector<uint32_t> all_even = {5, 7, 5, 7, 9, 9};
    assert(extractOddFrequencyElements(all_even).empty());

    // Single element appears once -> included once.
    std::vector<uint32_t> single = {42};
    assert(extractOddFrequencyElements(single) == std::vector<uint32_t>{42});

    // Mixed example from the task description.
    std::vector<uint32_t> mixed = {1, 2, 3, 1, 2, 1};
    assert(extractOddFrequencyElements(mixed) == std::vector<uint32_t>{3, 1});

    // Zero is handled correctly, and even counts are excluded.
    std::vector<uint32_t> with_zero = {0, 0, 0, 1, 2, 2, 3};
    assert(extractOddFrequencyElements(with_zero) == std::vector<uint32_t>{0, 1, 3});

    // First-occurrence order preserved when multiple odd elements.
    std::vector<uint32_t> order = {10, 20, 10, 30, 20, 40};
    assert(extractOddFrequencyElements(order) == std::vector<uint32_t>{30, 40});

    // Large values and duplicates.
    std::vector<uint32_t> large = {4294967295u, 1, 4294967295u, 2, 2, 3};
    assert(extractOddFrequencyElements(large) == std::vector<uint32_t>{1, 3});

    // Negative checks: ensure no assertion failures if all elements are odd.
    std::vector<uint32_t> all_odd = {8, 9, 10};
    assert(extractOddFrequencyElements(all_odd) == std::vector<uint32_t>{8, 9, 10});

    // Check that the function does not modify the input (const correctness).
    std::vector<uint32_t> original = {4, 4, 5};
    auto result = extractOddFrequencyElements(original);
    assert(original == std::vector<uint32_t>({4, 4, 5}));
    assert(result == std::vector<uint32_t>{5});
}
// The task is to filter a vector based on parity of element frequency, preserving first-occurrence order. A straightforward approach is to count frequencies using `std::unordered_map<uint32_t, size_t>` in a first pass, then in a second pass iterate the original vector and, for each element, if its count is odd and it hasn't been added yet, append it to the result. However, to preserve "first occurrence order" and avoid duplicates, we need a set (`std::unordered_set<uint32_t>`) to track which elements have already been included. Alternatively, we can use a map that stores both the count and the first-seen flag. Two passes suffice: first count, then second iterate and add if count is odd and not already added. Edge cases: empty input returns empty; all elements even counts returns empty; all elements odd counts returns all unique elements in first-occurrence order; repeated values with odd count are included only once. Time complexity is O(n) average due to hash map operations, and space complexity is O(k) where k is the number of distinct elements. If order is important, using `std::unordered_map` and `std::unordered_set` preserves average O(1) per operation.
