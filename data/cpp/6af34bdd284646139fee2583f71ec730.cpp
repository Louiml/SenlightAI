// You are asked to implement a **hash set using separate chaining with Binary Search Trees (BSTs)** as the underlying collision resolution mechanism, but without relying on any pre-existing container classes such as `std::set`, `std::map`, or `std::list`. Write a C++ function `bool hashSetContains(const std::vector<int>& keys, const std::vector<std::string>& operations, int target)` that processes a sequence of `add` and `remove` operations on a hash set (with keys being integers), and then returns `true` if the hash set contains the `target` integer after all operations are applied. The hash set must use a fixed number of buckets (e.g., 769), with each bucket being a BST. For simplicity, the function will internally define the BST node structure and BST operations (insert, delete, search) as private helpers, and will implement the hash function as `key % bucketCount`. The operations vector contains strings, where `"add:k"` means insert key `k` (if not already present) and `"remove:k"` means delete key `k` (if present, do nothing otherwise). Duplicate additions are ignored. The function should handle negative keys correctly (use `std::abs` or handle modulo manually to ensure non-negative index). The final `contains` check should be done after all operations are applied.

// **Approach:**  
// We implement a self-contained hash set that uses an array of `bucketCount` buckets, each bucket being a BST. For each operation, compute the bucket index via a deterministic hash function (e.g., `key % bucketCount`), but ensure the index is non-negative (e.g., `(key % bucketCount + bucketCount) % bucketCount`). Each BST supports insert (no duplicates), delete (handle leaf, one child, two children cases), and search.  
// - **Insert:** Traverse BST; if key exists, do nothing; otherwise, add a new node.  
// - **Delete:** Find node, then handle three cases: leaf (delete and return nullptr), one child (replace with child), two children (replace with inorder predecessor, then recursively delete predecessor).  
// - **Search:** Traverse until found or leaf reached.  
//
// After processing all operations, call search on the appropriate bucket for `target`.  
// **Edge cases:**  
// - Empty input operations: return whether target is in empty set (false if never added).  
// - Removing a key that doesn't exist: no effect.  
// - Negative keys: use a hash function that yields non-negative indices.  
// - Duplicate adds: ignore.  
// - Keys may be any integer (including 0).  
//
// **Complexity:**  
// Let `N` be the number of distinct inserted keys, `K` be bucket count (769). Time per operation is `O(log(N/K))` on average, worst-case `O(N/K)` if a bucket becomes deep. Space is `O(K + N)`.  
// **Why BST?** Unlike a linked list, this gives `O(log m)` per bucket for `m` elements in that bucket, improving worst-case.

#include <vector>
#include <string>
#include <cstdlib>

bool hashSetContains(const std::vector<int>& keys, const std::vector<std::string>& operations, int target) {
    // BST node structure for each bucket
    struct TreeNode {
        int val;
        TreeNode* left;
        TreeNode* right;
        TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
    };

    // Helper: insert into BST, returns new root
    auto insertBST = [](TreeNode* root, int val, auto& insertRef) -> TreeNode* {
        if (!root) return new TreeNode(val);
        if (val < root->val) root->left = insertRef(root->left, val, insertRef);
        else if (val > root->val) root->right = insertRef(root->right, val, insertRef);
        // val equal -> do nothing
        return root;
    };

    // Helper: find predecessor (max in left subtree)
    auto predecessor = [](TreeNode* root) -> TreeNode* {
        while (root->right) root = root->right;
        return root;
    };

    // Helper: delete a key from BST, returns new root
    auto deleteBST = [&](TreeNode* root, int val, auto& deleteRef) -> TreeNode* {
        if (!root) return nullptr;
        if (val < root->val) {
            root->left = deleteRef(root->left, val, deleteRef);
        } else if (val > root->val) {
            root->right = deleteRef(root->right, val, deleteRef);
        } else {
            // Node found
            if (!root->left && !root->right) {
                delete root;
                return nullptr;
            }
            if (!root->left) {
                TreeNode* rightChild = root->right;
                delete root;
                return rightChild;
            }
            if (!root->right) {
                TreeNode* leftChild = root->left;
                delete root;
                return leftChild;
            }
            // Two children: use predecessor
            TreeNode* pred = predecessor(root->left);
            root->val = pred->val;
            root->left = deleteRef(root->left, pred->val, deleteRef);
        }
        return root;
    };

    // Helper: search in BST
    auto searchBST = [](TreeNode* root, int val) -> bool {
        while (root) {
            if (root->val == val) return true;
            else if (val < root->val) root = root->left;
            else root = root->right;
        }
        return false;
    };

    const int bucketCount = 769;
    std::vector<TreeNode*> buckets(bucketCount, nullptr);

    // Hash function ensuring non-negative index
    auto hash = [&](int key) -> int {
        int h = key % bucketCount;
        if (h < 0) h += bucketCount;
        return h;
    };

    // Process operations
    for (const std::string& op : operations) {
        // Parse operation type and key
        size_t colonPos = op.find(':');
        std::string type = op.substr(0, colonPos);
        int key = std::atoi(op.substr(colonPos + 1).c_str());
        int idx = hash(key);

        if (type == "add") {
            buckets[idx] = insertBST(buckets[idx], key, insertBST);
        } else if (type == "remove") {
            buckets[idx] = deleteBST(buckets[idx], key, deleteBST);
        }
        // Unknown operations ignored
    }

    // Final contains check
    int targetIdx = hash(target);
    return searchBST(buckets[targetIdx], target);
}

#include <cassert>
#include <vector>
#include <string>

// Assume hashSetContains defined as above

int main() {
    // Test 1: Basic add and contains
    std::vector<int> keys1 = {1, 2, 3};
    std::vector<std::string> ops1 = {"add:1", "add:2", "add:3"};
    assert(hashSetContains(keys1, ops1, 2) == true);
    assert(hashSetContains(keys1, ops1, 4) == false);

    // Test 2: Duplicate adds ignored
    std::vector<int> keys2 = {1};
    std::vector<std::string> ops2 = {"add:5", "add:5", "add:5"};
    assert(hashSetContains(keys2, ops2, 5) == true);

    // Test 3: Remove existing and non-existing
    std::vector<int> keys3 = {1, 2, 3};
    std::vector<std::string> ops3 = {"add:10", "add:20", "remove:10", "remove:999"};
    assert(hashSetContains(keys3, ops3, 10) == false);
    assert(hashSetContains(keys3, ops3, 20) == true);

    // Test 4: Negative keys
    std::vector<int> keys4 = {-1, -2};
    std::vector<std::string> ops4 = {"add:-1", "add:-2"};
    assert(hashSetContains(keys4, ops4, -1) == true);
    assert(hashSetContains(keys4, ops4, -3) == false);

    // Test 5: Remove all, then target absent
    std::vector<int> keys5 = {7};
    std::vector<std::string> ops5 = {"add:7", "remove:7"};
    assert(hashSetContains(keys5, ops5, 7) == false);

    // Test 6: Empty operations
    std::vector<int> keys6 = {};
    std::vector<std::string> ops6 = {};
    assert(hashSetContains(keys6, ops6, 0) == false);

    // Test 7: Mixed operations with large keys
    std::vector<int> keys7 = {1000000, -500000};
    std::vector<std::string> ops7 = {"add:1000000", "add:-500000", "remove:1000000"};
    assert(hashSetContains(keys7, ops7, -500000) == true);

    // Test 8: Zero and duplicate
    std::vector<int> keys8 = {0, 0, 1};
    std::vector<std::string> ops8 = {"add:0", "add:1", "add:0"};
    assert(hashSetContains(keys8, ops8, 0) == true);
    assert(hashSetContains(keys8, ops8, 1) == true);

    return 0;
}
