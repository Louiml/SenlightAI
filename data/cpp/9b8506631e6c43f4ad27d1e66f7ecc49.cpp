Write a C++ function that takes a vector of integers as input and returns an integer indicating whether the array is strictly increasing. Specifically, return `1` if every element is strictly greater than the previous element (i.e., `a[i] > a[i-1]` for all `i` from 1 to n-1), and return `0` otherwise. The function must handle arrays of any length, including empty or single-element arrays (which are trivially strictly increasing). Do not modify the input vector.
The solution is straightforward: iterate through the array from index 1 to n-1, comparing each element to its predecessor. If at any point the current element is not strictly greater than the previous one (i.e., `a[i] <= a[i-1]`), the array is not strictly increasing, so we immediately return `0`. If we complete the loop without finding such a violation, the array is strictly increasing and we return `1`. Edge cases: an empty array or a single-element array will not enter the loop, so they correctly return `1` (since there are no adjacent pairs to violate the condition). The algorithm runs in O(n) time and uses O(1) auxiliary space.
#include <vector>

// Returns 1 if the vector is strictly increasing, 0 otherwise.
// An empty or single-element vector is considered strictly increasing.
int isStrictlyIncreasing(const std::vector<int>& a) {
    for (std::size_t i = 1; i < a.size(); ++i) {
        if (a[i] <= a[i - 1]) {
            return 0;
        }
    }
    return 1;
}
#include <cassert>
#include <vector>

// Prototype of the solution function
int isStrictlyIncreasing(const std::vector<int>& a);

int main() {
    // Test basic increasing sequence
    assert(isStrictlyIncreasing({1, 2, 3, 4}) == 1);
    
    // Test non-increasing (equal adjacent)
    assert(isStrictlyIncreasing({1, 2, 2, 3}) == 0);
    
    // Test non-increasing (decrease)
    assert(isStrictlyIncreasing({5, 3, 1}) == 0);
    
    // Test single element
    assert(isStrictlyIncreasing({42}) == 1);
    
    // Test empty vector
    assert(isStrictlyIncreasing({}) == 1);
    
    // Test increasing with negative numbers
    assert(isStrictlyIncreasing({-5, -3, -1, 0}) == 1);
    
    // Test decreasing with negative numbers
    assert(isStrictlyIncreasing({-1, -3, -5}) == 0);
    
    // Test long increasing sequence
    assert(isStrictlyIncreasing({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) == 1);
    
    // Test long sequence with a break at the end
    assert(isStrictlyIncreasing({1, 2, 3, 4, 5, 5}) == 0);
    
    // Test sequence where break is at the beginning
    assert(isStrictlyIncreasing({2, 1, 3, 4}) == 0);
    
    return 0;
}
