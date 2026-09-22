Given a vector of integers `arr` that contains a permutation of the integers from 0 to n-1 (where n is the size of the array), write a C++ function that returns the maximum number of chunks we can split the array into so that after sorting each chunk individually and then concatenating them in order, the entire array becomes sorted in ascending order (i.e., equal to [0, 1, 2, ..., n-1]). The input array is non-empty, contains no duplicates, and each element is in the range [0, n-1]. The function should be named `maxChunksToSorted` and take a `const std::vector<int>&` as its parameter, returning an integer representing the maximum number of valid chunks.
// The key observation is that for any prefix of the array from index 0 to i, if the maximum number in that prefix is equal to i (or equivalently, the sum of the prefix equals the sum of 0..i, since the array is a permutation without duplicates), then that prefix can form a valid chunk. This is because the chunk will contain exactly the numbers 0 through i (possibly in different order), and sorting it will produce the correct subsequence for that part of the sorted array. The algorithm iterates through the array while maintaining two running sums: `runningSum` accumulates the actual values seen so far, and `expectedSum` accumulates the integers 0, 1, 2, ... i. Whenever `runningSum == expectedSum`, we know the current prefix is a valid chunk, so we increment the chunk counter. Since the array is a permutation, the final sum always matches, so the last chunk is always formed. Edge case: the smallest possible valid chunk count is 1 (for example, when the array is already sorted but we could still split into n chunks; but when the array is reverse sorted, only 1 chunk works). The algorithm works correctly for any permutation including cases where many chunks are possible (e.g., [0,1,2] → 3 chunks). Time complexity is O(n) and space complexity is O(1), since we only use a few integer variables.
#include <vector>

// Returns the maximum number of chunks the array can be split into such that
// sorting each chunk individually and concatenating results in a fully sorted array.
int maxChunksToSorted(const std::vector<int>& arr) {
    int runningSum = 0;
    int expectedSum = 0;
    int chunks = 0;
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        runningSum += arr[i];
        expectedSum += i;
        if (runningSum == expectedSum) {
            ++chunks;
        }
    }
    return chunks;
}
#include <cassert>
#include <vector>

// Declare the function (already defined above in the solution section)
int maxChunksToSorted(const std::vector<int>& arr);

int main() {
    // Already sorted array: each element can be a separate chunk
    assert(maxChunksToSorted({0, 1, 2, 3}) == 4);
    
    // Reverse sorted: only the whole array can be a chunk
    assert(maxChunksToSorted({3, 2, 1, 0}) == 1);
    
    // Mixed case: chunks [1,0], [2,3,4]
    assert(maxChunksToSorted({1, 0, 2, 3, 4}) == 4);
    
    // Single element
    assert(maxChunksToSorted({0}) == 1);
    
    // Another mixed case: chunks [2,1,0], [3], [4]
    assert(maxChunksToSorted({2, 1, 0, 3, 4}) == 3);
    
    // Array where each pair swaps: [1,0,3,2] → chunks [1,0],[3,2]
    assert(maxChunksToSorted({1, 0, 3, 2}) == 2);
    
    // Already sorted but with larger numbers
    assert(maxChunksToSorted({0, 1, 2, 3, 4, 5}) == 6);
    
    // All chunks possible: each element equals its index
    assert(maxChunksToSorted({0, 1, 2, 3, 4}) == 5);
    
    return 0;
}
