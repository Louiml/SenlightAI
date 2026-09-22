// Given a string `s` (1 ≤ |s| ≤ 200,000) consisting of lowercase English letters, and a list of `T` pattern strings `t_i` (1 ≤ |t_i| ≤ 100, T ≤ 100), write a C++ function that returns the number of patterns for which there exists a split position `k` (0 ≤ k ≤ |t_i|-1) such that the prefix `t_i[0..k]` appears as a contiguous substring of `s` immediately followed by (but not overlapping) a reversed copy of the suffix `t_i[k+1..|t_i|-1]` appearing later in `s` (i.e., the concatenated string `prefix + reverse(suffix)` is a substring of `s`, with the boundary between prefix and reversed suffix occurring at the split position). The function must be efficient enough for the constraint sizes, using suffix array and RMQ techniques.

#include <cassert>
#include <string>
#include <vector>

// Include the solution code here (omitted for brevity, but assume it's above)

int main() {
    // Test 1: Simple case
    {
        std::string s = "abacaba";
        std::vector<std::string> pats = {"abc", "aba", "bac"};
        assert(count_valid_patterns(s, pats) == 1); // "aba": split at 1 -> "a" + "ab" reversed is "ba" -> "aba" appears
    }
    // Test 2: No matches
    {
        std::string s = "xyz";
        std::vector<std::string> pats = {"abc", "def"};
        assert(count_valid_patterns(s, pats) == 0);
    }
    // Test 3: All possible splits work
    {
        std::string s = "aaa";
        std::vector<std::string> pats = {"aa"}; // split 0: "a" + "a" reversed = "aa"
        assert(count_valid_patterns(s, pats) == 1);
    }
    // Test 4: Split must not overlap
    {
        std::string s = "ab";
        std::vector<std::string> pats = {"ab"}; // split 0: "a" + "b" reversed = "ab" (needs a after a, s has only one a)
        // a found at 0, reversed b starts at -1? Now reverse "b" in s = "ba" reversed string is "ab". Reversed suffix "b" appears at pos 1 in reversed string => original pos 0. Need start_suf >= start_pre + 1 => 0 >= 0+1 false. So no.
        assert(count_valid_patterns(s, pats) == 0);
    }
    // Test 5: Multiple patterns
    {
        std::string s = "abcabc";
        std::vector<std::string> pats = {"abc", "bca", "cab"};
        // abc: split 2 -> "ab" + "c" reversed = "ab"+"c" = "abc" appears
        // bca: split 2 -> "bc"+"a" reversed = "bc"+"a" = "bca" appears
        // cab: split 2 -> "ca"+"b" reversed = "ca"+"b" = "cab" appears
        assert(count_valid_patterns(s, pats) == 3);
    }
    // Test 6: Empty pattern list
    {
        std::string s = "hello";
        std::vector<std::string> pats = {};
        assert(count_valid_patterns(s, pats) == 0);
    }
    // Test 7: Pattern length 1 (no split possible)
    {
        std::string s = "a";
        std::vector<std::string> pats = {"a"};
        assert(count_valid_patterns(s, pats) == 0); // no split because m-1 = 0, so no iterations
    }
    // Test 8: Long pattern with repeated characters
    {
        std::string s = "aaaaaa";
        std::vector<std::string> pats = {"aaaa"}; // split 2: "aa"+"aa" reversed = "aa"+"aa" = "aaaa" appears multiple times
        assert(count_valid_patterns(s, pats) == 1);
    }
    // Test 9: Overlap prevention
    {
        std::string s = "abba";
        std::vector<std::string> pats = {"aba"}; // split 1: "a"+"ba" reversed = "a"+"ab" = "aab" not substring
        assert(count_valid_patterns(s, pats) == 0);
    }
    // Test 10: Large single query
    {
        std::string s = "abcdefghijklmnopqrstuvwxyz";
        std::vector<std::string> pats = {"abcxyz"}; // split 2: "abc"+"xyz" reversed = "abc"+"zyx" = "abczyx" not substring
        assert(count_valid_patterns(s, pats) == 0);
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

// Build suffix array for a string
vector<int> build_suffix_array(const string& in) {
    int n = in.size();
    vector<int> out(n), bucket(n), pos_bucket(n), bpos(n), temp(n);
    iota(out.begin(), out.end(), 0);
    sort(out.begin(), out.end(), [&](int a, int b) { return in[a] < in[b]; });
    int c = 0;
    for (int i = 0; i < n; i++) {
        bucket[i] = c;
        if (i + 1 == n || in[out[i]] != in[out[i + 1]]) c++;
    }
    for (int h = 1; h < n && c < n; h <<= 1) {
        for (int i = 0; i < n; i++) pos_bucket[out[i]] = bucket[i];
        for (int i = n - 1; i >= 0; i--) bpos[bucket[i]] = i;
        fill(temp.begin(), temp.end(), 0);
        for (int i = 0; i < n; i++) {
            if (out[i] >= n - h) temp[bpos[bucket[i]]++] = out[i];
        }
        for (int i = 0; i < n; i++) {
            if (out[i] >= h) temp[bpos[pos_bucket[out[i] - h]]++] = out[i] - h;
        }
        c = 0;
        for (int i = 0; i + 1 < n; i++) {
            int a = (bucket[i] != bucket[i + 1]) ||
                    (temp[i] >= n - h) ||
                    (pos_bucket[temp[i + 1] + h] != pos_bucket[temp[i] + h]);
            bucket[i] = c;
            c += a;
        }
        bucket[n - 1] = c++;
        temp.swap(out);
    }
    return out;
}

class SuffixArrayRMQ {
    vector<vector<int>> st;
    vector<int> log2;
    int n;
public:
    SuffixArrayRMQ(const string& s) {
        n = s.size();
        auto sa = build_suffix_array(s);
        int K = 0;
        while ((1 << K) <= n) K++;
        st.assign(K, vector<int>(n));
        st[0] = sa;
        for (int j = 1; j < K; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[j][i] = min(st[j-1][i], st[j-1][i + (1 << (j-1))]);
            }
        }
        log2.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) log2[i] = log2[i/2] + 1;
    }
    // Query minimum SA index in range [l, r]
    int query(int l, int r) {
        if (l > r) return INT_MAX;
        int j = log2[r - l + 1];
        return min(st[j][l], st[j][r - (1 << j) + 1]);
    }
};

// Binary search for lower bound of prefix match
int lower_bound_prefix(const string& text, const SuffixArrayRMQ& rmq, int l, int r, const string& pat, int start_pat, int len) {
    int low = l, high = r;
    while (low < high) {
        int mid = (low + high) / 2;
        int pos = rmq.query(mid, mid);
        if (text.compare(pos, len, pat, start_pat, len) >= 0) high = mid;
        else low = mid + 1;
    }
    if (text.compare(rmq.query(low, low), len, pat, start_pat, len) != 0) return -1;
    return low;
}

// Binary search for upper bound of prefix match
int upper_bound_prefix(const string& text, const SuffixArrayRMQ& rmq, int l, int r, const string& pat, int start_pat, int len) {
    int low = l, high = r;
    while (low < high) {
        int mid = (low + high + 1) / 2;
        int pos = rmq.query(mid, mid);
        if (text.compare(pos, len, pat, start_pat, len) <= 0) low = mid;
        else high = mid - 1;
    }
    if (text.compare(rmq.query(low, low), len, pat, start_pat, len) != 0) return -1;
    return low;
}

// Main solution function
int count_valid_patterns(const string& s, const vector<string>& patterns) {
    int n = s.size();
    string rs = s;
    reverse(rs.begin(), rs.end());

    SuffixArrayRMQ sa_normal(s);
    SuffixArrayRMQ sa_rev(rs);

    int T = patterns.size();
    vector<vector<int>> pre(T), suf(T);
    for (int i = 0; i < T; i++) {
        int m = patterns[i].size();
        pre[i].assign(m + 1, -1);
        suf[i].assign(m + 1, -1);
    }

    // For each pattern, compute pre and suf
    for (int idx = 0; idx < T; idx++) {
        const string& pat = patterns[idx];
        int m = pat.size();
        // Normal: find prefix matches for increasing lengths
        int l_n = 0, r_n = n - 1;
        string prefix;
        for (int len = 1; len <= m; len++) {
            // We need to compare pat[0..len-1] with text starting at some positions
            // Use incremental binary search but easier: do full binary search
            int l2 = lower_bound_prefix(s, sa_normal, l_n, r_n, pat, 0, len);
            if (l2 == -1) break;
            int r2 = upper_bound_prefix(s, sa_normal, l_n, r_n, pat, 0, len);
            if (r2 == -1) break;
            pre[idx][len] = sa_normal.query(l2, r2);
            l_n = l2; r_n = r2;
        }
        // Reversed: find suffix matches for decreasing lengths (from end)
        string rev_pat = pat;
        reverse(rev_pat.begin(), rev_pat.end());
        int l_r = 0, r_r = n - 1;
        // For each len = 1..m, consider reversed suffix of length len
        for (int len = 1; len <= m; len++) {
            // Reversed suffix of pat is rev_pat[0..len-1]
            int l2 = lower_bound_prefix(rs, sa_rev, l_r, r_r, rev_pat, 0, len);
            if (l2 == -1) break;
            int r2 = upper_bound_prefix(rs, sa_rev, l_r, r_r, rev_pat, 0, len);
            if (r2 == -1) break;
            // The starting position in reversed string corresponds to position in original
            // For suffix of length len, the start in reversed string is p, then in original it becomes n - p - len
            int min_start_rev = sa_rev.query(l2, r2);
            // To get the latest possible start in original, we want the largest p (since reversed start is small)
            // But the RMQ gives min SA index in reversed suffix array, which is the smallest index in reversed string
            // Actually we want the largest original start, i.e., smallest p (since original start = n - p - len, so smaller p gives larger original start)
            // Wait: p is position in reversed string. If p is small, then original position n - p - len is large.
            // So to get the largest original start, we need the minimum p among matching suffixes. So use RMQ min.
            suf[idx][len] = n - min_start_rev - len;
            l_r = l2; r_r = r2;
        }
    }

    int ans = 0;
    for (int idx = 0; idx < T; idx++) {
        int m = patterns[idx].size();
        for (int split = 0; split < m - 1; split++) {
            int pref_len = split + 1;
            int suf_len = m - pref_len;
            if (pre[idx][pref_len] == -1 || suf[idx][suf_len] == -1) continue;
            int start_pre = pre[idx][pref_len];
            int start_suf = suf[idx][suf_len];
            if (start_suf >= start_pre + pref_len) {
                ans++;
                break;
            }
        }
    }
    return ans;
}

// The core idea is to check all possible split positions in each pattern and determine if the prefix and reversed suffix can be placed contiguously in `s` (with the reversed suffix starting at some index ≥ end of prefix in `s`). We do this by building a suffix array for `s` and for the reversed `s` (to handle reversed patterns). For each pattern, we binary search on the suffix array to find the range of suffixes that match each prefix length, and similarly for the reversed pattern on the reversed string. Using a sparse table for RMQ over the suffix array (which stores the minimum suffix array index), we can find the earliest and latest occurrence positions of any prefix match in linear time per length. Specifically, for each split point `i` in the pattern, we know the earliest starting index `pre` of the prefix `t[0..i]` in `s`, and the latest starting index `suf` of the reversed suffix `t[i+1..]` in the reversed string (which corresponds to an ending position in the original string). The condition for validity is that the reversed suffix's starting position in `s` is ≥ `pre + i + 1` (no overlap and correct order). We precompute `pre[pos][len]` for each pattern and each prefix length, and `suf[pos][len]` for each pattern and each length (of reversed suffix), using binary search on suffix arrays and RMQ to get the min/max suffix array positions. The total complexity is O(|s| log |s| + Σ|t_i| log |s|) for building suffix arrays and binary searches, with O(|s| log |s|) for sparse table build, and O(Σ|t_i|) for final checks. Space is O(|s| log |s|) for sparse table.
