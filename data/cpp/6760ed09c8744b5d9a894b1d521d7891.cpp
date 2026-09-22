// Write a C++ function `bool canBuildBinaryTree(const std::vector<int>& preorder, const std::vector<int>& inorder)` that returns `true` if the given preorder and inorder traversal sequences (each containing unique integers) can form a valid binary tree, and `false` otherwise. The function must verify that the two sequences are consistent (e.g., same length, same set of values, and that the division of subtrees is valid at every step). Do not actually construct the tree; only validate feasibility. Handle edge cases including empty vectors (return `false` unless both are empty, in which case return `true`), mismatched lengths, and sequences that contain the same value in different positions.
// The solution mimics the recursive reconstruction logic but avoids building nodes. At each recursion step, the first element of the current preorder subrange is the root value. Search for that value in the current inorder subrange. If the value is not found, the sequences are inconsistent and the function returns `false`. If found, the number of elements to the left of the root in inorder gives the size of the left subtree. This size must match the number of elements in the corresponding preorder subrange (after the root). Then recursively validate the left and right subranges. Base case: if the subranges are empty, return `true`; if one is empty and the other is not, return `false`. Additional early checks ensure both vectors have the same size and, after sorting copies, contain the same elements; however, this sorting step is not strictly necessary for correctness but improves early rejection. Time complexity is O(n²) in the worst case (e.g., skew tree) due to linear search per recursion; space complexity is O(n) for recursion stack and O(n) for the sorted copies if used.
#include <vector>
#include <algorithm>

// Helper that validates subranges defined by indices.
bool validate(const std::vector<int>& pre, int preL, int preR,
              const std::vector<int>& in, int inL, int inR) {
    // Empty subranges on both sides.
    if (preL > preR && inL > inR) return true;
    // Mismatch of lengths.
    if (preL > preR || inL > inR) return false;
    
    int rootVal = pre[preL];
    // Find root in inorder.
    int inPos = inL;
    while (inPos <= inR && in[inPos] != rootVal) ++inPos;
    if (inPos > inR) return false; // not found
    
    int leftSize = inPos - inL;
    int rightSize = inR - inPos;
    
    // Check that preorder subrange length matches.
    if (preR - preL != leftSize + rightSize) return false;
    
    // Recurse on left and right.
    bool leftOk = validate(pre, preL+1, preL+leftSize, in, inL, inPos-1);
    bool rightOk = validate(pre, preL+leftSize+1, preR, in, inPos+1, inR);
    return leftOk && rightOk;
}

// Public function: returns true if preorder and inorder can construct a binary tree.
bool canBuildBinaryTree(const std::vector<int>& preorder, const std::vector<int>& inorder) {
    if (preorder.empty() && inorder.empty()) return true;
    if (preorder.size() != inorder.size()) return false;
    if (preorder.empty()) return false;
    
    // Quick check: same multisets.
    std::vector<int> p = preorder, i = inorder;
    std::sort(p.begin(), p.end());
    std::sort(i.begin(), i.end());
    if (p != i) return false;
    
    int n = preorder.size();
    return validate(preorder, 0, n-1, inorder, 0, n-1);
}
#include <cassert>
#include <vector>

// The solution function is above; for brevity assume it is included.

int main() {
    // Standard binary tree.
    std::vector<int> pre1 = {1,2,4,7,3,5,6,8};
    std::vector<int> in1  = {4,7,2,1,5,3,8,6};
    assert(canBuildBinaryTree(pre1, in1) == true);

    // Complete binary tree.
    std::vector<int> pre2 = {1,2,4,5,3,6,7};
    std::vector<int> in2  = {4,2,5,1,6,3,7};
    assert(canBuildBinaryTree(pre2, in2) == true);

    // Only left children.
    std::vector<int> pre3 = {1,2,3,4,5};
    std::vector<int> in3  = {5,4,3,2,1};
    assert(canBuildBinaryTree(pre3, in3) == true);

    // Only right children.
    std::vector<int> pre4 = {1,2,3,4,5};
    std::vector<int> in4  = {1,2,3,4,5};
    assert(canBuildBinaryTree(pre4, in4) == true);

    // Single node.
    std::vector<int> pre5 = {1};
    std::vector<int> in5  = {1};
    assert(canBuildBinaryTree(pre5, in5) == true);

    // Mismatched sequences (value 8 not found).
    std::vector<int> pre6 = {1,2,4,5,3,6,7};
    std::vector<int> in6  = {4,2,8,1,6,3,7};
    assert(canBuildBinaryTree(pre6, in6) == false);

    // Mismatched length.
    std::vector<int> pre7 = {1,2};
    std::vector<int> in7  = {1};
    assert(canBuildBinaryTree(pre7, in7) == false);

    // Both empty.
    std::vector<int> pre8 = {};
    std::vector<int> in8  = {};
    assert(canBuildBinaryTree(pre8, in8) == true);

    // One empty, one not.
    std::vector<int> pre9 = {1};
    std::vector<int> in9  = {};
    assert(canBuildBinaryTree(pre9, in9) == false);

    // Same values but wrong order (root not in inorder).
    std::vector<int> pre10 = {1,2,3};
    std::vector<int> in10  = {2,3,4}; // different set
    assert(canBuildBinaryTree(pre10, in10) == false);

    return 0;
}
