// Write a C++ function `removeSmallerThanNext` that takes a vector of integers `v` and a positive integer `k`, and returns a new vector obtained by removing exactly `k` elements from the original vector, where each removed element must be strictly smaller than the element immediately following it in the *current* vector at the time of removal. The removals are performed in a single left-to-right pass: you examine the vector from index 0 upward, and whenever you encounter an element that is less than the next element, you remove it (decrementing `k`), and then continue checking from that same position (now occupied by the next element). You may assume the input is such that exactly `k` such removable elements exist (i.e., no need to handle insufficient removals). Return the resulting vector after all removals. The original vector must not be modified. The function signature should be `std::vector<int> removeSmallerThanNext(const std::vector<int>& v, int k)`. The relative order of the remaining elements must be preserved.

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above (not repeated here for brevity).

int main() {
    // Basic example from the prompt
    std::vector<int> a = {20, 10, 25, 30, 40};
    std::vector<int> r1 = removeSmallerThanNext(a, 2);
    assert((r1 == std::vector<int>{25, 30, 40}));

    // All elements in increasing order: removals happen at the end
    std::vector<int> b = {1, 2, 3, 4, 5};
    std::vector<int> r2 = removeSmallerThanNext(b, 2);
    assert((r2 == std::vector<int>{1, 2, 3}));

    // Decreasing order: no element is smaller than its next, so remove from end
    std::vector<int> c = {5, 4, 3, 2, 1};
    std::vector<int> r3 = removeSmallerThanNext(c, 3);
    assert((r3 == std::vector<int>{5, 4}));

    // Equal elements: none smaller, remove from end
    std::vector<int> d = {7, 7, 7, 7};
    std::vector<int> r4 = removeSmallerThanNext(d, 1);
    assert((r4 == std::vector<int>{7, 7, 7}));

    // Mixed pattern with multiple removals in one pass
    std::vector<int> e = {3, 1, 2, 4, 0, 5};
    // Step: i=0: stack [3]; i=1: 1<3? no, but 3 is not <1, so push 1 -> [3,1]; i=2: 1<2 -> pop 1 (count=1), then 3<2? no, push 2 -> [3,2]; i=3: 2<4 -> pop 2 (count=2), 3<4 -> pop 3 (count=3), push 4 -> [4]; i=4: 4<0? no, push 0 -> [4,0]; i=5: 0<5 -> pop 0 (count=4), 4<5 -> pop 4 (count=5), push 5 -> [5]; then remaining elements after index 5: none. Result [5]. But test with k=2: should stop after popping 2 (at i=3) then push 4 and continue? Actually let's compute with k=2: loop until count==2. At i=2: top=1<2, pop 1 (count=1), top=3<2? no, push 2. At i=3: top=2<4, pop 2 (count=2) -> stop. push 4? Actually the loop condition checks count!=k at the start of each iteration, so after popping at i=3, count==2, we exit the for loop without pushing 4. Then we append remaining from index 3 onward: [4,0,5]. So result: stack currently [3,4]? Wait let's do carefully: 
    // Actually correct simulation for k=2:
    // i=0: st empty push 20? No, using e = {3,1,2,4,0,5}
    // i=0: push 3 -> st [3]
    // i=1: top=3<1? no, push 1 -> st [3,1]
    // i=2: top=1<2? yes -> pop 1 (count=1), top=3<2? no, push 2 -> st [3,2]
    // i=3: top=2<4? yes -> pop 2 (count=2) => count==k, break loop without pushing 4.
    // After loop: count==k, so no extra removals. Stack has [3]. Append remaining from i=3: v[3]=4, v[4]=0, v[5]=5 => result = [3,4,0,5].
    std::vector<int> r5 = removeSmallerThanNext(e, 2);
    assert((r5 == std::vector<int>{3, 4, 0, 5}));

    // Single element
    std::vector<int> f = {42};
    std::vector<int> r6 = removeSmallerThanNext(f, 0);
    assert((r6 == std::vector<int>{42}));

    // k = 0 returns copy
    std::vector<int> g = {9, 8, 7};
    std::vector<int> r7 = removeSmallerThanNext(g, 0);
    assert((r7 == g));

    // Large k removes all but maybe none? Here k=length, should return empty vector
    std::vector<int> h = {1, 3, 2};
    // Process: i=0 push 1; i=1: top=1<3 pop (count=1) push 3; i=2: top=3<2? no push 2; after loop count=1<k=3, remove from stack: pop 2 (count=2), pop 3 (count=3) => stack empty. result empty.
    std::vector<int> r8 = removeSmallerThanNext(h, 3);
    assert((r8.empty()));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <vector>
#include <stack>
#include <algorithm>

// Remove exactly k elements where each removed element is strictly smaller
// than the element immediately after it at the time of removal.
// The input vector is not modified. Returns the resulting vector.
std::vector<int> removeSmallerThanNext(const std::vector<int>& v, int k) {
    std::vector<int> result;
    std::stack<int> st;
    int count = 0;
    int i = 0;
    int n = static_cast<int>(v.size());

    // First pass: process all elements, removing as we go
    for (; i < n && count != k; ++i) {
        int current = v[i];
        // While the top of the stack is smaller than the current element,
        // we can remove it because it is smaller than its next element.
        while (!st.empty() && st.top() < current && count != k) {
            st.pop();
            ++count;
        }
        st.push(current);
    }

    // If we still need to remove more elements, remove from the end
    // of the current stack (the largest elements, safe to remove).
    while (count < k && !st.empty()) {
        st.pop();
        ++count;
    }

    // Transfer stack contents to result (reverse order)
    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }
    std::reverse(result.begin(), result.end());

    // Append any unconsumed remaining elements from the original vector
    for (; i < n; ++i) {
        result.push_back(v[i]);
    }

    return result;
}

// The problem is a classic "remove smaller-than-next" operation, which can be efficiently solved using a monotonic stack. The key insight is that when processing the array left to right, we want to maintain a stack of elements that are guaranteed to be non-decreasing (from bottom to top). Whenever a new element `x` arrives, we compare it with the top of the stack. If the top is strictly smaller than `x`, then that top element (and possibly several others) are "candidates" for removal because they are smaller than the next element (which is `x`). We pop and remove them one by one, decrementing `k` each time, until either we have removed `k` elements or the stack becomes empty or the top is no longer smaller than `x`. Then we push `x` onto the stack. After the loop, if we still need to remove more elements (i.e., `k > 0`), we remove them from the end of the stack (since the remaining suffix of the array is non-decreasing, the rightmost elements are the largest and safe to remove). Finally, the stack contains the remaining elements in correct order (after reversing because stack gives reverse order). Edge cases: empty vector, `k=0` (return copy), all elements equal (no removals needed), already increasing sequence (removals happen from the end after the loop). Time complexity is O(n) because each element is pushed once and popped at most once, plus reversing takes O(n). Space complexity is O(n) for the stack and result.
