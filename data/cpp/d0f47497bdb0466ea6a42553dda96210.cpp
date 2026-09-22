/*
Given a level-order representation of a binary tree stored in an array (where the root is at index 0, and for any node at index i its left child is at 2*i+1 and its right child is at 2*i+2), implement a C++ function that returns the sum of all leaf node values. A leaf node is defined as a node that has no children (both children indices are out of the array bounds or the nodes at those indices are not present because the array may contain some "missing" nodes represented by a special sentinel value like -1). You may assume the array is non-empty and contains only positive integers, with -1 used to indicate that a child does not exist. The function should take the array as a `std::vector<int>` and return an `int`.
*/

#include <vector>
#include <functional>

// Computes the sum of all leaf node values in a binary tree stored as a level-order array.
// -1 in the array represents a missing node. Indices follow: root at 0, children at 2*i+1 and 2*i+2.
int sumOfLeafNodes(const std::vector<int>& arr) {
    // Sentinel value indicating absence of a node.
    const int MISSING = -1;
    
    // Recursive helper to traverse the tree and accumulate leaf values.
    std::function<int(int)> traverse = [&](int index) -> int {
        // Out-of-bounds or missing node: contributes nothing.
        if (index >= static_cast<int>(arr.size()) || arr[index] == MISSING) {
            return 0;
        }
        
        int leftIdx = 2 * index + 1;
        int rightIdx = 2 * index + 2;
        
        // Determine if this node is a leaf: both children are absent
        // (either out of bounds or marked as MISSING).
        bool leftExists = leftIdx < static_cast<int>(arr.size()) && arr[leftIdx] != MISSING;
        bool rightExists = rightIdx < static_cast<int>(arr.size()) && arr[rightIdx] != MISSING;
        
        if (!leftExists && !rightExists) {
            return arr[index];
        }
        
        // Not a leaf: sum contributions from existing children.
        int sum = 0;
        if (leftExists) {
            sum += traverse(leftIdx);
        }
        if (rightExists) {
            sum += traverse(rightIdx);
        }
        return sum;
    };
    
    // Handle empty input gracefully (though the problem guarantees non-empty).
    if (arr.empty()) {
        return 0;
    }
    return traverse(0);
}

#include <cassert>
#include <vector>

int sumOfLeafNodes(const std::vector<int>& arr); // declaration from solution

int main() {
    // Example from the snippet: arr = {1,2,3,4,5,6,7,7,8,8}
    // Tree: 1 with children 2,3; 2 has 4,5; 3 has 6,7; 4 has 7,8; 5 has 8
    // Leaves: 7 (from 4's left), 8 (from 4's right), 8 (from 5's left), 6? 
    // Let's check: node 3 has children 6 and 7. 6 and 7 are leaves. 
    // Node 5 has left child 8 (leaf) and no right child (index 11 out of bounds) → 8 leaf.
    // Node 4 has left=7 leaf, right=8 leaf. Node 7 (index 6) has children at 13,14 out of bounds → leaf.
    // Sum: 7+8+8+6+7 = 36? Let's verify manually. 
    // Actually indexes: arr[0]=1, [1]=2 [2]=3 [3]=4 [4]=5 [5]=6 [6]=7 [7]=7 [8]=8 [9]=8
    // Leaves: arr[7]=7 (node 4's left) leaf, arr[8]=8 (node 4's right) leaf, arr[9]=8 (node 5's left) leaf, arr[5]=6 (node 3's left) leaf, arr[6]=7 (node 3's right) leaf. Sum = 7+8+8+6+7 = 36.
    std::vector<int> arr1 = {1,2,3,4,5,6,7,7,8,8};
    assert(sumOfLeafNodes(arr1) == 36);

    // Simple tree: root only.
    std::vector<int> arr2 = {42};
    assert(sumOfLeafNodes(arr2) == 42);

    // Tree with missing right child: {1,2,-1}
    // Leaves: node 2 (index 1) has no children (indices 3,4 out) → leaf, value 2.
    // Root has left child but no right, so root is not leaf.
    std::vector<int> arr3 = {1,2,-1};
    assert(sumOfLeafNodes(arr3) == 2);

    // Tree: root and left child only, -1 for right of left.
    // {10, 20, -1, 30, -1} → indices: 0:10, 1:20, 2:-1, 3:30, 4:-1
    // Node 20 (idx1) has left=30 (idx3), right=-1 → not leaf. Node 30 (idx3) has children idx7,8 out → leaf, sum=30.
    std::vector<int> arr4 = {10, 20, -1, 30, -1};
    assert(sumOfLeafNodes(arr4) == 30);

    // All nodes are leaves? Example: {1, -1, -1} → root has no children logic: left idx1 = -1, right idx2=-1 → leaf, sum=1.
    std::vector<int> arr5 = {1, -1, -1};
    assert(sumOfLeafNodes(arr5) == 1);

    // Larger tree with missing middle nodes.
    // {1, 2, -1, 4, 5, -1, -1} 
    // Root(0)=1; left(1)=2; right(2)=missing. Node2(left=3=4, right=4=5). Node4(3) has children idx7,8 out → leaf 4. Node5(4) has children idx9,10 out → leaf 5. Node2 is not leaf. Node1 has no right child, but has left child → not leaf. Sum = 4+5=9.
    std::vector<int> arr6 = {1, 2, -1, 4, 5, -1, -1};
    assert(sumOfLeafNodes(arr6) == 9);

    // Balanced full tree: {1,2,3,4,5,6,7} → leaves: 4,5,6,7 sum=22
    std::vector<int> arr7 = {1,2,3,4,5,6,7};
    assert(sumOfLeafNodes(arr7) == 22);

    // Tree with root -1 (empty tree representation) → sum should be 0.
    std::vector<int> arr8 = {-1};
    assert(sumOfLeafNodes(arr8) == 0);
    return 0;
}

// The approach is to simulate a recursive or iterative traversal of the binary tree from the array, but since the array is a complete representation (with possible -1 markers), we can use a recursive function that checks the bounds and skip nodes with value -1. At each valid node, we check if it is a leaf: a node is a leaf if both its children indices are out of bounds OR the child values are -1 (meaning they don't exist). If it is a leaf, add its value to the sum; otherwise, recursively process its left and right children. Important edge cases: (1) an empty array is not allowed but a root that is -1 should be handled gracefully (return 0), (2) the tree may be unbalanced so some left children exist while right children are -1, (3) sentinel -1 nodes should not be counted as leaves. Time complexity is O(n) where n is the array size, as each index is visited at most once. Space complexity is O(h) where h is the height of the tree due to recursion stack, worst-case O(n) for a skewed tree.
