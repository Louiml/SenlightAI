Write a C++ function named `nextSmallerElement` that takes a vector of integers `arr` and its size `n` (where `n >= 0`) and returns a vector of integers of the same size. For each index `i` in the input array, the returned vector should contain the next smaller element to the right of `arr[i]`, meaning the first element `arr[j]` with `j > i` such that `arr[j] < arr[i]`. If no such element exists (either because `i` is the last index or all elements to the right are greater than or equal to `arr[i]`), the corresponding result should be `-1`. The function must handle an empty input vector (returning an empty vector) and arrays with duplicate values correctly, and the solution should run in \(O(n)\) time using a stack or deque approach. The function signature must be: `std::vector<int> nextSmallerElement(const std::vector<int>& arr, int n);` where `n` is the size of `arr`, but the function should still work correctly even if `n` is not provided (you may ignore the parameter or use it for validation, but the function must use the vector's actual size for logic if needed).
// The solution processes the array from right to left to find the next smaller element efficiently. For each element, we maintain a monotonic stack (or deque) of candidate "next smaller" values. The stack stores elements in increasing order from bottom to top (i.e., the smallest candidate on top). When processing `arr[i]`, we pop elements from the stack that are **greater than or equal to** `arr[i]`, because any such element cannot be the next smaller for `arr[i]` or for any element to the left of `i` (since `arr[i]` itself is smaller and comes earlier in position). After popping, if the stack is empty, then there is no smaller element to the right, so we set `ans[i] = -1`. Otherwise, the top of the stack is the nearest smaller element to the right. We then push `arr[i]` onto the stack, as it becomes a candidate for elements further left. This works because the stack always contains elements to the right of the current index in increasing order, and the top is the smallest among them. Edge cases: (1) Empty vector: return empty vector. (2) Single element: the only element has no right neighbor, so result is `-1`. (3) Duplicates: since we pop when `st.top() >= arr[i]`, a duplicate value will be popped (because it's not strictly smaller), ensuring we skip equal values and find the first strictly smaller one. Time complexity: \(O(n)\) because each element is pushed onto and popped from the stack at most once. Space complexity: \(O(n)\) for the output vector and the stack, but the stack holds at most `n` elements in the worst case (e.g., strictly decreasing array, where no pops occur). The algorithm is identical to the "next greater element" problem but with the comparison reversed and processing from right to left.
#include <vector>
#include <stack>

// Return a vector where ans[i] is the next element to the right of arr[i]
// that is strictly smaller than arr[i], or -1 if no such element exists.
std::vector<int> nextSmallerElement(const std::vector<int>& arr, int n) {
    if (n == 0) return {};

    std::vector<int> ans(n);
    std::stack<int> st;

    // Process from right to left
    for (int i = n - 1; i >= 0; --i) {
        // Pop elements that are not strictly smaller than arr[i]
        while (!st.empty() && st.top() >= arr[i]) {
            st.pop();
        }

        // If stack is empty, no smaller element exists to the right
        ans[i] = st.empty() ? -1 : st.top();

        // Push current element for future comparisons
        st.push(arr[i]);
    }

    return ans;
}
#include <cassert>
#include <vector>

// Include the solution function here (or link it)
std::vector<int> nextSmallerElement(const std::vector<int>& arr, int n);

int main() {
    // Basic test
    std::vector<int> arr1 = {4, 5, 2, 10, 8};
    std::vector<int> res1 = nextSmallerElement(arr1, arr1.size());
    assert(res1 == std::vector<int>({2, 2, -1, 8, -1}));

    // Strictly increasing array -> all -1 except last
    std::vector<int> arr2 = {1, 2, 3, 4};
    std::vector<int> res2 = nextSmallerElement(arr2, arr2.size());
    assert(res2 == std::vector<int>({-1, -1, -1, -1}));

    // Strictly decreasing array -> immediate next element
    std::vector<int> arr3 = {5, 4, 3, 2, 1};
    std::vector<int> res3 = nextSmallerElement(arr3, arr3.size());
    assert(res3 == std::vector<int>({4, 3, 2, 1, -1}));

    // Duplicates
    std::vector<int> arr4 = {3, 3, 1, 2};
    std::vector<int> res4 = nextSmallerElement(arr4, arr4.size());
    assert(res4 == std::vector<int>({1, 1, -1, -1}));

    // Negative numbers
    std::vector<int> arr5 = {-2, -1, -3, 0};
    std::vector<int> res5 = nextSmallerElement(arr5, arr5.size());
    assert(res5 == std::vector<int>({-3, -3, -1, -1}));

    // Single element
    std::vector<int> arr6 = {7};
    std::vector<int> res6 = nextSmallerElement(arr6, arr6.size());
    assert(res6 == std::vector<int>({-1}));

    // Empty vector
    std::vector<int> arr7;
    std::vector<int> res7 = nextSmallerElement(arr7, 0);
    assert(res7.empty());

    // Large elements and mixed
    std::vector<int> arr8 = {100, 50, 200, 25, 30};
    std::vector<int> res8 = nextSmallerElement(arr8, arr8.size());
    assert(res8 == std::vector<int>({50, 25, 25, -1, -1}));

    return 0;
}
