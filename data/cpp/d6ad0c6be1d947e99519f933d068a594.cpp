Write a C++ function `int findMaxAfterPop(std::vector<int>& nums, int k)` that simulates the behavior of a fixed-capacity stack modeled after the provided `Stack` class. The function should push each element from the input vector `nums` onto a stack with capacity `k` (i.e., the maximum number of elements it can hold). If a push operation would exceed the capacity (stack overflow), the function must ignore that value (do not push it) and instead record `-1` as the result for that step. After every push attempt (whether successful or ignored), if the stack is non-empty, the function returns the top element of the stack (as in `peek()`); if the stack is empty, it returns `-1`. However, if a successful push causes the stack to become full exactly at capacity, the function must immediately pop the top element (as if simulating a "overflow pop" behavior) before returning the result for that step. The function should return the last result obtained after processing all elements. Edge cases: if `k <= 0`, the stack cannot hold any elements, so every operation returns `-1`. If the vector is empty, return `-1`. The function must not use any standard library stack/queue, only manual array-based logic or a `std::vector` as the underlying storage.

#include <cassert>
#include <vector>

int findMaxAfterPop(std::vector<int>& nums, int k); // declaration

int main() {
    // Basic case: capacity 2, push 3 elements -> overflow on third, result -1
    std::vector<int> v1 = {1,2,3};
    assert(findMaxAfterPop(v1, 2) == -1);

    // Capacity 1: push 5, stack becomes full, pop -> stack empty, result -1
    std::vector<int> v2 = {5};
    assert(findMaxAfterPop(v2, 1) == -1);

    // Capacity 3, push four elements: last overflow -> -1
    std::vector<int> v3 = {10,20,30,40};
    assert(findMaxAfterPop(v3, 3) == -1);

    // Capacity 4, push exactly 4: each push fills and pops, after each the top is previous element
    // Process: push 1 -> full, pop -> empty -> result -1
    // push 2 -> result 2 (since top=0 after push, not full? Wait capacity 4: push first element top=0 not full, result 1? Actually let's simulate: 
    // Initially top=-1. push 1 -> top=0, not full (capacity 4), result=1
    // push 2 -> top=1, not full, result=2
    // push 3 -> top=2, result=3
    // push 4 -> top=3, becomes full, pop -> top=2, result=3 (the new top after pop)
    // So final result should be 3
    std::vector<int> v4 = {1,2,3,4};
    assert(findMaxAfterPop(v4, 4) == 3);

    // Empty vector
    std::vector<int> v5;
    assert(findMaxAfterPop(v5, 3) == -1);

    // Zero capacity
    std::vector<int> v6 = {1,2};
    assert(findMaxAfterPop(v6, 0) == -1);

    // Negative capacity
    std::vector<int> v7 = {1};
    assert(findMaxAfterPop(v7, -2) == -1);

    // Negative values: capacity 2, push -5, -10, -3
    // push -5 -> top=0 result=-5
    // push -10 -> top=1 becomes full -> pop -> top=0 result=-5
    // push -3 -> top=1 becomes full -> pop -> top=0 result=-5
    // final result -5
    std::vector<int> v8 = {-5, -10, -3};
    assert(findMaxAfterPop(v8, 2) == -5);

    // Capacity 2, push two elements: after second becomes full, pop, result is first element
    std::vector<int> v9 = {42, 7};
    assert(findMaxAfterPop(v9, 2) == 42);
}

#include <vector>
#include <cstddef>

// Simulates push/pop behavior on a fixed-capacity stack.
// Returns the last result after processing all elements.
// On overflow, returns -1 for that step; after a successful push that fills the stack, pops before returning.
int findMaxAfterPop(std::vector<int>& nums, int k) {
    if (k <= 0 || nums.empty()) {
        return -1;
    }

    std::vector<int> storage(static_cast<std::size_t>(k));
    int top = -1;
    int lastResult = -1;

    for (int x : nums) {
        if (top == k - 1) { // stack full -> overflow
            lastResult = -1; // ignore value, record -1
        } else {
            storage[++top] = x;
            if (top == k - 1) { // stack became full after successful push
                top--; // pop top element
            }
            lastResult = (top >= 0) ? storage[top] : -1;
        }
    }

    return lastResult;
}

// The solution requires implementing a manual stack with a fixed capacity `k`. Maintain an integer `top` index and a `std::vector<int> storage` of size `k` (or simply a `std::vector<int>` that we resize dynamically). For each element in `nums`: if `top == capacity - 1` then we have overflow – we ignore the value and set the current result to `-1` (or if the stack is empty at that moment, return `-1`; but since overflow implies the stack is full, it is non-empty, so we should instead return the current top? The problem states: "After every push attempt (whether successful or ignored), if the stack is non-empty, the function returns the top element; if empty, returns -1." So for overflow, the stack remains unchanged (full and non-empty), so the result should be the current top, not -1. But wait careful reading: "If a push operation would exceed the capacity (stack overflow), the function must ignore that value (do not push it) and instead record `-1` as the result for that step." That contradicts the previous sentence. The intended behavior appears to be: on overflow, return `-1` immediately for that step. For successful push, if the stack becomes full (i.e., `top == capacity-1` after pushing), then pop the top element before returning the result, and the result after pop is the new top (or `-1` if the stack becomes empty). So the algorithm: Initialize `top = -1`. For each `x` in `nums`: if `top == capacity-1`, then overflow ignore, and set result = -1. Else push `x` by doing `storage[++top] = x`. Then if `top == capacity-1` (stack full after successful push), then pop by doing `top--` (since we just pushed to full, popping leaves `top` at `capacity-2`, non-empty unless capacity==1). Then if `top == -1` result = -1, else result = storage[top]. At the end of all iterations, return the last computed result. Edge cases: capacity <= 0: always result = -1. Empty vector: return -1. Time complexity O(n), space O(k) for storage. Need to handle negative values as numbers, not special.
