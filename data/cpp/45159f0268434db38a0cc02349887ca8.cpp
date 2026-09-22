Given two strings representing preorder and inorder traversals of a binary tree (with unique node labels, each a single uppercase letter), write a C++ function `std::string postorder(const std::string& pre, const std::string& in)` that reconstructs the tree and returns its postorder traversal as a string. The input strings are non-empty, have equal length, and contain only distinct uppercase letters ('A'–'Z'). Do not allocate tree nodes; reconstruct the traversal recursively using only the input strings and output the postorder sequence.

#include <cassert>
#include <string>

// Forward declaration of the solution function (test-only).
std::string postorder(const std::string& pre, const std::string& in);

int main() {
    assert(postorder("ABC", "BAC") == "BCA");
    assert(postorder("AB", "AB") == "BA");          // skewed left
    assert(postorder("AB", "BA") == "BA");          // skewed right (same as above here)
    assert(postorder("A", "A") == "A");             // single node
    assert(postorder("DBAC", "ABCD") == "ACBD");    // balanced two-level tree
    assert(postorder("GEA", "AGE") == "AEG");       // chain with 3 nodes
    assert(postorder("ABCDEF", "CBDAEF") == "CDBFEA");
    assert(postorder("XYZW", "WZYX") == "WZYX");    // all left children
    assert(postorder("XWZY", "WXYZ") == "WZYX");    // all right children
    assert(postorder("ACB", "ABC") == "BCA");       // root left child then right
    return 0;
}

#include <string>

// Reconstruct tree postorder from preorder and inorder traversals.
// The labels are unique uppercase letters. Returns the postorder string.
std::string postorder(const std::string& pre, const std::string& in) {
    // Recursive helper that builds postorder by processing preorder.
    // Returns the next unused index in preorder after processing subtree.
    int build(const std::string& pre, const std::string& in,
              int pi, int low, int high, std::string& out) {
        if (low >= high) {
            return pi;  // Empty subtree, nothing to do.
        }
        // The current root is pre[pi]; find it in inorder[low..high).
        char root = pre[pi];
        int i = low;
        while (i < high && in[i] != root) {
            ++i;
        }
        // Reconstruct left subtree, then right subtree.
        pi = build(pre, in, pi + 1, low, i, out);
        pi = build(pre, in, pi, i + 1, high, out);
        // Append root after both children (postorder).
        out.push_back(root);
        return pi;
    }

    std::string result;
    result.reserve(pre.size());  // Preallocate for efficiency.
    build(pre, in, 0, 0, static_cast<int>(pre.size()), result);
    return result;
}

// The key observation is that in a preorder traversal, the first element is the root of the (sub)tree. In the inorder traversal, elements to the left of that root form the left subtree, and elements to the right form the right subtree. The algorithm recursively processes the preorder string as a sequence of roots. Starting with the first character of preorder as the root of the whole tree, locate that character in the current inorder segment (bounded by indices `low` and `high`). The left subtree corresponds to the inorder segment `[low, i)`, and the right subtree to `[i+1, high)`. Recursively process the left subtree first (consuming the next preorder index), then the right subtree, and finally append the root to the output postorder string. Because node labels are unique, we can find the root index in inorder by scanning the segment linearly. The recursion consumes each preorder character exactly once, so the total time is O(n²) in the worst case due to linear scans at each recursion level, and O(n) auxiliary space for the recursion stack and output string. Edge cases include a single-node tree (postorder equals that node), a skewed tree (one subtree empty), and any valid pair of traversals with distinct labels; the function must handle empty segments gracefully by returning immediately.
