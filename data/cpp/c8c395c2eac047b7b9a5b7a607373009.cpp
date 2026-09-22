Write a C++ function `sortStackDescending` that takes a `std::stack<int>` containing integers (with the top element representing the last element of the input sequence) and returns a new stack where the elements are sorted in **descending order from bottom to top**, i.e., the largest element is at the top of the returned stack. The function must sort the stack **in-place using only one additional stack** (no other containers like arrays, vectors, or deques are allowed), and must not modify the original input stack (i.e., it should work on a copy or operate on the parameter by value). The input stack may be empty, contain duplicate values, positive/negative numbers, or already be sorted. The returned stack's top should be the largest integer, and the bottom should be the smallest.
// The solution uses a classic stack-sorting algorithm with one auxiliary stack. The core idea: repeatedly pop the top element from the input stack (`cur`). While the auxiliary stack is non-empty and its top is **greater** than `cur` (so that the auxiliary stack becomes sorted in increasing order from top to bottom), we pop that top and push it back onto the input stack, then push `cur` onto the auxiliary stack. This process places `cur` in the correct position relative to the auxiliary stack's elements. After processing all input elements, the auxiliary stack will contain the elements sorted in **non-decreasing order from top to bottom**, meaning the smallest is at the top. To achieve descending order from bottom to top (largest at top), we need to adjust the comparison: we pop from the auxiliary stack when its top is **less** than `cur` (so that larger elements end up on top). Alternatively, we can keep the original algorithm and then reverse the auxiliary stack into the input stack, but since the task requires returning a new stack, we can simply modify the comparison to `cur > extraS.top()` (as in the snippet) which yields descending order from bottom to top. Edge cases: empty stack returns empty; all duplicates: the inner while never triggers, and elements are pushed in order; negative numbers: comparisons work naturally. Time complexity: O(n²) worst-case (when the input is sorted in reverse), but O(n) for already sorted input. Space complexity: O(n) for the auxiliary stack, excluding the input.
#include <stack>

// Sort the given stack in descending order from bottom to top (largest on top).
// Uses only one auxiliary stack. The original stack is not modified (passed by value).
std::stack<int> sortStackDescending(std::stack<int> input) {
    std::stack<int> auxiliary;

    while (!input.empty()) {
        int current = input.top();
        input.pop();

        // While auxiliary is non-empty and its top is smaller than current,
        // pop it back to input to maintain the correct order.
        while (!auxiliary.empty() && current > auxiliary.top()) {
            input.push(auxiliary.top());
            auxiliary.pop();
        }
        auxiliary.push(current);
    }
    return auxiliary;
}
#include <cassert>
#include <stack>
#include <deque>

int main() {
    // Empty stack
    std::stack<int> empty;
    assert(sortStackDescending(empty).empty());

    // Single element
    std::stack<int> single(std::deque<int>{5});
    auto sortedSingle = sortStackDescending(single);
    assert(sortedSingle.size() == 1 && sortedSingle.top() == 5);

    // Already descending (largest on top)
    std::stack<int> desc(std::deque<int>{9, 5, 2});
    auto sortedDesc = sortStackDescending(desc);
    assert(sortedDesc.top() == 9);
    sortedDesc.pop();
    assert(sortedDesc.top() == 5);
    sortedDesc.pop();
    assert(sortedDesc.top() == 2);

    // Unsorted with duplicates and negatives
    std::stack<int> mixed(std::deque<int>{-3, 4, -3, 10, 4, 0});
    auto sortedMixed = sortStackDescending(mixed);
    assert(sortedMixed.top() == 10);
    sortedMixed.pop();
    assert(sortedMixed.top() == 4);
    sortedMixed.pop();
    assert(sortedMixed.top() == 4);
    sortedMixed.pop();
    assert(sortedMixed.top() == 0);
    sortedMixed.pop();
    assert(sortedMixed.top() == -3);
    sortedMixed.pop();
    assert(sortedMixed.top() == -3);

    // Reverse sorted (smallest on top originally)
    std::stack<int> asc(std::deque<int>{1, 2, 3, 4});
    auto sortedAsc = sortStackDescending(asc);
    assert(sortedAsc.top() == 4);
    sortedAsc.pop();
    assert(sortedAsc.top() == 3);
    sortedAsc.pop();
    assert(sortedAsc.top() == 2);
    sortedAsc.pop();
    assert(sortedAsc.top() == 1);

    // Original stack unchanged (passed by value)
    std::stack<int> original(std::deque<int>{3, 1, 2});
    sortStackDescending(original);
    assert(original.top() == 2); // still 2 (top of original)

    return 0;
}
