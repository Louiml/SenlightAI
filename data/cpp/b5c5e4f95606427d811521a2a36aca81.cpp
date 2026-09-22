You are given a sequence of `n` integers (possibly negative) that represents the changes to a hero's health, processed one by one in order. The hero starts with 0 health. For each integer, the hero's health changes by that amount. However, the hero may choose to "discard" (i.e., permanently remove) some of the previously seen numbers to ensure that the health is never negative after each step. Discarding a number means subtracting its value from the current health sum (as if it were never added). Your goal is to maximize the number of numbers that are kept (i.e., not discarded) after processing all `n` integers, while ensuring the health never becomes negative at any point. Write a C++ function that takes a vector of integers and returns the maximum possible number of kept numbers. The input `n` can be up to `2e5`, and each integer can be up to `1e9` in absolute value.

// This is a classic greedy problem with a priority queue. The key observation: any non-negative number should always be kept because it only increases health and never causes a deficit. For negative numbers, it might be beneficial to temporarily accept them, but if at any point the total health sum drops below zero, we must discard the most negative number we have encountered so far (i.e., the one with the largest absolute value among those we kept) to restore positivity. This is equivalent to maintaining a min-heap (or a max-heap of negated values) of all numbers we have kept. We process each number: add it to the current sum, push it into a priority queue that stores the smallest (most negative) number at the top (by pushing `-value` into a max-heap). If the sum becomes negative, we pop the smallest value from the priority queue (which is the most harmful negative number), add its negation back to the sum (i.e., subtract its value from sum), and do not count it as kept. At the end, the size of the priority queue is the maximum number of kept numbers. Edge cases: all numbers are non-negative → keep all; large negative numbers early → discard them immediately. Time complexity: O(n log n) due to heap operations, space O(n).

#include <queue>
#include <vector>

// Returns the maximum number of integers that can be kept such that
// the cumulative sum never becomes negative, by potentially discarding
// the most negative previously seen numbers when needed.
int maxKeptNumbers(const std::vector<int>& nums) {
    std::priority_queue<int> minHeap; // stores -value, so top is the most negative kept number
    long long sum = 0;
    
    for (int value : nums) {
        sum += value;
        minHeap.push(-value); // push negative to make largest magnitude negative appear on top
        if (sum < 0) {
            int mostNegative = minHeap.top();
            minHeap.pop();
            sum += mostNegative; // undo its contribution (mostNegative is negative, so adding it reduces the negative sum)
        }
    }
    return static_cast<int>(minHeap.size());
}

#include <cassert>
#include <vector>

// Declaration of the function under test
int maxKeptNumbers(const std::vector<int>& nums);

int main() {
    // Basic all non-negative
    assert(maxKeptNumbers({1, 2, 3, 4}) == 4);
    // Single negative
    assert(maxKeptNumbers({-5}) == 0);
    // Mixed sequence where discarding later is beneficial
    assert(maxKeptNumbers({3, -5, 1}) == 2); // keep 3, discard -5, keep 1
    // Large negative early
    assert(maxKeptNumbers({-10, 20, -1}) == 2); // discard -10, keep 20 and -1
    // All negative
    assert(maxKeptNumbers({-1, -2, -3}) == 0);
    // Balanced with zero
    assert(maxKeptNumbers({0, -1, 2}) == 3); // 0, -1, 2 keeps sum 1, never negative
    // Edge with equal magnitude negatives
    assert(maxKeptNumbers({5, -5, 5}) == 3); // sum always 5 or 0, keep all
    // Large n with pattern
    assert(maxKeptNumbers({1, -2, 1, 1}) == 3); // discard -2, keep three 1's
    // Test with duplicates
    assert(maxKeptNumbers({-3, -3, 10}) == 2); // keep one -3 and 10, discard other -3
    // Verify with a larger case
    std::vector<int> large;
    for (int i = 0; i < 100000; ++i) large.push_back(i % 2 == 0 ? 1000000 : -999999);
    assert(maxKeptNumbers(large) == 50000); // only the positive numbers are kept, negatives cause sum to drop
    return 0;
}
