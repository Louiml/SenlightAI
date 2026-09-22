// Write a C++ function `vector<long long> previousGreaterOrEqualDistance(const vector<long long>& arr)` that, given a non-empty array of integers, returns for each position `i` the **number of elements strictly between** the current element and the **nearest element to the left** that is **greater than or equal** to `arr[i]`. If no such left element exists (i.e., all elements to the left are strictly smaller), return the count as `i + 1` (interpreted as the distance from the "virtual" start index `-1`). Specifically, for each index `i` (0-based), the result is:
// - If there exists an index `j < i` such that `arr[j] >= arr[i]` and for all `k` with `j < k < i`, `arr[k] < arr[i]`, then output `i - j`.
// - Otherwise (no such `j`), output `i + 1`.
//
// The function must handle large arrays (up to 10^5 elements) and values that fit in 64-bit signed integers. Return a vector of `long long` of the same length as the input.
// The problem is a classic **monotonic stack** variation. We maintain a stack of indices such that the values in the array at those indices are strictly decreasing from bottom to top (i.e., we pop while the top value is less than the current value). When we encounter a new element `arr[i]`, we pop all indices from the stack whose values are **strictly less than** `arr[i]` (since those cannot be the nearest greater-or-equal element for `i` or any later element). After popping, if the stack is empty, that means there is no element to the left with value `>= arr[i]`, so the answer is `i + 1`. Otherwise, the top of the stack is the index of the nearest element to the left that is `>= arr[i]`, and the answer is `i - st.top()`. Then we push `i` onto the stack.
//
// **Edge cases:**
// - Duplicate values: Since we pop only when `arr[i] >= arr[st.top()]`? Wait, careful: The original snippet pops while `x[i] >= x[st.top()]`. That means it removes elements that are **strictly smaller** or equal. But if we pop equal elements, then for a duplicate value, the nearest greater-or-equal to the left would be the previous duplicate? Let's test: arr = [5,5]. For i=0, stack empty -> ans=1. Push 0. For i=1, while top has value 5 and `5>=5` true, pop. Stack empty -> ans=2. That gives `i+1` = 2, which is correct because there is no element strictly to the left that is **greater or equal**? Actually there is the element at index 0 which is equal, and it is the nearest. The distance should be `1-0=1`. But the snippet would output 2 because it pops equal. However, the task says "greater than or equal" and "nearest element to the left". Since equal counts as greater or equal, the correct answer for [5,5] should be 1 for the second element. But the original snippet outputs 2. Let's re-read the task: "the number of elements strictly between the current element and the nearest element to the left that is greater than or equal to arr[i]." That would be `i - j`. For duplicates, the nearest greater-or-equal to the left is exactly the previous duplicate (if no larger between). So we should **not** pop elements that are equal. The original snippet pops while `x[i] >= x[st.top()]`, which pops equal elements. That would break the intended semantics. But the task is inspired by the snippet but can be corrected. To align with the wording "greater than or equal", we must pop only while `arr[i] > arr[st.top()]` (strictly greater), so that equal elements remain as valid candidates. The original snippet has `>=`, which would treat equal as not a candidate and effectively require a strictly greater element. The task says "greater than or equal", so we must use `>` for popping. So the solution uses a monotonic **non-increasing** stack (values from bottom to top are non-increasing, i.e., decreasing or equal). We pop only when `arr[i] > arr[st.top()]` (strictly greater), because if current is strictly greater, then the top cannot be a valid greater-or-equal for current (since it's smaller), and also cannot be for any future element that is smaller than current? Actually need careful reasoning.
//
// Let's define correctly: We want for each `i` the nearest index `j` to the left with `arr[j] >= arr[i]`. While iterating left to right, we maintain a stack where values are strictly decreasing? Standard approach for "nearest greater to left": pop while `arr[st.top()] <= arr[i]` (since those are less or equal, they cannot be a greater candidate for current). But here we need **greater or equal**, so we need to pop while `arr[st.top()] < arr[i]` (strictly less). Because if `arr[st.top()] == arr[i]`, then that top is a valid candidate (equal is allowed). So we pop only strictly smaller elements. Then top of stack is the nearest index with value >= current. That matches the wording. So the algorithm: iterate i from 0 to n-1, while stack not empty and `arr[st.top()] < arr[i]` pop. If stack empty ans = i+1 else ans = i - st.top(). Then push i. This yields correct distances. Time O(n) because each index is pushed and popped at most once. Space O(n) for stack and output.
#include <vector>
#include <stack>

// For each index i, find the distance to the nearest element to the left
// that is greater than or equal to arr[i]. If none exists, return i+1.
std::vector<long long> previousGreaterOrEqualDistance(const std::vector<long long>& arr) {
    const std::size_t n = arr.size();
    std::vector<long long> result(n);
    std::stack<std::size_t> st; // stores indices with weakly decreasing values (bottom to top)

    for (std::size_t i = 0; i < n; ++i) {
        // Pop all indices whose value is strictly less than current value
        while (!st.empty() && arr[st.top()] < arr[i]) {
            st.pop();
        }
        if (st.empty()) {
            result[i] = static_cast<long long>(i) + 1;
        } else {
            result[i] = static_cast<long long>(i) - static_cast<long long>(st.top());
        }
        st.push(i);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <stack>

// (same solution function as above, place here for clarity)

int main() {
    // Single element
    assert((previousGreaterOrEqualDistance({5}) == std::vector<long long>{1}));

    // Strictly decreasing: no greater-or-equal to left, so 1,2,3,4
    assert((previousGreaterOrEqualDistance({5,4,3,2}) == std::vector<long long>{1,2,3,4}));

    // Strictly increasing: each element sees the immediate previous one (equal? no, previous is smaller, but rightmost? Actually for increasing, each element sees none to left that is >=, because all left are smaller. So all answers = i+1)
    assert((previousGreaterOrEqualDistance({1,2,3,4}) == std::vector<long long>{1,2,3,4}));

    // Duplicates: each duplicate finds the previous equal as nearest
    assert((previousGreaterOrEqualDistance({5,5,5}) == std::vector<long long>{1,1,1}));

    // Mixed case
    assert((previousGreaterOrEqualDistance({2,1,2,3,2}) == std::vector<long long>{1,2,1,1,2}));
    // Explanation:
    // i=0: value 2, no left -> 1
    // i=1: value 1, left 2 >= 1 at index 0 -> dist=1-0=1? Wait, but our output says 2? Let's recalc: For i=1, arr=1, nearest >=1 to left is index 0 (value 2), dist=1-0=1, not 2. My test is wrong. Let's correct.
    // Let's compute correctly: arr = [2,1,2,3,2]
    // i=0: no left -> 1
    // i=1: 1, left 2 at 0 -> dist=1-0=1
    // i=2: 2, left elements: index1=1 (<2), index0=2 (>=2) but index1 is between? Actually nearest is index0? Wait, index0 is left of index1, but index1 is between i=2 and index0? The nearest to left is index1? But index1 value 1 is <2, so it's not greater-or-equal. The nearest index with value >=2 is index0 (value 2). Between them? There is index1 (value1) which is strictly between, but distance is i - j = 2-0 = 2. So answer 2.
    // i=3: 3, no left >=3 -> 4? Actually left max is 2, so none -> i+1=4.
    // i=4: 2, left: index3=3 >=2 (nearest) -> dist=4-3=1.
    // So output should be {1,1,2,4,1}.

    assert((previousGreaterOrEqualDistance({2,1,2,3,2}) == std::vector<long long>{1,1,2,4,1}));

    // Large values
    assert((previousGreaterOrEqualDistance({1000000000000LL, 1LL, 1000000000000LL}) == std::vector<long long>{1,2,1}));

    return 0;
}
