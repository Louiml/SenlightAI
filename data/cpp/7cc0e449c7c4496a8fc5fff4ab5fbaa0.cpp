// Write a C++ function `string adjustString(string s)` that takes a non-empty string and repeatedly scans it from right to left, swapping adjacent characters whenever the current character differs from the one immediately to its left. Each such swap increments a counter. The scanning repeats from the end of the string after each complete pass, but the process stops after exactly one full pass from the last index down to index 1 (i.e., the loop runs exactly once over the string). The function should return the total number of swaps performed during that single pass as a string (e.g., "3"). The input may contain any printable ASCII characters (including spaces, but assume the input is given as a single line without spaces for simplicity, so the string will contain only non-whitespace characters). The length of the string is at least 1.
#include <cassert>
#include <string>

// Function declaration matching the solution
std::string adjustString(std::string s);

int main() {
    // Single character: no swaps
    assert(adjustString("a") == "0");
    // All identical characters: no swaps
    assert(adjustString("aaa") == "0");
    // All different characters: swap at every step
    assert(adjustString("abc") == "2");  // j=2: swap c,b → "acb", j=1: swap c? wait step: original "abc"
    // Compute manually: "abc" length 3, j=2: s[2]='c' != s[1]='b' → swap → "acb", swaps=1
    // j=1: s[1]='c' != s[0]='a' → swap → "cab", swaps=2. Result "2".
    assert(adjustString("ab") == "1");
    assert(adjustString("aba") == "2"); // j=2: 'a'!='b' swap → "aab", j=1: 'a'=='a' no swap → total 1? Wait recheck: "aba" indices: 0='a',1='b',2='a'. j=2: s[2]='a' != s[1]='b' → swap → "aab", swaps=1; j=1: s[1]='a' == s[0]='a' no swap → total 1. So assert should be "1"
    assert(adjustString("aba") == "1");
    assert(adjustString("aab") == "1"); // j=2: 'b'!='a' swap → "aba", j=1: 'a'=='a' no swap → total 1
    assert(adjustString("baa") == "1"); // j=2: 'a'=='a' no swap, j=1: 'a'!='b' swap → total 1
    assert(adjustString("abcd") == "3"); // each pair differs, pass: j=3 swap d,c → "abdc", j=2 swap d,b → "adbc", j=1 swap d,a → "dabc" total 3
    assert(adjustString("hello") == "2"); // h e l l o: j=4 o!=l swap → "helol", j=3 l==l no, j=2 e!=l swap → "hleol", j=1 h!=l? Wait after second swap string "hleol": j=2? Let's just trust the code, but assert "2" is correct from manual: originally "hello": indices 0=h,1=e,2=l,3=l,4=o. j=4: o!=l swap → "helol" (indices: h,e,l,o,l) swaps=1; j=3: o!=l? Wait check: after swap, s[3]='o', s[2]='l' → o!=l, swap → "heoll" swaps=2; j=2: s[2]='o', s[1]='e' → swap → "hoell" swaps=3; j=1: s[1]='o', s[0]='h' → swap → "ohell" swaps=4. Hmm that gives 4. Let me recalc: Actually "hello" length 5. j=4: s[4]='o' vs s[3]='l' → diff → swap → "helol", swaps=1. j=3: s[3]='o' vs s[2]='l' → diff → swap → "heoll", swaps=2. j=2: s[2]='o' vs s[1]='e' → diff → swap → "hoell", swaps=3. j=1: s[1]='o' vs s[0]='h' → diff → swap → "ohell", swaps=4. So "hello" → "4". Change assert.
    assert(adjustString("hello") == "4");
    // Mixed case with digits
    assert(adjustString("a1b2") == "3"); // all adjacent differ
    return 0;
}
#include <string>

// Performs a single right-to-left pass over the string, swapping adjacent
// characters whenever they differ. Returns the total number of swaps as a string.
std::string adjustString(std::string s) {
    int swaps = 0;
    int n = static_cast<int>(s.length());
    for (int j = n - 1; j >= 1; --j) {
        if (s[j] != s[j - 1]) {
            std::swap(s[j], s[j - 1]);
            ++swaps;
        }
    }
    return std::to_string(swaps);
}
// The given snippet performs a single left-to-right? Actually, it scans from `n-1` down to `1` once. For each position `j`, if `line[j] != line[j-1]`, it swaps these two adjacent characters and increments `times`. This is essentially doing one pass from the end to the beginning, swapping any adjacent pair where the characters differ. The algorithm is straightforward: for each index from the last down to the second, compare with the previous character; if different, swap and increment counter. After the loop, return the counter. The original code prints the counter, but our function will return it as a string to match the requirement. Edge cases: length 1 → loop does nothing → returns "0". Strings with all identical characters → no swaps → "0". Strings with alternating characters → swaps at almost every step. The time complexity is O(n) because we iterate exactly n-1 times, each performing constant work. Space complexity is O(1) extra (ignoring the input and output strings). The function should be const-correct: take `const string&` but since we need to modify the input, we must copy it into a local `string s` parameter by value. So signature: `std::string adjustString(std::string s)`.
