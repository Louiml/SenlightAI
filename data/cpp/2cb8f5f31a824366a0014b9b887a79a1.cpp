Write a C++ function that sorts an array (std::vector<int>) containing only the integers 0, 1, and 2 in-place using a counting-based approach (not the Dutch National Flag algorithm). The input vector may be empty, may contain duplicates, and is guaranteed to contain only values from {0, 1, 2}. The function must return void and modify the vector so that all zeros appear first, followed by all ones, then all twos. Implement the function with a descriptive name like `sortZeroOneTwo` and ensure it only includes necessary headers. The function should be const-correct with respect to not modifying the input if passed by const reference, but since in-place modification is required, use a non-const reference parameter.
// The solution directly counts the occurrences of 0, 1, and 2 in three separate integer counters during a single pass through the vector. After counting, perform a second pass over the vector, writing zeros first (using the zero counter), then ones, then twos, decrementing each counter as values are placed. This works because the input is guaranteed to contain only these three values, so the sum of counters equals the vector size. Edge cases include an empty vector (both loops do nothing), a vector with only one distinct value (counters for others are zero, and placement proceeds correctly), and vectors with many duplicates (counting handles them uniformly). Time complexity is O(n) because we make exactly two passes over n elements, and space complexity is O(1) since only three integer counters are used, independent of input size.
#include <vector>

// Sorts a vector containing only 0, 1, and 2 in-place.
// All zeros appear first, then ones, then twos.
void sortZeroOneTwo(std::vector<int>& nums) {
    int count0 = 0, count1 = 0, count2 = 0;
    
    // First pass: count occurrences of each value
    for (int val : nums) {
        if (val == 0) {
            ++count0;
        } else if (val == 1) {
            ++count1;
        } else {
            ++count2;
        }
    }
    
    // Second pass: overwrite the vector with sorted order
    int index = 0;
    for (int i = 0; i < count0; ++i) {
        nums[index++] = 0;
    }
    for (int i = 0; i < count1; ++i) {
        nums[index++] = 1;
    }
    for (int i = 0; i < count2; ++i) {
        nums[index++] = 2;
    }
}
#include <cassert>
#include <vector>

// The solution function is expected to be defined above.

int main() {
    std::vector<int> empty;
    sortZeroOneTwo(empty);
    assert(empty.empty());

    std::vector<int> allZero = {0, 0, 0};
    sortZeroOneTwo(allZero);
    assert(allZero == std::vector<int>({0, 0, 0}));

    std::vector<int> allOne = {1, 1, 1};
    sortZeroOneTwo(allOne);
    assert(allOne == std::vector<int>({1, 1, 1}));

    std::vector<int> mix1 = {2, 1, 0};
    sortZeroOneTwo(mix1);
    assert(mix1 == std::vector<int>({0, 1, 2}));

    std::vector<int> mix2 = {1, 2, 0, 2, 1, 0};
    sortZeroOneTwo(mix2);
    assert(mix2 == std::vector<int>({0, 0, 1, 1, 2, 2}));

    std::vector<int> mix3 = {2, 2, 0, 1, 0, 1, 2};
    sortZeroOneTwo(mix3);
    assert(mix3 == std::vector<int>({0, 0, 1, 1, 2, 2, 2}));

    std::vector<int> singleZero = {0};
    sortZeroOneTwo(singleZero);
    assert(singleZero == std::vector<int>({0}));

    std::vector<int> singleOne = {1};
    sortZeroOneTwo(singleOne);
    assert(singleOne == std::vector<int>({1}));

    std::vector<int> singleTwo = {2};
    sortZeroOneTwo(singleTwo);
    assert(singleTwo == std::vector<int>({2}));

    return 0;
}
