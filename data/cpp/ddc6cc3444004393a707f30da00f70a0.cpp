// Write a C++ function `bool isPlayable(const std::string& s)` that determines whether a given string `s` consisting only of lowercase English letters can be reduced to an empty string by repeatedly removing any two adjacent characters that are different from each other, and merging the two characters into a single copy of the second character (i.e., if you have `xy` adjacent with `x != y`, you can replace them with `y`, so the string shrinks by one character each operation). The function should return `true` if there exists a sequence of such operations that eventually results in an empty string, and `false` otherwise. Note that you can perform operations in any order and as many times as you like, but you cannot alter characters except through this merging rule.
#include <cassert>
#include <string>

// Solution function declaration (assume it is defined above)
bool canFullyRemove(const std::string& s);

int main() {
    assert(canFullyRemove("") == true);          // empty string requires no removals
    assert(canFullyRemove("a") == false);        // single character cannot be removed
    assert(canFullyRemove("ab") == true);        // remove the pair
    assert(canFullyRemove("aa") == false);       // no differing adjacent pair
    assert(canFullyRemove("aba") == false);      // any removal leaves one character
    assert(canFullyRemove("abba") == true);      // remove first pair, then second pair
    assert(canFullyRemove("abc") == false);      // after removing (a,b) leaves 'c'
    assert(canFullyRemove("aabb") == false);     // pairs are equal, cannot remove
    assert(canFullyRemove("abab") == true);      // remove (a,b) -> "ab", then remove (a,b)
    assert(canFullyRemove("aaab") == false);     // only one differing pair, leaves 'aa' or 'a'
    return 0;
}
#include <string>
#include <stack>

// Returns true if the entire string can be removed by repeatedly deleting
// adjacent pairs of different characters.
bool canFullyRemove(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (!st.empty() && st.top() != c) {
            st.pop();  // remove the matched pair: top character and current character
            // the current character is consumed, so it is not pushed
        } else {
            st.push(c);
        }
    }
    return st.empty();
}
// The key observation is that the merging operation `xy -> y` (where `x != y`) removes the left character and keeps the right one. Therefore, if we scan the string left to right, we can simulate this process using a stack: when the current character differs from the character on top of the stack, we know that the top character can be removed by merging it with the current one, leaving the current character on top. If they are equal, we just push the current character. However, there is a critical twist: the problem requires us to eventually reduce the entire string to empty, not just to a single distinct character. At the end of this greedy stack simulation, if we have exactly one character left in the stack, that means the string cannot be fully emptied because that one character has no partner to merge with. If the stack is empty, that would mean we somehow removed all characters, but with the rule that each merge reduces length by exactly one and removes the left character, the total number of characters removed equals the number of merges. Since each merge removes one character, to empty the string, the number of merges must equal the string length, which is impossible because each merge consumes exactly two adjacent characters and produces one, so the length reduces by one per merge; starting with length `n`, you need `n-1` merges to reach length 1, and you cannot go below 1. Wait—but the problem says "reduce to an empty string". Is that even possible? Let’s check: if you have a single character, you have no two adjacent characters, so you cannot do anything. For any string of length `n`, after `n-1` merges you get length 1, and then no more moves. So it is impossible to ever reach length 0 under this rule unless the original string length is 0. The given code in the snippet returns `YES` only when the final stack size is not 1? Actually the snippet reads a string, builds a stack-like process (using a string `t`) and at the end prints `YES` if `t.size() != 1`. It prints `NO` when `t.size() == 1`. That suggests that the intended problem is: can you reduce the string such that at least two characters remain? Or maybe the actual rule is different—let’s reinterpret: The snippet's logic: for each character `x` in `s`, if `t` is non-empty and `t.back() != x`, then if `t.size() == 1`, print `NO` and return; otherwise clear `t`. Then append `x` to `t`. At the end, print `NO` if `t.size() == 1`, else `YES`. This suggests that the process is: whenever you have a character different from the top of the stack, if the stack has more than one character, you can pop the entire stack (clear it), but if the stack has exactly one character, you cannot merge it? That is weird.
//
// Given the instruction to create an independent task inspired by the snippet, but not necessarily preserving exact semantics, I will derive a meaningful variation. The original snippet is likely from a problem about reducing adjacent different characters, but the exact rule is ambiguous. To make a clean, solvable task, I propose the following: Given a string of lowercase letters, you may repeatedly choose any adjacent pair `(a,b)` with `a != b` and delete both characters (i.e., remove them from the string). Determine if you can delete the entire string. This is a standard problem (like Codeforces "Remove Adjacent Equal" but here different). The solution: use a stack. If the current character is different from the top of the stack, then we can pair them and pop the top, effectively deleting both. If equal, push. At the end, if the stack is empty, return true; else false. This is clean, has clear edge cases (empty string returns true, single character returns false, "ab" returns true, "aa" returns false, "aab" returns true because you can remove 'a' and 'b' leaving 'a'? Actually "aab": stack push 'a', next 'a' equal push, next 'b' different from top 'a' -> pop 'a' and skip 'b'? That would leave 'a' in stack? Let's simulate: stack after 'a': ['a'], after second 'a': ['a','a'], at 'b': top 'a' != 'b', so pop top and ignore 'b'? That would remove one 'a' and one 'b', leaving one 'a'. Not empty. But you could first remove the middle 'a' and 'b'? They are adjacent? positions 2 and 3 are 'a' and 'b' different, remove them -> "a" left. So not empty. Another order: remove 'a' at pos1 and 'b' at pos3? Not adjacent. So "aab" is impossible. Good.
//
// Thus I will define the task: Given a string `s` of lowercase English letters, you can repeatedly select any adjacent pair of characters that are different and remove both from the string. The string shortens by 2 each time. Determine whether you can remove all characters. Return `true` if possible, `false` otherwise. This is a clear, independent problem. Solution: Use a stack. For each character `c` in `s`: if the stack is non-empty and `stack.top() != c`, then pop the top and discard `c` (they cancel). Else, push `c`. At the end, return `stack.empty()`. Edge cases: empty string -> true (no operations needed). Single character -> false. "ab" -> true. "aa" -> false. "aba" -> stack: push 'a', 'b' differs from top 'a' -> pop 'a', skip 'b'? leaves empty, then push 'a'? Actually need to process each character sequentially: 
// Initialize empty stack. 
// c='a': stack empty -> push 'a' -> stack: ['a']
// c='b': top 'a' != 'b' -> pop -> stack empty, then we discard 'b' (do not push)
// c='a': stack empty -> push 'a' -> stack: ['a']
// Final stack has 'a' -> false. But is it actually possible? "aba": remove first 'a' and 'b' (adjacent) -> leaves "a" -> cannot remove. Remove 'b' and last 'a' -> leaves "a" -> no. So false. 
// "abba": 
// c='a' push ['a']
// c='b' different -> pop -> empty, discard 'b'
// c='b' push ['b']
// c='a' different -> pop -> empty, discard 'a'
// Final empty -> true. Indeed remove first 'a' and first 'b' -> "ba"; then remove 'b' and 'a' -> empty. 
// Thus algorithm works. Complexity O(n) time, O(n) space.
