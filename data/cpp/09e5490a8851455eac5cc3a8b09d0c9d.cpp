Write a standalone C++ function `char findCommonAncestor(const std::vector<char>& insertionOrder, char first, char second)` that takes a vector of characters representing the order in which nodes are inserted into a binary search tree (BST) that stores single characters using their ASCII values for comparison, and returns the lowest common ancestor (LCA) of the two given characters `first` and `second`. The characters are distinct and both are guaranteed to exist in the tree. The LCA is defined as the deepest node that is an ancestor of both nodes (a node can be an ancestor of itself). The function must handle any insertion order, including cases where one character is an ancestor of the other, and should use O(1) auxiliary space beyond the input vector. The result should be returned as a `char` value. Assume the input vector is non-empty and contains only printable ASCII characters.

// The key observation is that in a binary search tree, the lowest common ancestor of two nodes is the first node whose value lies strictly between the two target values (or equals one of them if one is an ancestor of the other). When inserting nodes into a BST, the structure depends on insertion order, but we do not need to build the tree. Instead, we can simulate the insertion order: we know that the first inserted node becomes the root. As we scan the insertion order from the beginning to the end, the first node whose value is between `first` and `second` (inclusive on one side if equal) must be the LCA. Why? Because before encountering this node, all previously inserted nodes are either less than both or greater than both, so the targets lie in the same subtree of those nodes. The first node that "separates" them (i.e., lies between them) must be where the paths to the two targets diverge, and that node is the deepest common ancestor. Edge cases: (1) If one of the target values equals the first separating node, then that node is the LCA. (2) If one target is smaller and the other larger, the first node between them is the LCA. (3) If the first inserted node happens to be between them, that root is the LCA. We need to be careful with ordering: determine the lower and upper bounds by comparing `first` and `second`, then scan the insertion order. The first character `c` such that `c >= lower && c <= upper` is the answer. Since the input is guaranteed distinct and both targets exist, this always finds a valid LCA. Time complexity is O(n) for n insertions, and auxiliary space is O(1) because we only scan the vector without building any tree structure. No special handling is needed for duplicates because all characters are distinct.

#include <vector>
#include <cstddef>

// Find the lowest common ancestor of two characters in a BST defined by insertion order.
// The insertion order vector represents the sequence of insertions into an initially empty BST.
// Characters are compared by their ASCII values.
char findCommonAncestor(const std::vector<char>& insertionOrder, char first, char second) {
    char lower = (first < second) ? first : second;
    char upper = (first < second) ? second : first;
    
    // The first node in insertion order whose value lies within [lower, upper] is the LCA.
    for (std::size_t i = 0; i < insertionOrder.size(); ++i) {
        char current = insertionOrder[i];
        if (current >= lower && current <= upper) {
            return current;
        }
    }
    // Should never reach here for valid inputs, but return a sentinel.
    return '\0';
}

#include <cassert>
#include <vector>

char findCommonAncestor(const std::vector<char>& insertionOrder, char first, char second);

int main() {
    // Case 1: Root is the LCA
    std::vector<char> order1 = {'m', 'a', 'z', 'b', 'y'};
    assert(findCommonAncestor(order1, 'a', 'z') == 'm');
    
    // Case 2: One node is the ancestor of the other
    std::vector<char> order2 = {'d', 'b', 'f', 'a', 'c'};
    assert(findCommonAncestor(order2, 'b', 'c') == 'b');
    
    // Case 3: LCA is deeper than root
    std::vector<char> order3 = {'g', 'c', 'k', 'a', 'e', 'i', 'm'};
    assert(findCommonAncestor(order3, 'a', 'e') == 'c');
    
    // Case 4: LCA on right side
    std::vector<char> order4 = {'g', 'c', 'k', 'a', 'e', 'i', 'm'};
    assert(findCommonAncestor(order4, 'i', 'm') == 'k');
    
    // Case 5: Root is inserted first and is in range
    std::vector<char> order5 = {'x', 'a', 'b', 'c'};
    assert(findCommonAncestor(order5, 'b', 'c') == 'x');
    
    // Case 6: Targets are adjacent in insertion order, still works
    std::vector<char> order6 = {'p', 'q', 'r', 's'};
    assert(findCommonAncestor(order6, 'q', 's') == 'q');
    
    // Case 7: All nodes on one side
    std::vector<char> order7 = {'a', 'b', 'c', 'd'};
    assert(findCommonAncestor(order7, 'a', 'd') == 'a');
    
    // Case 8: Reverse order insertion
    std::vector<char> order8 = {'z', 'y', 'x', 'w'};
    assert(findCommonAncestor(order8, 'y', 'w') == 'y');
    
    // Case 9: Random order with distinct characters
    std::vector<char> order9 = {'h', 'd', 'l', 'b', 'f', 'j', 'n', 'a', 'c', 'e', 'g', 'i', 'k', 'm', 'o'};
    assert(findCommonAncestor(order9, 'a', 'o') == 'h');
    assert(findCommonAncestor(order9, 'b', 'g') == 'd');
    assert(findCommonAncestor(order9, 'j', 'o') == 'l');
    
    // Case 10: Single-element tree (both targets are the same, but input guarantee distinct, so use same char)
    std::vector<char> order10 = {'q'};
    assert(findCommonAncestor(order10, 'q', 'q') == 'q');
    
    return 0;
}
