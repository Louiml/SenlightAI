Write a C++ function `recoverPermutation(int n, auto query)` that takes an integer `n` (2 ≤ n ≤ 10^5) and a query function `query(i, j, type)` which performs an interactive bitwise operation between elements at 1-indexed positions `i` and `j` in a hidden array of distinct integers in `[0, n-1]`. The `type` parameter is 0 for AND, 1 for OR, and 2 for XOR, and each query returns the corresponding bitwise result. The function must determine and return the original array as a `vector<int>` of length `n` using at most `n+1` queries. The hidden array is guaranteed to be a permutation of numbers from 0 to n-1, and the query function will output to standard output and read from standard input (similar to an interactive judge). Your solution must handle the fact that only 3 queries are needed to determine the first two elements, and then infer the rest using XOR chain queries.
The key insight is that from AND and XOR of two numbers, we can recover both numbers uniquely: for each bit, if AND has 1, both numbers have 1; if XOR has 1, exactly one has 1, so assign the 1 to the first number (and 0 to the second) arbitrarily, but we must later resolve the ambiguity. So from query(1,2,AND) and query(1,2,XOR), we can construct a candidate pair (a1, a2) where a1 and a2 might be swapped. To disambiguate, we ask query(2,3,OR) and use the fact that a2 OR a3 must equal the true OR; we have already assigned a2 from the candidate pair, and we can test whether (a2 candidate OR a3?) matches. But we don't know a3 yet, so we need a clever check: given candidate a2, we can compute a3 = (OR(2,3) ^ candidate_a2) only if candidate_a2 is correct; but actually we can use the property that for the correct a2, (a2 OR a3) will have all bits set that are in either, and we can use XOR to compute a3 after we fix a2. The standard trick: compute candidate a1 and a2 from AND and XOR, then query OR(2,3). Compute a3 = OR(2,3) XOR a2 (since for any a2, a3 = (a2 OR a3) ^ a2 only holds if a2 has a bit set only when a3 also has it? Actually the correct relation is: a3 = and not, hmm). Let's derive: Let A=a2, B=a3. We know R=A|B, and we don't know A yet. But from the candidate pair, we have two candidates for A: either ans1 or ans2. For each candidate A', we can compute B' = R ^ A'? That's not correct. The correct formula: B = (R & ~A) | (A & ~A)? No. Better: From A and R=A|B, we can determine B only if we also know A&B. But we don't. However, the key trick is: if we take the candidate A_guess, then compute B_guess = R & (~A_guess)? That gives bits that are in R but not in A_guess, which must be in B. But B might also have bits that are in A_guess if those bits are in both. So B_guess = R ^ A_guess? That is not correct either.

Standard solution: We query AND(1,2), XOR(1,2) → get (x,y) pair. Then compute a1 and a2 candidates. Then query OR(2,3). Now, note that for the correct a2, we have: a3 = (a2 ^ (a2|a3) ^ (a2&a3))? No. The easiest way: Use the relation a1 XOR a2 = valx (known). We also know a1 AND a2 = vala. From these two we can get a1 and a2 uniquely (bitwise: if XOR bit is 0 and AND bit is 0 → both 0; if XOR bit 0 and AND bit 1 → both 1; if XOR bit 1 and AND bit 0 → one is 1, other 0 — but we don't know which). So there are two possibilities: (x,y) and (y,x). Now we query OR(2,3) = valo. For each candidate a2, we can compute candidate a3 = valo ^ a2? Actually, if a2 is correct, then a3 = (valo ^ a2) but only if a2 has no bit set that a3 doesn't have? Let's check: a3 = valo ^ a2? If a bit is 1 in both a2 and a3, then valo bit is 1, a2 bit 1, xor gives 0, but a3 should be 1 — wrong. So that's incorrect.

The official approach: Use the fact that a2 OR a3 must have all bits that a2 has. So if candidate a2 has a bit set that is not in valo, it's impossible, meaning candidate is wrong. Actually, since valo = a2|a3, every bit set in a2 must be set in valo. So we check: for candidate a2, if (a2 & ~valo) != 0, then that candidate is wrong. But both candidates might pass? Because a2 bits are a subset of a2|a3 always. So that check always passes for the correct one, but might also pass for the wrong one if the wrong one also has bits subset of valo. So not enough.

Simpler: Query AND(1,2), XOR(1,2), then we have two possible (a1,a2). Query XOR(2,3) and XOR(1,3) — but that would be more queries. The classic solution uses only n+1 queries: query AND(1,2), XOR(1,2), then for i from 3 to n, query XOR(i-1,i). Also query OR(2,3) to disambiguate. The trick: From XOR(1,2) and XOR(2,3) we can compute XOR(1,3) = XOR(1,2) XOR XOR(2,3). Also we can compute AND(1,3) from AND(1,2), AND(2,3) but we don't have AND(2,3). However, we can use the fact that given a1, a2 candidates and a3 = ? We have two candidate pairs for (a1, a2). For each, we can derive a3 = XOR(2,3) XOR candidate_a2? No, because XOR(2,3) = a2 XOR a3, so a3 = XOR(2,3) XOR a2. That's correct! Because XOR is bitwise, a3 = (a2 XOR a3) XOR a2. Yes, that works perfectly! Because XOR is its own inverse. So for each candidate a2, we compute a3_candidate = (XOR(2,3) XOR a2_candidate). That gives a potential a3. Then we have a triple (a1_candidate, a2_candidate, a3_candidate). Now, we can validate: compute OR(2,3) = a2 OR a3. We have a2_candidate and a3_candidate, so we can check if (a2_candidate | a3_candidate) == OR(2,3) which we query. Whichever candidate pair satisfies this equation is the correct one. Since the array is a permutation of distinct numbers from 0 to n-1, and there is exactly one correct assignment, only one candidate will pass. Then once we have a1, a2, a3, we can compute all others via XOR(i-1,i) queries.

Edge cases: n=2, we still need to disambiguate; we can query OR(1,2) and check which candidate pair matches. Actually with n=2, we have two candidates, we can check using OR(1,2) similarly: candidate pair (a1,a2) must satisfy (a1|a2) == OR(1,2). Only one will. So we can handle n=2 with 3 queries too (AND, XOR, OR). For n>=3, we use AND(1,2), XOR(1,2), XOR(2,3), XOR(3,4)... total (n-1) XOR queries + 1 AND + 1 OR = n+1 queries. The disambiguation uses OR(2,3) and XOR(2,3) (already have since i=3 gives XOR(2,3)). So total queries: AND(1,2), XOR(1,2), OR(2,3), and XOR(i-1,i) for i=3..n → that's 3 + (n-2) = n+1 queries. Good.

Time complexity: O(n * bits) for bitwise operations constant. Overall O(n). Space O(n).
#include <bits/stdc++.h>
using namespace std;

// Function to recover a hidden permutation of [0, n-1] using at most n+1 queries.
// query(i, j, type) returns:
//   type 0: AND of elements at 1-indexed positions i and j
//   type 1: OR of elements at 1-indexed positions i and j
//   type 2: XOR of elements at 1-indexed positions i and j
// Returns the recovered array as a vector<int> of length n.
vector<int> recoverPermutation(int n, const function<int(int,int,int)>& query) {
    // Query first two elements
    int and12 = query(1, 2, 0);
    int xor12 = query(1, 2, 2);

    // Build two candidate pairs from AND and XOR
    int candA1 = 0, candA2 = 0;
    // For each bit, if AND has 1 -> both are 1
    // If XOR has 1 -> one is 1, the other 0 (assign arbitrarily to candA1)
    for (int bit = 0; bit < 31; ++bit) {
        int a = (and12 >> bit) & 1;
        int x = (xor12 >> bit) & 1;
        if (a == 1) {
            candA1 |= (1 << bit);
            candA2 |= (1 << bit);
        } else if (x == 1) {
            candA1 |= (1 << bit);
        }
    }

    vector<int> result(n);
    if (n == 2) {
        // Disambiguate using OR(1,2)
        int or12 = query(1, 2, 1);
        if ((candA1 | candA2) == or12) {
            result[0] = candA1;
            result[1] = candA2;
        } else {
            // swap
            result[0] = candA2;
            result[1] = candA1;
        }
        return result;
    }

    // For n >= 3, query XOR(2,3) and OR(2,3)
    int xor23 = query(2, 3, 2);
    int or23 = query(2, 3, 1);

    // Try both candidate assignments for a1, a2
    vector<pair<int,int>> candidates = {{candA1, candA2}, {candA2, candA1}};
    bool found = false;
    for (const auto& [a1_cand, a2_cand] : candidates) {
        int a3_cand = xor23 ^ a2_cand;
        if ((a2_cand | a3_cand) == or23) {
            result[0] = a1_cand;
            result[1] = a2_cand;
            result[2] = a3_cand;
            found = true;
            break;
        }
    }
    // The problem guarantees a valid permutation, so found must be true.
    assert(found);

    // Now compute remaining elements using XOR chain
    int prev = result[1];
    for (int i = 3; i <= n; ++i) {
        int xor_prev_cur = query(i-1, i, 2);
        int cur = xor_prev_cur ^ prev;
        result[i-1] = cur; // 0-indexed
        prev = cur;
    }

    return result;
}
#include <bits/stdc++.h>
using namespace std;

// Mock query function for testing
int main() {
    // Test 1: n = 5, hidden array = {0, 1, 2, 3, 4}
    vector<int> hidden1 = {0, 1, 2, 3, 4};
    int n1 = (int)hidden1.size();
    auto query1 = [&](int i, int j, int type) -> int {
        i--; j--;
        if (type == 0) return hidden1[i] & hidden1[j];
        if (type == 1) return hidden1[i] | hidden1[j];
        return hidden1[i] ^ hidden1[j];
    };
    vector<int> rec1 = recoverPermutation(n1, query1);
    assert(rec1 == hidden1);

    // Test 2: random permutation n = 6
    vector<int> hidden2 = {5, 0, 3, 2, 1, 4};
    int n2 = (int)hidden2.size();
    auto query2 = [&](int i, int j, int type) -> int {
        i--; j--;
        if (type == 0) return hidden2[i] & hidden2[j];
        if (type == 1) return hidden2[i] | hidden2[j];
        return hidden2[i] ^ hidden2[j];
    };
    vector<int> rec2 = recoverPermutation(n2, query2);
    assert(rec2 == hidden2);

    // Test 3: n = 2
    vector<int> hidden3 = {1, 0};
    int n3 = (int)hidden3.size();
    auto query3 = [&](int i, int j, int type) -> int {
        i--; j--;
        if (type == 0) return hidden3[i] & hidden3[j];
        if (type == 1) return hidden3[i] | hidden3[j];
        return hidden3[i] ^ hidden3[j];
    };
    vector<int> rec3 = recoverPermutation(n3, query3);
    assert(rec3 == hidden3);

    // Test 4: n = 3
    vector<int> hidden4 = {2, 0, 1};
    int n4 = (int)hidden4.size();
    auto query4 = [&](int i, int j, int type) -> int {
        i--; j--;
        if (type == 0) return hidden4[i] & hidden4[j];
        if (type == 1) return hidden4[i] | hidden4[j];
        return hidden4[i] ^ hidden4[j];
    };
    vector<int> rec4 = recoverPermutation(n4, query4);
    assert(rec4 == hidden4);

    // Test 5: n = 10, reverse permutation
    vector<int> hidden5(10);
    for (int i = 0; i < 10; ++i) hidden5[i] = 9 - i;
    int n5 = (int)hidden5.size();
    auto query5 = [&](int i, int j, int type) -> int {
        i--; j--;
        if (type == 0) return hidden5[i] & hidden5[j];
        if (type == 1) return hidden5[i] | hidden5[j];
        return hidden5[i] ^ hidden5[j];
    };
    vector<int> rec5 = recoverPermutation(n5, query5);
    assert(rec5 == hidden5);

    cout << "All tests passed!" << endl;
    return 0;
}
