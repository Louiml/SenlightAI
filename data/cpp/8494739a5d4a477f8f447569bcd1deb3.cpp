Given two integers `num` (the length of a desired sequence) and `total` (the required sum of that sequence), write a C++ function that returns a vector of `num` consecutive integers whose sum equals `total`. The integers must be consecutive and in increasing order, and there is exactly one valid solution for any given input (the problem guarantees `num > 0` and that a solution exists). For example, if `num = 3` and `total = 12`, the result is `{3, 4, 5}` because `3+4+5=12`. If `num = 5` and `total = 5`, the result is `{-1, 0, 1, 2, 3}`. The function should handle negative totals and sequences that include negative numbers.
#include <cassert>
#include <vector>

int main() {
    // Example from the problem
    assert(consecutiveSequence(3, 12) == std::vector<int>({3, 4, 5}));
    assert(consecutiveSequence(5, 5) == std::vector<int>({-1, 0, 1, 2, 3}));
    
    // Single element
    assert(consecutiveSequence(1, 10) == std::vector<int>({10}));
    assert(consecutiveSequence(1, -7) == std::vector<int>({-7}));
    
    // Two elements
    assert(consecutiveSequence(2, 3) == std::vector<int>({1, 2}));
    assert(consecutiveSequence(2, -3) == std::vector<int>({-2, -1}));
    
    // Negative total with larger sequence
    assert(consecutiveSequence(4, -10) == std::vector<int>({-4, -3, -2, -1}));
    
    // Zero total
    assert(consecutiveSequence(3, 0) == std::vector<int>({-1, 0, 1}));
    assert(consecutiveSequence(7, 0) == std::vector<int>({-3, -2, -1, 0, 1, 2, 3}));
    
    // Larger sequence
    assert(consecutiveSequence(10, 55) == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
    
    // Large values
    assert(consecutiveSequence(4, 100) == std::vector<int>({23, 24, 25, 26}));
    
    // Negative numbers and positive total • ensure offset
    assert(consecutiveSequence(4, 2) == std::vector<int>({-1, 0, 1, 2}));
    
    return 0;
}
#include <vector>

/**
 * @brief Return a vector of `num` consecutive integers whose sum equals `total`.
 * 
 * The problem guarantees a unique solution. The returned vector is in increasing order.
 * 
 * @param num The number of consecutive integers (must be > 0).
 * @param total The required sum of those integers.
 * @return std::vector<int> The consecutive integer sequence.
 */
std::vector<int> consecutiveSequence(int num, int total) {
    // Compute the first element using the arithmetic series formula.
    // Sum = num * first + (0 + 1 + ... + num-1)
    // So first = (total - num*(num-1)/2) / num
    int first = (total - num * (num - 1) / 2) / num;
    
    std::vector<int> result;
    result.reserve(num);
    for (int i = 0; i < num; ++i) {
        result.push_back(first + i);
    }
    return result;
}
// The key insight is that the sum of `num` consecutive integers starting at a value `a` is `a + (a+1) + ... + (a+num-1) = num * a + (0 + 1 + ... + (num-1)) = num * a + num*(num-1)/2`. Setting this equal to `total`, we solve for `a = (total - num*(num-1)/2) / num`. Since `total` and `num` are integers and the problem guarantees an integer solution, the division is exact. So the first element is `a`, and the sequence is `a, a+1, ..., a+num-1`.  
// The provided snippet uses a different but equivalent approach: it initializes a vector with `-num` and adds indices to get a base sequence like `{-num, -num+1, ..., -1}` (which sums to `-num*num + num*(num-1)/2`), then computes the current sum and adjusts each element by `(total - sum)/num`. This works because shifting every element by the same amount preserves consecutiveness and changes the sum by `num * shift`.  
// Edge cases: `num = 1` gives a single element equal to `total`. `num = 2` gives two consecutive numbers summing to `total`. Negative numbers appear naturally when `total` is small relative to `num`.  
// Time complexity is O(num) to fill and adjust the vector, and space complexity is O(num) for the result.
