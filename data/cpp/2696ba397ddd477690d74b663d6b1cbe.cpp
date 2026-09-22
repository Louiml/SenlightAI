/*
Write a C++ function that, given a string `s` consisting only of lowercase English letters, returns the minimum number of steps required to delete the entire string. In one step, you may delete any contiguous substring that is a palindrome. A single character is considered a palindrome. The function must compute the result using dynamic programming on the substring intervals, exactly as modeled by the provided snippet, but with a clear interface and proper const correctness. The input length will be between 1 and 500.
*/
#include <string>
#include <vector>
#include <algorithm>

/**
 * Returns the minimum number of steps to delete the entire string.
 * In one step, a contiguous palindrome substring can be deleted.
 * A single character counts as a palindrome.
 */
int minStepsToDeleteString(const std::string& s) {
    const int n = static_cast<int>(s.length());
    if (n == 0) return 0;

    // dp[i][j] = minimum steps to delete substring s[i..j] (inclusive)
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

    for (int len = 1; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            const int j = i + len - 1;
            if (len == 1) {
                dp[i][j] = 1;
                continue;
            }

            // Strategy 1: delete s[i] alone, then delete the rest
            dp[i][j] = 1 + dp[i + 1][j];

            // Strategy 2: if first two characters are equal,
            // delete s[i] and s[i+1] together (if s[i+1] is present)
            if (i + 1 <= j && s[i] == s[i + 1]) {
                if (i + 2 <= j) {
                    dp[i][j] = std::min(dp[i][j], 1 + dp[i + 2][j]);
                } else {
                    dp[i][j] = std::min(dp[i][j], 1);
                }
            }

            // Strategy 3: for each later matching character,
            // delete the inner part first, then the outer pair
            // and the remaining suffix.
            for (int k = i + 2; k <= j; ++k) {
                if (s[i] == s[k]) {
                    int innerCost = (i + 1 <= k - 1) ? dp[i + 1][k - 1] : 0;
                    int suffixCost = (k + 1 <= j) ? dp[k + 1][j] : 0;
                    dp[i][j] = std::min(dp[i][j], innerCost + suffixCost);
                }
            }
        }
    }

    return dp[0][n - 1];
}
#include <cassert>
#include <string>

// The solution function is defined above (or included before this).
// We assume it is declared in the same translation unit.

int main() {
    // Single character
    assert(minStepsToDeleteString("a") == 1);

    // Two identical characters -> delete both as palindrome in one step
    assert(minStepsToDeleteString("aa") == 1);

    // Two different characters -> need two steps
    assert(minStepsToDeleteString("ab") == 2);

    // Entire string is a palindrome
    assert(minStepsToDeleteString("ababa") == 1);

    // Alternating string
    assert(minStepsToDeleteString("abab") == 2); // delete "aba" then "b" ? Actually "aba" palindrome, then "b" -> 2 steps; or "bab"+"a" -> 2

    // All same characters -> one delete of whole palindrome
    assert(minStepsToDeleteString("aaaa") == 1);

    // Mixed case
    assert(minStepsToDeleteString("abcba") == 1);

    // Non-palindrome with same endpoints
    assert(minStepsToDeleteString("abca") == 2); // delete "abc"? Not palindrome, better: delete "a" then "bc" then "a"? Actually compute: dp[0][3] for "abca": len4, dp=1+dp[1][3], dp[1][3] for "bca" -> 1+dp[2][3]=1+1=2? dp[2][3]="ca" -> 2, so dp[1][3]=? also check s[1]==s[3]? 'b'!='a', so dp[1][3]=1+dp[2][3]=1+2=3? Wait len3: dp[1][3] = 1+dp[2][3] =1+2=3, but also check s[1]==s[2]? no. So dp[0][3]=1+3=4. But also for K=3, s[0]=='a'==s[3]'a', then dp[1][2] + dp[4][3] = dp[1][2] (for "bc") = 2, total 2. So answer 2. That's correct.

    // Longer test
    assert(minStepsToDeleteString("abacaba") == 1); // whole is palindrome

    // Random example
    assert(minStepsToDeleteString("abcddcba") == 1); // palindrome

    // Example from known problem
    assert(minStepsToDeleteString("abba") == 1); // palindrome

    return 0;
}
// The key observation is that deleting a palindrome substring in one step can be combined with deletions of adjacent or inner parts. Define `dp[i][j]` as the minimal steps to delete substring `s[i..j]` (inclusive).  
// Base case: for length 1, `dp[i][i] = 1`.  
// For longer substrings, we consider two strategies:  
// 1. Delete the first character alone (1 step) and then delete the rest: `1 + dp[i+1][j]`.  
// 2. If the first character matches a later character at position `k` (where `k > i`), we can delete the substring `s[i..k]` as a palindrome in one step—but only if the inner part `s[i+1..k-1]` is deleted first (cost `dp[i+1][k-1]`), then delete the outer palindrome in 1 step, plus delete the remainder `s[k+1..j]` (cost `dp[k+1][j]`). However, the snippet uses a slightly different formulation: it treat the entire `s[i..k]` as a palindrome (since `s[i]==s[k]` and the inner part is deleted separately), but the cost is `dp[i+1][k-1] + dp[k+1][j]` (no extra +1 for the outer deletion? Actually the snippet uses `dp[i+1][k-1] + dp[k+1][j]` directly, meaning the outer palindrome deletion is merged with the inner deletion step? Let’s examine: The idea is that after deleting the inner palindrome `s[i+1..k-1]`, the characters `s[i]` and `s[k]` become adjacent, and since they are equal, they can be deleted together in one step with whatever remains? Actually the known problem "Minimum steps to delete a string" (LeetCode 1547? No, this is a known dynamic programming problem) has a recurrence: `dp[i][j] = 1 + dp[i+1][j]`, and if `s[i]==s[i+1]` then `dp[i][j] = min(dp[i][j], 1 + dp[i+2][j])`, and for any `k` with `s[i]==s[k]`, `dp[i][j] = min(dp[i][j], dp[i+1][k-1] + dp[k+1][j])`. That is exactly the snippet. The reasoning: When `s[i]==s[k]`, the inner part `s[i+1..k-1]` must be deleted first (cost `dp[i+1][k-1]`), then `s[i]` and `s[k]` become adjacent and equal, and they can be deleted together with the remaining substring `s[k+1..j]`? Actually the known solution does not add +1 for the outer pair because they are part of the same deletion step? Wait, but the recurrence does not add +1 for the outer deletion, so how do we delete `s[i]` and `s[k]`? The correct logic is: If `s[i]==s[k]`, you can delete the whole substring `s[i..k]` in one step only if the inside is a palindrome, but here we are not requiring it to be a palindrome; instead we allow the inside to be deleted in multiple steps, and then the outer characters become adjacent and can be deleted along with the inner? Actually the known problem "Remove Boxes" is different. For this problem, the standard recurrence is: `dp[i][j] = 1 + dp[i+1][j]` (delete first char alone), and if `s[i]==s[i+1]`, `dp[i][j] = min(dp[i][j], 1 + dp[i+2][j])`. And for any `k` with `s[i]==s[k]`, `dp[i][j] = min(dp[i][j], dp[i+1][k-1] + dp[k+1][j])`. The reasoning: If `s[i]==s[k]`, you can delete the inner part `s[i+1..k-1]` first, then you have `s[i]` and `s[k]` adjacent, and because they are equal, you can delete them together in one step along with the remaining `s[k+1..j]`? But the recurrence does not add +1 for that step. Actually it does: the `dp[i+1][k-1]` is the cost to delete the inside, and then you need one more step to delete the pair `s[i]` and `s[k]`? But the recurrence adds `dp[k+1][j]` without an extra +1, implying that the deletion of `s[i]` and `s[k]` is combined with the deletion of the rest? That seems off. Let me re-read the snippet: For `K` from `i+2` to `j`, if `str[i]==str[K]`, then `dp[i][j] = min(dp[i+1][K-1] + dp[K+1][j], dp[i][j])`. This is a known recurrence from the problem "Minimum steps to delete string" (GeeksforGeeks). The interpretation: If `s[i]==s[k]`, then you can delete `s[i]` and `s[k]` together with the substring `s[k+1..j]` in the same step? Actually the standard explanation: The idea is that you delete the substring `s[i+1..k-1]` first (cost `dp[i+1][k-1]`). Then you have `s[i]` followed by `s[k]` adjacent. Since they are equal, they can be deleted together in one step with `s[k+1..j]`? But the recurrence does not add +1 for that deletion. I think the actual logic is: When `s[i]==s[k]`, you can delete the entire substring `s[i..k]` in one step only if the inside `s[i+1..k-1]` is deleted in previous steps? But you cannot delete the inside without also deleting the outer? The correct recurrence known from the problem "Minimum steps to delete a string" (where you can delete a palindrome substring in one step) is:  
// `dp[i][j] = 1 + dp[i+1][j]` (delete first char alone).  
// If `s[i]==s[i+1]`, then `dp[i][j] = min(dp[i][j], 1 + dp[i+2][j])`.  
// For any `k` with `s[i]==s[k]` and `k>i+1`, we do: `dp[i][j] = min(dp[i][j], dp[i+1][k-1] + dp[k+1][j])`.  
// This recurrence is correct if we consider that when `s[i]==s[k]`, the substring `s[i..k]` is not necessarily a palindrome, but we can delete `s[i+1..k-1]` first (in `dp[i+1][k-1]` steps), then the remaining substring is `s[i] + s[k] + s[k+1..j]`. Since `s[i]==s[k]`, the pair `s[i]` and `s[k]` are adjacent and equal. But they are not a palindrome by themselves? A pair of identical characters is a palindrome of length 2, so they can be deleted in one step. However, that step also can combine with `s[k+1..j]`? Actually you would delete `s[i]` and `s[k]` together in one step, then delete `s[k+1..j]` in `dp[k+1][j]` steps, giving total `dp[i+1][k-1] + 1 + dp[k+1][j]`. But the recurrence lacks the `+1`. The correct known solution for this problem (from GeeksforGeeks "Minimum steps to delete a string") actually uses `dp[i][j] = min(dp[i][j], dp[i+1][k-1] + dp[k+1][j])` without an extra +1, because the deletion of `s[i]` and `s[k]` is considered "free" when they are equal? That is odd. Let me check: The problem "Minimum steps to delete a string" where you can delete a palindrome substring in one step has a known solution:  
// `dp[i][j] = 1 + dp[i+1][j]`  
// If `s[i]==s[i+1]`, then `dp[i][j] = min(dp[i][j], 1 + dp[i+2][j])`  
// For `k` from `i+2` to `j`, if `s[i]==s[k]`, then `dp[i][j] = min(dp[i][j], dp[i+1][k-1] + dp[k+1][j])`  
// And indeed that is what the snippet does. The reasoning is that when `s[i]==s[k]`, you can delete the inner part `s[i+1..k-1]` first, and then the two equal characters `s[i]` and `s[k]` become adjacent, and since they are equal, they can be deleted together with the rest `s[k+1..j]`? No, they can be deleted together as one palindrome (length 2) in one step, but the recurrence does not count that step? Actually maybe the recurrence counts that step implicitly because the `dp[i+1][k-1]` already includes the deletion of all characters between them, and then after that, the characters `s[i]` and `s[k]` are adjacent, but you need one step to delete them. However, the recurrence `dp[i][j]` is defined as the minimum steps to delete `s[i..j]`. If you delete `s[i+1..k-1]` in `dp[i+1][k-1]` steps, then you have the string `s[i] + s[k] + s[k+1..j]`. You can delete `s[i]` and `s[k]` together in one step (since they are equal, palindrome length 2), and then delete `s[k+1..j]` in `dp[k+1][j]` steps, total `dp[i+1][k-1] + 1 + dp[k+1][j]`. But the recurrence uses `dp[i+1][k-1] + dp[k+1][j]` without the +1. Why? Because the deletion of `s[i]` and `s[k]` can be combined with the deletion of `s[k+1..j]`? Not possible. Actually the known trick: If `s[i]==s[k]`, then you can delete the whole substring `s[i..k]` together with the part `s[k+1..j]`? No. Let me think of an example: "ababa". dp[0][4]?  
// The correct answer for "ababa" is 3? Let's manually: delete "aba" (palindrome) and "ba"? Not sure. Actually let's check known problem: Given a string, you can delete a palindrome substring in one step. Minimum steps. For "ababa" you can delete "ababa" (which is palindrome) in 1 step. So answer 1. The recurrence: dp[0][4]: len=5. i=0,j=4. dp[0][4] = 1 + dp[1][4]. dp[1][4] = ? string "baba" – not palindrome. dp[1][4] = 1+dp[2][4]=1+dp[2][4] where "aba" is palindrome so dp[2][4]=1, so dp[1][4]=2. So dp[0][4] = 3. But we know answer is 1. So there must be another path: for K from 2 to 4, if s[0]==s[K]? s[0]='a', s[2]='a', s[4]='a'. For K=2: dp[1][1] + dp[3][4] = 1 + (dp[3][4]) where "ba" is not palindrome, dp[3][4]=? len=2, dp[3][4]=1+dp[4][4]=2, so total 1+2=3. For K=4: dp[1][3] + dp[5][4]? dp[5][4] is out of bounds? Actually j=4, so K<=j, K=4, then dp[1][3] + dp[5][4] but dp[5][4] is invalid because i>j? In the snippet, they use dp[K+1][j] where K=4 gives dp[5][4] which is outside the initialized 0? They initialize dp[i][j]=0 for all, and for len loop, they only fill when j<N, but accessing dp[5][4] would be out of bounds because N=5, dp[5][4] is actually in the 2D array (N+1 x N+1), so dp[5][4] exists and is 0. So for K=4, dp[1][3] + dp[5][4] = dp[1][3] + 0. dp[1][3] is substring "bab" – not palindrome, dp[1][3] = 1+dp[2][3]=1+1=2? Actually dp[2][3]='a'? Wait s[1..3]="bab", dp[1][3]=1+dp[2][3]=1+1=2, total 2. So dp[0][4] = min(3,3,2)=2. Not 1. But the correct answer is 1 because the whole string is palindrome. So the recurrence might be wrong? Actually I recall that this problem is slightly different: You can delete any palindrome substring in one step, and you want minimum steps. The recurrence given is for "minimum steps to delete string where you can delete a palindrome substring" but the recurrence might have an extra case: you also consider whole substring being palindrome, which is covered by the case K=j if s[i]==s[j] and the inner is a palindrome? Not necessarily. Actually the known solution for this problem (e.g., LeetCode 1547? No, this is "Strange Printer" which is different). Let me search memory: There is a problem "Minimum steps to delete a string" on GeeksforGeeks where you can delete a palindrome substring in one step. The solution uses DP with recurrence as given. But I think the recurrence as given is correct. Let me test with "ababa": dp[0][4] should be 1 because the whole is palindrome. With recurrence, for K=4, dp[1][3] + dp[5][4] = dp[1][3] + 0. dp[1][3] for "bab" is? "bab" is palindrome, so dp[1][3] should be 1. Then dp[0][4] = min(1+dp[1][4], ... , 1+0)=1. So if dp[1][3] computes correctly as 1, then good. Let's compute dp[1][3] for "bab": len=3, i=1,j=3. dp[1][3] = 1+dp[2][3] (dp[2][3] for "a" is 1) =2. But also check s[1]==s[3]? 'b'=='b', then for K from i+2=3 to 3: K=3, dp[2][2] + dp[4][3] = 1+0=1. So dp[1][3]=min(2,1)=1. Good. Then dp[0][4] with K=4 gives dp[1][3]+0=1. So correct. So the recurrence works. So the algorithm: For each substring length from 1 to N, compute dp[i][j] for all i,j. Initialize dp to 0. For len=1, dp[i][i]=1. For len>1: dp[i][j] = 1 + dp[i+1][j] (note dp[i+1][j] must have length len-1, which is already computed because we iterate len increasing). If i+1 <= j and s[i]==s[i+1], dp[i][j] = min(dp[i][j], 1 + dp[i+2][j]) (where dp[i+2][j] length len-2, already computed). Then for K from i+2 to j, if s[i]==s[K], dp[i][j] = min(dp[i][j], dp[i+1][K-1] + dp[K+1][j]). Here dp[i+1][K-1] has length K-1 - (i+1) +1 = K-i-1, which is less than len, so computed. dp[K+1][j] has length j - (K+1)+1 = j-K, also less than len. So all dependencies have shorter lengths, so DP works. Edge cases: empty string? Problem likely non-empty, but we can handle length 0 by returning 0. The input may contain only lowercase letters. Complexity: O(N^3) time due to the loop over K, O(N^2) space. N up to 500, so O(125 million) operations, which is okay in C++ within typical limits.
