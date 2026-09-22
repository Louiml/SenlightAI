Write a C++ function `containsDuplicate` that takes a `const std::vector<int>&` and returns `true` if any integer value appears at least twice in the array, and `false` if every element is distinct. The function must handle empty arrays (return `false`), arrays with one element (return `false`), negative numbers, zeros, and large positive numbers. The solution should not modify the input array and should work efficiently for up to hundreds of thousands of elements. Do not assume any particular order of elements.
#include <cassert>
#include <vector>

// The solution function is declared above (in the same file before main).
int main() {
    std::vector<int> test1 = {1, 2, 3, 1};
    assert(containsDuplicate(test1) == true);

    std::vector<int> test2 = {1, 2, 3, 4};
    assert(containsDuplicate(test2) == false);

    std::vector<int> test3 = {1, 1, 1, 3, 3, 4, 3, 2, 4, 2};
    assert(containsDuplicate(test3) == true);

    std::vector<int> test4 = {};
    assert(containsDuplicate(test4) == false);

    std::vector<int> test5 = {0};
    assert(containsDuplicate(test5) == false);

    std::vector<int> test6 = {-1, -2, -1};
    assert(containsDuplicate(test6) == true);

    std::vector<int> test7 = {5, -3, 0, 2, 5};
    assert(containsDuplicate(test7) == true);

    std::vector<int> test8 = {10, 20, 30, 40, 50};
    assert(containsDuplicate(test8) == false);
}
#include <vector>
#include <unordered_set>

// Returns true if any value appears at least twice in the input vector;
// returns false if all elements are distinct.
bool containsDuplicate(const std::vector<int>& nums) {
    std::unordered_set<int> seen;
    for (const int& num : nums) {
        if (seen.find(num) != seen.end()) {
            return true;
        }
        seen.insert(num);
    }
    return false;
}
// The most straightforward approach is to use a hash set (e.g., `std::unordered_set<int>`) to track previously seen elements. Iterate through the vector, for each element check if it already exists in the set. If it does, return `true` immediately because a duplicate has been found. Otherwise, insert the element into the set and continue. If the loop completes without finding duplicates, return `false`. This works for all edge cases: an empty vector or single-element vector will never trigger a duplicate, and the function handles any integer values including negative and zero. Time complexity is O(n) on average (with `unordered_set`), and O(n) in the worst case if hash collisions degrade. Space complexity is O(n) in the worst case for storing up to n distinct elements. The function is `const` correct by taking a `const` reference to avoid copying the vector and by not modifying the input.
