/*
Write a C++ function named `hasDuplicate` that takes a non-empty vector of 64-bit signed integers (`std::vector<long long>`) by const reference and returns `true` if any integer value appears at least twice, and `false` if every value is distinct. The function must handle vectors of arbitrary length up to 100,000 elements, values ranging from -10^9 to 10^9, and must not modify the input vector. The solution must be efficient enough to handle the maximum constraint comfortably, and must correctly handle edge cases like vectors with one element, duplicates at the beginning or end, and large negative and positive values.
*/
#include <vector>
#include <unordered_set>

// Returns true if any value appears at least twice in nums, false otherwise.
bool hasDuplicate(const std::vector<long long>& nums) {
    std::unordered_set<long long> seen;
    seen.reserve(nums.size());  // Avoid rehashing, improves performance

    for (long long num : nums) {
        if (seen.find(num) != seen.end()) {
            return true;
        }
        seen.insert(num);
    }
    return false;
}
#include <cassert>
#include <vector>

// Assume hasDuplicate is declared above

int main() {
    std::vector<long long> v1 = {1, 2, 3, 1};
    assert(hasDuplicate(v1) == true);

    std::vector<long long> v2 = {1, 2, 3, 4};
    assert(hasDuplicate(v2) == false);

    std::vector<long long> v3 = {1, 1, 1, 3, 3, 4, 3, 2, 4, 2};
    assert(hasDuplicate(v3) == true);

    std::vector<long long> v4 = {5};
    assert(hasDuplicate(v4) == false);

    std::vector<long long> v5 = {1000000000, -1000000000, 0, 1000000000};
    assert(hasDuplicate(v5) == true);

    std::vector<long long> v6 = {9, 8, 7, 6, 5};
    assert(hasDuplicate(v6) == false);

    std::vector<long long> v7 = {0, 0};
    assert(hasDuplicate(v7) == true);

    // Edge: two identical large negative numbers
    std::vector<long long> v8 = {-999999999, -999999999};
    assert(hasDuplicate(v8) == true);

    // Duplicate at end
    std::vector<long long> v9 = {1, 2, 3, 4, 5, 5};
    assert(hasDuplicate(v9) == true);

    // No duplicate but many elements
    std::vector<long long> v10 = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
    assert(hasDuplicate(v10) == false);

    return 0;
}
// The problem asks to detect duplicates in an array, which can be solved efficiently using a hash set. The main algorithm iterates through each element, checking if it already exists in the set; if so, return `true` immediately. If not, insert the element and continue. If the loop completes without finding a duplicate, return `false`. Important edge cases include: an empty vector (though constraints say non-empty, we can still handle it by returning `false`), a vector with only one element (always `false`), duplicates that appear early or late, and very large values (which `long long` accommodates up to about 9×10^18). Using a hash set (`std::unordered_set`) gives average O(1) insertion and lookup, so total time complexity is O(n) on average, and space complexity is O(n) in the worst case for storing unique elements. An alternative approach is sorting the vector and checking adjacent elements, which uses O(n log n) time and O(1) extra space if sorting in place, but since we cannot modify the input, we'd need a copy, making it O(n) space anyway. The hash set approach is simpler and faster on average. The `const` reference ensures we don't accidentally modify the input.
