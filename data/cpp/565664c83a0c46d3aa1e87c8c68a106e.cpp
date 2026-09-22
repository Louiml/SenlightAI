// Write a C++ function `void removeBottomHalf(std::stack<int>& s)` that removes the bottom half of the elements from a non-empty stack while preserving the relative order of the remaining elements. The bottom half is defined as the first `N/2` elements that were pushed (i.e., the elements closest to the bottom of the stack). If the stack size `N` is odd, then `floor(N/2)` elements are removed. For example: if the stack contains (from top to bottom) `4 3 2 1`, then `N=4` and `floor(4/2)=2`, so the bottom two elements `1` and `2` are removed, leaving `4 3` (top to bottom). The function should modify the stack in-place and not return anything. Push order matters: the original stack contains elements pushed in the order given (so the bottom element is the one pushed first).

The challenge is that we cannot directly access elements at the bottom of a stack; we can only access the top. To remove the bottom half, we need to first move all elements into a temporary container that preserves order, then identify which elements belong to the bottom half, and finally rebuild the stack with only the top half. The approach is:  
1. Get the current size `N` of the stack.  
2. Determine `toRemove = N / 2` (integer division).  
3. Pop all elements from the stack into a vector, pushing them back into the vector. Since popping from the stack visits elements from top to bottom, the vector will have the top element first and the bottom element last.  
4. Now, the bottom half are the last `toRemove` elements of that vector (since the last element in the vector is the bottom of the original stack).  
5. Keep only the first `N - toRemove` elements of the vector (those that were originally in the top half).  
6. Clear the original stack (it should already be empty after step 3, but just in case).  
7. Push the kept elements back onto the stack in reverse order of the vector, because when we push into a stack, the last pushed becomes the top. So we iterate the vector from the last kept element down to the first, pushing each onto the stack. This restores the original top-to-bottom order for the remaining elements.  
Edge cases:  
- If `N` is 0 (empty stack) but the task says non-empty; still, we can handle it safely by returning.  
- If `N` is 1, `toRemove = 0`, nothing is removed, the stack remains unchanged.  
- The function modifies in-place; no memory allocation issues beyond a temporary vector.  
Time complexity is O(N) because we pop and push each element at most twice. Space complexity is O(N) for the temporary vector.

#include <stack>
#include <vector>

// Remove the bottom half of elements from a non-empty stack, preserving order of the top half.
void removeBottomHalf(std::stack<int>& s) {
    if (s.empty()) {
        return;
    }

    const int n = static_cast<int>(s.size());
    const int toRemove = n / 2;

    // Move all elements from stack to vector (top of stack becomes first in vector).
    std::vector<int> temp;
    temp.reserve(n);
    while (!s.empty()) {
        temp.push_back(s.top());
        s.pop();
    }

    // Keep only the first (n - toRemove) elements (the original top half).
    // The kept elements are in temp[0] ... temp[n - toRemove - 1].
    // We must push them back in reverse order so that the stack order is correct.
    for (int i = n - toRemove - 1; i >= 0; --i) {
        s.push(temp[i]);
    }
    // Note: The stack is now cleared and rebuilt with the top half.
}

#include <cassert>
#include <stack>
#include <vector>

void removeBottomHalf(std::stack<int>& s); // Declaration from solution

static std::vector<int> stackToVector(const std::stack<int>& s) {
    std::stack<int> copy = s;
    std::vector<int> vec;
    while (!copy.empty()) {
        vec.push_back(copy.top());
        copy.pop();
    }
    // vec is from top to bottom; reverse to get bottom-to-top order for assertion clarity
    std::reverse(vec.begin(), vec.end());
    return vec;
}

int main() {
    // Test 1: Even size, remove 2 out of 4
    std::stack<int> s1;
    s1.push(1); s1.push(2); s1.push(3); s1.push(4); // bottom=1, top=4
    removeBottomHalf(s1);
    std::vector<int> v1 = stackToVector(s1);
    assert((v1 == std::vector<int>{3, 4}));

    // Test 2: Odd size, remove floor(5/2)=2 out of 5
    std::stack<int> s2;
    s2.push(10); s2.push(20); s2.push(30); s2.push(40); s2.push(50); // bottom=10, top=50
    removeBottomHalf(s2);
    std::vector<int> v2 = stackToVector(s2);
    assert((v2 == std::vector<int>{30, 40, 50}));

    // Test 3: Single element, remove 0
    std::stack<int> s3;
    s3.push(7);
    removeBottomHalf(s3);
    std::vector<int> v3 = stackToVector(s3);
    assert((v3 == std::vector<int>{7}));

    // Test 4: Two elements, remove 1
    std::stack<int> s4;
    s4.push(5); s4.push(6); // bottom=5, top=6
    removeBottomHalf(s4);
    std::vector<int> v4 = stackToVector(s4);
    assert((v4 == std::vector<int>{6}));

    // Test 5: Large number of elements, even (10) remove 5
    std::stack<int> s5;
    for (int i = 1; i <= 10; ++i) s5.push(i);
    removeBottomHalf(s5);
    std::vector<int> v5 = stackToVector(s5);
    assert((v5 == std::vector<int>{6, 7, 8, 9, 10}));

    // Test 6: Odd size (9) remove 4, keep 5
    std::stack<int> s6;
    for (int i = 1; i <= 9; ++i) s6.push(i);
    removeBottomHalf(s6);
    std::vector<int> v6 = stackToVector(s6);
    assert((v6 == std::vector<int>{5, 6, 7, 8, 9}));

    return 0;
}
