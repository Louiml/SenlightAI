/*
Write a C++ function `std::string lexicographicallyLargestDistinctSubsequence(const std::string& s)` that, given a non-empty string `s` consisting only of lowercase English letters, returns the lexicographically largest subsequence of `s` that contains each distinct character of `s` exactly once. In other words, if a character appears multiple times in `s`, it may appear at most once in the result, and the result must contain every distinct character that exists in the original string. The order of characters in the result must respect their original order in `s` (i.e., it is a subsequence), but the goal is to make the resulting string as large as possible lexicographically (standard dictionary order). The function should not modify the input string and should handle any valid lowercase input.
*/
#include <string>
#include <stack>
#include <vector>
#include <algorithm>

// Returns the lexicographically largest subsequence of s that contains each distinct character exactly once.
std::string lexicographicallyLargestDistinctSubsequence(const std::string& s) {
    std::vector<int> remaining(26, 0);
    for (char c : s) {
        remaining[c - 'a']++;
    }

    std::vector<bool> inStack(26, false);
    std::stack<char> st;

    for (char c : s) {
        int idx = c - 'a';
        remaining[idx]--;

        if (inStack[idx]) {
            continue;
        }

        while (!st.empty() && st.top() < c && remaining[st.top() - 'a'] > 0) {
            inStack[st.top() - 'a'] = false;
            st.pop();
        }

        st.push(c);
        inStack[idx] = true;
    }

    std::string result;
    while (!st.empty()) {
        result.push_back(st.top());
        st.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <string>

// The function is declared above; this main verifies it.
int main() {
    // Basic cases
    assert(lexicographicallyLargestDistinctSubsequence("a") == "a");
    assert(lexicographicallyLargestDistinctSubsequence("ab") == "b a" == "ba");
    assert(lexicographicallyLargestDistinctSubsequence("abc") == "cba"? no, "abc" -> largest distinct subsequence is "cba"? Let's check: all distinct, must include all, but keep order: 'a' then 'b' then 'c' -> "abc" is only subsequence with all three, so result is "abc". Actually "cba" is not a subsequence because order must be preserved. So test with "abc" -> "abc".
    assert(lexicographicallyLargestDistinctSubsequence("abc") == "abc");
    assert(lexicographicallyLargestDistinctSubsequence("cba") == "cba");
    assert(lexicographicallyLargestDistinctSubsequence("bca") == "ca"? Let's reason: distinct chars b,c,a. Need all three. Order-preserving subsequences containing all three are only "bca" itself (since all appear once). So result should be "bca". Test that.
    assert(lexicographicallyLargestDistinctSubsequence("bca") == "bca");
    
    // Duplicate characters
    assert(lexicographicallyLargestDistinctSubsequence("aaa") == "a");
    assert(lexicographicallyLargestDistinctSubsequence("abac") == "bc"? Actually distinct chars: a,b,c. Need all three. Subsequences with all three: "abc" (a from pos1, b from pos2, c from pos4) and "bac"? Not order. "bac" not possible because b before a? positions: a1, b2, a3, c4. "abc" = a1,b2,c4. "bac" = b2,a3,c4? but b2 then a3 then c4 is valid "bac"? Actually b2, a3, c4 is a subsequence: b,a,c. Compare "abc" vs "bac": "bac" > "abc" because 'b' > 'a'. So result should be "bac". Check: s="abac" - distinct a,b,c. Greedy: process a (rem a=2 after dec), push a. b (rem b=0 after dec), b > a and a remaining? a remaining =1 after dec? Actually count initial: a=2, b=1, c=1. Process a: rem a=1, not in stack, push a. Process b: rem b=0, b>a and rem[a]=1>0, pop a, push b. Process a: rem a=0, a in stack? no, a< b and rem[b]=0 so no pop, push a → stack: b,a. Process c: rem c=0, c>a and rem[a]=0 so no pop, c>b and rem[b]=0 so no pop, push c → stack: b,a,c → reverse: c,a,b? Wait stack from bottom to top: b, a, c. Pop yields c, a, b, reverse gives b, a, c = "bac". Yes correct.
    assert(lexicographicallyLargestDistinctSubsequence("abac") == "bac");
    
    // Larger example
    assert(lexicographicallyLargestDistinctSubsequence("cbacdcbc") == "cdb a"? Let's compute: distinct: a,b,c,d. Need all four. Known answer from similar problem (LeetCode 316/1081) for smallest distinct subsequence is "acdb", but for largest lexicographic distinct subsequence is "cdba"? Let's manually: s = c b a c d c b c. Positions: c1,b2,a3,c4,d5,c6,b7,c8. Need a,b,c,d. Greedy: process c1 (rem c=3 after dec), push c. b2 (rem b=1), b<c and rem[c]=3>0, pop c, push b. a3 (rem a=0), a<b and rem[b]=1>0, pop b, push a. c4 (rem c=2), c>a and rem[a]=0 so no pop, push c → stack bottom: a,c. d5 (rem d=0), d>c and rem[c]=2>0, pop c, d>a and rem[a]=0, push d → stack: a,d. c6 (rem c=1), c<d and rem[d]=0, c>a and rem[a]=0, push c → stack: a,d,c. b7 (rem b=0), b<c and rem[c]=1>0, pop c, b<d and rem[d]=0, b>a and rem[a]=0, push b → stack: a,d,b. c8 (rem c=0), c>b and rem[b]=0, c<d and rem[d]=0, c>a and rem[a]=0, push c → stack: a,d,b,c. Pop yields c,b,d,a, reverse gives a,d,b,c? Actually bottom: a,d,b,c → pop gives c,b,d,a → reverse gives a,d,b,c = "adbc". But check: is "adbc" lexicographically largest? Let's enumerate all subsequences with distinct set a,b,c,d. Only one occurrence of 'a' (pos3), one 'b' (pos2 or7), one 'c' (pos1,4,6,8), one 'd' (pos5). We must pick a (pos3), d(pos5), and then we need b and c. To maximize lexicographic, we want large characters early. The greedy gave adbc. Compare with other orders: possible orders respecting positions: b must come after a? No, b2 is before a3, but a3 after b2. If we pick b2, then a3, then d5, then c? c after d? c6,c8 after d, so could be b a d c = "badc". Also b a c d? but d after c? c6,c8 are after d5? Actually d5 then c6 gives "badc", but "badc" vs "adbc": compare first char: 'a' vs 'b' → 'b' > 'a', so "badc" is larger. But greedy gave "adbc". Did we miss? Let's re-evaluate greedy with s="cbacdcbc". Let me simulate properly with counts: initial counts: a=1, b=2, c=4, d=1? Actually s: c,b,a,c,d,c,b,c → a:1, b:2, c:4, d:1. Process:
i0 c: rem c=3, push c. stack [c]
i1 b: rem b=1, b<c and rem[c]=3>0, pop c, push b. stack [b]
i2 a: rem a=0, a<b and rem[b]=1>0, pop b, push a. stack [a]
i3 c: rem c=2, c>a and rem[a]=0, push c. stack [a,c]
i4 d: rem d=0, d>c and rem[c]=2>0, pop c, d>a and rem[a]=0, push d. stack [a,d]
i5 c: rem c=1, c<d and rem[d]=0, c>a and rem[a]=0, push c. stack [a,d,c]
i6 b: rem b=0, b<c and rem[c]=1>0, pop c, b<d and rem[d]=0, b>a and rem[a]=0, push b. stack [a,d,b]
i7 c: rem c=0, c>b and rem[b]=0, c<d and rem[d]=0, c>a and rem[a]=0, push c. stack [a,d,b,c]
Result stack bottom->top: a,d,b,c -> pop reverse gives "adbc". But as argued, "badc" is lexicographically larger because 'b' > 'a'. So the greedy is flawed? Wait, the algorithm from the snippet is for *largest* subsequence, but the classic problem (LeetCode 316) is for *smallest* subsequence. For largest, the comparison should be `st.top() < s[i]`? In the given snippet, it uses `st.top() < s[i]` meaning it pops smaller characters, leading to larger result. But my manual example suggests that "adbc" is not the largest. Let me enumerate all valid subsequences containing a,b,c,d exactly once. Positions: a only at 3, d only at 5. Then we need b and c such that order is a subsequence. Possible orders respecting original positions:
- Pick a3, then d5, then we can pick b7 and c8? But order a3,d5,b7,c8 gives "adbc". Also a3,d5,c6,b7 gives "adcb" because order a,d,c,b is possible? a3,d5,c6,b7 → yes, that's "adcb". Compare "adcb" vs "adbc": at first three same "adc" vs "adb", 'c' > 'b', so "adcb" > "adbc". Also a3,b7,d5? No, b7 is after d5, so can't have b before d if we pick d5. a3,b2? b2 is before a3, so if we pick a3 we cannot pick b2 because order would be b2 then a3? Actually subsequence order must follow original order. If we pick b2, then a3 is after b2, so we can pick b2,a3,d5,c8 → "badc". Compare "badc" vs "adcb": first char 'b' > 'a', so "badc" is larger. Also "bacd"? Can we have b2,a3,c4,d5 → "bacd": compare "bacd" vs "badc": "bacd" vs "badc": first three "bac" vs "bad", 'c' < 'd', so "badc" larger. Also "bcad"? b2,c4,a3? No a3 is before c4? Actually a3 is index 2 (0-based), c4 index 3, so b2,a3,c4,d5 is valid. What about b2,c6,a3? No a3 is before c6. So best is "badc"? Check "bdca"? b2,d5,c6,a3? a3 is before d5, so can't have a after d. "bdc a" no. So largest is "badc". But greedy gave "adbc". So the algorithm as given is incorrect for producing largest? Let me check the original snippet: it uses `cnt[st.top() - 'a'] != 0` which is correct, and `st.top() < s[i]` which pops smaller. But it also has a condition: `if (visited[s[i] - 'a']) continue;` which skips if already present. In our example, when we process the second 'c' at i5, it is already visited? At i3 we pushed c, so i5's c is visited, so it skips, but we popped c at i4, so visited[c] became 0, so at i5 we push c. That's fine. However, the issue is that when we process the 'b' at i6, we have stack [a,d,c] and b<c, so we pop c because rem[c]=1>0, giving [a,d], then b<d and rem[d]=0, so we push b, giving [a,d,b]. Then i7 c: stack [a,d,b], c>b, but rem[b]=0, c<d, so push c → [a,d,b,c]. That gives "adbc". But the optimal is "badc" which would require not popping 'c' at i4? Let's see: if we kept 'c' instead of 'd'? Actually to get "badc", we need to pick b2 first, then a3, then d5, then c8. The greedy processed b2 but then popped it when a3 came because a<b and rem[b]>0. That is correct for smallest subsequence, but for largest we might want to keep b2? However, if we keep b2, then when a3 comes, we have stack [b], a<b and rem[b]=1>0, so we would pop b to make room for a? That is the opposite. Actually for largest, we want to pop smaller when a larger appears later. But b is smaller than a? No, 'b' > 'a'. So when 'a' appears, we should NOT pop 'b' because 'b' is larger. But the condition `st.top() < s[i]` pops when stack top is SMALLER than current. Here st.top()='b' and s[i]='a', so 'b' < 'a' is false, so no pop. That means with the given algorithm, when processing a3, stack is [b] from previous, we don't pop b, we push a → [b,a]. Then at c4, top is 'a' < 'c' and rem[a]=0, so no pop, push c → [b,a,c]. At d5, top 'c' < 'd' and rem[c]? At start counts c=4, after processing c1 (rem c=3), b2 (rem b=1), a3 (rem a=0), c4 (rem c=2), so rem[c]=2>0, pop c, then top 'a' < 'd' and rem[a]=0, push d → [b,a,d]. At c6, top 'd'? Actually stack [b,a,d], top='d', c<d, and rem[d]=0, also c > a? but top is d, so no pop, push c → [b,a,d,c]. At b7, top 'c'? Actually stack [b,a,d,c], top='c', b<c and rem[c]=1>0, pop c, then top 'd', b<d and rem[d]=0, then top 'a', b>a and rem[a]=0, then top 'b', b==b? Actually stack after popping c: [b,a,d], top='d', then b<d, rem[d]=0, then top='a', b>a, rem[a]=0, then top='b', b==b, no pop, push b? But wait we have visited[b]? We set visited[b]=1 earlier when pushed b at i1. Then when we popped b? Did we ever pop b? No, we didn't pop b because at a3 we did not pop b (since b > a). So b remains in stack from i1. But we are now at i6 (b7), and b is already visited, so we should skip. Indeed, in the algorithm, at i6 we have `if (visited[s[i] - 'a']) continue;` because b is already in stack. So we skip b7. Then i7 c: stack [b,a,d,c]? Actually after skipping b, we process c8, top is c? Stack is [b,a,d] (because we had [b,a,d,c]? Wait, let's redo carefully with algorithm:

Initialize counts: a=1, b=2, c=4, d=1.
Loop:
i0 c: rem c=3, visited[c]=0, while st not empty? empty, push c, visited[c]=1. stack [c]
i1 b: rem b=1, visited[b]=0, while st.top()='c' < 'b'? false (c > b), so no pop, push b, visited[b]=1. stack [c,b] (bottom c, top b)
i2 a: rem a=0, visited[a]=0, while st.top()='b' < 'a'? false (b > a), so no pop, push a, visited[a]=1. stack [c,b,a]
i3 c: rem c=2, visited[c]=1 so skip.
i4 d: rem d=0, visited[d]=0, while st.top()='a' < 'd' and rem[a]=0? condition requires cnt[st.top()-'a'] != 0. rem[a]=0, so condition false. So no pop. Then push d. stack [c,b,a,d]
i5 c: rem c=1, visited[c]=0 (since we had visited[c]=1 from i0 but never popped c, so visited[c] still 1? Actually we never popped c, so visited[c] remains 1. So skip c6? Wait at i5 index is c (the 6th character), visited[c] is still 1 because we never popped c, so we skip. So we don't push this c.
i6 b: rem b=0, visited[b]=1 (we never popped b), so skip.
i7 c: rem c=0, visited[c]=1, skip.
End stack: [c,b,a,d] bottom to top. Pop yields d,a,b,c, reverse gives c,b,a,d = "cbad". Is "cbad" valid? It contains c (i0), b (i1), a (i2), d (i4) as subsequence order. Compare "cbad" vs "badc": first char 'c' > 'b', so "cbad" is larger! Yes, so the algorithm gives "cbad" which is indeed lexicographically largest. My earlier simulation was wrong because I incorrectly popped b at a3 (I thought we pop, but condition says pop when top < current, not top > current). The algorithm is correct. So the test case should be "cbad". Let's verify with the given snippet logic: It indeed would produce "cbad". So in the test, assert(lexicographicallyLargestDistinctSubsequence("cbacdcbc") == "cbad").
    
    // More tests
    assert(lexicographicallyLargestDistinctSubsequence("bbac") == "cba"? Let's check: distinct a,b,c. Positions: b1,b2,a3,c4. The only subsequence with all three is b1,a3,c4 = "bac", or b2,a3,c4 = "bac". "cba" not possible because c is last. "bac" is the only. But maybe "bca"? b1,c4,a3? No a3 before c4. So result "bac". Test.
    assert(lexicographicallyLargestDistinctSubsequence("bbac") == "bac");
    
    // String with all distinct increasing order
    assert(lexicographicallyLargestDistinctSubsequence("abcdefghijklmnopqrstuvwxyz") == "abcdefghijklmnopqrstuvwxyz");
    
    // String with all distinct reverse order
    assert(lexicographicallyLargestDistinctSubsequence("zyxwvutsrqponmlkjihgfedcba") == "zyxwvutsrqponmlkjihgfedcba");
    
    // Single character repeated
    assert(lexicographicallyLargestDistinctSubsequence("zzzzz") == "z");
    
    return 0;
}
// The problem is a classic greedy stack-based algorithm used to build the lexicographically largest subsequence with distinct characters. The idea is to iterate through the string while maintaining a stack that represents the current best subsequence built so far. For each character, we first decrease its remaining count (the number of occurrences still to be processed later). If the character is already in the stack, we skip it (since duplicate characters cannot appear again). Otherwise, while the stack is not empty, the top of the stack is smaller than the current character, and the top character still appears later in the string (remaining count > 0), we pop the top because we can replace it with the current larger character later in the string (this yields a larger lexicographic result). After popping, we mark that character as not visited. Then we push the current character onto the stack and mark it as visited. After processing the entire string, the stack contains the final result in reverse order; we build the output string by popping characters and prepending them. The time complexity is O(n) because each character is pushed and popped at most once. The space complexity is O(1) for the visited/count arrays (of size 26) and O(n) for the stack and result string in the worst case. Edge cases include strings with all identical characters (the result is just that single character), strings already in increasing order (the stack never pops), and strings in decreasing order (frequent pops, but still linear).
