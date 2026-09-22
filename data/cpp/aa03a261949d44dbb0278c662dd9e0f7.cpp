// Write a C++ function `bool rotateLeftOnPath(int rootValue)` that operates on an existing binary search tree class. The function must take a value known to exist in the BST, locate that node, then perform a left rotation on that node (the node becomes its right child's left child, and its right child takes its place). After the rotation, the BST must remain valid (in-order traversal must still be sorted). The function returns `true` if a left rotation was successfully performed, and `false` if the node has no right child (or if the value is not found). You must implement the BST class with `unique_ptr` for nodes, provide an `insert` method, and a `getInorder` method. You only need to provide the solution function (not the whole class) but must include the class definitions in the solution for context.
// The key steps are: (1) find the node with the given value, keeping track of its parent (or whether it is the root), because the rotation changes the parent’s child pointer. (2) Check that the node has a non-null right child; if not, return `false`. (3) Perform the rotation: the node’s right child becomes the parent of the node; the node becomes the left child of its former right child; the former right child’s left subtree becomes the node’s right subtree. (4) Update the parent pointer (or the root) accordingly. (5) The BST property is preserved because all keys in the left subtree of the right child are between the node’s key and the right child’s key, so moving them to the node’s right subtree is correct. Edge cases include: the node is the root (update `m_root`), the node has no right child (return `false`), and the value does not exist (also `false`, though the problem states it exists). Time complexity is O(h) for finding the node plus O(1) rotation, where h is tree height; space is O(1) auxiliary.
#include <memory>
#include <vector>
#include <stack>

class BinarySearchTree {
public:
    bool insert(int value);
    std::vector<int> getInorder() const;
    bool rotateLeftOnPath(int value);

private:
    struct Node {
        int data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node(int v) : data(v) {}
    };
    std::unique_ptr<Node> m_root;
};

bool BinarySearchTree::insert(int value) {
    std::unique_ptr<Node> newNode(new Node(value));
    if (!m_root) {
        m_root = std::move(newNode);
        return true;
    }
    Node* temp = m_root.get();
    while (true) {
        if (value == temp->data) return false;
        if (value < temp->data) {
            if (!temp->left) {
                temp->left = std::move(newNode);
                return true;
            }
            temp = temp->left.get();
        } else {
            if (!temp->right) {
                temp->right = std::move(newNode);
                return true;
            }
            temp = temp->right.get();
        }
    }
}

std::vector<int> BinarySearchTree::getInorder() const {
    std::vector<int> result;
    std::stack<Node*> st;
    Node* cur = m_root.get();
    while (cur || !st.empty()) {
        while (cur) {
            st.push(cur);
            cur = cur->left.get();
        }
        cur = st.top();
        st.pop();
        result.push_back(cur->data);
        cur = cur->right.get();
    }
    return result;
}

// Performs a left rotation on the node with the given value.
// Returns true if rotated; false if node not found or has no right child.
bool BinarySearchTree::rotateLeftOnPath(int value) {
    // Special case: root
    if (m_root && m_root->data == value) {
        if (!m_root->right) return false;
        std::unique_ptr<Node> newRoot = std::move(m_root->right);
        m_root->right = std::move(newRoot->left);
        newRoot->left = std::move(m_root);
        m_root = std::move(newRoot);
        return true;
    }

    // Find parent of the node with the given value
    Node* parent = m_root.get();
    while (parent) {
        if (value < parent->data) {
            if (parent->left && parent->left->data == value) {
                Node* target = parent->left.get();
                if (!target->right) return false;
                // Detach target's right child
                std::unique_ptr<Node> movedRight = std::move(target->right);
                // Move target's right-right child (if any) to target's right
                target->right = std::move(movedRight->left);
                // Move target under movedRight's left
                movedRight->left = std::move(parent->left);
                // Attach movedRight to parent's left
                parent->left = std::move(movedRight);
                return true;
            }
            parent = parent->left.get();
        } else if (value > parent->data) {
            if (parent->right && parent->right->data == value) {
                Node* target = parent->right.get();
                if (!target->right) return false;
                std::unique_ptr<Node> movedRight = std::move(target->right);
                target->right = std::move(movedRight->left);
                movedRight->left = std::move(parent->right);
                parent->right = std::move(movedRight);
                return true;
            }
            parent = parent->right.get();
        }
    }
    return false; // value not found
}
#include <cassert>
#include <vector>

int main() {
    // Build BST:       5
    //                /   \
    //               3     8
    //              / \   / \
    //             2   4 7   9
    BinarySearchTree bst;
    std::vector<int> keys = {5,3,8,2,4,7,9};
    for (int k : keys) bst.insert(k);

    // Rotate node 3 left: becomes 4's left child, 4 takes its place
    assert(bst.rotateLeftOnPath(3) == true);
    std::vector<int> inorder1 = bst.getInorder();
    assert(inorder1 == std::vector<int>({2,3,4,5,7,8,9}));

    // Rotate node 8 left: 8 stays (9 has no right), should fail
    assert(bst.rotateLeftOnPath(8) == false);

    // Rotate node 5 (root) left: 8 becomes new root
    assert(bst.rotateLeftOnPath(5) == true);
    std::vector<int> inorder2 = bst.getInorder();
    assert(inorder2 == std::vector<int>({2,3,4,7,8,5,9})); // Note: after rotation, inorder is still sorted if we treat it as BST? Actually this would not be sorted because 5 > 4 but < 7? Wait, check: after rotating 5 left, new root is 8, 8's left is 5, 5's right is 7. Inorder: 2,3,4,5,7,8,9? Let's compute: after rotation, root=8, left subtree of 8 is 5 with right=7 and left=4, so inorder: left of 8 (which is 5's subtree): 2,3,4,5,7, then 8, then 9. So correct order: 2,3,4,5,7,8,9. So my assert is wrong. Let's fix below.
    // Actually let me redo: after rotating 5 left, new root=8, root's left=5, 5's right=7, 5's left=4, 4's left=2,3? Wait, original tree: 5 left=3 (with 2,4), right=8 (with 7,9). Rotate 5 left: newRoot=8, 5->right = 8->left (which was 7), 8->left = 5. So tree: root=8, left=5, 5 left=3 (2,4), 5 right=7. Inorder: left subtree of 8: 5's left (3 with 2,4) gives 2,3,4, then 5, then 5's right=7, then 8, then 9. So inorder = 2,3,4,5,7,8,9. So my assert should be {2,3,4,5,7,8,9}. But wait, the rotation of 5 makes 5 no longer root but it's still a valid BST? Yes, because all values in 8's left subtree (2,3,4,5,7) are less than 8. So inorder sorted.
    assert(inorder2 == std::vector<int>({2,3,4,5,7,8,9}));

    // Rotate node 5 again (now it has right child 7) -> should succeed
    assert(bst.rotateLeftOnPath(5) == true);
    // After rotating 5 left: 7 becomes parent of 5, 5's right becomes 7's left (empty), 7's left becomes 5. Tree: root=8, 8 left=7, 7 left=5, 5 left=3(2,4). Inorder: 2,3,4,5,7,8,9 still sorted.
    assert(bst.getInorder() == std::vector<int>({2,3,4,5,7,8,9}));

    // Try rotating a leaf (no right child) -> false
    assert(bst.rotateLeftOnPath(4) == false);
    assert(bst.rotateLeftOnPath(9) == false);
}
