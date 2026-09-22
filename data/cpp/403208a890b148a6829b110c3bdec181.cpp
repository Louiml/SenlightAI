Write a C++ function that, given three binary strings of equal even length `2n` (in no particular order) and an integer `n`, constructs and returns a single binary string of length exactly `3n` such that all three input strings appear as subsequences of the returned string, not necessarily contiguously. The function signature is `std::string mergeTernarySubsequences(int n, const std::string& a, const std::string& b, const std::string& c)`. The output string must have exactly `3n` characters, and each of the three inputs must be a subsequence of it. When multiple valid outputs exist, any correct one is acceptable. The inputs are guaranteed to be valid binary strings (only '0' and '1') each of length `2n`, and `n` is positive. The solution must be efficient for sizes up to `n = 200,000`, so avoid quadratic-time approaches.
#include <cassert>
#include <string>

// The function to test is declared above (e.g., in a header).
// Here we provide the test driver.

int main() {
    // Basic test: n=1, strings of length 2
    {
        int n = 1;
        std::string a = "00";
        std::string b = "01";
        std::string c = "11";
        std::string res = mergeTernarySubsequences(n, a, b, c);
        assert(res.size() == 3);
        // Verify each is a subsequence
        auto isSubseq = [](const std::string& text, const std::string& pat) {
            size_t j = 0;
            for (size_t i = 0; i < text.size() && j < pat.size(); ++i) {
                if (text[i] == pat[j]) ++j;
            }
            return j == pat.size();
        };
        assert(isSubseq(res, a));
        assert(isSubseq(res, b));
        assert(isSubseq(res, c));
    }

    // n=2, all identical strings
    {
        int n = 2;
        std::string s = "1010";
        std::string res = mergeTernarySubsequences(n, s, s, s);
        assert(res.size() == 6);
        // Should contain s as subsequence; simplest check is that res contains all characters
        auto isSubseq = [](const std::string& text, const std::string& pat) {
            size_t j = 0;
            for (size_t i = 0; i < text.size() && j < pat.size(); ++i) {
                if (text[i] == pat[j]) ++j;
            }
            return j == pat.size();
        };
        assert(isSubseq(res, s));
    }

    // n=3, mixed strings
    {
        int n = 3;
        std::string a = "010101";
        std::string b = "101010";
        std::string c = "000111";
        std::string res = mergeTernarySubsequences(n, a, b, c);
        assert(res.size() == 9);
        auto isSubseq = [](const std::string& text, const std::string& pat) {
            size_t j = 0;
            for (size_t i = 0; i < text.size() && j < pat.size(); ++i) {
                if (text[i] == pat[j]) ++j;
            }
            return j == pat.size();
        };
        assert(isSubseq(res, a));
        assert(isSubseq(res, b));
        assert(isSubseq(res, c));
    }

    // n=4, stress test with many zeros and ones
    {
        int n = 4;
        std::string a = "00001111";
        std::string b = "11110000";
        std::string c = "01010101";
        std::string res = mergeTernarySubsequences(n, a, b, c);
        assert(res.size() == 12);
        auto isSubseq = [](const std::string& text, const std::string& pat) {
            size_t j = 0;
            for (size_t i = 0; i < text.size() && j < pat.size(); ++i) {
                if (text[i] == pat[j]) ++j;
            }
            return j == pat.size();
        };
        assert(isSubseq(res, a));
        assert(isSubseq(res, b));
        assert(isSubseq(res, c));
    }

    // n=5, one string all zeros, one all ones, one alternating
    {
        int n = 5;
        std::string a(10, '0');
        std::string b(10, '1');
        std::string c = "0101010101";
        std::string res = mergeTernarySubsequences(n, a, b, c);
        assert(res.size() == 15);
        auto isSubseq = [](const std::string& text, const std::string& pat) {
            size_t j = 0;
            for (size_t i = 0; i < text.size() && j < pat.size(); ++i) {
                if (text[i] == pat[j]) ++j;
            }
            return j == pat.size();
        };
        assert(isSubseq(res, a));
        assert(isSubseq(res, b));
        assert(isSubseq(res, c));
    }

    // n=1, extreme case with all zeros and all ones
    {
        int n = 1;
        std::string a = "00";
        std::string b = "00";
        std::string c = "11";
        std::string res = mergeTernarySubsequences(n, a, b, c);
        assert(res.size() == 3);
        auto isSubseq = [](const std::string& text, const std::string& pat) {
            size_t j = 0;
            for (size_t i = 0; i < text.size() && j < pat.size(); ++i) {
                if (text[i] == pat[j]) ++j;
            }
            return j == pat.size();
        };
        assert(isSubseq(res, a));
        assert(isSubseq(res, b));
        assert(isSubseq(res, c));
    }

    return 0;
}
#include <string>
#include <vector>
#include <algorithm>

// Given three binary strings a, b, c each of length 2*n,
// return a binary string of length exactly 3*n that contains
// a, b, and c as subsequences.
std::string mergeTernarySubsequences(int n, const std::string& a, const std::string& b, const std::string& c) {
    std::vector<int> pos = {0, 0, 0};      // current positions in each string
    std::vector<std::string> strs = {a, b, c};
    std::string result;
    result.reserve(3 * n);

    while (pos[0] < 2 * n || pos[1] < 2 * n || pos[2] < 2 * n) {
        int cnt0 = 0, cnt1 = 0;
        // Count current characters among strings not yet fully consumed
        for (int i = 0; i < 3; ++i) {
            if (pos[i] < 2 * n) {
                if (strs[i][pos[i]] == '0') ++cnt0;
                else ++cnt1;
            }
        }
        // Choose majority character (if tie, choose '0')
        char ch = (cnt1 > cnt0) ? '1' : '0';
        result.push_back(ch);

        // Advance all pointers whose current character matches ch
        for (int i = 0; i < 3; ++i) {
            if (pos[i] < 2 * n && strs[i][pos[i]] == ch) {
                ++pos[i];
            }
        }
    }

    // If result is shorter than 3*n (possible only when all pointers finished early),
    // pad with '0' to reach exactly 3*n.
    while (static_cast<int>(result.size()) < 3 * n) {
        result.push_back('0');
    }

    return result;
}
// The problem is a classic constructive algorithm based on the pigeonhole principle on pairs of strings. Since there are three strings of length `2n`, and each position in a string is either '0' or '1', by considering any two strings, the majority bit at the current positions of those two strings can be chosen to advance at least one of them. A greedy strategy works: maintain pointers `p1`, `p2`, `p3` starting at 0 for the three strings. At each step, look at the characters at the current pointers of all three strings (if some pointers have reached the end, we skip them). Count how many pointers show '0' and how many show '1'. Choose the character that appears at least as many times as the other (in the original code, they choose the character with the smaller count if counts differ, which effectively chooses the majority when comparing counts; the code uses `(tot[0]<tot[1])` meaning choose '0' if fewer '0's, otherwise '1' — this is a tie-breaking that still advances at least two pointers? Actually let's reason more carefully: the original code counts how many current characters are '0' and how many are '1' among the three pointers. It then chooses `now = (tot[0] < tot[1]) ? '0' : '1'`. This means if `tot[1] > tot[0]` (more '1's), it chooses '0', which is the minority. But then it advances only those pointers that have `now`. This could advance only one pointer if the minority count is 1. However, the known correct approach is to choose the majority character, which guarantees advancing at least two pointers (or all three). The provided code chooses the minority? Let's re-read: `tot[0]=0; tot[1]=0;` then increments. `char now=(tot[0]<tot[1])+'0';` So if `tot[1]` is greater than `tot[0]`, `now` becomes '0'. That means if more pointers show '1', we chose '0'. That is choosing the character with *fewer* occurrences. That would advance only the minority pointers, potentially only one. But the code then later appends leftover from one string and pads with zeros. Is that guaranteed to work? The standard known solution (see Codeforces problem "Ternary String" or similar) actually chooses the majority character to ensure at least two pointers advance, so after at most `3n` steps all three pointers reach the end. The provided snippet seems to choose minority, but the problem guarantees that the output length is exactly `3n` and all three are subsequences. Let me verify with an example: n=1, strings "00", "00", "11". Current pointers at 0: chars '0','0','1' → tot[0]=2, tot[1]=1 → since tot[0]<tot[1] is false, now='1'. Advance only pointer3 to 1. Now strings: p1=0('0'), p2=0('0'), p3=1('1') → tot[0]=2,tot[1]=1 → now='1' again, advance p3 to 2 (end). Now len=2. Then we have p1=0,p2=0, p3>2n. Since p3>2n, op = (p1>p2?1:2) = 2. Then append rest of string2: s[2] from pos 0 to end, which is "00", so output becomes "11"+"00" = "1100", length 4 which is 3n? For n=1, 3n=3, but we got length 4. But the code later does `while(len<n+n+n) Ans[++len]='0';` which would pad to length 3n=3, but the original string was "1100" and padding would make it length 3? Actually the code's `while (pos[op]<=n+n) Ans[++len]=s[op][pos[op]],pos[op]++;` appends rest of the chosen string until its end. In the example, op=2 and string2 is "00", so from pos 0 to end, that's 2 characters, making len become 4 already. Then `while(len<n+n+n)` pad with zeros, but len=4 > 3, so no padding, and then it prints a 4-character string, which is wrong for the problem statement (output must be exactly 3n). So the provided code actually produces length up to 4n? Let me re-read the code: The while loop that builds the common prefix runs while `len < n+n+n && pos[1]<=n+n && pos[2]<=n+n && pos[3]<=n+n`. So it stops when either len reaches 3n or any pointer exceeds 2n. In our example, after two steps len=2, pointers: p1=0,p2=0,p3=2 (end). Since p3 <= 2n (2<=2) actually p3=2 equals 2n, so condition `pos[3]<=n+n` is true (2<=2), so loop continues? Wait p3=2, n+n=2, so equal, still true. So it continues. Third iteration: p1=0('0'), p2=0('0'), p3=2? But p3 is now 2 which is out of bounds for string length 2 (indices 1 and 2). Actually strings are indexed from 1 in the code, s[k][pos[k]] for pos from 1 to 2n. So if p3=2, that's a valid character (the last one). So we read s[3][2] = '1'. So current chars: '0','0','1' → now='1' again. Advance p3 to 3 (which >2n). len becomes 3. Now loop condition: len=3 < 3 (3n=3) is false, exit. Then we have p1=0, p2=0, p3=3>2n. op = (p1< p2? actually op = (pos[1]>pos[2]?1:2) because pos[3]>n+n, so the code checks `if (pos[1]>n+n) op=(pos[2]>pos[3]?2:3); else if (pos[2]>n+n) op=(pos[1]>pos[3]?1:3); else op=(pos[1]>pos[2]?1:2);` Here pos[1]=0 (since we never advanced p1 or p2), pos[2]=0. So else branch: `op=(pos[1]>pos[2]?1:2)` → 0>0 false → op=2. Then while pos[2]<=2n: append s[2][0]? Actually pos[2] starts at 1 in the code (pos[k]=1 initially). So we append the rest of string2 from position 1 to 2n, which is "00" two characters, making len from 3 to 5. That gives a length 5 string, which is longer than 3n. But the problem requires exactly 3n. So the code as given is actually incorrect? Let me check the original Codeforces problem: This is solution for problem "CF 1516C? No, it's "1325E"? Actually I recall a problem where you need to construct a string of length 3n that contains three binary strings of length 2n as subsequences. The known correct algorithm is to choose the majority character at each step, ensuring at least two pointers advance, so after at most 3n steps all three are done, and you don't need extra padding. The provided snippet chooses the minority, which can cause one pointer to lag, but then it appends the rest of that string and pads with zeros, which might exceed 3n. So perhaps the snippet has a bug? But the task is to create a new task *inspired* by the snippet, not necessarily replicate its exact logic. So we can design a correct task based on the core idea: given three binary strings of length 2n, produce a string of length 3n that contains all as subsequences. The correct solution is to use the majority rule: at each step, among the current characters of the three pointers (skipping those already exhausted), pick the character that appears at least as often as the other (i.e., the majority, and if tie, choose either). This guarantees at least two pointers advance (if all three characters are not all same, the majority count is at least 2; if all same, all three advance). Since each pointer can advance at most 2n times, the total number of steps until all three finish is at most 3n (because each step advances at least 2 pointers, so total advancement sum is at least 2*steps; total possible sum of advancements is 3*2n=6n, so steps ≤ 3n). But we need exactly 3n characters; after all three pointers finish, if we have fewer than 3n characters, we can pad with arbitrary bits (e.g., '0') to reach length 3n, and padding does not affect subsequence property because we already included all three strings in the prefix. So the algorithm: maintain three indices, while any index < 2n, repeat: count '0' and '1' among the current characters for indices that are < 2n. Choose `ch` = majority character (if count0 >= count1 choose '0' else '1'; or if count1 > count0 choose '1' else '0' – we'll define: majority = (cnt1 > cnt0) ? '1' : '0'). Append `ch` to result. Advance each pointer that is < 2n and whose current character equals `ch`. When all pointers reach 2n, break. If result length < 3n, append '0' until length 3n. This yields exactly 3n. Time complexity: each iteration advances at least 2 pointers, total advancements = 6n, so at most 3n iterations, each O(1), so O(n). Space O(n). Edge cases: n=1, strings of length 2. Also consider when two strings are identical, still works.
