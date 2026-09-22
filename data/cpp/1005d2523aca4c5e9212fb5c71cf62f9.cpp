// Write a C++ function `std::vector<int> spanOfSmaller(const std::vector<int>& arr)` that, for each position `i` in the input array, computes the "span" defined as the number of contiguous elements to the left of `i` (including `i` itself) that are greater than or equal to `arr[i]`. More precisely, find the nearest index to the left of `i` where the value is strictly less than `arr[i]` (or `-1` if none exists), then the span is `i - thatIndex`. For example, for `arr = {3, 1, 2, 2}`, the output should be `{1, 1, 2, 3}` because at index 3 (value 2), the nearest smaller to the left is at index 1 (value 1), so span = 3 - 1 = 2? Wait: recheck – actually index 3 is the second 2, nearest strictly smaller is at index 1 (value 1), so span = 3 - 1 = 2? But I said 3? Let's revise: The standard definition: for each i, find the nearest j < i with arr[j] < arr[i]; if none, j = -1; span = i - j. For arr = {3,1,2,2}: i=0 (3): no smaller -> -1 -> span = 0-(-1)=1. i=1 (1): no smaller -> -1 -> span=2? That's wrong because 1 is the smallest so far but we need strict less, so none -> -1 -> span=1-(-1)=2? Actually i - (-1) = i+1 = 2, so span=2? But typical stock span problem uses greater or equal? Let's clarify: The task is to return an array where each element is the number of consecutive previous elements that are **greater than or equal** to the current element, including itself, until a smaller element is found. For arr={3,1,2,2}: i=0: {3} -> 1. i=1: {1} -> 1 (no previous >=1 because 3>1 but we stop at first smaller? Actually we want contiguous previous elements that are >= current, so for i=1, previous element 3 is >1, so it counts? But we stop when we find an element that is **strictly smaller**? Let's use the classic "stock span" definition: span = number of consecutive previous days with price <= current? Different. To avoid ambiguity, the task will clearly state: For each index i, the span is the length of the longest contiguous subarray ending at i in which all elements are **greater than or equal** to arr[i]. That means we count backward from i while arr[j] >= arr[i]. If we hit an element smaller than arr[i], we stop. So for arr={3,1,2,2}: i=0: just {3} ->1. i=1: {1} ->1 because previous 3>1 but we stop immediately? Actually contiguous subarray ending at i: we can include j=i-1 if arr[j] >= arr[i]; here 3>=1 so include, then j=i-2 out of bounds, so span=2? But if we include 3, then the subarray {3,1} has all elements >= arr[1]=1? 3>=1 yes, so span should be 2. But typical "next smaller element to left" span is i - index_of_previous_smaller. That gives for i=1, previous smaller is none, so span=1-(-1)=2. So consistent. So let's define: For each i, find the nearest index j < i such that arr[j] < arr[i]; if none, j = -1; then span = i - j. That matches the "contiguous subarray ending at i with all elements >= arr[i]". For arr={3,1,2,2}: 
// i=0 (3): j=-1 -> span=0-(-1)=1
// i=1 (1): j=-1 -> span=1-(-1)=2 (check: {3,1} all >=1? 3>=1 yes, so 2)
// i=2 (2): nearest smaller to left is at j=1 (1<2) -> span=2-1=1? Wait contiguous subarray ending at 2 with all >=2: {2} only? Because previous element 1<2, so stop, so span=1. Yes.
// i=3 (2): nearest smaller to left is at j=1 (1<2) -> span=3-1=2 (subarray {2,2} both >=2, but we can include index2? Actually subarray ending at 3: indices 1,2,3? start from i=3, go left while arr[j]>=arr[3]=2: j=2:2>=2 include, j=1:1<2 stop, so span=3-1=2. So output {1,2,1,2}? Wait compute: i=0:1, i=1:2, i=2:1, i=3:2 => {1,2,1,2}. That seems right. So write a function that returns that.

#include <cassert>
#include <vector>
#include <iostream>

// The function is defined above (for test, include the implementation or assume it's in scope)

int main() {
    // Basic case with duplicates and a smaller element in between
    std::vector<int> a1 = {3, 1, 2, 2};
    std::vector<int> r1 = spanOfSmaller(a1);
    assert(r1 == std::vector<int>({1, 2, 1, 2}));

    // Strictly increasing: each element has no smaller to left, so span = i+1
    std::vector<int> a2 = {1, 2, 3, 4};
    std::vector<int> r2 = spanOfSmaller(a2);
    assert(r2 == std::vector<int>({1, 2, 3, 4}));

    // Strictly decreasing: each element's nearest smaller is the previous one
    std::vector<int> a3 = {5, 4, 3, 2, 1};
    std::vector<int> r3 = spanOfSmaller(a3);
    assert(r3 == std::vector<int>({1, 1, 1, 1, 1}));

    // All equal: no strictly smaller anywhere, so span = i+1
    std::vector<int> a4 = {7, 7, 7};
    std::vector<int> r4 = spanOfSmaller(a4);
    assert(r4 == std::vector<int>({1, 2, 3}));

    // Single element
    std::vector<int> a5 = {42};
    std::vector<int> r5 = spanOfSmaller(a5);
    assert(r5 == std::vector<int>({1}));

    // Empty input yields empty output
    std::vector<int> a6;
    std::vector<int> r6 = spanOfSmaller(a6);
    assert(r6.empty());

    // Mixed negatives and zero
    std::vector<int> a7 = {-2, -1, -3, 0, -1};
    std::vector<int> r7 = spanOfSmaller(a7);
    // Manual: 
    // i=0: -2 -> span=1
    // i=1: -1 -> no smaller left (since -2 < -1? -2 is NOT smaller than -1? Actually -2 < -1, so nearest smaller is index0) -> span=1-0=1? But wait check contiguous >= condition: for -1, previous -2 >= -1? -2 is NOT >= -1, so stop, span=1. Yes.
    // i=2: -3 -> no smaller -> span=3
    // i=3: 0 -> nearest smaller is at index2 (-3) -> span=3-2=1? Actually accessible: previous -3 <0, stop, span=1.
    // i=4: -1 -> previous 0 >= -1? yes, include; previous -3 >= -1? no, stop; also index2 -3 < -1, so nearest smaller is index2? But we have 0 at index3 which is >= -1, but we stop at the first smaller from left going backwards? That would be index2. So span=4-2=2? Let's compute with stack: 
    // i=0 push 0
    // i=1: arr[0]=-2 >= -1? -2 >= -1 false, so no pop. stack top 0 -> span=1-0=1. push 1.
    // i=2: arr[1]=-1 >= -3? true pop. arr[0]=-2 >= -3? true pop. stack empty -> span=3. push 2.
    // i=3: arr[2]=-3 >= 0? false. stack top 2 -> span=3-2=1. push 3.
    // i=4: arr[3]=0 >= -1? true pop. arr[2]=-3 >= -1? false. stack top 2 -> span=4-2=2. push 4.
    // So r7 = {1,1,3,1,2}. Check that.
    assert(r7 == std::vector<int>({1, 1, 3, 1, 2}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <vector>
#include <stack>

// Compute for each position i the number of contiguous previous elements
// (including i) that are all >= arr[i]. Equivalent to the distance to the
// nearest strictly smaller element on the left, or i+1 if none exists.
std::vector<int> spanOfSmaller(const std::vector<int>& arr) {
    std::vector<int> span(arr.size());
    std::stack<int> st; // indices with strictly increasing values from bottom to top

    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        // Pop elements that are not strictly smaller (i.e., >= current)
        while (!st.empty() && arr[st.top()] >= arr[i]) {
            st.pop();
        }
        // If stack is empty, no smaller element to the left, span = i+1
        // Else nearest smaller is at st.top(), so span = i - st.top()
        span[i] = (st.empty() ? i + 1 : i - st.top());
        st.push(i);
    }
    return span;
}

// We use a monotonic stack that maintains indices of elements in increasing order of their values (strictly increasing). Iterate left to right. For each index i, while the stack is not empty and arr[stack.top()] >= arr[i], pop. The nearest index with a strictly smaller value to the left is then the top of the stack (if it exists) or -1 if empty. The span is `i - (stack.empty() ? -1 : stack.top())`. Then push i. This works because the stack always stores indices such that the values at those indices are strictly increasing from bottom to top. Popping removes all indices whose values are >= current, because they cannot be the nearest smaller for any future element. Edge cases: an empty input array returns an empty vector (or we can specify input has at least one element but for robustness return empty). When all elements are equal, each span equals the current index plus one, because no strictly smaller exists. Time complexity: O(n) since each index is pushed and popped at most once. Space complexity: O(n) for the stack and output. The algorithm is a classic monotonic stack application.
