Write a C++ function named `characterFrequency` that accepts a non-empty string (which may contain uppercase letters, lowercase letters, digits, spaces, punctuation, or any printable ASCII characters) and returns a string containing the frequency of each distinct character from the input string. The output must be sorted in ascending order of the character’s ASCII value, and for each character, output the character immediately followed by its count, with each pair separated by a single space. For example, for input `"aab"` the output should be `"a2 b1"`. The function must be case-sensitive (`'A'` and `'a'` are distinct) and must preserve the original characters in the output, including spaces and punctuation (e.g., a space is output as a space before its count). If the input contains only one distinct character, that character with its count is the entire output.

The solution uses a `std::map<char, int>` to store the frequency of each character. Iterating through the input string character by character, increment the counter for each character in the map. Since `std::map` automatically keeps keys sorted in ascending ASCII order, iterating over the map and constructing the output string with each character followed by its integer count (converted to a string) gives the required order. Edge cases: an empty input is not allowed per specification, but if it occurred, the function could return an empty string; characters like spaces are handled naturally because they are just `char` values. Digits and punctuation are also handled by the map. Time complexity is O(n log k) where n is the length of the input and k is the number of distinct characters (since each map insertion/update is O(log k)); space complexity is O(k) for the map and O(total output length) for the result.

#include <string>
#include <map>
#include <sstream>

// Returns a string with each distinct character followed by its count,
// sorted by ASCII value and space-separated.
std::string characterFrequency(const std::string& input) {
    std::map<char, int> freq;
    for (char ch : input) {
        ++freq[ch];
    }
    
    std::ostringstream result;
    for (const auto& pair : freq) {
        if (result.tellp() > 0) {
            result << ' ';
        }
        result << pair.first << pair.second;
    }
    return result.str();
}

#include <cassert>
#include <string>

std::string characterFrequency(const std::string& input);

int main() {
    assert(characterFrequency("aab") == "a2 b1");
    assert(characterFrequency("articles") == "a1 c1 e1 i1 l1 r1 s1 t1");
    assert(characterFrequency("takeuforward") == "a2 d1 e1 f1 k1 o1 r2 t1 u1 w1");
    assert(characterFrequency("Extraordinary") == "E1 a1 d1 i1 n1 o1 r2 t1 x1 y1");
    assert(characterFrequency("") == "");
    assert(characterFrequency("AAA") == "A3");
    assert(characterFrequency("aaAA") == "A2 a2");
    assert(characterFrequency("1 2 1") == " 1 1 2 1");  // space appears once, then '1' twice, then '2' once
    assert(characterFrequency("zZ") == "Z1 z1");
    assert(characterFrequency("hello world!") == " 1 !1 d1 e1 h1 l3 o1 r1 w1");
}
