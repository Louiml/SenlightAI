// Write a C++ function that computes the Merkle root of a set of transaction IDs using a partial Merkle tree construction. The function should take a vector of `uint256` hashes (representing transaction IDs) and a vector of booleans indicating which transactions are "matched" (i.e., relevant). It must return the Merkle root hash (a `uint256`) computed from the full tree, but constructed efficiently by only storing hashes for nodes that are ancestors of matched transactions and using the standard Bitcoin-style double-SHA256 combination for internal nodes. The function must handle edge cases such as zero transactions (returning a zero hash), a single transaction (returning that transaction's hash), and non-power-of-two tree widths by duplicating the last hash when a right child is missing. It must also validate that the input sizes are consistent (matching sizes, non-empty transaction list) and return a zero hash on invalid input. Do not use external libraries; implement your own SHA-256 double hash using a simple, self-contained hash function (e.g., a fixed, non-cryptographic placeholder is acceptable for the task, but the structure must correctly combine hashes).
The core idea is to build a binary Merkle tree where each leaf is a transaction hash and each internal node is the hash of its two children (left and right). Since we only need the root, we can compute it recursively without storing the entire tree. The partial tree construction from the snippet is adapted to a simpler eager computation: for a given set of hashes and match flags, we recursively compute the hash at each node by first checking if the node is an ancestor of any matched transaction (using a range check on the leaf indices covered by the node). If it is not an ancestor of any match, we can immediately compute the subtree's hash using a helper function that computes the full subtree hash without recursion (just by combining leaves bottom-up). If it is an ancestor, we recurse into the two children (if both exist; otherwise duplicate the left child's hash). At the leaf level (height 0), if the leaf is matched, we use its transaction hash; if not, we still use the transaction hash because even unmatched leaves are needed to compute parent hashes. The key optimization is that we don't need to store bits and hashes separately; we just return the root hash directly. For invalid inputs (empty transaction list, size mismatch between hashes and matches), return a zero `uint256`. For a single transaction, return its hash. The algorithm runs in O(n) time in the worst case (when all transactions are matched, we visit every node) and O(log n) auxiliary stack space for recursion, but the helper for full subtree computation is O(size of subtree). Overall, for n leaf nodes, the total time is O(n) because each node is processed at most once. Space complexity is O(n) for the input vectors and O(log n) recursion depth.
#include <cstdint>
#include <vector>
#include <stdexcept>
#include <cstring>

// Placeholder uint256 type (simplified for the task; real usage would use a proper 256-bit hash)
struct uint256 {
    uint64_t data[4]; // 256 bits

    uint256() { std::memset(data, 0, sizeof(data)); }
    bool operator==(const uint256& other) const {
        return std::memcmp(data, other.data, sizeof(data)) == 0;
    }
    bool operator!=(const uint256& other) const { return !(*this == other); }
};

// Placeholder double-SHA256 hash function (not cryptographically secure; for the task structure only)
// In a real implementation, this would hash the concatenation of left and right hashes twice with SHA-256.
uint256 doubleSha256(const uint256& left, const uint256& right) {
    uint256 result;
    // For the task, we'll produce a deterministic hash based on XOR and addition to demonstrate structure.
    for (int i = 0; i < 4; ++i) {
        result.data[i] = left.data[i] ^ right.data[i];
        result.data[i] += left.data[i] * 31 + right.data[i] * 17;
    }
    return result;
}

// Compute the hash of a full subtree (no pruning) given the leaf hashes.
uint256 fullSubtreeHash(int height, unsigned int pos, const std::vector<uint256>& vTxid) {
    if (height == 0) {
        return vTxid[pos];
    }
    unsigned int width = 1u << height; // number of nodes at this level
    unsigned int leftPos = pos * 2;
    unsigned int rightPos = pos * 2 + 1;
    uint256 left = fullSubtreeHash(height - 1, leftPos, vTxid);
    uint256 right;
    // If right child is beyond the tree width at the lower level, duplicate left.
    if (rightPos < (1u << (height - 1))) {
        // Actually, the tree width at height-1 is the number of leaves divided by 2^... 
        // Simpler: check if rightPos < (1u << (height - 1))? That's not correct. 
        // The tree width at height-1 is the number of nodes at that level, which is the same as the number of leaves at that level? 
        // For a complete binary tree, the number of nodes at height h is 2^h. But we have a non-complete tree.
        // We need to know the total number of leaves (vTxid.size()). Let's pass it or compute.
    }
    // The above is incorrect; we need to know the actual tree width. So we must pass leaf count.
    // For a complete tree, the number of leaves is a power of two, but for arbitrary n, we need to handle.
    // We'll revise: compute width at each level as ceil(n / 2^height). 
    // Better approach: compute recursively with a helper that knows the leaf count.
    // Since the task specifies we accept a vector of hashes, we know vTxid.size().
    // We'll write a separate helper that uses the actual tree width.
    // But to keep the solution concise, we'll just call a function that computes the full subtree directly by iterating over leaves.
    // Let's implement fullSubtreeHash iteratively: 
    // At height h, the subtree covers leaves from pos*2^h to (pos+1)*2^h - 1, clamped to n.
    // Compute the complete Merkle tree for that range using a loop.
    // However, for clarity, we'll implement a simpler recursive that passes the total leaf count.
    return left; // placeholder to avoid compilation error; actual implementation below.
}

// The actual solution function:
uint256 computeMerkleRoot(const std::vector<uint256>& vTxid, const std::vector<bool>& vMatch) {
    if (vTxid.empty() || vTxid.size() != vMatch.size()) {
        return uint256(); // invalid input, return zero hash
    }
    if (vTxid.size() == 1) {
        return vTxid[0];
    }

    // Recursive helper: computes the hash of the node at (height, pos) in a tree with n leaves.
    // height is the distance from the leaves (leaf height=0).
    // The leaves are indexed from 0 to n-1.
    // This node covers leaves from pos*2^height to (pos+1)*2^height - 1 (clamped to n).
    // Returns the Merkle hash for that subtree, possibly pruning if no matched leaves inside.
    std::function<uint256(int, unsigned int)> build = [&](int height, unsigned int pos) -> uint256 {
        unsigned int startLeaf = pos * (1u << height);
        unsigned int endLeaf = (pos + 1) * (1u << height) - 1;
        if (endLeaf >= vTxid.size()) endLeaf = vTxid.size() - 1;
        if (startLeaf > endLeaf) {
            return uint256(); // Should not happen
        }

        // Check if any leaf in this range is matched.
        bool hasMatch = false;
        for (unsigned int i = startLeaf; i <= endLeaf; ++i) {
            if (vMatch[i]) { hasMatch = true; break; }
        }

        if (height == 0) {
            // Leaf: always use the transaction hash (even if not matched, needed for parent computation)
            return vTxid[startLeaf];
        }

        if (!hasMatch) {
            // No matched leaves below; we can compute the full subtree hash directly (bottom-up).
            // We'll compute it iteratively to avoid recursion overhead.
            // The subtree has nodes at each level; we can compute level by level.
            // But for simplicity, we just compute recursively without pruning.
            // Since we already know no matches, we could compute the full hash directly without checking again.
            // But for correctness, we'll still call a function that computes the full hash.
            // To avoid code duplication, we'll define a lambda that computes full hash for this range.
            std::function<uint256(int, unsigned int)> full = [&](int h, unsigned int p) -> uint256 {
                if (h == 0) return vTxid[p];
                unsigned int leftEnd = (p * 2 + 1) * (1u << (h - 1)) - 1;
                unsigned int rightStart = (p * 2 + 1) * (1u << (h - 1));
                uint256 left = full(h - 1, p * 2);
                uint256 right;
                if (rightStart < vTxid.size()) {
                    right = full(h - 1, p * 2 + 1);
                } else {
                    right = left;
                }
                return doubleSha256(left, right);
            };
            return full(height, pos);
        }

        // Otherwise, descend into children.
        uint256 left = build(height - 1, pos * 2);
        // Determine if right child exists (i.e., there is at least one leaf in right subtree)
        unsigned int rightStartLeaf = (pos * 2 + 1) * (1u << (height - 1));
        uint256 right;
        if (rightStartLeaf < vTxid.size()) {
            right = build(height - 1, pos * 2 + 1);
        } else {
            right = left;
        }
        return doubleSha256(left, right);
    };

    // Compute height of the tree: smallest h such that 2^h >= n.
    int height = 0;
    while ((1u << height) < vTxid.size()) ++height;
    return build(height, 0);
}
#include <cassert>
#include <vector>
#include <functional>

// Include the solution here (or assume it's above). We'll duplicate necessary types for testing.

int main() {
    // Single transaction
    uint256 h1;
    h1.data[0] = 1;
    std::vector<uint256> v1 = {h1};
    std::vector<bool> m1 = {true};
    assert(computeMerkleRoot(v1, m1) == h1);

    // Two transactions
    uint256 h2;
    h2.data[0] = 2;
    std::vector<uint256> v2 = {h1, h2};
    std::vector<bool> m2 = {true, false};
    uint256 root2 = doubleSha256(h1, h2);
    assert(computeMerkleRoot(v2, m2) == root2);

    // Three transactions (non-power-of-two)
    uint256 h3;
    h3.data[0] = 3;
    std::vector<uint256> v3 = {h1, h2, h3};
    std::vector<bool> m3 = {false, true, false};
    // Compute expected: tree - leaves at height 0: h1,h2,h3. Height 1: node0 = H(h1,h2), node1 = H(h3,h3). Height 2: root = H(node0, node1)
    uint256 node0 = doubleSha256(h1, h2);
    uint256 node1 = doubleSha256(h3, h3);
    uint256 expected3 = doubleSha256(node0, node1);
    assert(computeMerkleRoot(v3, m3) == expected3);

    // Empty input should return zero hash
    std::vector<uint256> vEmpty;
    std::vector<bool> mEmpty;
    assert(computeMerkleRoot(vEmpty, mEmpty) == uint256());

    // Size mismatch
    std::vector<uint256> v4 = {h1, h2};
    std::vector<bool> m4 = {true}; // wrong size
    assert(computeMerkleRoot(v4, m4) == uint256());

    // All matched vs none matched should give same root (since we still use all leaves)
    std::vector<bool> mAllTrue = {true, true, true};
    assert(computeMerkleRoot(v3, mAllTrue) == expected3);
    std::vector<bool> mAllFalse = {false, false, false};
    assert(computeMerkleRoot(v3, mAllFalse) == expected3);

    // Four transactions (power of two)
    uint256 h4;
    h4.data[0] = 4;
    std::vector<uint256> v5 = {h1, h2, h3, h4};
    std::vector<bool> m5 = {true, false, false, true};
    uint256 n0 = doubleSha256(h1, h2);
    uint256 n1 = doubleSha256(h3, h4);
    uint256 expected4 = doubleSha256(n0, n1);
    assert(computeMerkleRoot(v5, m5) == expected4);

    // A larger test: 5 transactions
    std::vector<uint256> v6 = {h1, h2, h3, h4, h1}; // duplicate hash not allowed in real but fine here
    std::vector<bool> m6 = {false, true, false, false, true};
    // Compute expected:
    // Height 0: leaves h1,h2,h3,h4,h1
    // Height 1: nodes: a=H(h1,h2), b=H(h3,h4), c=H(h1,h1) (for last duplicate)
    // Height 2: nodes: d=H(a,b), e=H(c,c)
    // Height 3: root=H(d,e)
    uint256 a = doubleSha256(h1, h2);
    uint256 b = doubleSha256(h3, h4);
    uint256 c = doubleSha256(h1, h1);
    uint256 d = doubleSha256(a, b);
    uint256 e = doubleSha256(c, c);
    uint256 expected5 = doubleSha256(d, e);
    assert(computeMerkleRoot(v6, m6) == expected5);

    return 0;
}
