// Write a C++ function `int maxSubarrayXOR(int n, const vector<int>& arr)` that, given an array of non-negative integers, returns the maximum XOR value obtainable from any contiguous subarray (including subarrays of length 1). The function must efficiently handle arrays up to size \(10^5\) with values up to \(10^9\). Use a binary trie to store prefix XOR values and query the maximum XOR for each prefix in \(O(1)\) per bit, tracking the overall maximum across all prefixes.
The key insight is that the XOR of any subarray `arr[l..r]` equals `prefixXOR[r] ^ prefixXOR[l-1]`, where `prefixXOR[i]` is the XOR of elements from index 0 to i. Therefore, the maximum subarray XOR is the maximum XOR between any two prefix XOR values (including an initial prefix of 0 for the empty prefix before index 0). We build a binary trie where each node has left/right children representing bit 0/1 and a `freq` counter indicating how many numbers have passed through that node. Insert the initial prefix 0. For each new prefix value `pre`, traverse the trie from the most significant bit (bit 31) down to bit 0, at each step greedily choosing the opposite bit branch if it exists (because that maximizes the XOR), adding `(1 << bit)` to the answer when such an opposite branch is available, else taking the same-bit branch. Update the global maximum with this query result, then insert the current prefix into the trie. Edge cases: empty subarray is not allowed, so we must consider at least one element; ensure the trie's `freq` counts are decremented properly if removals were needed, but here we only insert, so `freq` is always >0 for inserted nodes. Time complexity is \(O(32n)\) = \(O(n)\) per array, and space is \(O(32n)\) in the worst case for the trie nodes.
#include <vector>
#include <algorithm>

struct TrieNode {
    int freq;
    TrieNode* left;
    TrieNode* right;
    TrieNode() : freq(0), left(nullptr), right(nullptr) {}
};

void insertTrie(int value, TrieNode* head) {
    TrieNode* cur = head;
    for (int bit = 31; bit >= 0; --bit) {
        int b = (value >> bit) & 1;
        if (b) {
            if (!cur->right) cur->right = new TrieNode();
            cur = cur->right;
        } else {
            if (!cur->left) cur->left = new TrieNode();
            cur = cur->left;
        }
        cur->freq++;
    }
}

int queryMaxXOR(int value, const TrieNode* head) {
    const TrieNode* cur = head;
    int ans = 0;
    for (int bit = 31; bit >= 0; --bit) {
        int b = (value >> bit) & 1;
        if (b) {
            // Prefer left (bit 0) to make XOR bit 1
            if (cur->left && cur->left->freq > 0) {
                ans += (1 << bit);
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        } else {
            // Prefer right (bit 1) to make XOR bit 1
            if (cur->right && cur->right->freq > 0) {
                ans += (1 << bit);
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
    }
    return ans;
}

int maxSubarrayXOR(int n, const std::vector<int>& arr) {
    TrieNode* head = new TrieNode();
    int prefix = 0;
    int best = 0;
    insertTrie(0, head);  // empty prefix
    for (int i = 0; i < n; ++i) {
        prefix ^= arr[i];
        best = std::max(best, queryMaxXOR(prefix, head));
        insertTrie(prefix, head);
    }
    // Clean up memory (not strictly necessary but good practice)
    // For simplicity in competitive setting, we skip deletion; in production, implement a destructor.
    return best;
}
#include <cassert>
#include <vector>

// Assume maxSubarrayXOR is declared above

int main() {
    // Single element
    assert(maxSubarrayXOR(1, {5}) == 5);
    // Simple array
    assert(maxSubarrayXOR(3, {1, 2, 3}) == 3);  // subarray [1,2] XOR=3
    // All zeros
    assert(maxSubarrayXOR(3, {0, 0, 0}) == 0);
    // Larger numbers
    assert(maxSubarrayXOR(4, {8, 1, 2, 12}) == 15); // [1,2,12] XOR=15
    // Negative not allowed per spec, but ensure non-negative inputs work
    assert(maxSubarrayXOR(2, {10, 10}) == 10);
    // Known tricky: all equal
    assert(maxSubarrayXOR(5, {7, 7, 7, 7, 7}) == 7);
    // Mixed including 0
    assert(maxSubarrayXOR(4, {0, 3, 5, 8}) == 13); // [5,8] XOR=13
    // Large array (small size)
    std::vector<int> large(100000, 123456789);
    assert(maxSubarrayXOR(100000, large) == 123456789);
    return 0;
}
