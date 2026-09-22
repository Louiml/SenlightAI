// Write a C++ function named `decideChatStatus` that takes a single non-empty string `name` (containing only lowercase English letters) and returns the string `"CHAT WITH HER!"` if the number of distinct characters in `name` is even, and `"IGNORE HIM!"` if it is odd. The function must be case-sensitive (though input is lowercase), handle strings of any length up to 100, and correctly count each character only once. Do not modify the input string.
#include <cassert>
#include <string>

// The declaration of decideChatStatus is assumed to be available.
std::string decideChatStatus(const std::string& name);

int main() {
    assert(decideChatStatus("a") == "IGNORE HIM!");
    assert(decideChatStatus("ab") == "CHAT WITH HER!");
    assert(decideChatStatus("abc") == "IGNORE HIM!");
    assert(decideChatStatus("aaa") == "IGNORE HIM!");
    assert(decideChatStatus("aabb") == "CHAT WITH HER!");
    assert(decideChatStatus("abca") == "CHAT WITH HER!");
    assert(decideChatStatus("xyzz") == "CHAT WITH HER!");
    assert(decideChatStatus("hello") == "IGNORE HIM!"); // distinct: h,e,l,o = 4 even
    assert(decideChatStatus("world") == "CHAT WITH HER!"); // distinct: w,o,r,l,d = 5 odd
    assert(decideChatStatus("codeforces") == "IGNORE HIM!"); // distinct: c,o,d,e,f,r,s = 7 odd
    return 0;
}
#include <string>
#include <set>

// Return "CHAT WITH HER!" if the number of distinct characters in name is even,
// otherwise return "IGNORE HIM!".
std::string decideChatStatus(const std::string& name) {
    std::set<char> distinctChars;
    for (char c : name) {
        distinctChars.insert(c);
    }
    return (distinctChars.size() % 2 == 0) ? "CHAT WITH HER!" : "IGNORE HIM!";
}
// The core algorithm is simple: iterate over each character in the input string and insert it into a `std::set<char>`. Because a set automatically discards duplicates and only stores unique elements, the size of the set after processing the entire string equals the number of distinct characters. Then, check if `set.size() % 2 == 0` to decide whether to return the "even" or "odd" answer. Edge cases include a string of length 1 (one distinct character → odd → return "IGNORE HIM!") and a string with all identical characters (always odd for length ≥1). Time complexity is O(n log n) due to set insertion, where n is the string length, and space complexity is O(u) where u is the number of unique characters (at most 26 for lowercase letters, but in general up to n). No special attention is needed for empty input because the problem guarantees non-empty.
