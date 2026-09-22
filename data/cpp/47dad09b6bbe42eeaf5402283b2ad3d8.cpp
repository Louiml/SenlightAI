/*
Write a C++ function `std::vector<int> nextGreaterFrequency(const std::vector<int>& arr)` that, for each element in the input array, returns the next element to its right whose frequency of occurrence in the entire original array is strictly greater than the frequency of the current element. If no such element exists to the right, return `-1` for that position. The frequency of each value is counted over the whole input array. For example, given `{1, 1, 2, 3, 4, 2, 1}`, frequencies are `1 → 3`, `2 → 2`, `3 → 1`, `4 → 1`. The result should be `{2, -1, 3, 4, -1, -1, -1}` because: for index 0 (value 1, freq 3), the next element to the right with freq > 3 is none, so -1; for index 1 (value 1, freq 3), none → -1; for index 2 (value 2, freq 2), the next element with freq > 2 is value 3 at index 3 (freq 1? wait, freq of 3 is 1, not >2; actually check conditions carefully). Ensure your algorithm handles duplicates, all elements with same frequency, and arbitrary element values (non-negative integers). The function must not modify the input.
*/
#include <vector>
#include <stack>
#include <unordered_map>

// For each element, find the next element to its right whose frequency in the whole array is strictly greater.
// If none exists, use -1.
std::vector<int> nextGreaterFrequency(const std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) return {};

    // Count frequency of each value.
    std::unordered_map<int, int> freq;
    for (int x : arr) {
        ++freq[x];
    }

    std::vector<int> result(n, -1);
    std::stack<int> st;  // stores indices

    st.push(0);
    for (int i = 1; i < n; ++i) {
        // If current has lower or equal frequency, just push (no greater found yet)
        if (freq[arr[i]] <= freq[arr[st.top()]]) {
            st.push(i);
        } else {
            // Pop all indices with frequency strictly less than current
            while (!st.empty() && freq[arr[i]] > freq[arr[st.top()]]) {
                result[st.top()] = arr[i];
                st.pop();
            }
            st.push(i);
        }
    }

    // Remaining indices have no greater-frequency element to the right
    // result already initialized to -1, so nothing extra needed.

    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Example from task
    std::vector<int> a1 = {1, 1, 2, 3, 4, 2, 1};
    std::vector<int> r1 = nextGreaterFrequency(a1);
    // Frequencies: 1:3, 2:2, 3:1, 4:1
    // For each element, find next with freq strictly greater:
    // index0 (1, f=3) -> none => -1
    // index1 (1, f=3) -> none => -1
    // index2 (2, f=2) -> next element with freq >2? index3=3 (f=1) no, index4=4 (f=1) no, index5=2 (f=2) not greater, index6=1 (f=3) yes at index6 => 1
    // Wait check again: For index2, elements to right: 3 (f=1), 4 (f=1), 2 (f=2), 1 (f=3). The first with f > 2 is 1 at index6. So result[2]=1
    // index3 (3, f=1) -> next with f>1? index4=4 (f=1) no, index5=2 (f=2) yes => 2
    // index4 (4, f=1) -> next with f>1? index5=2 (f=2) yes => 2
    // index5 (2, f=2) -> next with f>2? index6=1 (f=3) yes => 1
    // index6 (1, f=3) -> none => -1
    // So expected: {-1, -1, 1, 2, 2, 1, -1}
    std::vector<int> e1 = {-1, -1, 1, 2, 2, 1, -1};
    assert(r1 == e1);

    // All same values -> all -1 (frequencies equal, no strictly greater)
    std::vector<int> a2 = {5, 5, 5};
    std::vector<int> r2 = nextGreaterFrequency(a2);
    assert(r2 == std::vector<int>({-1, -1, -1}));

    // Single element
    std::vector<int> a3 = {42};
    assert(nextGreaterFrequency(a3) == std::vector<int>({-1}));

    // Empty
    std::vector<int> a4 = {};
    assert(nextGreaterFrequency(a4).empty());

    // Strictly increasing frequencies pattern
    // arr = {1, 2, 1, 2} frequencies: 1:2, 2:2 -> all equal -> all -1
    std::vector<int> a5 = {1, 2, 1, 2};
    assert(nextGreaterFrequency(a5) == std::vector<int>({-1, -1, -1, -1}));

    // Mixed: arr = {3, 1, 3, 1, 2} frequencies: 3:2, 1:2, 2:1
    // index0 (3,f2) -> next with f>2? none -> -1
    // index1 (1,f2) -> next with f>2? none -> -1
    // index2 (3,f2) -> next: 1(f2) no, 2(f1) no -> -1
    // index3 (1,f2) -> next: 2(f1) no -> -1
    // index4 (2,f1) -> none -> -1
    // all -1
    std::vector<int> a6 = {3, 1, 3, 1, 2};
    assert(nextGreaterFrequency(a6) == std::vector<int>({-1, -1, -1, -1, -1}));

    // Test where answer exists
    // arr = {1, 2, 2, 1, 3} frequencies: 1:2, 2:2, 3:1
    // index0 (1,f2) -> next with f>2? none -> -1
    // index1 (2,f2) -> next: 2(f2) no, 1(f2) no, 3(f1) no -> -1
    // index2 (2,f2) -> next: 1(f2) no, 3(f1) no -> -1
    // index3 (1,f2) -> next: 3(f1) no -> -1
    // index4 (3,f1) -> none -> -1, all -1
    std::vector<int> a7 = {1, 2, 2, 1, 3};
    assert(nextGreaterFrequency(a7) == std::vector<int>({-1, -1, -1, -1, -1}));

    // Better test: arr = {1, 2, 2, 3, 1} frequencies: 1:2, 2:2, 3:1
    // index0 (1,f2) -> next: 2(f2) no, 2(f2) no, 3(f1) no, 1(f2) no -> -1
    // index1 (2,f2) -> next: 2(f2) no, 3(f1) no, 1(f2) no -> -1
    // index2 (2,f2) -> next: 3(f1) no, 1(f2) no -> -1
    // index3 (3,f1) -> next: 1(f2) yes -> 1
    // index4 (1,f2) -> none -> -1
    std::vector<int> a8 = {1, 2, 2, 3, 1};
    assert(nextGreaterFrequency(a8) == std::vector<int>({-1, -1, -1, 1, -1}));

    std::cout << "All tests passed." << std::endl;
    return 0;
}
// The problem is a variation of "Next Greater Element" but comparing frequencies instead of values. First, compute the frequency of each distinct integer in the array using a hash map (or vector if elements are bounded). Then process the array from left to right using a monotonic stack that stores indices. The stack maintains indices whose "next greater frequency" has not yet been found, in decreasing order of their frequencies (since we pop when a larger frequency appears). For each index `i` from 1 to n-1, compare `freq[arr[i]]` with `freq[arr[st.top()]]`. If the current frequency is less than the top's frequency, push `i` (since the top might still find a greater frequency later). If equal, push `i` as well (strictly greater is required). If greater, then while the stack is non-empty and `freq[arr[i]] > freq[arr[st.top()]]`, set the result for `st.top()` to `arr[i]` and pop; after that, push `i`. After the loop, any remaining indices in the stack have no element to the right with a strictly greater frequency, so set their result to -1. Edge cases: empty array returns empty vector; array of size 1 returns {-1}; all elements same value → all -1; values with same frequency but different positions; large frequencies; values not starting from 0 or negative (if using vector, need to offset or use unordered_map). Time complexity O(n) because each index is pushed and popped at most once. Space O(n) for the stack and result vector, plus O(unique values) for frequency storage.
