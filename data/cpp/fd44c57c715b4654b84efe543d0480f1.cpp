Given a balanced parentheses string consisting only of '(' and ')' characters, write a C++ function `std::string bracketLevelBits(const std::string& s)` that processes the string from left to right, tracking the current nesting depth (starting at 0). For each character, before updating the depth for an opening parenthesis or after updating the depth for a closing parenthesis, output the parity (0 if even, 1 if odd) of the current depth. Specifically: when you see '(', output the parity of the depth *before* incrementing, then increment. When you see ')', decrement the depth first, then output the parity of the new depth. The function must return a string of length equal to the input, consisting of '0' and '1' characters, representing these parities in order. The input is guaranteed to be a valid balanced parentheses sequence, so the depth never goes negative and ends at 0. For example, for "()" the output is "01", and for "(())" the output is "0011".
The main idea is to simulate a simple counter that tracks the current nesting depth. We iterate over each character in the input string exactly once. For an opening parenthesis, we first record the parity of the current depth, then increment the depth. For a closing parenthesis, we first decrement the depth, then record the parity of the new depth. Since the input is guaranteed to be balanced, the depth never becomes negative, and we do not need to handle invalid input. Edge cases include an empty string (which should produce an empty output) and a string with no nesting like "()()" which yields "0101". The algorithm runs in O(n) time because we do one pass over the string, and O(n) auxiliary space for the output string (though if we consider the returned string as part of the output, the extra space beyond that is O(1)). The approach is straightforward and does not require any additional data structures beyond a simple integer counter.
#include <string>

// Returns a string of length s.size() where each character is '0' or '1'
// representing the parity of the nesting depth at each step.
std::string bracketLevelBits(const std::string& s) {
    std::string result;
    result.reserve(s.size());
    int depth = 0;
    for (char ch : s) {
        if (ch == '(') {
            result.push_back((depth % 2) ? '1' : '0');
            ++depth;
        } else {  // ch == ')'
            --depth;
            result.push_back((depth % 2) ? '1' : '0');
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Declare the function (for testing, we assume it is defined above)
std::string bracketLevelBits(const std::string& s);

int main() {
    assert(bracketLevelBits("") == "");
    assert(bracketLevelBits("()") == "01");
    assert(bracketLevelBits("(())") == "0011");
    assert(bracketLevelBits("()()") == "0101");
    assert(bracketLevelBits("(()())") == "001011");
    assert(bracketLevelBits("((()))") == "000111");
    assert(bracketLevelBits("()(())") == "010011");
    assert(bracketLevelBits("((())())") == "00011010");
    return 0;
}
