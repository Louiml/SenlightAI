/*
Write a C++ function that takes a non-empty vector of integers and returns the majority element, defined as the element that appears more than `n/2` times (where `n` is the size of the vector). You may assume that a majority element always exists in the input. The function must be efficient and avoid using extra container like hash maps if possible. Implement the function with the signature `int majorityElement(const std::vector<int>& nums)`. Consider edge cases such as vectors of size 1, all identical elements, and large inputs. Do not include a `main` function in your solution; only provide the function.
*/

#include <vector>

// Returns the majority element (appears more than n/2 times) in nums.
// Assumes such an element always exists.
int majorityElement(const std::vector<int>& nums) {
    int candidate = 0;
    int count = 0;
    
    for (int num : nums) {
        if (count == 0) {
            candidate = num;
            count = 1;
        } else if (num == candidate) {
            ++count;
        } else {
            --count;
        }
    }
    
    return candidate;
}

#include <cassert>
#include <vector>

// Include the solution function here or via header

int main() {
    // Single element
    std::vector<int> v1 = {5};
    assert(majorityElement(v1) == 5);
    
    // All same elements
    std::vector<int> v2 = {7, 7, 7, 7};
    assert(majorityElement(v2) == 7);
    
    // Majority in middle
    std::vector<int> v3 = {1, 2, 3, 2, 2};
    assert(majorityElement(v3) == 2);
    
    // Majority at end
    std::vector<int> v4 = {10, 20, 30, 10, 10, 10};
    assert(majorityElement(v4) == 10);
    
    // Negative numbers
    std::vector<int> v5 = {-1, -1, -2, -1};
    assert(majorityElement(v5) == -1);
    
    // Large vector with repeated majority
    std::vector<int> v6;
    for (int i = 0; i < 1000; ++i) v6.push_back(42);
    for (int i = 0; i < 500; ++i) v6.push_back(1);
    assert(majorityElement(v6) == 42);
    
    // Example from snippet (corrected vector)
    std::vector<int> v7 = {6, 2, 5, 2, 7, 3, 2, 2, 2, 2};
    assert(majorityElement(v7) == 2);
    
    // Alternating but majority still wins
    std::vector<int> v8 = {3, 3, 4, 3, 4, 3};
    assert(majorityElement(v8) == 3);
    
    return 0;
}

// The optimal solution uses the Boyer-Moore majority vote algorithm, which finds the majority element in O(n) time and O(1) space. The idea is to maintain a candidate and a counter. Initially, the counter is 0 and there is no candidate. Iterate through each element: if the counter is 0, set the current element as the candidate and set counter to 1. Otherwise, if the current element equals the candidate, increment the counter; else decrement it. At the end, the candidate is the majority element (guaranteed to exist). This works because the majority element, appearing more than n/2 times, will survive all cancellations. Edge cases: size 1 vector immediately returns that element; vectors with all same elements will keep counter increased; if no majority exists, the result is undefined, but the problem guarantees existence. Time complexity is O(n), space complexity is O(1) excluding input storage.
