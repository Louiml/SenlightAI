// Write a C++ function `multiTapEncode` that takes a string containing only lowercase English letters and spaces, and returns the multi-tap telephone encoding of that string. In multi-tap encoding, each letter maps to a sequence of repeated digits as follows: 'a'→"2", 'b'→"22", 'c'→"222", 'd'→"3", 'e'→"33", 'f'→"333", 'g'→"4", 'h'→"44", 'i'→"444", 'j'→"5", 'k'→"55", 'l'→"555", 'm'→"6", 'n'→"66", 'o'→"666", 'p'→"7", 'q'→"77", 'r'→"777", 's'→"7777", 't'→"8", 'u'→"88", 'v'→"888", 'w'→"9", 'x'→"99", 'y'→"999", 'z'→"9999", and a space maps to "0". The function must preserve the original order and return the concatenated digit sequence as a string.
The solution precomputes a mapping array `keypad[26]` where each element is the multi-tap digit string for the corresponding letter ('a' at index 0, 'b' at 1, etc.). Then, iterate over each character of the input string: if the character is a space, append "0"; otherwise, compute the index `ch - 'a'` and append the corresponding digit string from the mapping. This approach works directly because the input is guaranteed to contain only lowercase letters and spaces; no need to handle uppercase or punctuation. Edge cases include an empty input (returns empty string) and a string with multiple spaces (each space contributes one "0"). The time complexity is \(O(n \cdot k)\), where \(n\) is the length of the input string and \(k\) is the maximum length of a digit sequence (up to 4, as in "9999" for 'z'), so effectively \(O(n)\). The auxiliary space complexity is \(O(1)\) for the fixed mapping plus \(O(n)\) for the output string, which is unavoidable.
#include <string>
#include <vector>

// Return the multi-tap telephone encoding of a lowercase-alphabet-and-space string.
std::string multiTapEncode(const std::string& input) {
    static const std::vector<std::string> keypad = {
        "2", "22", "222",       // a, b, c
        "3", "33", "333",       // d, e, f
        "4", "44", "444",       // g, h, i
        "5", "55", "555",       // j, k, l
        "6", "66", "666",       // m, n, o
        "7", "77", "777", "7777", // p, q, r, s
        "8", "88", "888",       // t, u, v
        "9", "99", "999", "9999"  // w, x, y, z
    };

    std::string result;
    result.reserve(input.size() * 4);  // Max expansion is 4 digits per char.
    for (char ch : input) {
        if (ch == ' ') {
            result += '0';
        } else {
            result += keypad[ch - 'a'];
        }
    }
    return result;
}
#include <cassert>
#include <string>

std::string multiTapEncode(const std::string& input);

int main() {
    assert(multiTapEncode("a") == "2");
    assert(multiTapEncode("z") == "9999");
    assert(multiTapEncode("hello world") == "4433555555666096667775553");
    assert(multiTapEncode("abc") == "222222");
    assert(multiTapEncode("   ") == "000");
    assert(multiTapEncode("sos") == "77777777");
    assert(multiTapEncode("") == "");
    assert(multiTapEncode("v") == "888");
    assert(multiTapEncode("i love cpp") == "44455566633388855566622277777");
    assert(multiTapEncode("x yz") == "9909999");
    return 0;
}
