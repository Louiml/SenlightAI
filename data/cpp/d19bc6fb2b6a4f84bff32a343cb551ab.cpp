/*
Write a C++ function named `countAfterCancellation` that takes a `const std::vector<std::string>&` as input and returns an integer representing the number of strings remaining after repeatedly removing adjacent duplicate strings. The removal process works as follows: process the strings from left to right, maintaining a stack. When the current string equals the top of the stack, pop the stack (canceling the pair); otherwise, push the current string onto the stack. After processing all strings, the function should return the final size of the stack (i.e., the number of strings that remain). The input vector may contain any number of strings (including zero), and duplicates are only removed when they appear consecutively in the original sequence after considering previous cancellations. For example, given `{"ab","ad","ad","ak","ac"}`, the result is 3, and given `{"a","b","b","a"}`, the result is 0 because all cancel out.
*/

#include <vector>
#include <string>
#include <stack>

// Returns the number of strings remaining after repeatedly removing adjacent duplicate strings.
int countAfterCancellation(const std::vector<std::string>& input) {
    std::stack<std::string> stack;
    for (const auto& str : input) {
        if (stack.empty() || str != stack.top()) {
            stack.push(str);
        } else {
            stack.pop();
        }
    }
    return static_cast<int>(stack.size());
}

#include <cassert>
#include <vector>
#include <string>

// Assume countAfterCancellation is defined above.

int main() {
    std::vector<std::string> v1 = {"ab", "ad", "ad", "ak", "ac"};
    assert(countAfterCancellation(v1) == 3);

    std::vector<std::string> v2 = {"a", "b", "b", "a"};
    assert(countAfterCancellation(v2) == 0);

    std::vector<std::string> v3 = {};
    assert(countAfterCancellation(v3) == 0);

    std::vector<std::string> v4 = {"x"};
    assert(countAfterCancellation(v4) == 1);

    std::vector<std::string> v5 = {"x", "x"};
    assert(countAfterCancellation(v5) == 0);

    std::vector<std::string> v6 = {"a", "b", "c", "c", "b", "d"};
    assert(countAfterCancellation(v6) == 2); // "a" and "d" remain

    std::vector<std::string> v7 = {"a", "a", "b", "b", "c"};
    assert(countAfterCancellation(v7) == 1); // "c" remains

    std::vector<std::string> v8 = {"a", "b", "a"};
    assert(countAfterCancellation(v8) == 3); // no consecutive duplicates

    std::vector<std::string> v9 = {"", ""};
    assert(countAfterCancellation(v9) == 0); // empty strings cancel

    std::vector<std::string> v10 = {"a", "b", "c", "a", "b", "c"};
    assert(countAfterCancellation(v10) == 6); // none cancel

    return 0;
}

// The solution uses a stack to simulate the cancellation process. Iterate through each string in the input vector. If the stack is empty, push the current string. Otherwise, compare the current string with the top of the stack. If they are equal, pop the top (cancel the pair); if not, push the current string. This effectively removes consecutive duplicates in a nested way—e.g., `{"a","b","b","a"}` cancels the two `"b"`s, making the two `"a"`s adjacent, which then cancel as well. Edge cases include an empty vector (returns 0) and a vector with no consecutive duplicates (returns the vector size). The algorithm runs in O(n) time, where n is the number of strings, because each string is pushed and popped at most once. The space complexity is O(n) in the worst case (e.g., when no duplicates exist, the stack holds all strings). Use `const` reference for the input to avoid copying, and `std::stack<std::string>` to manage the cancellation.
