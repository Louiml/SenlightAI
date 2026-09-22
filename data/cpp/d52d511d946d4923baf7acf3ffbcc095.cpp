/*
Write a C++ function that takes a vector of integers and returns the majority element — the value that appears more than half the time. The input vector is guaranteed to be non-empty and to always contain a majority element. The vector size can range from 1 to 50,000, and each integer can be as low as -10⁹ and as high as 10⁹. Your function should be named `findMajorityElement` and must accept the vector by const reference to avoid copying. The function must be efficient for large inputs, and you may assume the majority condition always holds (no need to validate). Return the majority element as an `int`.
*/
#include <vector>

// Returns the majority element in nums (appears > n/2 times).
int findMajorityElement(const std::vector<int>& nums) {
    int candidate = nums[0];
    int count = 1;
    for (size_t i = 1; i < nums.size(); ++i) {
        if (count == 0) {
            candidate = nums[i];
            count = 1;
        } else if (nums[i] == candidate) {
            ++count;
        } else {
            --count;
        }
    }
    return candidate;
}
#include <cassert>
#include <vector>

// Include the solution function here or via header.

int main() {
    std::vector<int> v1 = {3, 2, 3};
    assert(findMajorityElement(v1) == 3);

    std::vector<int> v2 = {2, 2, 1, 1, 1, 2, 2};
    assert(findMajorityElement(v2) == 2);

    std::vector<int> v3 = {1};
    assert(findMajorityElement(v3) == 1);

    std::vector<int> v4 = {-5, -5, 2, -5};
    assert(findMajorityElement(v4) == -5);

    std::vector<int> v5 = {1000000000, 1000000000, 1000000000};
    assert(findMajorityElement(v5) == 1000000000);

    std::vector<int> v6 = {7, 7, 7, 8, 7};
    assert(findMajorityElement(v6) == 7);

    std::vector<int> v7 = {1, 2, 1, 1, 3, 1};
    assert(findMajorityElement(v7) == 1);

    return 0;
}
// The key idea is the Boyer-Moore majority vote algorithm. It works by maintaining a candidate value and a counter. Start with the first element as candidate and counter = 1. Then iterate from the second element onward. If the counter is 0, set the current element as the new candidate and reset counter to 1. If the current element equals the candidate, increment counter; otherwise decrement counter. At the end, the candidate is guaranteed to be the majority element because the majority value appears more than n/2 times, so its net contribution to the counter (increments minus decrements) ensures it survives all cancellations. Edge cases include a vector of length 1 (the single element is the majority), and vectors with all identical elements (the counter never hits 0, and the candidate is correct throughout). Time complexity is O(n) for a single pass, with O(1) auxiliary space. No sorting or hash map is needed, making it optimal for the given constraints.
