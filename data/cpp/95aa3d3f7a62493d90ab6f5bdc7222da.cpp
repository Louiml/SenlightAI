Write a C++ function that takes a string `A` consisting only of lowercase English letters and returns a string of the same length where the character at each position `i` is the first non-repeating character among the substring `A[0..i]`. If no such character exists for that prefix, place a `'#'` at that position. For example, for input `"aabc"`, the output should be `"a#bb"` because: at index 0 only `'a'` appears (non-repeating → `'a'`); at index 1 `'a'` repeats so no non‑repeating exists → `'#'`; at index 2 `'a'` repeats but `'b'` is first non‑repeating → `'b'`; at index 3 `'a'` repeats, `'b'` repeats, but `'c'` is first non‑repeating → `'c'`? Wait, recalc: prefix `"aabc"` → counts: a=2,b=1,c=1 → first non‑repeat in order is `'b'`, so the output is `"a#bb"`? Actually let's trace: indices: 0:'a'→"a"; 1:'a'→"a#" (since a repeats); 2:'b'→ prefix "aab": a=2,b=1 → first non‑repeat is b → output "a#b"; 3:'c'→ prefix "aabc": a=2,b=1,c=1 → first non‑repeat is b (b comes before c) → output "a#bb". Yes, correct.
The problem is solved using a hash map to count character occurrences and a queue to maintain the order of characters as they appear. For each character in the input string, increment its count and push it into the queue. Then, while the queue is not empty and the character at the front has a count greater than 1, pop it (it is now a repeating character and can never become non‑repeating again). If the queue becomes empty, no non‑repeating character exists for this prefix, so append `'#'`. Otherwise, the front of the queue is the first character that has appeared exactly once so far, so append that character. This works because the queue preserves the order of first appearance, and we only remove characters once they have been seen more than once. 

Edge cases: an empty string (should return an empty string), a string with all same characters (e.g., `"aaa"` → `"a##"`), and a string where all characters become repeats eventually (e.g., `"abab"` → `"aabb"`? Let's trace: 0:a→"a"; 1:b→"ab"; 2:a→ counts a=2,b=1 → first non‑repeat is b → "abb"; 3:b→ counts a=2,b=2 → no non‑repeat → "abb#"? Actually prefix "abab": counts a=2,b=2 → queue order a,b but both counts >1 → pop both → empty → '#' → output "abb#" yes). Time complexity is O(n) because each character is pushed once and popped at most once, and each operation inside the loop is O(1). Space complexity is O(1) because the alphabet is fixed (26 lowercase letters) but using a hash map we treat it as O(1) in practice, and the queue holds at most 26 distinct characters (but in worst case could hold up to n if all characters are distinct, so O(n) space in the worst case if alphabet size is unbounded, but given the constraint it is O(26)=O(1)). We'll mention O(1) auxiliary space for the map and O(1) for queue given bounded alphabet, but to be safe we can say O(min(n, alphabet_size)) which is O(1) for lowercase letters.
#include <string>
#include <unordered_map>
#include <queue>

// Returns a string where each character at position i is the first non-repeating
// character in the prefix A[0..i], or '#' if none exists.
std::string firstNonRepeating(const std::string& A) {
    std::unordered_map<char, int> count;
    std::queue<char> q;
    std::string ans;
    ans.reserve(A.size());

    for (char ch : A) {
        ++count[ch];
        q.push(ch);

        while (!q.empty() && count[q.front()] > 1) {
            q.pop();
        }

        if (q.empty()) {
            ans.push_back('#');
        } else {
            ans.push_back(q.front());
        }
    }
    return ans;
}
#include <cassert>
#include <string>

// Forward declaration of the solution function (usually in a header)
std::string firstNonRepeating(const std::string& A);

int main() {
    assert(firstNonRepeating("aabc") == "a#bb");
    assert(firstNonRepeating("a") == "a");
    assert(firstNonRepeating("aa") == "a#");
    assert(firstNonRepeating("ab") == "ab");
    assert(firstNonRepeating("abab") == "abb#");
    assert(firstNonRepeating("abcabc") == "abc###");  // Trace: a,b,c then a→b,c? Actually: prefix abcabc: after 3rd a: counts a=2,b=1,c=1 → first non-repeat is b → output a,b,c,b? Let's carefully: indices: 0:a→"a";1:b→"ab";2:c→"abc";3:a→ counts a=2,b=1,c=1 → queue front a count>1 → pop, next b count=1 → output b → "abcb";4:b→ counts a=2,b=2,c=1 → queue front b count>1 pop, next c count=1 → output c → "abcbc";5:c→ counts a=2,b=2,c=2 → all counts>1 → pop all → '#' → "abcbc#". So expected "abcbc#" not "abc###". Let's correct the test.
    assert(firstNonRepeating("abcabc") == "abcbc#");
    assert(firstNonRepeating("zz") == "z#");
    assert(firstNonRepeating("abcd") == "abcd");
    assert(firstNonRepeating("") == "");
    assert(firstNonRepeating("ttt") == "t##");
    // Additional check with mixed pattern
    assert(firstNonRepeating("aabbcc") == "a#b#c#");  // Trace: 0:a→a;1:a→#;2:b→b;3:b→#;4:c→c;5:c→#
    return 0;
}
