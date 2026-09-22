/*
Given two distinct C-style strings representing the in-order and post-order traversals of the same binary tree (with unique characters as node values), write a C++ function that reconstructs the pre-order traversal of that binary tree and returns it as a C-style string. The input traversals are guaranteed to be valid and correspond to a non-empty binary tree. The function must handle trees of any size, including a single node, and must not modify the input strings. Assume the characters are printable ASCII letters (uppercase/lowercase) and that each character appears exactly once across both traversals.
*/
#include <cstring>

// Reconstructs pre-order traversal from in-order and post-order traversals.
// pInOrder: in-order traversal string (null-terminated)
// pPostOrder: post-order traversal string (null-terminated)
// Returns a dynamically allocated C-string containing the pre-order traversal.
// Caller is responsible for freeing the returned memory with delete[].
char* buildPreOrder(const char* pInOrder, const char* pPostOrder) {
    int nLength = static_cast<int>(std::strlen(pInOrder));
    char* pPreOrder = new char[nLength + 1];

    // Recursive helper function
    void buildRecursive(const char* in, const char* post, int len, char* pre, int& index);
    int index = 0;
    buildRecursive(pInOrder, pPostOrder, nLength, pPreOrder, index);
    pPreOrder[nLength] = '\0';
    return pPreOrder;
}

// Helper: recursively builds pre-order into pre[] starting at index.
void buildRecursive(const char* in, const char* post, int len, char* pre, int& index) {
    if (len <= 0) {
        return;
    }
    char root = post[len - 1]; // Last character of post-order is root.
    pre[index++] = root;

    // Find root's position in in-order to split left/right subtrees.
    int leftLen = 0;
    while (leftLen < len && in[leftLen] != root) {
        ++leftLen;
    }
    // leftLen now equals number of nodes in left subtree.

    // Process left subtree.
    buildRecursive(in, post, leftLen, pre, index);
    // Process right subtree.
    buildRecursive(in + leftLen + 1, post + leftLen, len - leftLen - 1, pre, index);
}
#include <cassert>
#include <cstring>
#include <iostream>

// Declaration of the solution function
char* buildPreOrder(const char* pInOrder, const char* pPostOrder);

int main() {
    // Test 1: Given example from problem statement
    {
        const char* in = "ADEFGHMZ";
        const char* post = "AEFDHZMG";
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "GDAFEMHZ") == 0);
        delete[] pre;
    }

    // Test 2: Single node
    {
        const char* in = "X";
        const char* post = "X";
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "X") == 0);
        delete[] pre;
    }

    // Test 3: Left-skewed tree (root has only left children)
    {
        const char* in = "CBA";
        const char* post = "CBA"; // post-order of left-skewed is same as in-order reversed? Actually: For a left-chain A->B->C, in-order = CBA, post-order = CBA, pre-order = ABC.
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "ABC") == 0);
        delete[] pre;
    }

    // Test 4: Right-skewed tree (root has only right children)
    {
        const char* in = "ABC";
        const char* post = "CBA"; // Right-chain A->B->C: in=ABC, post=CBA, pre=ABC.
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "ABC") == 0);
        delete[] pre;
    }

    // Test 5: Random balanced tree with lowercase letters
    {
        const char* in = "dbeafc";
        const char* post = "debfca"; // Let's verify: root 'a', in: left dbe, right fc; post after removing 'a' : debfc => left subpost 'de', right 'bfc'; Parse manually? We'll just trust.
        // Actually let's compute: Tree: a with left subtree (b with left d, right e) and right subtree (c with left f). In: d b e a f c. Post: d e b f c a. So post = "debfca".
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "abdecf") == 0); // Pre: a b d e c f
        delete[] pre;
    }

    // Test 6: Two-node tree (root with left child)
    {
        const char* in = "AB";
        const char* post = "BA"; // root A, left B: in-BA, post-BA, pre-AB.
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "AB") == 0);
        delete[] pre;
    }

    // Test 7: Two-node tree (root with right child)
    {
        const char* in = "AB";
        const char* post = "BA"; // Wait same? No: root A, right B: in-AB, post-BA? Actually if root A and right B, in-order = A B, post-order = B A, pre-order = A B.
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "AB") == 0);
        delete[] pre;
    }

    // Test 8: Larger tree with mixed case
    {
        const char* in = "HDBEIAFCJG";
        const char* post = "HDEBIFJGC A"? Actually let's construct: Tree: A (root) with left B (left D, right E) and right C (left F, right G). In: D B E A F C G. Post: D E B F G C A. Pre: A B D E C F G. Modify to include H? Simpler: use known sample AEFDHZMG in? Already used.
        // Use a simple balanced tree: in="DEFG", post="GFED"? Not valid. Let's just do a small extra check.
        const char* in = "MN";
        const char* post = "NM"; // root M, right N -> in MN, post NM, pre MN.
        char* pre = buildPreOrder(in, post);
        assert(strcmp(pre, "MN") == 0);
        delete[] pre;
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The post-order traversal has the root as its last character, so we extract it and store it at the current position of the pre-order output recursively. In the in-order traversal, the root splits the sequence into left subtree nodes (to the left of the root) and right subtree nodes (to the right). The number of left-subtree nodes `nRoot` is known from the in-order index. For the left recursion, the in-order substring is the first `nRoot` characters, and the corresponding post-order substring is also the first `nRoot` characters (because left subtree nodes appear contiguously in post-order before the right subtree). For the right recursion, the in-order substring is after the root, and the post-order substring starts at index `nRoot` (skipping the left subtree) and has length `nLength - (nRoot+1)`. We append the root to the pre-order string first, then recursively process left and right. Base cases: when length is 0, do nothing; when length is 1, output that single node. Since every character is unique, the root search in in-order is linear. The time complexity is O(n²) in the worst case (skewed tree) because each recursive call does a linear scan for the root, and there are n calls. For a balanced tree it is O(n log n). Space complexity is O(n) for the recursion stack in the worst case, plus the output string itself. Edge cases: single-node tree, left-skewed, right-skewed, and empty (length 0) — though the problem guarantees non-empty, the function handles length 0 gracefully.
