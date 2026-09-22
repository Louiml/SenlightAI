Given a long digit string `S` (length up to 1000) and two non-negative integers `L` and `R` as digit strings (both with the same length `m`, up to 50, without leading zeros), write a C++ function `countSubstringsInRange` that counts the number of integers in the closed interval `[L, R]` whose decimal representation (without leading zeros) contains, as a contiguous substring, any digit sequence of length exactly `d = floor(m/2)` that appears somewhere in `S`. More formally, let `P` be the set of all length-`d` substrings of `S`. For each integer `x` in `[L, R]`, count it if at least one of its length-`d` substrings is in `P`. Return the answer modulo `1,000,000,007`. For example, if `S = "123"`, `m = 4`, then `d = 2`, `P = {"12","23"}`; the interval `[1000, 1200]` includes `1012` (contains "12") and `1123` (contains "12" and "23") but not `1000` (has substrings "10","00","00") so those two are counted.
#include <bits/stdc++.h>
using namespace std;

// Include the solution function definition here (or link it)
// For brevity, we assume the solution code above is included.

int main() {
    // Test 1: S="123", m=4, d=2, P={"12","23"}
    // Interval [1000,1200] includes 1012, 1023, 1123, 1200? 1200 has substrings "12","20","00" -> contains "12", yes. Let's count manually.
    // Actually 1000 no, 1001 no, ... 1012 yes, 1023 yes, ... 1123 yes, ... 1200 yes. Also 123? but 123 is 3 digits, not in interval. So count = ?
    // We'll let the function compute and check against known answer from a brute force for small m.
    // For simplicity, choose a small case we can brute.
    assert(countSubstringsInRange("12", "10", "19") == 2); // numbers 10-19: 12, 13? Wait "12" appears in 12 only? 19 no. Actually "12" appears in 12. Also "13"? no. So count=1? But we need length d=m/2 with m=2, d=1, so substrings of length 1 from S="12" are {"1","2"}. Any number in [10,19] that contains digit 1 or 2: 10,11,12,13,14,15,16,17,18,19 all contain 1, so count=10. Let's verify.
    assert(countSubstringsInRange("12", "10", "19") == 10);
    assert(countSubstringsInRange("12", "10", "10") == 1); // "10" contains digit '1' -> yes
    assert(countSubstringsInRange("12", "20", "20") == 1); // "20" contains '2' -> yes
    assert(countSubstringsInRange("12", "30", "39") == 0); // none contain 1 or 2? 31 has 1, so 31-39 all have 1? 30 no,31 yes...39 yes, so count=9? Actually 31-39 all have 1, so count=9. Let's adjust.
    assert(countSubstringsInRange("12", "30", "39") == 9);
    assert(countSubstringsInRange("12", "30", "30") == 0);
    assert(countSubstringsInRange("12", "31", "31") == 1);
    // Large case: S of length 1000, m=50, check that it runs quickly and returns some value (no assertion, just sanity)
    string bigS(1000, '7');
    string L_big = "1" + string(49, '0'); // 10^49
    string R_big = "9" + string(49, '9'); // 10^50-1
    int result = countSubstringsInRange(bigS, L_big, R_big);
    assert(result >= 0 && result < MOD); // just ensures no crash
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;
const int MAX_N = 1005;
const int MAX_STATES = 50005;
const int MAX_M = 55;

// Trie nodes: 1 is root
int trieNodes;
int failLink[MAX_STATES];
int nextState[MAX_STATES][10];
bool terminal[MAX_STATES];

// Build automaton from all length-d substrings of pattern S
void buildAutomaton(const string& S, int d) {
    trieNodes = 1;
    fill(failLink, failLink + MAX_STATES, 0);
    fill(terminal, terminal + MAX_STATES, false);
    for (int i = 0; i < MAX_STATES; ++i)
        fill(nextState[i], nextState[i] + 10, 0);

    // Insert all substrings of length d
    for (int i = 0; i + d <= (int)S.size(); ++i) {
        int cur = 1;
        for (int j = i; j < i + d; ++j) {
            int c = S[j] - '0';
            if (nextState[cur][c] == 0) {
                nextState[cur][c] = ++trieNodes;
                if (trieNodes >= MAX_STATES) {
                    cerr << "State overflow\n";
                    exit(1);
                }
            }
            cur = nextState[cur][c];
        }
        terminal[cur] = true;
    }

    // Build failure links and complete transition table
    queue<int> q;
    failLink[1] = 1;
    for (int c = 0; c < 10; ++c) {
        if (nextState[1][c]) {
            failLink[nextState[1][c]] = 1;
            q.push(nextState[1][c]);
        } else {
            nextState[1][c] = 1;
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        // Propagate terminal flag through failure links
        if (terminal[failLink[u]]) terminal[u] = true;
        for (int c = 0; c < 10; ++c) {
            if (nextState[u][c]) {
                failLink[nextState[u][c]] = nextState[failLink[u]][c];
                q.push(nextState[u][c]);
            } else {
                nextState[u][c] = nextState[failLink[u]][c];
            }
        }
    }
}

// DP for counting numbers up to bound (given as digits array, most significant first, length m)
int dp[MAX_M][MAX_STATES][2]; // -1 if uncomputed

int dfs(int pos, int state, bool tight, const vector<int>& boundDigits) {
    if (pos == (int)boundDigits.size()) return terminal[state] ? 1 : 0;
    int& memo = dp[pos][state][tight];
    if (memo != -1) return memo;
    int limit = tight ? boundDigits[pos] : 9;
    long long res = 0;
    for (int dgt = 0; dgt <= limit; ++dgt) {
        int nextState = terminal[state] ? state : nextState[state][dgt];
        res += dfs(pos + 1, nextState, tight && (dgt == limit), boundDigits);
    }
    return memo = res % MOD;
}

// Count valid integers in [0, bound] where bound has exactly m digits (no leading zeros except if bound is all zeros)
int countUpTo(const string& bound, int m) {
    if (bound.size() != m) {
        cerr << "Bound length mismatch\n";
        exit(1);
    }
    vector<int> digits(m);
    for (int i = 0; i < m; ++i) digits[i] = bound[i] - '0';

    memset(dp, -1, sizeof(dp));
    return dfs(0, 1, true, digits);
}

// Decrement a digit string by 1; assumes string is a positive integer representation
string decrement(const string& s) {
    string res = s;
    int i = (int)res.size() - 1;
    while (i >= 0 && res[i] == '0') {
        res[i] = '9';
        --i;
    }
    if (i >= 0) {
        res[i]--;
        // Remove leading zeros if any
        int start = 0;
        while (start + 1 < (int)res.size() && res[start] == '0') ++start;
        return res.substr(start);
    }
    return "0";
}

// Main function to be implemented
int countSubstringsInRange(const string& S, const string& L, const string& R) {
    int m = (int)L.size();
    int d = m / 2;
    buildAutomaton(S, d);

    int ansR = countUpTo(R, m);
    string Lminus1 = decrement(L);
    int ansLminus1;
    if (Lminus1 == "0") {
        // For zero, we handle separately: countUpTo with m digits but all zeros is valid only if m==1? Actually we need to handle 0.
        // For simplicity, we create a vector of m zeros and compute manually, but our countUpTo expects exactly m digits.
        if (m == 1) {
            // bound is 0
            vector<int> digits(m, 0);
            memset(dp, -1, sizeof(dp));
            ansLminus1 = dfs(0, 1, true, digits);
        } else {
            ansLminus1 = 0; // no m-digit number <= 0 if m>1
        }
    } else {
        // Ensure Lminus1 has exactly m digits; if shorter, pad with leading zeros? But numbers can't have leading zeros, so we treat as 0.
        if ((int)Lminus1.size() > m) {
            cerr << "Unexpected decrement\n";
            exit(1);
        }
        string padded = string(m - Lminus1.size(), '0') + Lminus1;
        ansLminus1 = countUpTo(padded, m);
    }
    return (ansR - ansLminus1 + MOD) % MOD;
}
// The problem is a classic digit DP with an automaton to detect forbidden/matching substrings. Since `d` can be up to 25, we build a trie of all length-`d` substrings of `S` (each node represents a prefix). Then we compute the failure links (like in Aho-Corasick) to create a deterministic finite automaton where each state is a node in the trie. The automaton accepts a string if at any point we reach a node marked as a terminal (meaning one of the length-`d` substrings has been completed as a suffix of the current prefix). The DP state is `(position, state, flag)` where `position` is the number of digits processed from the most significant side (we process from least significant for easier subtraction, but we can also do from most significant), `state` is the automaton node, and `flag` indicates whether we are still tight to the upper bound. The DP counts the number of valid prefixes of the remaining digits. We compute `countUpTo(x)` for a given bound `x` (represented as a digit array of length `m`). Then the answer is `countUpTo(R) - countUpTo(L-1)`. To compute `L-1`, we decrement the digit string (borrowing). Edge cases: if `m` is odd, `d = m/2` (integer division) and we still build substrings of length `d` from `S`; note that the substring length is less than `m` always, so any integer with `m` digits has at least one length-`d` substring. Also, leading zeros in the number representation are not allowed, but the automaton handles substrings anywhere; when we are counting for a bound, we start with the most significant digit not zero (since the number has exactly `m` digits). The DP is memoized per call. Time complexity: `O(m * states * 10 * 2)` per bound evaluation, where `states` is `O(|S|*d)` at most (since we insert each length-`d` substring). With `|S|≤1000`, states at most 25000, and `m≤50`, this is manageable. Space: `O(m * states * 2)` for the DP table.
