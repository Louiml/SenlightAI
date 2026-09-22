Given a binary string `s` of length `n` representing positions where `'1'` denotes a building and `'0'` denotes empty space, write a C++ function `computeNearestOnes` that for each index `i` from `0` to `n-1` computes the total number of pairs `(j, k)` such that `j <= i < k`, `s[j] == '1'`, `s[k] == '1'`, and all positions between `j` and `k` (exclusive) contain at least one `'0'`. If for a given `i` it is impossible to find any such pair (because the number of ones to the right of `i` is zero, or there aren’t enough ones overall), the function should output `-1` for that index. The output is a space-separated list of integers printed in order from `i=0` to `i=n-1` to standard output, one line per test case. The function should handle multiple test cases, reading `n` and the string for each, and print results accordingly. For a string consisting entirely of `'1'`, no pair exists for any `i` because there is no `'0'` between any two ones, so all outputs are `-1`. The algorithm must run in `O(n)` time per test case and `O(n)` auxiliary space.
// The core observation is to count, for each possible `i`, how many pairs of ones surround `i` with at least one zero in between. A pair `(j,k)` with `j < i < k` and `s[j]=s[k]='1'` qualifies if there is at least one `'0'` strictly between `j` and `k`. The efficient way is to precompute prefix counts of ones and suffix counts of ones, but more directly: for each `i`, the number of valid pairs equals the number of ones to the left of `i` multiplied by the number of ones to the right of `i` **minus** the cases where there is no zero between the chosen left and right ones. However, the condition "at least one zero between" is automatically satisfied if there is at least one zero anywhere in the substring `(j,k)`. Since `i` lies between `j` and `k`, if there is a zero to the left of `i` (but after `j`) or a zero to the right of `i` (but before `k`), the condition holds. A simpler combinatorial derivation: the total number of pairs with `j < i < k` and both ones is `left_ones * right_ones`. From these, we must subtract pairs where the interval `(j,k)` contains no zero. Such a pair would require that all positions between `j` and `k` are `'1'`, which is impossible because `i` is between them and could be either `'0'` or `'1'`. If `s[i] == '0'`, then every such pair automatically has a zero, so no subtraction. If `s[i] == '1'`, then we need to ensure there is a zero in `(j,k)`; but since `i` is `'1'`, the only way to have no zero is if the entire segment from `j` to `k` is all ones, which would mean `j` and `k` are consecutive ones with no zero between. That means `j` must be the nearest `'1'` to the left of `i` and `k` the nearest `'1'` to the right of `i` with no zero between; but if `s[i]` is `'1'`, then there is no zero between `j` and `k` only if there is no zero at all in that interval, which would imply the interval consists entirely of ones and includes `i`. But since there is at least one `'1'` at `i`, the interval `(j,k)` could be all ones only if `j` and `k` are immediate neighbors with no zero between, which is impossible because `i` is between them and is `'1'`; actually if the entire segment is ones, then there is no zero, but then `j` and `k` are not necessarily immediate; however, if there is no zero between `j` and `k`, then every position is `'1'`, so `s[i]='1'` is fine. But then the pair still qualifies because the condition "at least one zero between j and k" fails, so we must subtract. So for `s[i]='1'`, we subtract the number of pairs `(j,k)` with `j < i < k`, both ones, and no zero in `(j,k)`. That count equals the product of consecutive ones runs? Actually, if there is no zero between `j` and `k`, then `j` and `k` must be in the same maximal block of consecutive ones that contains `i`. For a block of length `L` ones, the number of such pairs with `i` in the block is `(left_in_block) * (right_in_block)`, where `left_in_block` is the number of ones to the left of `i` within the same block, and `right_in_block` similarly. So the correct formula: for each `i`, `answer[i] = left_ones_total * right_ones_total - (if s[i]=='1' then (left_ones_same_block * right_ones_same_block) else 0)`. But we also need to check feasibility: if `right_ones_total == 0` (i.e., `ones[0] - ones[i] <= 0`? Actually ones to the right of i means indices > i, so count = `ones[i+1]` if we precompute suffix ones starting at index i+1). If there are no ones to the right (`right_ones == 0`) or no ones to the left (`left_ones == 0`), then no valid pair, so print `-1`. Also if `ones[0] < 2` (fewer than 2 ones total), then no pair for any i. The provided snippet uses a clever rolling sum with `pre` and `tp` to compute these values efficiently without explicit left/right multiplication for each i, but the essence is the same. The implementation below uses prefix/suffix ones and a block tracking to compute directly. Time complexity O(n) per test, space O(n) for prefix or suffix arrays.
//
// Edge cases: all zeros (no ones), all ones (no zero between any pair), single one, multiple test cases, n up to large. The function prints `-1` where no valid pair exists.
#include <vector>
#include <string>
#include <iostream>

// For each index i, print the number of pairs of ones (j,k) with j < i < k
// and at least one '0' strictly between j and k. If no such pair exists, print -1.
void computeNearestOnes(int n, const std::string& s) {
    std::vector<int> left_ones(n, 0);   // left_ones[i] = number of '1's in s[0..i-1]
    std::vector<int> right_ones(n, 0);  // right_ones[i] = number of '1's in s[i+1..n-1]
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        left_ones[i] = cnt;
        if (s[i] == '1') ++cnt;
    }
    cnt = 0;
    for (int i = n - 1; i >= 0; --i) {
        right_ones[i] = cnt;
        if (s[i] == '1') ++cnt;
    }

    // Track consecutive ones blocks to subtract pairs with no zero.
    std::vector<int> block_id(n, -1);
    std::vector<int> block_len;       // total length of each block
    std::vector<int> block_pos;       // position of each block's start
    int bid = 0;
    for (int i = 0; i < n; ) {
        if (s[i] == '1') {
            int start = i;
            while (i < n && s[i] == '1') {
                block_id[i] = bid;
                ++i;
            }
            block_len.push_back(i - start);
            block_pos.push_back(start);
            ++bid;
        } else {
            ++i;
        }
    }

    for (int i = 0; i < n; ++i) {
        // Check if there is at least one '1' on both sides.
        if (left_ones[i] == 0 || right_ones[i] == 0) {
            std::cout << -1;
        } else {
            long long total_pairs = 1LL * left_ones[i] * right_ones[i];
            long long subtract = 0;
            if (s[i] == '1') {
                int id = block_id[i];
                // position within block
                int pos_in_block = i - block_pos[id];
                int left_in_block = pos_in_block;  // ones to left within same block
                int right_in_block = block_len[id] - pos_in_block - 1;
                subtract = 1LL * left_in_block * right_in_block;
            }
            std::cout << (total_pairs - subtract);
        }
        if (i != n - 1) std::cout << " ";
    }
    std::cout << "\n";
}
#include <cassert>
#include <sstream>

// Redirect cout to test the printing function.
std::string capture(int n, const std::string& s) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    computeNearestOnes(n, s);
    std::cout.rdbuf(old);
    return oss.str();
}

int main() {
    assert(capture(3, "111") == "-1 -1 -1\n");
    assert(capture(3, "000") == "-1 -1 -1\n");
    assert(capture(2, "10") == "-1 -1\n");
    assert(capture(4, "1001") == "-1 1 1 -1\n");
    assert(capture(5, "10101") == "1 2 2 2 1\n");
    assert(capture(1, "1") == "-1\n");
    assert(capture(1, "0") == "-1\n");
    assert(capture(6, "110011") == "1 1 -1 -1 1 1\n");
    assert(capture(5, "01010") == "1 1 1 1 1\n");
    assert(capture(7, "1011001") == "1 2 2 1 2 2 1\n");
    return 0;
}
