/*
Write a C++ function that takes a string as input and returns a boolean value indicating whether the string contains the letters of "hello" as a subsequence (in order, but not necessarily consecutively). The function should be case-sensitive and ignore any characters that are not lowercase letters. For example, "hxelxlxo" should be true because it contains h, e, l, l, o in order, even though there are extra characters in between. Empty strings, strings shorter than 5 characters, or strings that do not preserve the order should return false.
*/
#include <string>

// Returns true if the string contains "hello" as a subsequence (case-sensitive).
bool containsHelloSubsequence(const std::string& s) {
    const std::string target = "hello";
    size_t targetIndex = 0;

    for (char ch : s) {
        if (targetIndex < target.size() && ch == target[targetIndex]) {
            ++targetIndex;
        }
    }

    return targetIndex == target.size();
}
int main() {
    assert(containsHelloSubsequence("hello") == true);
    assert(containsHelloSubsequence("hxelxlxo") == true);
    assert(containsHelloSubsequence("hlelo") == false); // wrong order
    assert(containsHelloSubsequence("hello world") == true);
    assert(containsHelloSubsequence("hell") == false);
    assert(containsHelloSubsequence("") == false);
    assert(containsHelloSubsequence("HELLO") == false); // case-sensitive
    assert(containsHelloSubsequence("h e l l o") == true); // spaces ignored
    assert(containsHelloSubsequence("hhhhelllllooo") == true);
    assert(containsHelloSubsequence("helo") == false); // missing second l
}
// The task is essentially checking if "hello" is a subsequence of the input string. The main algorithm iterates through each character of the input string once, maintaining a pointer `i` (or an index) into the target word "hello". For each character in the input, if it matches the current target character, we advance the pointer. If the pointer reaches 5 (the length of "hello"), we have found the full subsequence and can return true immediately. If we finish the loop without reaching the end of the target, we return false.  
//
// Edge cases:  
// - Input shorter than 5 characters: the loop will finish before the pointer reaches 5, returning false.  
// - Input with characters that are not lowercase letters: those characters are simply ignored because the comparison fails, and we do not advance the pointer.  
// - Duplicate letters: e.g., "helllo" still returns true because the third 'l' is ignored after the second 'l' advances the pointer, and the 'o' at the end completes the sequence.  
// - Case sensitivity: "HELLO" should return false because uppercase letters do not match.  
// - The target itself as input: "hello" returns true because each character matches in sequence.  
//
// Time complexity: O(n), where n is the length of the input string, because we scan it once. Space complexity: O(1), as we use a fixed number of variables.
