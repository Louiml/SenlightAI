Write a C++ function named `maxConsecutiveOnes` that takes a `const std::vector<int>&` containing only binary values (0 or 1) and returns the maximum number of consecutive 1's found in the vector. The function should handle an empty vector (returning 0), a vector with no 1's (returning 0), and vectors where the longest run of 1's appears at the beginning, middle, or end. The function must not modify the input vector and should be efficient for large inputs.
// The solution uses a linear scan over the vector. Maintain two variables: `currentCount` to track the length of the current run of consecutive 1's, and `maxCount` to store the longest run seen so far. Iterate through each element: if the element is 1, increment `currentCount`; otherwise, reset `currentCount` to 0. After each update, if `currentCount` exceeds `maxCount`, update `maxCount`. This works because the run resets only when a 0 is encountered, and the maximum is always updated when a run ends or even in the middle of a run (safe to do after each element). Edge cases include an empty vector (loop never runs, returns 0), a vector of all zeros (currentCount never increases, maxCount stays 0), and a vector ending with 1's (the final run is captured because the update happens inside the loop, not after). Time complexity is O(n) with a single pass, and space complexity is O(1) auxiliary, not counting the input vector itself.
#include <vector>

// Returns the maximum number of consecutive 1's in the input binary vector.
// Returns 0 for an empty vector or a vector with no 1's.
int maxConsecutiveOnes(const std::vector<int>& nums) {
    int currentCount = 0;
    int maxCount = 0;

    for (int value : nums) {
        if (value == 1) {
            ++currentCount;
        } else {
            currentCount = 0;
        }

        if (currentCount > maxCount) {
            maxCount = currentCount;
        }
    }

    return maxCount;
}
#include <cassert>
#include <vector>

int maxConsecutiveOnes(const std::vector<int>& nums);

int main() {
    // Empty vector
    assert(maxConsecutiveOnes({}) == 0);
    
    // All zeros
    assert(maxConsecutiveOnes({0,0,0}) == 0);
    
    // All ones
    assert(maxConsecutiveOnes({1,1,1,1}) == 4);
    
    // Single one
    assert(maxConsecutiveOnes({1}) == 1);
    
    // Single zero
    assert(maxConsecutiveOnes({0}) == 0);
    
    // Mixed with run in middle
    assert(maxConsecutiveOnes({0,1,1,0,1,1,1,0}) == 3);
    
    // Run at beginning
    assert(maxConsecutiveOnes({1,1,0,0,1}) == 2);
    
    // Run at end
    assert(maxConsecutiveOnes({0,0,1,1,1}) == 3);
    
    // Alternating
    assert(maxConsecutiveOnes({1,0,1,0,1}) == 1);
    
    // Longest run at end after a shorter run
    assert(maxConsecutiveOnes({1,1,0,1,1,1}) == 3);
    
    return 0;
}
