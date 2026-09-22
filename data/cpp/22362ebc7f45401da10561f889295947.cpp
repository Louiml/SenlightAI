/*
Write a C++ function `bool isPopOrder(const std::vector<int>& pushV, const std::vector<int>& popV)` that determines whether the sequence `popV` is a valid pop order for the given push sequence `pushV` when using a single stack. The function must simulate the standard stack push/pop operations: elements are pushed onto the stack in the order they appear in `pushV`, and at any point (including between pushes) you may pop elements from the top of the stack. The sequence `popV` is valid if there exists a sequence of push/pop operations that yields `popV` as the exact order in which elements are popped. Both vectors are non-empty and contain distinct integers, but not necessarily sorted. Your implementation must be `const`-correct and efficient, handling any mismatch in lengths (if lengths differ, return `false` immediately). Return `true` if `popV` is a valid pop order, `false` otherwise.
*/
#include <vector>
#include <stack>

// Determines whether popV is a valid pop sequence for pushV using a single stack.
bool isPopOrder(const std::vector<int>& pushV, const std::vector<int>& popV) {
    if (pushV.size() != popV.size()) {
        return false;
    }

    std::stack<int> st;
    std::size_t popIndex = 0;

    for (std::size_t i = 0; i < pushV.size(); ++i) {
        st.push(pushV[i]);

        while (!st.empty() && st.top() == popV[popIndex]) {
            st.pop();
            ++popIndex;
        }
    }

    return st.empty() && popIndex == popV.size();
}
#include <cassert>
#include <vector>
#include "solution.h" // assume the function is declared here

int main() {
    std::vector<int> push1 = {1, 2, 3, 4, 5};
    std::vector<int> pop1 = {4, 5, 3, 2, 1};
    assert(isPopOrder(push1, pop1) == true);

    std::vector<int> pop2 = {4, 3, 5, 1, 2};
    assert(isPopOrder(push1, pop2) == false);

    std::vector<int> push3 = {1, 2, 3};
    std::vector<int> pop3 = {3, 2, 1};
    assert(isPopOrder(push3, pop3) == true);

    std::vector<int> pop4 = {1, 2, 3};
    assert(isPopOrder(push3, pop4) == true);

    std::vector<int> push5 = {2, 1, 3};
    std::vector<int> pop5 = {3, 1, 2};
    assert(isPopOrder(push5, pop5) == false);

    std::vector<int> push6 = {5, 7, 8, 9};
    std::vector<int> pop6 = {9, 8, 7, 5};
    assert(isPopOrder(push6, pop6) == true);

    std::vector<int> pop7 = {5, 7, 8, 9};
    assert(isPopOrder(push6, pop7) == true);

    std::vector<int> push8 = {1};
    std::vector<int> pop8 = {1};
    assert(isPopOrder(push8, pop8) == true);

    std::vector<int> pop9 = {2};
    assert(isPopOrder(push8, pop9) == false);

    std::vector<int> push10 = {10, 20, 30};
    std::vector<int> pop10 = {30, 10, 20};
    assert(isPopOrder(push10, pop10) == false);

    return 0;
}
// The core idea is to simulate the stack behavior greedily. Iterate through each element in `pushV`, pushing it onto the stack. Immediately after each push, check whether the top of the stack matches the next element in `popV` (tracked by an index). If it matches, pop it and advance the index; continue popping while the stack is non-empty and the top equals the next pop element. This greedy strategy works because if the top of the stack equals the required next pop, we must pop it now—deferring would be impossible later since the stack is LIFO and any later push would block it. After processing all pushes, any remaining elements in the stack must be popped in the exact order that matches the tail of `popV`; if the stack becomes empty and we have consumed all of `popV`, the order is valid. Edge cases: vectors of different lengths are immediately invalid; identical vectors (e.g., `[1,2,3]` push and `[3,2,1]` pop) are valid; empty vectors are not considered here but the algorithm would handle them. Time complexity is O(n) where n is the length of the vectors (each element is pushed once and popped at most once), and space complexity is O(n) for the stack in the worst case (e.g., when push order is already the pop order reversed, the stack grows to n before any pop).
