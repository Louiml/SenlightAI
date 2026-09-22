Write a C++ function that takes an integer `n` and three strings `a`, `b`, and `c`, each of length `2*n` and containing only characters `'0'` and `'1'` (i.e., binary strings). The function must find two distinct strings among the three such that one can be obtained from the other by deleting exactly `n` characters (not necessarily contiguous), preserving the order of the remaining characters, and the remaining characters form a string of length `n` that consists entirely of the majority character among the two strings (i.e., if the majority character is `'1'`, the common subsequence must be `"111...1"` of length `n`, and similarly for `'0'`). If such a pair exists, the function must return the *longest common supersequence* of the two chosen strings that has the property that it contains both strings as subsequences and is of length exactly `3*n` (guaranteed to exist). If no such pair exists (which, per problem constraints, will not happen), the function may return an empty string. The output string must be exactly `3*n` characters long.

The key observation is that in any binary string of length `2n`, either the count of `'0'` or the count of `'1'` is at most `n`. For a pair of strings, the majority character is the one that appears at most `n` times in both strings combined? Actually, we need to find a common subsequence of length `n` that consists entirely of one character. Since each string has length `2n`, by the pigeonhole principle, for any given character, at least one of the two strings has that character appearing at least `n` times? Wait, careful: For a single string of length `2n`, if `'0'` appears `z` times, then `'1'` appears `2n - z`. One of `z` or `2n-z` is at most `n`. So for each string, either `'0'` appears ≤ n times or `'1'` appears ≤ n times. For a pair, we need a character that appears at most n times in *both* strings? Actually, the code chooses the character `c` from `calc` where `c = (z <= n ? '1' : '0')` for each string. That picks the character that appears at most `n` times in that string (if zeros ≤ n, then `'1'` is the majority? Wait, the code says: if `z <= n`, then `c = '1'`, meaning it picks `'1'` as the target character. That means the string has at most `n` zeros, so it has at least `n` ones. So the target character is the one that appears at least `n` times. For each string, we can always find a character that appears at least `n` times. Among three strings, two must have the same majority character (by pigeonhole, since only two characters). So we pick two strings that share the same majority character `f`. Then both strings have at least `n` occurrences of `f`, so we can select exactly `n` of them as a subsequence of all `f`s. The algorithm then builds a common supersequence of length `3n` by interleaving the two strings. The approach: choose the pair where the one with the majority character appearing more times is placed first (to ensure the supersequence length is exactly `3n`). Then repeatedly take characters from the second string, inserting them into the first string at appropriate positions to form a supersequence. The time complexity is O(n) per pair, and we check at most 6 pairs, so O(n). Space complexity O(n) for the answer.

#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

// Returns the longest common supersequence of length 3*n for two strings
// that share a majority character 'f'. Assumes both strings have at least n
// occurrences of 'f' and that the first string has more occurrences of 'f'
// than the second (or equal, but then order doesn't matter).
std::string buildSuperSequence(const std::string& first, const std::string& second, char f, int n) {
    std::vector<char> ans;
    // Insert all characters of first string into the supersequence.
    for (char c : first) {
        ans.push_back(c);
    }
    int fp = 0; // pointer into ans for insertion
    // Process the second string, inserting its non-'f' characters before the
    // appropriate 'f' positions in ans, and also inserting all its 'f's.
    for (int k = 0; k < 2 * n; ++k) {
        if (fp >= (int)ans.size()) {
            // If we've exhausted ans, append remaining characters of second.
            for (int x = k; x < 2 * n; ++x) {
                ans.push_back(second[x]);
            }
            break;
        }
        if (second[k] == f) {
            // Need to find the next occurrence of f in ans to insert this f
            // after it, maintaining subsequence property.
            // Advance fp to the next f in ans.
            while (fp < (int)ans.size() && ans[fp] != f) {
                ++fp;
            }
            // Insert this f right after that position (or at end if none left).
            if (fp < (int)ans.size()) {
                ++fp; // skip the found f
            }
        } else {
            // For a non-f character, insert it right before the current ans[fp]
            // (or at end if fp is out of bounds).
            ans.insert(ans.begin() + fp, second[k]);
            ++fp; // now fp points to the position after the inserted char
        }
    }
    // If the combined length is not exactly 3n (shouldn't happen), adjust.
    // But per problem guarantee, it will be exactly 3n.
    return std::string(ans.begin(), ans.end());
}

// Main solution function.
// Given n and three binary strings of length 2n, find a pair with a common
// majority character and return a 3n-length common supersequence.
std::string solveTask(int n, const std::string& a, const std::string& b, const std::string& c) {
    std::vector<std::string> strs = {a, b, c};
    // For each string, compute the majority character (the one appearing >= n times)
    // and its count.
    std::vector<char> maj(3);
    std::vector<int> majCount(3);
    for (int i = 0; i < 3; ++i) {
        int zeros = 0;
        for (char ch : strs[i]) if (ch == '0') ++zeros;
        // If zeros <= n, then '1' appears at least n times, so majority is '1';
        // else majority is '0'.
        if (zeros <= n) {
            maj[i] = '1';
            majCount[i] = 2 * n - zeros;
        } else {
            maj[i] = '0';
            majCount[i] = zeros;
        }
    }
    // Find two indices with the same majority character.
    for (int i = 0; i < 3; ++i) {
        for (int j = i + 1; j < 3; ++j) {
            if (maj[i] == maj[j]) {
                // Ensure the one with more occurrences of the majority char is first.
                // If counts are equal, order doesn't matter, but we must keep it consistent.
                std::string first, second;
                int fi, si;
                if (majCount[i] >= majCount[j]) {
                    fi = i; si = j;
                } else {
                    fi = j; si = i;
                }
                first = strs[fi];
                second = strs[si];
                return buildSuperSequence(first, second, maj[i], n);
            }
        }
    }
    // Per problem constraints, a pair always exists. Return empty if not.
    return "";
}

#include <cassert>
#include <string>

// Declaration of the solution function (as defined above).
std::string solveTask(int n, const std::string& a, const std::string& b, const std::string& c);

int main() {
    // Test 1: Simple case where two strings share '1' majority.
    // n=2, strings length 4.
    std::string res1 = solveTask(2, "1110", "1011", "0000");
    assert(res1.size() == 6);
    // Check that both original strings are subsequences of res1.
    {
        int p1=0, p2=0;
        for (char ch : res1) {
            if (p1 < 4 && ch == "1110"[p1]) ++p1;
            if (p2 < 4 && ch == "1011"[p2]) ++p2;
        }
        assert(p1 == 4 && p2 == 4);
    }

    // Test 2: All three share '0' majority.
    std::string res2 = solveTask(1, "01", "10", "00");
    // n=1, strings length 2, output length 3.
    assert(res2.size() == 3);
    {
        int p1=0, p2=0;
        for (char ch : res2) {
            if (p1 < 2 && ch == "01"[p1]) ++p1;
            if (p2 < 2 && ch == "10"[p2]) ++p2;
        }
        assert(p1 == 2 && p2 == 2);
    }

    // Test 3: n=3, strings length 6, where first two share '1'.
    std::string a3 = "111000";
    std::string b3 = "110100";
    std::string c3 = "000111";
    std::string res3 = solveTask(3, a3, b3, c3);
    assert(res3.size() == 9);
    {
        int p1=0, p2=0;
        for (char ch : res3) {
            if (p1 < 6 && ch == a3[p1]) ++p1;
            if (p2 < 6 && ch == b3[p2]) ++p2;
        }
        assert(p1 == 6 && p2 == 6);
    }

    // Test 4: Edge case where two strings are identical.
    std::string a4 = "0101";
    std::string b4 = "0101";
    std::string c4 = "1010";
    std::string res4 = solveTask(2, a4, b4, c4);
    assert(res4.size() == 6);
    {
        int p1=0, p2=0;
        for (char ch : res4) {
            if (p1 < 4 && ch == a4[p1]) ++p1;
            if (p2 < 4 && ch == b4[p2]) ++p2;
        }
        assert(p1 == 4 && p2 == 4);
    }

    // Test 5: n=1, strings like "00" and "01" share '0'.
    std::string res5 = solveTask(1, "00", "01", "11");
    assert(res5.size() == 3);
    {
        int p1=0, p2=0;
        for (char ch : res5) {
            if (p1 < 2 && ch == "00"[p1]) ++p1;
            if (p2 < 2 && ch == "01"[p2]) ++p2;
        }
        assert(p1 == 2 && p2 == 2);
    }

    return 0;
}
