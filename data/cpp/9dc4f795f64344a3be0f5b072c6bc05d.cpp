/*
Write a C++ function `bool simulateStackSequence(const std::vector<int>& sequence)` that determines whether a given permutation of numbers `1` through `n` can be produced using a single stack operating in the standard way: you may push the next number from the input sequence `1, 2, 3, ..., n` (in that order) onto the stack at any time, and you may pop the top of the stack into the output at any time. The input vector `sequence` represents the desired output order. Your function should return `true` if the output can be generated exactly as given using this push/pop process without any extra data structure (except the stack itself), and `false` otherwise. For example, `[1, 2, 3]` can be produced by push, pop, push, pop, push, pop; `[2, 1, 3]` works; but `[3, 1, 2]` cannot be produced because after pushing 1, 2, 3 and popping 3, 1 is not on top. Your function must avoid using any global state and must be efficient for `n` up to 100,000.
*/
#include <vector>
#include <stack>

// Determine if the given sequence can be produced by pushing 1..n onto a stack
// in order and popping to generate the output sequence.
bool simulateStackSequence(const std::vector<int>& sequence) {
    std::stack<int> st;
    int nextToPush = 1;
    const int n = static_cast<int>(sequence.size());

    for (int target : sequence) {
        // If the stack is empty or the top is smaller than target,
        // push numbers up to the target (inclusive), then pop the target.
        if (st.empty() || st.top() < target) {
            // Push until we reach the target, but do not exceed n.
            while (nextToPush <= target && nextToPush <= n) {
                st.push(nextToPush);
                ++nextToPush;
            }
            // If we couldn't push up to target (target > n), sequence invalid.
            if (nextToPush - 1 != target) {
                return false;
            }
            st.pop();
        }
        // If the top equals target, simply pop.
        else if (st.top() == target) {
            st.pop();
        }
        // If the top is greater than target, target is unreachable.
        else {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Function prototype (declared in solution)
bool simulateStackSequence(const std::vector<int>& sequence);

int main() {
    // Basic valid sequences
    assert(simulateStackSequence({1, 2, 3}) == true);
    assert(simulateStackSequence({2, 1, 3}) == true);
    assert(simulateStackSequence({3, 2, 1}) == true);
    // A classic invalid sequence
    assert(simulateStackSequence({3, 1, 2}) == false);
    // Edge cases: empty, single element
    assert(simulateStackSequence({}) == true);
    assert(simulateStackSequence({1}) == true);
    // More complex valid and invalid
    assert(simulateStackSequence({1, 2, 3, 4}) == true);
    assert(simulateStackSequence({1, 3, 2, 4}) == true);
    assert(simulateStackSequence({1, 4, 3, 2}) == true);
    assert(simulateStackSequence({2, 4, 1, 3}) == false);
    assert(simulateStackSequence({4, 3, 2, 1}) == true);
    // Large n, valid sequence (like actually reverse or identity)
    std::vector<int> big(1000);
    for (int i = 0; i < 1000; ++i) big[i] = i + 1;
    assert(simulateStackSequence(big) == true);
    std::vector<int> reverseBig(1000);
    for (int i = 0; i < 1000; ++i) reverseBig[i] = 1000 - i;
    assert(simulateStackSequence(reverseBig) == true);
    // Invalid: first element not 1 and top can't be reached
    assert(simulateStackSequence({4, 1, 2, 3}) == false);
    return 0;
}
// The algorithm simulates the process step-by-step. Keep a stack and a variable `nextToPush` starting at 1 (the next number from the sequence 1..n). For each target value `t` in the input sequence, do the following:
// - If the stack is empty or the top of the stack is less than `t`, push numbers from `nextToPush` up to and including `t`, recording each push, then immediately pop (since the top is now `t`). This handles the case where the target is not yet on the stack.
// - If the top of the stack equals `t`, just pop it.
// - If the top of the stack is greater than `t`, then `t` cannot be the next output without violating stack order (since everything below the top is inaccessible), so return `false`.
// After processing all targets, if we never encountered the "greater than" case, the sequence is valid and return `true`. Edge cases include an empty sequence (should return `true` because you can just never push/pop), a sequence that is exactly `1..n` (trivially valid), and sequences where `nextToPush` exceeds `n` but we still need a number that was already popped (that will manifest as the top being greater than the target or the stack not containing the target—handled naturally). Time complexity is O(n) because each number is pushed at most once and popped at most once. Space complexity is O(n) in the worst case for the stack.
