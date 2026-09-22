// Write a standalone C++ function `buildHuffmanTree` that takes a `std::vector<int>` of frequency counts for byte values (indices 0-255) and returns a pointer to the root of a Huffman coding trie. Each trie node must store a frequency count, a byte symbol, pointers to two children (`c0` for bit 0, `c1` for bit 1), and a pointer to its parent. Use a priority queue to repeatedly merge the two nodes with the smallest frequencies, creating a new internal node with symbol 0 and a frequency equal to the sum of the two children. The leaves of the resulting tree must correspond exactly to the original bytes with nonzero frequencies. The function must handle edge cases such as an empty frequency vector or all-zero frequencies (returning `nullptr`), a single nonzero frequency (returning a leaf node), and multiple symbols with equal frequencies (the tie-breaking order should be consistent: the priority queue may order equal frequencies arbitrarily, but the tree must be valid). The function must be memory-safe: all nodes must be dynamically allocated, and the caller is responsible for deallocating the entire tree. Provide a helper function `deleteTree` that recursively frees all nodes. Your solution must include the definition of a `HuffmanNode` struct with the specified fields.
#include <cassert>
#include <vector>
#include <iostream>

// (Include the solution code here or link it)

int main() {
    // Test 1: Empty frequency vector
    std::vector<int> freqs1;
    HuffmanNode* root1 = buildHuffmanTree(freqs1);
    assert(root1 == nullptr);

    // Test 2: All zeros
    std::vector<int> freqs2(256, 0);
    HuffmanNode* root2 = buildHuffmanTree(freqs2);
    assert(root2 == nullptr);

    // Test 3: Single nonzero frequency
    std::vector<int> freqs3(256, 0);
    freqs3[7] = 5;
    HuffmanNode* root3 = buildHuffmanTree(freqs3);
    assert(root3 != nullptr);
    assert(root3->c0 == nullptr && root3->c1 == nullptr);
    assert(root3->symbol == 7);
    assert(root3->count == 5);
    deleteTree(root3);

    // Test 4: Two symbols
    std::vector<int> freqs4(256, 0);
    freqs4[1] = 2;
    freqs4[2] = 3;
    HuffmanNode* root4 = buildHuffmanTree(freqs4);
    assert(root4 != nullptr);
    assert(root4->c0 != nullptr && root4->c1 != nullptr);
    assert(root4->count == 5);
    assert(root4->symbol == 0);
    // Children should be leaves with symbols 1 and 2
    int childSym0 = root4->c0->symbol;
    int childSym1 = root4->c1->symbol;
    assert((childSym0 == 1 && childSym1 == 2) || (childSym0 == 2 && childSym1 == 1));
    // Parent pointers
    assert(root4->c0->p == root4);
    assert(root4->c1->p == root4);
    deleteTree(root4);

    // Test 5: Several symbols, verify total count equals sum of frequencies
    std::vector<int> freqs5(256, 0);
    freqs5[10] = 1;
    freqs5[20] = 4;
    freqs5[30] = 8;
    freqs5[40] = 2;
    HuffmanNode* root5 = buildHuffmanTree(freqs5);
    assert(root5 != nullptr);
    int totalCount = 0;
    // Manual traversal to collect all leaf counts
    std::vector<HuffmanNode*> stack;
    stack.push_back(root5);
    long long sum = 0;
    while (!stack.empty()) {
        HuffmanNode* n = stack.back();
        stack.pop_back();
        if (n->c0 == nullptr && n->c1 == nullptr) {
            sum += n->count;
        } else {
            stack.push_back(n->c0);
            stack.push_back(n->c1);
        }
    }
    assert(sum == 15);
    deleteTree(root5);

    // Test 6: Many symbols, ensure root has no parent and root count is total sum
    std::vector<int> freqs6(256, 0);
    for (int i = 0; i < 256; ++i) {
        freqs6[i] = (i * 3) % 7 + 1;  // nonzero all
    }
    long long expectedSum = 0;
    for (int i = 0; i < 256; ++i) expectedSum += freqs6[i];
    HuffmanNode* root6 = buildHuffmanTree(freqs6);
    assert(root6 != nullptr);
    // Count all leaf frequencies
    long long leafSum = 0;
    std::vector<HuffmanNode*> stk;
    stk.push_back(root6);
    while (!stk.empty()) {
        HuffmanNode* n = stk.back();
        stk.pop_back();
        if (n->c0 == nullptr && n->c1 == nullptr) {
            leafSum += n->count;
        } else {
            stk.push_back(n->c0);
            stk.push_back(n->c1);
        }
    }
    assert(leafSum == expectedSum);
    // Check root's parent is null
    assert(root6->p == nullptr);
    deleteTree(root6);

    std::cout << "All Huffman tree tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <queue>
#include <memory>

// Node structure for Huffman coding tree
struct HuffmanNode {
    int count;             // frequency of this node's subtree
    int symbol;            // byte value for leaves, 0 for internal nodes
    HuffmanNode* c0;       // child for bit 0
    HuffmanNode* c1;       // child for bit 1
    HuffmanNode* p;        // parent (nullptr for root)

    HuffmanNode(int cnt, int sym)
        : count(cnt), symbol(sym), c0(nullptr), c1(nullptr), p(nullptr) {}
};

// Comparator for priority queue (min-heap by count)
struct HuffmanNodePtrComp {
    bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) const {
        // If counts equal, order by symbol (leaves) or by pointer address for internal nodes
        if (lhs->count == rhs->count) {
            return lhs->symbol > rhs->symbol;  // smaller symbol pops first
        }
        return lhs->count > rhs->count;  // smaller count pops first
    }
};

// Recursively delete all nodes in the tree
void deleteTree(HuffmanNode* node) {
    if (node == nullptr) return;
    deleteTree(node->c0);
    deleteTree(node->c1);
    delete node;
}

// Build a Huffman coding tree from frequency counts
HuffmanNode* buildHuffmanTree(const std::vector<int>& freqs) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanNodePtrComp> forest;

    for (size_t i = 0; i < freqs.size(); ++i) {
        if (freqs[i] > 0) {
            forest.push(new HuffmanNode(freqs[i], static_cast<int>(i)));
        }
    }

    if (forest.empty()) {
        return nullptr;
    }

    while (forest.size() > 1) {
        HuffmanNode* left = forest.top(); forest.pop();
        HuffmanNode* right = forest.top(); forest.pop();

        HuffmanNode* parent = new HuffmanNode(left->count + right->count, 0);
        parent->c0 = left;
        parent->c1 = right;
        left->p = parent;
        right->p = parent;

        forest.push(parent);
    }

    HuffmanNode* root = forest.top();
    forest.pop();
    // Clear parent of root if it was set (shouldn't be, but for safety)
    root->p = nullptr;
    return root;
}
// The core algorithm is the classic Huffman coding construction. First, create a leaf node for every index `i` where `freqs[i] > 0`, with `symbol = i`, `count = freqs[i]`, and children `c0 = c1 = nullptr`. Push these leaves into a min-heap priority queue ordered by `count` (and perhaps `symbol` as a tie-breaker for deterministic behavior). If the heap is empty (no nonzero frequencies), return `nullptr`. If the heap has exactly one node, that node is the root (it is a leaf). Otherwise, while more than one node remains, pop the two smallest nodes, create a new internal node with `count = left.count + right.count`, `symbol = 0`, set `c0 = left` and `c1 = right`, set both children's `p` pointer to this new node, and push the new node back. When only one node remains, that node is the root. The key edge cases: an all-zero frequency vector must yield a `nullptr` root; a single nonzero frequency yields a single-node tree (which technically has no children, and encoding/decoding would need special handling but the tree itself is valid). Time complexity is O(n log n) where n is the number of nonzero frequencies, due to heap operations. Space complexity is O(n) for the heap and the tree itself. The reconstruction of codes from leaves to root is straightforward but not needed for the function itself; the function only builds the tree. Memory management: every node is allocated with `new`, and a recursive `deleteTree` function frees children first then the node.
