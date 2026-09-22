// Write a C++ function `countCommonCDs` that takes two vectors of integers representing CD catalog numbers owned by Jack and Jill, and returns the number of catalog numbers that appear in both collections. The input vectors may contain duplicate values, and the function should count only unique common catalog numbers (i.e., if Jack has CD 5 twice and Jill has CD 5 once, that counts as one common CD). The function should handle empty vectors and should not modify the input vectors. You may assume all catalog numbers are non-negative integers, but no upper bound is specified; the function must work efficiently for very large collections (up to 1,000,000 elements per vector in the worst case).

The core idea is to use a hash set (unordered_set) to store the unique catalog numbers from Jack's collection, then iterate through Jill's collection and count each catalog number that is found in the set, while ensuring duplicates in Jill's collection are only counted once. The simplest approach is to insert Jack's numbers into an unordered_set, then for each number in Jill's collection, check if it exists in the set; if yes, increment a counter and erase that number from the set to prevent double-counting if Jill has duplicates. This handles all edge cases: empty vectors (both empty yields 0), duplicates in either collection (set handles Jack's duplicates automatically, and erasing handles Jill's duplicates), and very large inputs (average O(1) per operation with the hash set). Time complexity is O(n + m) on average, where n is Jack's size and m is Jill's size, since each insert and lookup is average O(1). Space complexity is O(n) for the set, plus O(1) auxiliary space beyond that. If the problem requires worst-case guarantees, a sorted vector + two-pointer approach would be O(n log n + m log m) but the hash set is generally faster in practice. Since the task does not specify worst-case constraints, the hash set solution is appropriate.

#include <unordered_set>
#include <vector>

// Count the number of unique catalog numbers that appear in both Jack's and Jill's collections.
// The input vectors may contain duplicates; each unique common number is counted only once.
int countCommonCDs(const std::vector<int>& jack_cds, const std::vector<int>& jill_cds) {
    // Store Jack's unique CD catalog numbers.
    std::unordered_set<int> jack_set;
    for (int cd : jack_cds) {
        jack_set.insert(cd);
    }

    int common_count = 0;
    // For each CD in Jill's collection, if it exists in Jack's set, count it once and remove it
    // to avoid counting duplicates from Jill multiple times.
    for (int cd : jill_cds) {
        if (jack_set.find(cd) != jack_set.end()) {
            ++common_count;
            jack_set.erase(cd);  // Prevent counting the same CD again.
        }
    }
    return common_count;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with distinct common elements.
    std::vector<int> jack1 = {1, 2, 3, 4, 5};
    std::vector<int> jill1 = {3, 4, 5, 6, 7};
    assert(countCommonCDs(jack1, jill1) == 3); // {3,4,5}

    // Duplicates in both collections: common numbers counted once.
    std::vector<int> jack2 = {1, 1, 2, 2, 3};
    std::vector<int> jill2 = {2, 2, 3, 3, 4};
    assert(countCommonCDs(jack2, jill2) == 2); // {2,3}

    // No common elements.
    assert(countCommonCDs({1, 2}, {3, 4}) == 0);

    // One empty collection.
    assert(countCommonCDs({}, {1, 2}) == 0);
    assert(countCommonCDs({1, 2}, {}) == 0);

    // Both empty.
    assert(countCommonCDs({}, {}) == 0);

    // Identical collections (duplicates do not inflate count).
    std::vector<int> same = {5, 5, 5};
    assert(countCommonCDs(same, same) == 1); // Only {5}

    // Large single common element with many duplicates.
    std::vector<int> jack7(1000, 42);
    std::vector<int> jill7(1000, 42);
    assert(countCommonCDs(jack7, jill7) == 1);

    // All common but with extra unique elements.
    std::vector<int> jack8 = {10, 20, 30};
    std::vector<int> jill8 = {30, 20, 10};
    assert(countCommonCDs(jack8, jill8) == 3);

    return 0;
}
