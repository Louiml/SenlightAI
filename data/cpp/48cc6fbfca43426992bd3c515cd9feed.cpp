Write a C++ function named `decodeString` that takes a non-empty string `s` representing an encoded string and returns the fully decoded string. The encoding rule is: `k[encoded_string]`, where the `encoded_string` inside the square brackets will be repeated exactly `k` times. Note that `k` is guaranteed to be a positive integer, and the input string is always valid (i.e., no extra spaces, brackets are properly matched, and digits only appear before `[`). The input may contain nested encodings, such as `3[a2[c]]`, which should decode to `accaccacc`. Your function must handle arbitrary nesting depth and return the resulting string.
#include <cassert>
#include <string>

// The solution function is declared above.

int main() {
    assert(decodeString("3[a]2[bc]") == "aaabcbc");
    assert(decodeString("3[a2[c]]") == "accaccacc");
    assert(decodeString("2[abc]3[cd]ef") == "abcabccdcdcdef");
    assert(decodeString("abc3[cd]xyz") == "abccdcdcxyz");
    assert(decodeString("1[a]") == "a");
    assert(decodeString("10[a]") == "aaaaaaaaaa");
    assert(decodeString("2[3[a]b]") == "aaabaaab");
    assert(decodeString("") == ""); // although non-empty is guaranteed, still safe
    assert(decodeString("leetcode") == "leetcode");
    assert(decodeString("3[z]2[2[y]pq4[2[jk]e1[f]]]ef") == "zzzyypqjkjkefjkjkefjkjkefjkjkefyypqjkjkefjkjkefjkjkefjkjkefef");
    return 0;
}
#include <string>
#include <cctype>
#include <stack>

// Decode an encoded string following the rule k[encoded_string].
std::string decodeString(const std::string& s) {
    std::stack<char> stk;
    for (char ch : s) {
        if (ch == ']') {
            // Collect the substring inside the current brackets.
            std::string cur;
            while (stk.top() != '[') {
                cur = stk.top() + cur;
                stk.pop();
            }
            stk.pop(); // remove '['

            // Collect the repetition count (digits before '[').
            std::string num;
            while (!stk.empty() && std::isdigit(stk.top())) {
                num = stk.top() + num;
                stk.pop();
            }
            int times = std::stoi(num);

            // Expand and push back onto the stack.
            std::string expanded;
            for (int i = 0; i < times; ++i) {
                expanded += cur;
            }
            for (char c : expanded) {
                stk.push(c);
            }
        } else {
            stk.push(ch);
        }
    }

    // Build the final result from the stack.
    std::string result;
    while (!stk.empty()) {
        result = stk.top() + result;
        stk.pop();
    }
    return result;
}
// The solution uses a stack to process the string character by character. When a character other than `]` is encountered, it is pushed onto the stack. When `]` is found, the algorithm pops characters until the matching `[` is removed, collecting the substring between them (this is the current encoded segment). Then it continues popping any digit characters to build the repetition count `k` (which is at least 1). The collected substring is repeated `k` times, and the resulting expanded string is pushed back onto the stack, so it can be further processed if there are outer encodings. After scanning the entire input, the stack contains the fully decoded string in reverse order (since characters are pushed one by one), and we pop them all to build the final result. Edge cases include a string with no brackets (e.g., `"abc"` returns `"abc"`), a single repetition (e.g., `"1[a]"` returns `"a"`), and nested brackets (e.g., `"2[3[a]b]"`). Time complexity is O(|s| * |result|) in the worst case due to repeated copying, and space complexity is O(|result|) for the stack and final output.
