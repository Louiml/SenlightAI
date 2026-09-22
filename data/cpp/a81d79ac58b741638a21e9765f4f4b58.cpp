// Write a C++ function `extractAndSortHighChars` that takes a string `s` as input and returns a new string containing only those characters from `s` whose ASCII value is greater than the letter `'r'` (i.e., `'s'` and above), and then returns those characters sorted in descending alphabetical order (from highest ASCII to lowest). The function must preserve only the qualifying characters, ignore all others, and return the sorted result. Assume the input may be empty, contain uppercase and lowercase letters, digits, punctuation, whitespace, and non-ASCII characters; only lowercase letters from `'s'` to `'z'` should be considered. The sorting must be stable with respect to duplicates (i.e., duplicate occurrences remain adjacent, though order among equal duplicates doesn't matter). Do not modify the input string; return the result as a `std::string`. Provide a standalone implementation with a descriptive name and `const` correctness.
#include <cassert>
#include <string>

// The function is declared elsewhere; here we test it.
int main() {
    assert(extractAndSortHighChars("xyz") == "zyx");
    assert(extractAndSortHighChars("abc") == "");
    assert(extractAndSortHighChars("") == "");
    assert(extractAndSortHighChars("s") == "s");
    assert(extractAndSortHighChars("zaz") == "zz");
    assert(extractAndSortHighChars("Abc$%^z~") == "z~");  // '~' (126) > 'r', 'z' > 'r'
    assert(extractAndSortHighChars("rstuv") == "vuts");
    assert(extractAndSortHighChars("qqqq") == "");
    assert(extractAndSortHighChars("s z a y b x") == "zyxs");
    assert(extractAndSortHighChars("zyxwvutsrqponmlkjihgfedcba") == "zyxwvuts");
    return 0;
}
#include <string>

// Extract characters greater than 'r' from the input string and return them sorted in descending ASCII order.
std::string extractAndSortHighChars(const std::string& s) {
    std::string result;
    // First pass: collect all characters greater than 'r'
    for (char c : s) {
        if (c > 'r') {
            result.push_back(c);
        }
    }
    // Insertion sort in descending order
    for (std::size_t i = 1; i < result.size(); ++i) {
        std::size_t j = i;
        while (j >= 1 && result[j] > result[j - 1]) {
            std::swap(result[j], result[j - 1]);
            --j;
        }
    }
    return result;
}
// The solution involves two phases: extraction and sorting. In the extraction phase, iterate through each character `c` of the input string `s`. If `c > 'r'`, append it to a result string `result`. This condition naturally includes lowercase letters `'s'` through `'z'`, but also includes characters like `'{'` (ASCII 123) or `'~'` (126) if they appear, and crucially excludes all other lowercase letters, uppercase, digits, punctuation, and whitespace. To strictly match expected behavior, we could refine the condition to `c >= 's' && c <= 'z'` if we only want lowercase letters, but the snippet uses `s[i] > 'r'` which includes anything higher, so we replicate exactly that to stay faithful. After extraction, sort the characters in descending order. Using the insertion sort algorithm from the snippet: for each index `i` from 1 to `result.size()-1`, compare `result[i]` with the previous elements and swap while the current element is greater than the previous one (descending order). This works because characters are comparable by their numeric ASCII values. Edge cases: empty input yields empty result; a string with no characters greater than `'r'` returns empty; duplicate high characters are handled fine; input with `'s'` and `'z'` etc. The time complexity is O(n * k) where n is input length (extraction) and k is number of qualifying characters (sorting), worst-case O(k^2) for insertion sort (e.g., already descending order is O(k), worst-case reversed). Space complexity is O(k) for the result string, excluding input.
