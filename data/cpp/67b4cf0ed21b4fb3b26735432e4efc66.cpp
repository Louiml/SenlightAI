Write a C++ function `std::pair<int,int> longestValidParenthesesInfo(const std::string& s)` that takes a string containing only characters `'('` and `')'` (possibly empty) and returns a pair `{max_length, count_of_substrings_with_that_max_length}`. A valid parentheses substring is one where every opening bracket has a matching closing bracket in the correct order (e.g., `"()"`, `"(()())"` are valid; `")("`, `"(()"` are not). The function must compute the length of the longest valid parentheses substring and how many distinct substrings of that maximum length exist. If no valid substring exists (length 0), return `{0, 1}` (the empty substring counts as one valid substring of length 0, but the problem convention here matches the snippet: if max length is 0, the count is 1, representing the empty substring). Note that substrings are identified by their starting and ending indices, so overlapping substrings count separately. For example, in `"()()"`, the longest valid length is 2, and there are two such substrings: positions 0-1 and 2-3, so return `{2, 2}`. In `"(()())"` the longest length is 6, count 1.
The core idea is to mark which characters belong to at least one valid parentheses substring. We use a stack to match parentheses: traverse the string left to right; when encountering `'('`, push its index; when encountering `')'` and the stack is non-empty, pop the matching `'('` index and mark both that index and the current index as "belonging to a valid pair" (e.g., in a boolean array `valid[i] = true`). After this pass, any index not marked is part of an unmatched or improperly nested parenthesis. Then compute the longest consecutive run of `valid[i] == true` in the array; that run length is the maximum valid substring length because a valid parentheses substring cannot contain unmatched characters, and any run of matched characters forms a valid substring (since each match is proper and overlapping runs combine). To count how many substrings achieve this maximum length, we scan the `valid` array again and count how many positions `i` are such that the run ending at `i` has exactly `maxl` length. Equivalently, we can precompute a prefix-like array `f[i]` where `f[i] = f[i-1] + 1` if `valid[i]` else `0`; then `maxl` is the maximum of `f`; and the answer count is the number of indices where `f[i] == maxl`. Edge cases: empty string → no matches, `f` all zeros → `maxl=0` → return `{0,1}`. String like `")(("` → no valid pairs → `maxl=0` → `{0,1}`. String `"()(()"` → valid marks on indices 0,1 and 3,4; there is a gap at index 2, so runs are length 2 and 2, maxl=2, count=2 (two substrings `"()"` at positions 0-1 and the inner `"()"` at positions 3-4). Note that the algorithm correctly avoids counting overlapping runs like `"()"` inside `"(())"` because the entire `(())` is a single run of length 4, not two separate runs. Time complexity is O(n) for three linear passes (marking, computing f, counting). Space complexity is O(n) for the boolean array and stack (worst-case stack size n/2 for all opening brackets).
#include <string>
#include <vector>
#include <stack>
#include <utility>

// Return {max_length, count} of longest valid parentheses substrings.
std::pair<int,int> longestValidParenthesesInfo(const std::string& s) {
    const int n = static_cast<int>(s.size());
    std::vector<bool> valid(n, false);
    std::stack<int> stk;

    // Mark indices that belong to at least one matched pair.
    for (int i = 0; i < n; ++i) {
        if (s[i] == '(') {
            stk.push(i);
        } else if (s[i] == ')' && !stk.empty()) {
            int open_index = stk.top();
            stk.pop();
            valid[open_index] = true;
            valid[i] = true;
        }
    }

    // Compute longest consecutive true run and count positions ending that run.
    int max_length = 0;
    int count = 0;
    int current_length = 0;
    for (int i = 0; i < n; ++i) {
        if (valid[i]) {
            ++current_length;
            if (current_length > max_length) {
                max_length = current_length;
                count = 1;
            } else if (current_length == max_length) {
                ++count;
            }
        } else {
            current_length = 0;
        }
    }

    if (max_length == 0) {
        return {0, 1}; // Empty substring is the only valid one.
    }
    return {max_length, count};
}
#include <cassert>
#include <string>
#include <utility>

// Declare the function (already defined in solution).
std::pair<int,int> longestValidParenthesesInfo(const std::string& s);

int main() {
    // Empty string → no non-empty valid substring, count 1 for empty.
    assert(longestValidParenthesesInfo("") == std::make_pair(0, 1));
    // All unmatched.
    assert(longestValidParenthesesInfo(")(") == std::make_pair(0, 1));
    assert(longestValidParenthesesInfo("(((") == std::make_pair(0, 1));
    // Single pair.
    assert(longestValidParenthesesInfo("()") == std::make_pair(2, 1));
    // Two separate pairs, each length 2.
    assert(longestValidParenthesesInfo("()()") == std::make_pair(2, 2));
    // Nested pair, length 4, count 1.
    assert(longestValidParenthesesInfo("(())") == std::make_pair(4, 1));
    // Mixed: longest valid is "(()())" length 6, count 1.
    assert(longestValidParenthesesInfo("(()())") == std::make_pair(6, 1));
    // "()(()" → two separate runs of length 2.
    assert(longestValidParenthesesInfo("()(()") == std::make_pair(2, 2));
    // Complex with nested and separate: "(()))())(" → longest is "()" inside? Let's compute: string indices: 0( 1( 2) 3) 4) 5( 6) 7) 8( → valid marks: 0-3 all matched? Actually stack: push0, push1, at2 pop1 (valid 1,2), at3 pop0 (valid0,3), at4 unmatched ), at5 push5, at6 pop5 (valid5,6), at7 unmatched ), at8 unmatched (. Runs: indices 0-3 length4, indices5-6 length2. So max=4, count=1.
    assert(longestValidParenthesesInfo("(()))())(") == std::make_pair(4, 1));
    // All nested: "((()))" length 6, count 1.
    assert(longestValidParenthesesInfo("((()))") == std::make_pair(6, 1));
}
