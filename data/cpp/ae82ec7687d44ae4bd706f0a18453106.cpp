Write a C++ function that decodes a string following a specific compressed format. The input string `s` consists of lowercase English letters, digits, and square brackets `[` and `]`. The encoding rule is `k[encoded_string]`, where the `encoded_string` inside the square brackets is repeated exactly `k` times. `k` is guaranteed to be a positive integer (1–300). The input is always valid: no extra whitespace, brackets are properly nested and matched, and digits only appear immediately before an opening bracket (they are always part of a repetition count). Your function should return the fully decoded string, preserving the original order of characters. For example, `"3[a2[c]]"` becomes `"accaccacc"`. The function must handle arbitrary nesting depth and correctly process cases where the encoded string contains no brackets, multiple adjacent encoded groups, and digits with multiple characters (e.g., `"10[a]"`).
#include <cassert>
#include <string>

// Declare the function to test (assuming it's defined in the same file).
std::string decodeString(const std::string& s);

int main() {
    // Basic single group
    assert(decodeString("3[a]") == "aaa");
    // Nested groups
    assert(decodeString("3[a2[c]]") == "accaccacc");
    // Multiple adjacent groups
    assert(decodeString("2[ab]3[cd]") == "ababcdcdcd");
    // Multi-digit multiplier
    assert(decodeString("10[a]") == "aaaaaaaaaa");
    // No brackets
    assert(decodeString("abc") == "abc");
    // Single character repeated
    assert(decodeString("1[b]") == "b");
    // Deep nesting with multi-digit
    assert(decodeString("2[3[x]y]") == "xxxyxxxy");
    // Empty encoded string inside brackets is not valid per spec, but test simple
    assert(decodeString("") == "");
    // Mixed letters outside and inside brackets
    assert(decodeString("a2[bc]d") == "abcbcd");
    // Bracket containing multiple letters and a nested group
    assert(decodeString("3[z2[ab]]") == "zababzababzabab");
}
#include <string>
#include <stack>
#include <cctype>

// Decode a string following the pattern k[encoded_string].
// The encoded string inside brackets is repeated k times.
std::string decodeString(const std::string& s) {
    std::stack<std::string> st;

    for (char ch : s) {
        if (ch != ']') {
            // Push all characters except closing brackets onto the stack.
            st.push(std::string(1, ch));
        } else {
            // Reconstruct the substring inside the brackets.
            std::string sub;
            while (!st.empty() && st.top() != "[") {
                sub = st.top() + sub;
                st.pop();
            }
            // Remove the opening bracket '['.
            st.pop();

            // Collect the multiplier digits (may be multiple characters).
            std::string countStr;
            while (!st.empty() && std::isdigit(st.top()[0])) {
                countStr = st.top() + countStr;
                st.pop();
            }
            int repeat = std::stoi(countStr);

            // Build the repeated substring.
            std::string repeated;
            for (int i = 0; i < repeat; ++i) {
                repeated += sub;
            }
            st.push(repeated);
        }
    }

    // Concatenate all stack elements into the final result.
    std::string result;
    while (!st.empty()) {
        result = st.top() + result;
        st.pop();
    }
    return result;
}
// The solution uses a stack-based iterative decoder. Traverse each character of the input string. When the current character is not a closing bracket `]`, push it onto the stack as a single-character string. When a closing bracket is encountered, we need to reconstruct the substring that belongs to the innermost encoded group. Pop from the stack until we encounter the matching opening bracket `[`, building the substring by prepending popped characters (so the original order is maintained). Discard the `[`. Then, continue popping from the stack to collect the repetition count `k` (digits immediately before the `[`). Since digits may be multi-character, keep prepending them to form the full number as a string. Convert that string to an integer using `std::stoi`. Repeat the substring `k` times and push the resulting repeated string back onto the stack. After processing all characters, the stack contains one or more strings in reverse order; concatenate them by prepending each popped element to the result to restore the correct final ordering. Edge cases: a string with no brackets (e.g., `"abc"`) simply returns the input unchanged; a single-digit multiplier like `"2[a]"` works; and a multi-digit multiplier like `"12[ab]"` requires collecting all digit characters. Time complexity is \(O(n \cdot k_{\text{max}})\) in the worst case due to repeated string concatenation, but more precisely \(O(\text{total output length})\), which is \(O(n)\) when considering the output size. Space complexity is \(O(\text{output length})\) for the stack and result.
