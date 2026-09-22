/*
Write a C++ function that takes a vector of integers and returns the majority element, which is defined as an element that appears more than `n / 2` times in the vector of size `n`. If no such element exists, return `-1` (assuming all input elements are non-negative). The function should be named `findMajorityElement` and should accept the vector by const reference. The function must handle an empty vector (return `-1`), duplicate values, and a vector where no majority exists. The solution should not modify the input.
*/
#include <vector>
#include <unordered_map>

// Returns the majority element (appears more than n/2 times) if it exists,
// otherwise returns -1. Assumes all input elements are non-negative.
int findMajorityElement(const std::vector<int>& nums) {
    const std::size_t n = nums.size();
    if (n == 0) return -1;
    
    std::unordered_map<int, std::size_t> count;
    const std::size_t threshold = n / 2;
    
    for (const int value : nums) {
        ++count[value];
        if (count[value] > threshold) {
            return value;
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {3, 3, 4, 2, 3, 3, 3};
    assert(findMajorityElement(v1) == 3);

    std::vector<int> v2 = {1, 2, 3};
    assert(findMajorityElement(v2) == -1);

    std::vector<int> v3 = {5};
    assert(findMajorityElement(v3) == 5);

    std::vector<int> v4 = {};
    assert(findMajorityElement(v4) == -1);

    std::vector<int> v5 = {2, 2, 1, 1, 2};
    assert(findMajorityElement(v5) == 2);

    std::vector<int> v6 = {0, 0, 0, 1};
    assert(findMajorityElement(v6) == 0);

    std::vector<int> v7 = {1, 1, 2, 2, 2, 2, 1};
    assert(findMajorityElement(v7) == -1); // 2 appears 4 times, not > 3.5, so no majority

    std::vector<int> v8 = {9, 9, 9, 9, 8, 7, 9};
    assert(findMajorityElement(v8) == 9);

    return 0;
}
// The straightforward approach is to count occurrences of each element. A brute-force method compares each element against all others, which takes O(n²) time. A better approach is to use a hash map (std::unordered_map) to store counts while iterating through the vector once. For each element, increment its count in the map, and after updating, check if the count exceeds `n / 2`. If yes, return that element immediately. If the loop finishes without finding any such element, return `-1`. This works in O(n) average time and uses O(n) space for the map. Edge cases include an empty vector (return `-1`), a single element (it is trivially the majority), all elements identical (first element triggers the condition), and no majority (the loop completes without returning). The use of `const vector<int>&` ensures the input is not copied or modified. Since the problem guarantees non-negative integers (or we assume that), `-1` is a safe sentinel; if negative integers are allowed, the function should instead return a boolean and output the element via reference, but here we stick with the sentinel.
