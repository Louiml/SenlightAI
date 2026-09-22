// Write a standalone C++ function `std::string levelOrderTreeString(BinSST* root)` that takes a pointer to a Binary Search Tree (BST) node (where each node has `GetValue()`, `GetLeft()`, `GetRight()`) and returns a single string representing the tree in level-order (breadth-first), with each level on a new line, nodes separated by a single space, and missing children represented as `"x"`. The function must handle a `nullptr` root by returning an empty string. Duplicate values are allowed and inserted to the left (as per the original `Insert` logic). The output should not have leading/trailing spaces, and each newline should separate levels. You may assume the tree has at least one node when `root` is non-null. The provided `BinSST` class is fully defined (including `Find`, but you may ignore `Delete`), and you should not modify it.

#include <cassert>
#include <string>

// Assume BinSST is defined as in the snippet (constructor, Insert, GetLeft, GetRight, GetValue).
// Minimal header for testing:
class BinSST {
public:
    BinSST(int value) : _value(value), _left(nullptr), _right(nullptr) {}
    void Insert(int value) {
        if (value <= _value) {
            if (_left) _left->Insert(value);
            else _left = new BinSST(value);
        } else {
            if (_right) _right->Insert(value);
            else _right = new BinSST(value);
        }
    }
    int GetValue() const { return _value; }
    BinSST* GetLeft() const { return _left; }
    BinSST* GetRight() const { return _right; }
private:
    int _value;
    BinSST* _left;
    BinSST* _right;
};

int main() {
    // Empty tree
    assert(levelOrderTreeString(nullptr) == "");
    
    // Single node
    BinSST* root1 = new BinSST(5);
    assert(levelOrderTreeString(root1) == "5");
    
    // Simple balanced tree: 5, left 3, right 7
    BinSST* root2 = new BinSST(5);
    root2->Insert(3);
    root2->Insert(7);
    assert(levelOrderTreeString(root2) == "5\n3 7");
    
    // Tree with missing child: 5, left 3 (no right), right 7 (no left)
    BinSST* root3 = new BinSST(5);
    root3->Insert(3);
    root3->Insert(7);
    root3->Insert(2); // left-left of 3
    // Expected: level 0: 5; level 1: 3 7; level 2: 2 x x x (since 3 has left=2, right=x; 7 has left=x, right=x)
    assert(levelOrderTreeString(root3) == "5\n3 7\n2 x x x");
    
    // Duplicate values inserted to the left
    BinSST* root4 = new BinSST(4);
    root4->Insert(2);
    root4->Insert(2);
    root4->Insert(6);
    // Expected: level 0: 4; level 1: 2 6; level 2: x 2 x x (2 has left=x, right=2; 6 has left=x, right=x)
    assert(levelOrderTreeString(root4) == "4\n2 6\nx 2 x x");
    
    // Unbalanced right-only tree: 1,2,3
    BinSST* root5 = new BinSST(1);
    root5->Insert(2);
    root5->Insert(3);
    // Expected: level 0: 1; level 1: x 2; level 2: x x x 3; level 3: x x x x x x x x (but last level all x, so omitted)
    assert(levelOrderTreeString(root5) == "1\nx 2\nx x x 3");
    
    // Clean up (not strictly needed for assert tests)
    // In a real test you'd delete nodes to avoid leaks.
}

#include <string>
#include <vector>
#include <queue>
#include <utility>

// Forward declarations for the BinSST class (provided externally).
// The class has: int GetValue(); BinSST* GetLeft(); BinSST* GetRight();

// Returns a level-order string representation of the BST.
// Each level is on a new line, nodes separated by spaces, missing children as "x".
std::string levelOrderTreeString(BinSST* root) {
    if (root == nullptr) {
        return "";
    }
    
    std::string result;
    std::queue<BinSST*> q;
    q.push(root);
    
    while (!q.empty()) {
        int levelSize = static_cast<int>(q.size());
        std::vector<std::string> levelStrings;
        
        for (int i = 0; i < levelSize; ++i) {
            BinSST* node = q.front();
            q.pop();
            
            if (node == nullptr) {
                levelStrings.push_back("x");
            } else {
                levelStrings.push_back(std::to_string(node->GetValue()));
                q.push(node->GetLeft());
                q.push(node->GetRight());
            }
        }
        
        // Check if this level is entirely null markers (no actual nodes).
        bool allNull = true;
        for (const std::string& s : levelStrings) {
            if (s != "x") {
                allNull = false;
                break;
            }
        }
        if (allNull) {
            break; // Reached past the last real level.
        }
        
        // Append this level to result.
        if (!result.empty()) {
            result += "\n";
        }
        for (size_t i = 0; i < levelStrings.size(); ++i) {
            if (i > 0) {
                result += " ";
            }
            result += levelStrings[i];
        }
    }
    
    return result;
}

// To produce a level-order representation, use a queue that stores pointers to nodes. Start by pushing the root. For each level, record the number of nodes currently in the queue (this gives the width of the level). Then process exactly that many nodes: pop each, append its value (or `"x"` if `nullptr`), and push its left and right children (including `nullptr` to preserve structure) so that missing children are explicitly represented in the next level. However, be careful: a level that consists entirely of `nullptr` nodes should not be emitted, because that would add an extra line of `x x x ...` beyond the deepest actual node. Therefore, after collecting a level's strings, check if all entries are `"x"`; if so, stop. Otherwise, append the level string to the result, adding a newline if this is not the first level. The time complexity is O(n) where n is the total number of nodes (including null markers), and space complexity is O(n) for the queue in the worst case (a completely unbalanced tree). Edge cases include an empty tree (return empty string), a single node (return its value with no newline), and trees where some nodes have one child (the missing child should appear as `"x"` in the next level but not cause an extra all-`"x"` level).
