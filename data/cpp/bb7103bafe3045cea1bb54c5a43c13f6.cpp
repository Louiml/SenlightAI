Given a binary tree represented by its preorder and inorder traversals as null-terminated C-strings (each containing uppercase letters `'A'`..`'Z'`, with all characters distinct within a single tree), write a C++ function `std::string reconstructPostorder(const std::string& preorder, const std::string& inorder)` that returns the postorder traversal as a `std::string`. The tree contains between 1 and 26 nodes. The recursion must be implemented iteratively or recursively, but must handle the case where a subtree is empty (i.e., a missing left or right child) correctly without indexing out of bounds. The input strings are guaranteed to represent a valid binary tree.

#include <cassert>
#include <string>

// Include or paste the solution function here (or link it)

int main() {
    // Single node
    assert(reconstructPostorder("A", "A") == "A");

    // Left-skewed tree: preorder A B C, inorder C B A => postorder C B A
    assert(reconstructPostorder("ABC", "CBA") == "CBA");

    // Right-skewed tree: preorder A B C, inorder A B C => postorder C B A
    assert(reconstructPostorder("ABC", "ABC") == "CBA");

    // Balanced tree: preorder A B D E C F G, inorder D B E A F C G
    // Postorder: D E B F G C A
    assert(reconstructPostorder("ABDECFG", "DBEAFCG") == "DEBFGCA");

    // Example from the snippet: preorder = "DBAC", inorder = "ABCD"
    // Tree: root D, left = B (with left A, right C) => postorder A C B D
    assert(reconstructPostorder("DBAC", "ABCD") == "ACBD");

    // More complex: preorder "GDAFEMHZ", inorder "ADEFGHMZ"
    // Postorder should be "AEFDHZMG"
    assert(reconstructPostorder("GDAFEMHZ", "ADEFGHMZ") == "AEFDHZMG");

    // All 26 letters in a chain (right skew): preorder = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    // Inorder same => postorder = reverse of preorder
    std::string chain = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string expectedPost = "ZYXWVUTSRQPONMLKJIHGFEDCBA";
    assert(reconstructPostorder(chain, chain) == expectedPost);

    // Two nodes, left child: preorder "AB", inorder "BA" => postorder "BA"
    assert(reconstructPostorder("AB", "BA") == "BA");

    // Two nodes, right child: preorder "AB", inorder "AB" => postorder "BA"
    assert(reconstructPostorder("AB", "AB") == "BA");

    // A more random valid tree: preorder "FBA", inorder "BFA" => postorder "BAF"
    assert(reconstructPostorder("FBA", "BFA") == "BAF");

    return 0;
}

#include <string>
#include <unordered_map>
#include <algorithm>

// Helper that recursively reconstructs postorder.
// preS: start index in preorder, preLen: length of current subtree in preorder
// inS: start index in inorder, inLen: length of current subtree in inorder
// inPos: map from character to its index in inorder
// post: accumulates the result
void buildPostorder(const std::string& pre, int preS, int preLen,
                    const std::string& in, int inS, int inLen,
                    const std::unordered_map<char, int>& inPos,
                    std::string& post) {
    if (preLen <= 0) return;

    char root = pre[preS];
    int rootPosInInorder = inPos.at(root); // global position in inorder
    int leftLen = rootPosInInorder - inS; // length of left subtree
    int rightLen = inLen - leftLen - 1;   // length of right subtree

    // Recurse left subtree (if any)
    if (leftLen > 0) {
        buildPostorder(pre, preS + 1, leftLen,
                       in, inS, leftLen,
                       inPos, post);
    }
    // Recurse right subtree (if any)
    if (rightLen > 0) {
        buildPostorder(pre, preS + 1 + leftLen, rightLen,
                       in, inS + leftLen + 1, rightLen,
                       inPos, post);
    }
    // Append root last
    post.push_back(root);
}

// Public function: given preorder and inorder traversals, return postorder.
std::string reconstructPostorder(const std::string& preorder, const std::string& inorder) {
    // Build map from character to its index in inorder (all characters distinct)
    std::unordered_map<char, int> inPos;
    for (int i = 0; i < static_cast<int>(inorder.size()); ++i) {
        inPos[inorder[i]] = i;
    }

    std::string post;
    post.reserve(preorder.size());
    buildPostorder(preorder, 0, preorder.size(),
                   inorder, 0, inorder.size(),
                   inPos, post);
    return post;
}

// The solution uses the classic divide-and-conquer reconstruction of a binary tree from its preorder and inorder traversals. In preorder, the first character is always the root. In inorder, all characters to the left of the root's position form the inorder traversal of the left subtree, and all characters to the right form the inorder traversal of the right subtree. Since preorder also lists the left subtree's nodes before the right subtree's, we can recursively reconstruct. The postorder is built by first recursing to the left subtree, then to the right subtree, and finally appending the root. A direct recursive function can take the preorder start index and length of the current subtree, plus the inorder start index and length, to avoid copying strings. Edge cases include when a subtree has length 0 (do nothing), or when one side is missing (e.g., a skew tree). Time complexity is O(n) per node searched in the `find` step if done linearly, leading to O(n²) in the worst case; however, with distinct characters and a small alphabet (≤26), we can precompute positions in an array for O(1) lookups, resulting in O(n) total time. Space complexity is O(n) for the recursion stack and the output string.
//
// A simpler and robust approach: recursively build the postorder string by passing preorder and inorder as `std::string` and trimming them. But to avoid excessive copying, use indices. Since the problem guarantees distinct characters, we can precompute a position map for inorder. The recursive function can be implemented as a private helper (or a static function) that returns the postorder string, or better, appends to a string passed by reference. The main function `reconstructPostorder` initializes an empty string and calls the recursive helper.
