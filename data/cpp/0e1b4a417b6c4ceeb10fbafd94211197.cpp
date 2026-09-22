Write a C++ function `std::string removeKDigits(const std::string& num, int k)` that takes a non-negative integer represented as a string (with no leading zeros except for the number "0" itself) and an integer `k` (where `0 <= k <= num.length()`), and returns the smallest possible numeric string after removing exactly `k` digits from the original number. The function must preserve the order of the remaining digits and remove any leading zeros from the result. If the resulting number is empty (all digits removed), return `"0"`. For example, for `num = "1432219"` and `k = 3`, the smallest possible result is `"1219"`; for `num = "10200"` and `k = 1`, the result is `"200"`; for `num = "10"` and `k = 2`, the result is `"0"`.

The optimal strategy is to build the smallest number by greedily removing a digit whenever a larger digit appears before a smaller digit, because replacing a larger leading digit with a smaller following digit will always reduce the overall numeric value. This is naturally implemented using a monotonic stack: iterate through each digit of the input. While the stack is not empty, the top of the stack is greater than the current digit, and we still have `k` removals left, we pop the top (effectively removing that digit) and decrement `k`. After the popping loop, we push the current digit, but we skip pushing a leading zero (i.e., if the stack is empty and the current digit is `'0'`, we do not push it) to avoid any leading zeros in the final result. After processing all digits, if `k` is still positive, we remove that many digits from the end of the stack (since those trailing digits are the largest remaining and removing them minimizes the result). Finally, we extract the digits from the stack into a string and reverse it, because the stack stores digits in reverse order. Edge cases include: when all digits are removed (e.g., `"10"` and `k=2`), the stack becomes empty, so we return `"0"`; when the input has leading zeros or zeros that become significant after removals, we must handle them correctly (e.g., `"10200"` with `k=1` should yield `"0200"` → remove leading zero → `"200"`). Time complexity is O(n) because each digit is pushed and popped at most once. Space complexity is O(n) for the stack in the worst case.

#include <string>
#include <stack>
#include <algorithm>

// Removes exactly k digits from the string num to form the smallest possible number.
// Returns the result as a string, with no leading zeros (unless the result is "0").
std::string removeKDigits(const std::string& num, int k) {
    std::stack<char> st;
    
    for (char c : num) {
        // Remove larger digits from the stack if we can and if they are greater than current digit
        while (!st.empty() && st.top() > c && k > 0) {
            st.pop();
            --k;
        }
        // Skip leading zeros (when stack is empty and current digit is '0')
        if (st.empty() && c == '0') continue;
        st.push(c);
    }
    
    // If we still need to remove digits, remove from the end (largest remaining)
    while (!st.empty() && k > 0) {
        st.pop();
        --k;
    }
    
    // Build result from stack (reverse order)
    std::string result;
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }
    std::reverse(result.begin(), result.end());
    
    return result.empty() ? "0" : result;
}

#include <cassert>
#include <string>

// Declare the function for testing (assumes it is defined in the same file or linked)
std::string removeKDigits(const std::string& num, int k);

int main() {
    // Basic examples
    assert(removeKDigits("1432219", 3) == "1219");
    assert(removeKDigits("10200", 1) == "200");
    assert(removeKDigits("10", 2) == "0");
    
    // Edge cases with single digit or all digits removed
    assert(removeKDigits("9", 1) == "0");
    assert(removeKDigits("1", 0) == "1");
    
    // Leading zeros are stripped after removal
    assert(removeKDigits("102030", 2) == "30"); // removed '1' and '2'? Actually careful: expected "0" after removing? Let's test: "102030" remove 2 -> possible results: "0230" = "230", "0030" = "30", etc. Best is "30"? Actually removing '1' and '2' leaves "0030" -> "30"
    assert(removeKDigits("100", 1) == "0");
    
    // Large k removes all digits
    assert(removeKDigits("12345", 5) == "0");
    assert(removeKDigits("12345", 6) == "0"); // k is guaranteed <= length, but just in case
    
    // Already sorted ascending: remove from end
    assert(removeKDigits("12345", 2) == "123");
    
    // Already sorted descending: remove all leading highs
    assert(removeKDigits("54321", 2) == "321");
    
    // Zeros in middle and no leading zeros
    assert(removeKDigits("100200", 3) == "0");
    assert(removeKDigits("100200", 2) == "0"); // remove '1' and one '0' from start? Actually best is "0" (after removing '1' and '0', left "0200" -> "200"? Let's compute: "100200" remove 2 -> try: remove '1' and '0' (pos0 and pos1) -> "0200" -> "200"; remove '1' and first '0' after? Actually best is "0" if remove all non-zero? Let's test: remove '1' and '0' (the one after 1) leaves "0200" -> "200". Remove '1' and '0' (the first zero of the last block) leaves "10020"? Not smaller. Remove '1' and the '2' leaves "10000" -> "0"? That removes digit '2' and '1', leaving "0000" -> "0". So assert should be "0". Let's adjust.
    
    // Duplicate digits
    assert(removeKDigits("112", 1) == "11");
    
    return 0;
}
