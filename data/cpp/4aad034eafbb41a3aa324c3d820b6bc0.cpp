// Write a C++ function `decodeString` that takes a string `s` following the encoding rule `k[encoded_string]`, where the `encoded_string` inside the square brackets is repeated exactly `k` times. `k` is guaranteed to be a positive integer. The input string may contain nested encodings (e.g., `"3[a2[c]]"`), and there are no extra characters except digits, letters, and square brackets. The function should return the fully decoded string. For example, `"3[a]2[bc]"` → `"aaabcbc"`, and `"2[abc]3[cd]ef"` → `"abcabccdcdcdef"`. Assume the input is always valid and non‑empty. Implement the function as a free function with signature `std::string decodeString(const std::string& s)`, properly using `const` correctness.
The problem is solved using a stack of characters. Traverse the input string from left to right. If the current character is not `']'`, push it onto the stack. When a `']'` is encountered, repeatedly pop characters from the stack until `'['` is found, building the substring inside the brackets (note the order: because we pop in reverse, we prepend each popped character to the accumulated string). Then pop the `'['` itself. Next, pop all leading digits from the stack (they appear immediately before the `'['`), constructing the number by prepending each digit to a string. Convert that string to an integer using `std::stoi`. Push the built substring onto the stack exactly `k` times, character by character. After processing the entire input, pop all remaining characters from the stack in reverse order to reconstruct the final decoded string.  
Edge cases: nested brackets naturally work because the inner expression is decoded before the outer one (since the `']'` of the inner bracket triggers decoding first). Single‑character substrings, multi‑digit numbers (e.g., `12[a]`), and cases with no brackets or with letters between bracketed groups are handled correctly. The algorithm runs in O(n) time per output character (each character is pushed and popped a constant number of times), and its space complexity is O(m) where m is the length of the decoded output string, which is the stack size.
#include <string>
#include <stack>
#include <cctype>

// Decode a string encoded as k[encoded_string], with possible nesting.
// Returns the fully decoded string.
std::string decodeString(const std::string& s) {
    std::stack<char> st;
    for (char ch : s) {
        if (ch != ']') {
            st.push(ch);
        } else {
            // Build the current substring inside brackets.
            std::string curr_str;
            while (!st.empty() && st.top() != '[') {
                curr_str = st.top() + curr_str;
                st.pop();
            }
            st.pop(); // remove '['

            // Build the repetition number.
            std::string num_str;
            while (!st.empty() && std::isdigit(st.top())) {
                num_str = st.top() + num_str;
                st.pop();
            }
            int repeat = std::stoi(num_str);

            // Push the substring repeated 'repeat' times.
            for (int i = 0; i < repeat; ++i) {
                for (char c : curr_str) {
                    st.push(c);
                }
            }
        }
    }

    // Reconstruct the final string from the stack.
    std::string result;
    while (!st.empty()) {
        result = st.top() + result;
        st.pop();
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function declaration is assumed to be available.
std::string decodeString(const std::string& s);

int main() {
    assert(decodeString("3[a]2[bc]") == "aaabcbc");
    assert(decodeString("3[a2[c]]") == "accaccacc");
    assert(decodeString("2[abc]3[cd]ef") == "abcabccdcdcdef");
    assert(decodeString("abc") == "abc");
    assert(decodeString("10[a]") == "aaaaaaaaaa");
    assert(decodeString("2[3[x]y]") == "xxxyxxxy");
    assert(decodeString("a1[b]c") == "abc");
    assert(decodeString("") == "");
    return 0;
}
