/*
Write a C++ function that, given an array of digits (0-9) and a series of point updates that replace a digit at a specified position, returns the result for the entire array after each update. The result is a count of valid ways to split the sequence of digits into groups of size 1 or 2, where each group’s numeric value must be between 10 and 18 inclusive, and if the group is of size 2, its value must also be a "special" number: specifically, if the two-digit number is between 10 and 18, its complement to 19 (i.e., 19 - value) is used as a multiplier in the counting recurrence. More precisely, define a DP over the prefix of the array: let `dp[i]` be the number of valid decodings of the first `i` digits. Then `dp[0]=1`, and for each `i` from 1 to n, `dp[i] = dp[i-1]` (taking digit i as a single-digit group, always allowed) plus, if the last two digits form a number between 10 and 18, `dp[i-2] * (19 - twoDigitValue)` (taking the last two digits as a group). The task is to efficiently maintain the value `dp[n]` under point updates (changing a single digit) and output it after each update. The initial array is given as a string of digits. The function should accept the initial digit string and a list of update operations (each consisting of a 1-based index and a new digit 0-9), process all updates sequentially, and return a vector of the `dp[n] % 998244353` values after each update, where the DP is computed modulo 998244353. Handle the case where the array size n is at least 1, and updates always target valid indices.
*/

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 998244353;

struct DecodeData {
    long long a, b, c, d; // [l,r], [l,r-1], [l+1,r], [l+1,r-1]
};

long long boundaryFactor(int leftDigit, int rightDigit) {
    int twoDigit = leftDigit * 10 + rightDigit;
    if (10 <= twoDigit && twoDigit <= 18) {
        return 19 - twoDigit;
    }
    return 0;
}

DecodeData mergeDecodeData(const DecodeData &L, const DecodeData &R, int leftDigit, int rightDigit) {
    long long t = boundaryFactor(leftDigit, rightDigit);
    DecodeData res;
    res.a = (L.a * R.a + L.b * t % MOD * R.c) % MOD;
    res.b = (L.a * R.b + L.b * t % MOD * R.d) % MOD;
    res.c = (L.c * R.a + L.d * t % MOD * R.c) % MOD;
    res.d = (L.c * R.b + L.d * t % MOD * R.d) % MOD;
    return res;
}

DecodeData makeLeaf(int digit) {
    DecodeData leaf;
    leaf.a = 1;
    leaf.b = 1;
    leaf.c = 1;
    leaf.d = 0;
    return leaf;
}

// Process initial digit string and a list of updates (index 1-based, new digit).
// Returns a vector of answers after each update.
vector<long long> processDigitUpdates(const string &initialDigits, const vector<pair<int,int>> &updates) {
    int n = (int)initialDigits.size();
    vector<int> digits(n + 1); // 1-indexed
    for (int i = 1; i <= n; i++) digits[i] = initialDigits[i-1] - '0';

    // Simple segment tree stored in arrays for clarity.
    vector<DecodeData> tree(4 * n);

    function<void(int,int,int)> build = [&](int node, int l, int r) {
        if (l == r) {
            tree[node] = makeLeaf(digits[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(node*2, l, mid);
        build(node*2+1, mid+1, r);
        tree[node] = mergeDecodeData(tree[node*2], tree[node*2+1], digits[mid], digits[mid+1]);
    };

    function<void(int,int,int,int)> update = [&](int node, int l, int r, int pos) {
        if (l == r) {
            tree[node] = makeLeaf(digits[l]);
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(node*2, l, mid, pos);
        else update(node*2+1, mid+1, r, pos);
        tree[node] = mergeDecodeData(tree[node*2], tree[node*2+1], digits[mid], digits[mid+1]);
    };

    build(1, 1, n);

    vector<long long> answers;
    answers.reserve(updates.size());
    for (const auto &up : updates) {
        int idx = up.first;
        int newDigit = up.second;
        digits[idx] = newDigit;
        update(1, 1, n, idx);
        answers.push_back(tree[1].a);
    }
    return answers;
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Include the solution code here (the function and structs).

int main() {
    // Test 1: single digit
    {
        vector<pair<int,int>> updates = {{1,5}};
        auto res = processDigitUpdates("3", updates);
        assert(res.size() == 1);
        assert(res[0] == 1);
    }

    // Test 2: two digits, update second digit
    {
        // initial "12": dp[2] = dp[1] + dp[0]*7 = 1+7=8
        vector<pair<int,int>> updates = {{2,5}}; // becomes "15" (15 in 10-18 => factor 4)
        // dp[0]=1, dp[1]=1, dp[2]=1 + 1*4 = 5
        auto res = processDigitUpdates("12", updates);
        assert(res.size() == 1);
        assert(res[0] == 5);
    }

    // Test 3: three digits, update middle
    {
        // initial "123": dp[3]=8 (since 23>18)
        // update position 2 to '8' -> "183": 18 gives factor 1, 83>18
        // dp[0]=1, dp[1]=1, dp[2]=1+1*1=2, dp[3]=dp[2]+dp[1]*0=2
        vector<pair<int,int>> updates = {{2,8}};
        auto res = processDigitUpdates("123", updates);
        assert(res.size() == 1);
        assert(res[0] == 2);
    }

    // Test 4: multiple updates sequential
    {
        string s = "121";
        // initial "121": 
        // dp[0]=1, dp[1]=1 (1), dp[2]=1+1*7=8 (12), dp[3]=dp[2]+dp[1]*? (21>18) => 8
        // update pos1 to 9: "921" -> dp[1]=1, dp[2]=1+0=1 (92>18), dp[3]=dp[2]+dp[1]*? (21>18)=1
        // then update pos3 to 0: "920" -> dp[1]=1, dp[2]=1 (92>18), dp[3]=dp[2]+dp[1]*? (20>18)=1
        vector<pair<int,int>> updates = {{1,9}, {3,0}};
        auto res = processDigitUpdates("121", updates);
        assert(res.size() == 2);
        assert(res[0] == 1);
        assert(res[1] == 1);
    }

    // Test 5: all zeros
    {
        string s = "000";
        // No two-digit number is between 10 and 18, so dp[i]=1 for all i
        vector<pair<int,int>> updates = {{2,1}}; // becomes "010": still no valid two-digit
        auto res = processDigitUpdates("000", updates);
        assert(res[0] == 1);
    }

    // Test 6: larger random test with brute force for small n
    {
        mt19937 rng(12345);
        for (int n = 1; n <= 6; n++) {
            string s;
            for (int i = 0; i < n; i++) s.push_back('0' + rng()%10);
            // generate initial answer by brute force
            auto brute = [&](const string &str) {
                int m = str.size();
                vector<long long> dp(m+1,0);
                dp[0] = 1;
                for (int i = 1; i <= m; i++) {
                    // single digit
                    dp[i] = (dp[i] + dp[i-1]) % MOD;
                    // two digit
                    if (i >= 2) {
                        int two = (str[i-2]-'0')*10 + (str[i-1]-'0');
                        if (10 <= two && two <= 18) {
                            dp[i] = (dp[i] + dp[i-2] * (19 - two)) % MOD;
                        }
                    }
                }
                return dp[m];
            };
            // create a random update
            int pos = 1 + rng()%n;
            int newDigit = rng()%10;
            string updated = s;
            updated[pos-1] = '0' + newDigit;
            vector<pair<int,int>> updates = {{pos, newDigit}};
            auto res = processDigitUpdates(s, updates);
            assert(res.size() == 1);
            assert(res[0] == brute(updated));
        }
    }

    cout << "All tests passed!" << endl;
    return 0;
}

// The problem is a classic segment tree with a custom merge operation on DP states. For any contiguous segment of the array, we need to combine subsegments to compute the DP value for the whole segment. The key is to define a `dat` structure that stores four values:
// - `a`: DP for the segment when both ends are included (i.e., the number of ways to decode the segment assuming no forced grouping across the boundary).
// - `b`: DP for the segment when the rightmost digit is excluded (i.e., we consider a subsegment ending at the second-last digit).
// - `c`: DP for the segment when the leftmost digit is excluded (i.e., we consider a subsegment starting at the second digit).
// - `d`: DP for the segment when both ends are excluded (i.e., the interior subsegment).
//
// For a single digit `x`, these values are: `a = x+1`? Wait, careful: In the given snippet, the `dat` struct for a single value uses `a=i+1` where `i` is the digit? Actually, look at the snippet: `dat(int i){ a=i+1,b=1,c=1,d=0; }` – that seems odd. But we can reinterpret: The DP definition is exactly the classic "decode ways" with a twist: a single digit always contributes 1 way (dp[i-1] * 1), and a two-digit number between 10 and 18 contributes `dp[i-2] * (19 - twoDigitValue)`. So for a single digit, the base DP count for that segment alone should be 1 (i.e., `a=1`), and `b`, `c`, `d` represent states where we exclude ends. However, the snippet’s initialization seems unusual. For our task, we define a correct merge.
//
// Let’s define the segment tree node merge: For a segment split into left part `L` (covering positions [s..m]) and right part `R` (covering [m+1..e]), the boundary pair is the two digits at positions `m` and `m+1`. Let `temp` be `19 - (arr[m]*10 + arr[m+1])` if that two-digit number is between 10 and 18 inclusive, otherwise `0`. Then new values are:
// - `a = (L.a * R.a + L.b * temp * R.c) % MOD`
// - `b = (L.a * R.b + L.b * temp * R.d) % MOD`
// - `c = (L.c * R.a + L.d * temp * R.c) % MOD`
// - `d = (L.c * R.b + L.d * temp * R.d) % MOD`
//
// The reasoning: For the combined segment with both ends included, you can either take no cross-boundary pair (so combine L's full DP with R's full DP), or take a cross-boundary pair: that requires that the left segment’s rightmost digit is excluded (so state L.b) and the right segment’s leftmost digit is excluded (state R.c), and then multiply by the factor `temp`. Similarly for the other states.
//
// For a single digit `x`, initialize: `a=1` (there is exactly one way to decode a single digit as that digit alone), `b=1` (if the rightmost is excluded, that means we treat the segment as empty? Actually for a single digit, `b` is the DP when the rightmost is excluded, but excluding the only digit makes an empty segment: there is exactly 1 way to decode an empty segment (empty set). So `b=1`. Similarly `c=1`. `d=0` because excluding both ends of a single digit leaves an empty segment (should be 1? Wait, if both ends excluded from a single digit, the segment becomes empty, and there is 1 way (the empty decoding). But the snippet sets `d=0`. Let’s think: For a single element, the four states are:
// - a: whole segment: 1 way (the digit alone).
// - b: segment without rightmost: that’s an empty segment: 1 way.
// - c: segment without leftmost: empty: 1 way.
// - d: segment without both: empty: 1 way? But that would cause issues. Actually the snippet sets d=0, which suggests a different definition: maybe `d` is for when the segment is "interior" and must have at least one element? Let’s derive from the merge formula: For a single leaf, we don't need d to be meaningful because it’s only used in merges where the leaf is on the boundary. But to be safe, we can set `d=1` for a single element, because excluding both ends yields an empty segment and there is exactly one way to decode an empty sequence. However, the snippet uses `d=0`. Let’s test with a small example: Suppose we have two digits "1 2". The two-digit number is 12, between 10 and 18, so temp = 19 - 12 = 7. The DP should be: dp[0]=1, dp[1]=1 (digit 1 alone), dp[2]=dp[1] + dp[0]*7 = 1+7=8. Now compute with segment tree: Two leaves. Leaf for digit 1: a=1,b=1,c=1,d=0 (as per snippet). Leaf for digit 2: a=1,b=1,c=1,d=0. Merge:
// a = L.a*R.a + L.b*temp*R.c = 1*1 + 1*7*1 = 8 (correct).
// b = L.a*R.b + L.b*temp*R.d = 1*1 + 1*7*0 = 1.
// c = L.c*R.a + L.d*temp*R.c = 1*1 + 0*7*1 = 1.
// d = L.c*R.b + L.d*temp*R.d = 1*1 + 0*7*0 = 1.
// So final a=8. Good.
//
// Now if we have three digits "1 2 3". We can compute manually: dp[0]=1, dp[1]=1, dp[2]=1+1*7=8, dp[3]=dp[2] + (if last two 23 >18, no) = 8. Also consider groups: (1)(2)(3), (12)(3) -> 567? No, 12*? Wait the multiplier is 7, but that's for counting ways: each pair contributes a factor. Actually the DP recurrence is: dp[i] = dp[i-1]*1 + (if two-digit 10-18) dp[i-2]*19-temp? Actually the snippet: for each single digit, multiply by 1; for a pair, multiply by (19 - twoDigitValue). So dp[3] = dp[2] + dp[1]*0 (since 23>18) = 8. Using segment tree: two leaves for digits 1 and 2 (as above) give a=8,b=1,c=1,d=1 for segment [1,2]. Then merge with leaf digit 3 (a=1,b=1,c=1,d=0). Boundary pair is 23, temp=0. Merge:
// a = L.a*R.a + L.b*temp*R.c = 8*1 + 0 = 8.
// b = L.a*R.b + L.b*temp*R.d = 8*1 + 0 = 8.
// c = L.c*R.a + L.d*temp*R.c = 1*1 + 0 = 1.
// d = L.c*R.b + L.d*temp*R.d = 1*1 + 0 = 1.
// So final a=8, correct.
//
// Thus the snippet’s initialization of `d=0` for a leaf works because in merges that involve a leaf as the right part (using L.b and R.d), the R.d=0 correctly ignores cross-boundary pairs when the right segment is a single digit and we need to exclude its leftmost (which is also its rightmost) – but actually if R is a single digit and we exclude its leftmost, the right segment becomes empty, and the cross-boundary pair would combine the left segment's rightmost (excluded) with the empty right? That would be invalid, so zero is correct. However, when we have a segment of size >=2, its `d` can become nonzero. So the snippet is fine.
//
// The segment tree supports point updates in O(log n) time, and each update returns the root’s `a` value modulo 998244353. Initialization builds the tree in O(n) time. Total complexity: Let n be the length of the digit string, and q the number of updates. Building the tree is O(n), each update is O(log n). Space is O(n) for segment tree nodes.
//
// Edge cases: n=1: For a single digit, after any update the answer is always 1 (only one way: the digit alone). The segment tree with a single leaf gives a=1. Also, if the digit string has length 0? The problem says n>=1. Also updates always valid indices. The modulo is 998244353, and the DP values can become large, so use long long.
