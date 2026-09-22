// Write a C++ function `findMajorityElements` that takes a non-empty vector of integers and returns a vector containing all elements that appear more than `n / 3` times, where `n` is the size of the input vector. The result may contain zero, one, or two distinct elements (at most two can exist, since three elements would each need more than `n/3` occurrences, totaling more than `n`). The returned vector should be in any order, but must not contain duplicates. The function should handle vectors of any size, including size 1 and size 2, and should not modify the input vector. If no elements qualify, return an empty vector. The solution must use a hash map (e.g., `std::unordered_map`) to count frequencies and then collect qualifying elements. Ensure the function is `const`-correct by taking the input as `const std::vector<int>&`.
The problem is a direct application of frequency counting. Since we need elements that appear strictly more than `n/3` times, we can count occurrences of each element using a hash map (`std::unordered_map<int, int>`). After counting, iterate through the map and push any key whose value is greater than `n/3` into a result vector. Because at most two distinct elements can satisfy this condition (proof: if three did, their sum of counts would exceed `n`), the result vector will have size 0, 1, or 2. Edge cases: if the vector is empty (though the task says non-empty, we can handle it gracefully), the result is empty; if the vector has size 1, that single element appears once, which is greater than `1/3` (0.333), so it qualifies; if size 2, any element appearing twice qualifies because `2 > 2/3` is true; if all elements are the same, that element qualifies when `n > 0`. The algorithm runs in O(n) time on average (due to hash map operations) and O(n) auxiliary space for the hash map, plus O(1) extra for the result (since at most 2 elements). The input vector is not modified because we take it by const reference.
#include <vector>
#include <unordered_map>

// Return all elements that occur more than n/3 times in the input vector.
// At most two such elements can exist. The result is in unspecified order.
std::vector<int> findMajorityElements(const std::vector<int>& nums) {
    std::vector<int> result;
    if (nums.empty()) {
        return result;
    }

    std::unordered_map<int, int> freq;
    for (int num : nums) {
        ++freq[num];
    }

    const size_t threshold = nums.size() / 3;
    for (const auto& entry : freq) {
        if (entry.second > threshold) {
            result.push_back(entry.first);
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Single element qualifies.
    std::vector<int> v1 = {5};
    assert(findMajorityElements(v1) == std::vector<int>({5}));

    // Two elements, both qualify.
    std::vector<int> v2 = {1, 2};
    auto r2 = findMajorityElements(v2);
    assert(r2.size() == 2);
    std::sort(r2.begin(), r2.end());
    assert(r2 == std::vector<int>({1, 2}));

    // No majority in a large array.
    std::vector<int> v3 = {1, 2, 3, 4, 5};
    assert(findMajorityElements(v3).empty());

    // One element appears more than n/3 times (1 appears 3 times in size 7 => 3 > 2).
    std::vector<int> v4 = {1, 1, 1, 2, 2, 3, 4};
    assert(findMajorityElements(v4) == std::vector<int>({1}));

    // Two elements appear more than n/3 times (1 appears 3 times, 2 appears 3 times in size 8 => 3 > 2).
    std::vector<int> v5 = {1, 1, 1, 2, 2, 2, 3, 4};
    auto r5 = findMajorityElements(v5);
    assert(r5.size() == 2);
    std::sort(r5.begin(), r5.end());
    assert(r5 == std::vector<int>({1, 2}));

    // All identical elements qualify.
    std::vector<int> v6 = {7, 7, 7, 7};
    assert(findMajorityElements(v6) == std::vector<int>({7}));

    // Edge with n=2 and both different: both appear once, 1 > 0 (2/3 floor is 0), so both qualify.
    std::vector<int> v7 = {10, 20};
    auto r7 = findMajorityElements(v7);
    assert(r7.size() == 2);
    std::sort(r7.begin(), r7.end());
    assert(r7 == std::vector<int>({10, 20}));

    // n=3, each appears once: 1 > 1? false (1 > 1 is false), so empty.
    std::vector<int> v8 = {1, 2, 3};
    assert(findMajorityElements(v8).empty());
}
