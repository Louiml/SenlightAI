Write a C++ function `majorityElement` that accepts a non-empty vector of integers and returns the majority element—the element that appears more than `n / 2` times in the vector, where `n` is the vector's size. The input vector is guaranteed to contain a majority element, and the function must work correctly for vectors containing both positive and negative integers, as well as vectors of size 1. The function must not modify the input vector and should return the majority element as an `int`.

#include <cassert>
#include <vector>

int main() {
    std::vector<int> v1 = {3, 3, 3, 2, 3};
    assert(majorityElement(v1) == 3);

    std::vector<int> v2 = {2, 2, 1, 1, 2, 2};
    assert(majorityElement(v2) == 2);

    std::vector<int> v3 = {7};
    assert(majorityElement(v3) == 7);

    std::vector<int> v4 = {-1, -1, -1, 1};
    assert(majorityElement(v4) == -1);

    std::vector<int> v5 = {0, 0, 0, 1, 0};
    assert(majorityElement(v5) == 0);

    std::vector<int> v6 = {5, 5, 5, 5, 1, 2};
    assert(majorityElement(v6) == 5);

    std::vector<int> v7 = {10, 10, 10, 10, 9, 8, 10};
    assert(majorityElement(v7) == 10);

    std::vector<int> v8 = {1, 2, 1, 2, 1, 2, 1};
    assert(majorityElement(v8) == 1);

    std::vector<int> v9 = {-5, -5, -5, -5};
    assert(majorityElement(v9) == -5);

    std::vector<int> v10 = {100, 200, 100, 100, 100};
    assert(majorityElement(v10) == 100);

    return 0;
}

#include <vector>

// Return the majority element (appears more than n/2 times) from a non-empty vector.
// The input is guaranteed to contain a majority element.
int majorityElement(const std::vector<int>& nums) {
    int freq = 1;
    int number = nums[0];

    for (std::size_t i = 1; i < nums.size(); ++i) {
        if (nums[i] == number) {
            ++freq;
        } else {
            --freq;
        }

        if (freq == 0) {
            freq = 1;
            number = nums[i];
        }
    }

    return number;
}

// The solution uses the Boyer-Moore majority vote algorithm. It maintains a candidate element (`number`) and a counter (`freq`). Starting with the first element as the candidate with frequency 1, we iterate through the remaining elements. When the current element equals the candidate, we increment the counter; otherwise, we decrement it. If the counter reaches zero, we reset it to 1 and replace the candidate with the current element. Because a majority element exists (appearing more than `n/2` times), the candidate remaining at the end is guaranteed to be that majority element. This works because the majority element will survive a pairing with every non-majority element, leaving at least one occurrence of the majority candidate. Edge cases include vectors of size 1 (returns the single element) and vectors with negative numbers (the algorithm treats all integers uniformly). Time complexity is O(n) with a single pass, and space complexity is O(1) aside from the input vector itself.
