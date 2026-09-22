/*
Write a C++ function named `containsDuplicate` that takes a constant reference to a vector of integers and returns a `bool` indicating whether any value appears at least twice in the vector. The function must return `true` if at least one duplicate exists, and `false` if all elements are distinct. The function must be efficient for large inputs and must not modify the input vector. Consider edge cases such as an empty vector, a vector with a single element, and vectors containing negative numbers, zeros, and large values. The function should behave correctly regardless of the order of elements and should not rely on sorting.
*/
#include <vector>
#include <unordered_set>

// Returns true if nums contains any duplicate value, false otherwise.
bool containsDuplicate(const std::vector<int>& nums) {
    std::unordered_set<int> seen;
    for (int num : nums) {
        if (!seen.insert(num).second) {
            return true; // Insertion failed, duplicate found
        }
    }
    return false; // All elements distinct
}
#include <cassert>
#include <vector>

// Assume containsDuplicate is declared above

int main() {
    std::vector<int> empty;
    assert(containsDuplicate(empty) == false);

    std::vector<int> single = {42};
    assert(containsDuplicate(single) == false);

    std::vector<int> distinct = {1, 2, 3, 4, 5};
    assert(containsDuplicate(distinct) == false);

    std::vector<int> duplicate_at_end = {1, 2, 3, 4, 2};
    assert(containsDuplicate(duplicate_at_end) == true);

    std::vector<int> duplicate_at_start = {5, 5, 1, 2, 3};
    assert(containsDuplicate(duplicate_at_start) == true);

    std::vector<int> all_same = {7, 7, 7, 7};
    assert(containsDuplicate(all_same) == true);

    std::vector<int> with_negatives = {-1, 2, -3, 4, -1};
    assert(containsDuplicate(with_negatives) == true);

    std::vector<int> zeros = {0, 1, 2, 0};
    assert(containsDuplicate(zeros) == true);

    std::vector<int> large_distinct = {100000, 99999, 99998};
    assert(containsDuplicate(large_distinct) == false);

    std::vector<int> large_duplicate = {100000, 100000};
    assert(containsDuplicate(large_duplicate) == true);

    return 0;
}
// The solution uses a hash-based approach with an unordered set to track encountered values. The algorithm iterates through each integer in the input vector once. For each value, it attempts to insert it into the set. If the insertion fails (i.e., the value is already present), a duplicate has been found, and the function immediately returns `true`. If the loop completes without any failed insertion, all elements are distinct, and the function returns `false`. This approach handles the edge cases naturally: an empty vector returns `false` because the loop never executes; a single-element vector returns `false` because the first insertion always succeeds. The time complexity is \(O(n)\) on average because each set operation (insert and lookup) is \(O(1)\) on average, and the space complexity is \(O(n)\) in the worst case because the set may store all distinct elements. The function uses `const` reference to avoid copying the input vector and to guarantee that the input is not modified.
