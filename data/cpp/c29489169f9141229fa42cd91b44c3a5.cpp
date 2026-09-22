// Write a C++ function `buildPartialMerkleTree` that takes a vector of transaction IDs (`std::vector<uint256>`, where `uint256` is a fixed-size 256-bit integer type) and a vector of boolean match flags of the same size, and returns a `CPartialMerkleTree` object representing the partial Merkle tree that proves the matched transactions. The function must construct the tree correctly: the partial tree stores only the hashes needed to prove inclusion of the matched transactions, using the same recursive algorithm as the given snippet: a node is included if it is the parent of at least one matched leaf; otherwise its hash is stored. The returned tree must satisfy that `ExtractMatches` on it recovers exactly the matched transaction hashes and returns the correct Merkle root. Your implementation should define the `CPartialMerkleTree` class (with the constructor that takes `vTxid` and `vMatch`, `ExtractMatches`, `CalcHash`, `TraverseAndBuild`, `TraverseAndExtract`, and helper `CalcTreeWidth`), along with a minimal `uint256` type that supports the needed operations (hashing two child hashes). Use a simple byte-array-based hash function (e.g., a deterministic hash of the concatenated bytes of the child hashes) instead of real cryptographic hashing; the goal is to test tree structure correctness, not actual Bitcoin compatibility. The function signature should be `CPartialMerkleTree buildPartialMerkleTree(const std::vector<uint256>& vTxid, const std::vector<bool>& vMatch)`.

// The core is to build a partial Merkle tree from a list of transaction hashes and a match flag per transaction. The tree is a full binary tree with leaves at height 0 (the transaction hashes) and internal nodes at higher levels obtained by hashing the concatenation of left and right child. The tree height `nHeight` is the smallest `h` such that the number of leaves at that height (given by `CalcTreeWidth(h)` = ceil(nTransactions / 2^h)) is 1. The construction recursively traverses from the root (height `nHeight`, position 0). At each node, it checks if any leaf in its subtree (positions `pos << height` to `(pos+1) << height` - 1, bounded by `nTransactions`) is matched. If so, it appends `true` to `vBits` and (if height > 0) recurses into children; otherwise it appends `false` and stores the computed hash of that subtree. The stored hash for a leaf is simply the transaction ID; for an internal node it is the hash of the concatenation of left and right child hashes (right is same as left if the right child is beyond the tree width). The `ExtractMatches` method traverses the stored bits and hashes to recover the matched transaction IDs and recompute the root, verifying that all bits and hashes are consumed and the tree is well-formed. Edge cases: empty transaction list returns a bad tree; single transaction with no match stores the hash but returns no matched IDs; all transactions matched stores only the root hash; no transactions matched stores only the root hash. Time complexity is O(n log n) for build due to recursive hashing of subtrees (each node computed once), and O(n) for extraction. Space is O(n) for the stored hashes and bits.

#include <cstdint>
#include <vector>
#include <cassert>
#include <algorithm>
#include <stdexcept>

// Minimal 256-bit type for merkle tree leaves and internal nodes
struct uint256 {
    uint8_t data[32];
    uint256() { for (int i = 0; i < 32; ++i) data[i] = 0; }
    bool operator==(const uint256& other) const {
        for (int i = 0; i < 32; ++i) if (data[i] != other.data[i]) return false;
        return true;
    }
    bool operator!=(const uint256& other) const { return !(*this == other); }
    // Simple deterministic hash: XOR of bytes of left and right
    static uint256 hashPair(const uint256& left, const uint256& right) {
        uint256 result;
        for (int i = 0; i < 32; ++i) result.data[i] = left.data[i] ^ right.data[i];
        return result;
    }
};

// Partial Merkle Tree class
class CPartialMerkleTree {
public:
    CPartialMerkleTree() : nTransactions(0), fBad(true) {}
    CPartialMerkleTree(const std::vector<uint256>& vTxid, const std::vector<bool>& vMatch)
        : nTransactions(vTxid.size()), fBad(false) {
        vBits.clear();
        vHash.clear();
        if (nTransactions > 0) {
            int nHeight = 0;
            while (CalcTreeWidth(nHeight) > 1) nHeight++;
            TraverseAndBuild(nHeight, 0, vTxid, vMatch);
        }
    }

    uint256 ExtractMatches(std::vector<uint256>& vMatch) {
        vMatch.clear();
        if (nTransactions == 0 || fBad) return uint256();
        if (vHash.size() > nTransactions) return uint256();
        if (vBits.size() < vHash.size()) return uint256();
        int nHeight = 0;
        while (CalcTreeWidth(nHeight) > 1) nHeight++;
        unsigned int nBitsUsed = 0, nHashUsed = 0;
        uint256 hashMerkleRoot = TraverseAndExtract(nHeight, 0, nBitsUsed, nHashUsed, vMatch);
        if (fBad) return uint256();
        if ((nBitsUsed + 7) / 8 != (vBits.size() + 7) / 8) return uint256();
        if (nHashUsed != vHash.size()) return uint256();
        return hashMerkleRoot;
    }

private:
    unsigned int nTransactions;
    bool fBad;
    std::vector<bool> vBits;
    std::vector<uint256> vHash;

    unsigned int CalcTreeWidth(int height) const {
        return (nTransactions + (1 << height) - 1) >> height;
    }

    uint256 CalcHash(int height, unsigned int pos, const std::vector<uint256>& vTxid) {
        if (height == 0) {
            return vTxid[pos];
        } else {
            uint256 left = CalcHash(height - 1, pos * 2, vTxid);
            uint256 right;
            if (pos * 2 + 1 < CalcTreeWidth(height - 1))
                right = CalcHash(height - 1, pos * 2 + 1, vTxid);
            else
                right = left;
            return uint256::hashPair(left, right);
        }
    }

    void TraverseAndBuild(int height, unsigned int pos, const std::vector<uint256>& vTxid, const std::vector<bool>& vMatch) {
        bool fParentOfMatch = false;
        for (unsigned int p = pos << height; p < (pos + 1) << height && p < nTransactions; p++)
            fParentOfMatch |= vMatch[p];
        vBits.push_back(fParentOfMatch);
        if (height == 0 || !fParentOfMatch) {
            vHash.push_back(CalcHash(height, pos, vTxid));
        } else {
            TraverseAndBuild(height - 1, pos * 2, vTxid, vMatch);
            if (pos * 2 + 1 < CalcTreeWidth(height - 1))
                TraverseAndBuild(height - 1, pos * 2 + 1, vTxid, vMatch);
        }
    }

    uint256 TraverseAndExtract(int height, unsigned int pos, unsigned int& nBitsUsed, unsigned int& nHashUsed, std::vector<uint256>& vMatch) {
        if (nBitsUsed >= vBits.size()) {
            fBad = true;
            return uint256();
        }
        bool fParentOfMatch = vBits[nBitsUsed++];
        if (height == 0 || !fParentOfMatch) {
            if (nHashUsed >= vHash.size()) {
                fBad = true;
                return uint256();
            }
            const uint256& hash = vHash[nHashUsed++];
            if (height == 0 && fParentOfMatch)
                vMatch.push_back(hash);
            return hash;
        } else {
            uint256 left = TraverseAndExtract(height - 1, pos * 2, nBitsUsed, nHashUsed, vMatch);
            uint256 right;
            if (pos * 2 + 1 < CalcTreeWidth(height - 1))
                right = TraverseAndExtract(height - 1, pos * 2 + 1, nBitsUsed, nHashUsed, vMatch);
            else
                right = left;
            return uint256::hashPair(left, right);
        }
    }
};

// Solution function: build partial merkle tree from transaction ids and match flags
CPartialMerkleTree buildPartialMerkleTree(const std::vector<uint256>& vTxid, const std::vector<bool>& vMatch) {
    assert(vTxid.size() == vMatch.size());
    return CPartialMerkleTree(vTxid, vMatch);
}

#include <cassert>
#include <vector>
#include <iostream>

// Assume uint256 and CPartialMerkleTree are defined as above (include the solution header)

int main() {
    // Helper to create a uint256 with a specific byte pattern
    auto makeUint = [](uint8_t b) {
        uint256 h;
        for (int i = 0; i < 32; ++i) h.data[i] = b;
        return h;
    };

    // Test 1: Simple 2 transactions, both matched
    {
        std::vector<uint256> txids = { makeUint(1), makeUint(2) };
        std::vector<bool> match = { true, true };
        CPartialMerkleTree tree = buildPartialMerkleTree(txids, match);
        std::vector<uint256> matches;
        uint256 root = tree.ExtractMatches(matches);
        assert(root == uint256::hashPair(txids[0], txids[1]));
        assert(matches.size() == 2);
        assert(matches[0] == txids[0]);
        assert(matches[1] == txids[1]);
    }

    // Test 2: 4 transactions, only one matched
    {
        std::vector<uint256> txids = { makeUint(10), makeUint(11), makeUint(12), makeUint(13) };
        std::vector<bool> match = { false, true, false, false };
        CPartialMerkleTree tree = buildPartialMerkleTree(txids, match);
        std::vector<uint256> matches;
        uint256 root = tree.ExtractMatches(matches);
        // Expected root: hash of (hash(10,11), hash(12,13))
        uint256 h01 = uint256::hashPair(txids[0], txids[1]);
        uint256 h23 = uint256::hashPair(txids[2], txids[3]);
        uint256 expectedRoot = uint256::hashPair(h01, h23);
        assert(root == expectedRoot);
        assert(matches.size() == 1);
        assert(matches[0] == txids[1]);
    }

    // Test 3: 5 transactions, none matched
    {
        std::vector<uint256> txids = { makeUint(20), makeUint(21), makeUint(22), makeUint(23), makeUint(24) };
        std::vector<bool> match = { false, false, false, false, false };
        CPartialMerkleTree tree = buildPartialMerkleTree(txids, match);
        std::vector<uint256> matches;
        uint256 root = tree.ExtractMatches(matches);
        assert(matches.empty());
        // Correct merkle root with odd number of leaves: last duplicated
        uint256 h01 = uint256::hashPair(txids[0], txids[1]);
        uint256 h23 = uint256::hashPair(txids[2], txids[3]);
        uint256 h4dup = uint256::hashPair(txids[4], txids[4]);
        uint256 h01_23 = uint256::hashPair(h01, h23);
        uint256 expectedRoot = uint256::hashPair(h01_23, h4dup);
        assert(root == expectedRoot);
    }

    // Test 4: Single transaction, matched
    {
        std::vector<uint256> txids = { makeUint(30) };
        std::vector<bool> match = { true };
        CPartialMerkleTree tree = buildPartialMerkleTree(txids, match);
        std::vector<uint256> matches;
        uint256 root = tree.ExtractMatches(matches);
        assert(root == txids[0]);
        assert(matches.size() == 1);
        assert(matches[0] == txids[0]);
    }

    // Test 5: 6 transactions, all matched
    {
        std::vector<uint256> txids;
        for (int i = 0; i < 6; ++i) txids.push_back(makeUint((uint8_t)(40+i)));
        std::vector<bool> match(6, true);
        CPartialMerkleTree tree = buildPartialMerkleTree(txids, match);
        std::vector<uint256> matches;
        uint256 root = tree.ExtractMatches(matches);
        // Compute expected root manually
        uint256 h01 = uint256::hashPair(txids[0], txids[1]);
        uint256 h23 = uint256::hashPair(txids[2], txids[3]);
        uint256 h45 = uint256::hashPair(txids[4], txids[5]);
        uint256 h01_23 = uint256::hashPair(h01, h23);
        uint256 expectedRoot = uint256::hashPair(h01_23, h45);
        assert(root == expectedRoot);
        assert(matches.size() == 6);
        for (size_t i = 0; i < 6; ++i) assert(matches[i] == txids[i]);
    }

    // Test 6: Empty input should produce a bad tree
    {
        std::vector<uint256> txids;
        std::vector<bool> match;
        CPartialMerkleTree tree = buildPartialMerkleTree(txids, match);
        std::vector<uint256> matches;
        uint256 root = tree.ExtractMatches(matches);
        assert(matches.empty());
        // Default uint256 is all zeros; ExtractMatches returns zero for bad
        assert(root == uint256());
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}
