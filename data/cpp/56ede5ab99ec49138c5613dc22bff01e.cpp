// Given an array of block heights representing a blockchain, write a C++ function `int computeLastCommonAncestorHeight(const std::vector<int>& chainA, const std::vector<int>& chainB)` that determines the height of the last common ancestor of two chains. Each chain is represented as a vector of block heights where index 0 is the genesis block (height 0) and the last element is the tip. The chains may be of different lengths, but both share the same genesis block at height 0. The function should return the height (index) of the last block that is common to both chains. For example, if chainA = {0,1,2,3} and chainB = {0,1,2,4}, the last common ancestor is at height 2. Handle edge cases such as empty input (return -1), identical chains (return the last height), and chains sharing only the genesis block (return 0).

The problem is essentially finding the longest common prefix between two vectors. The main algorithm iterates over both vectors simultaneously, comparing elements at each index. Start from index 0 and continue while both indices are within bounds and the elements are equal. The last index where they match is the answer. If the first elements differ (which should not happen in a valid blockchain since both start at genesis), return -1. Edge cases: empty vectors return -1; if one vector is empty and the other is not, return -1 (since no genesis). Time complexity is O(min(n, m)) where n and m are the lengths of the input vectors. Space complexity is O(1) auxiliary, not counting the input storage. No sorting or preprocessing is needed; a simple linear scan works.

#include <vector>

/**
 * Computes the height of the last common ancestor of two block chains.
 * Each chain is represented as a vector of block heights, where index 0 is genesis (height 0).
 * Returns -1 if either chain is empty or no common ancestor exists.
 */
int computeLastCommonAncestorHeight(const std::vector<int>& chainA, const std::vector<int>& chainB) {
    if (chainA.empty() || chainB.empty()) {
        return -1;
    }

    int commonHeight = -1;
    size_t maxLen = std::min(chainA.size(), chainB.size());
    for (size_t i = 0; i < maxLen; ++i) {
        if (chainA[i] == chainB[i]) {
            commonHeight = static_cast<int>(i);
        } else {
            break;
        }
    }
    return commonHeight;
}

#include <cassert>
#include <vector>

int computeLastCommonAncestorHeight(const std::vector<int>& chainA, const std::vector<int>& chainB);

int main() {
    // Both chains share all blocks up to genesis only
    assert(computeLastCommonAncestorHeight({0}, {0}) == 0);
    
    // Identical chains
    std::vector<int> chain1 = {0, 1, 2, 3};
    std::vector<int> chain2 = {0, 1, 2, 3};
    assert(computeLastCommonAncestorHeight(chain1, chain2) == 3);
    
    // Diverging at height 2
    std::vector<int> chain3 = {0, 1, 2, 3};
    std::vector<int> chain4 = {0, 1, 2, 4};
    assert(computeLastCommonAncestorHeight(chain3, chain4) == 2);
    
    // Only genesis common
    std::vector<int> chain5 = {0, 10, 20};
    std::vector<int> chain6 = {0, 11, 21};
    assert(computeLastCommonAncestorHeight(chain5, chain6) == 0);
    
    // Different lengths, common prefix
    std::vector<int> chain7 = {0, 1, 2, 3, 4, 5};
    std::vector<int> chain8 = {0, 1, 2};
    assert(computeLastCommonAncestorHeight(chain7, chain8) == 2);
    
    // Empty chain
    std::vector<int> empty;
    assert(computeLastCommonAncestorHeight(empty, chain1) == -1);
    assert(computeLastCommonAncestorHeight(chain1, empty) == -1);
    
    // One element chains, same
    std::vector<int> only0 = {0};
    assert(computeLastCommonAncestorHeight(only0, only0) == 0);
    
    // Both empty
    assert(computeLastCommonAncestorHeight(empty, empty) == -1);
    
    return 0;
}
