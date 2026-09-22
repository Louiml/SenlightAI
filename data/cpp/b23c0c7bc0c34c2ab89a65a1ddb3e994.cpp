Write a C++ function that takes a pointer to the root of a binary tree (where each node stores an integer value and has pointers `hijo_izq` and `hijo_der` for left and right children) and returns the sum of all integer values stored in the tree. The function should handle empty trees (null pointer) by returning 0, and should work for any binary tree shape, including skewed trees and trees with only one node. The function must be recursive and must not modify the tree. The node structure is defined as a class `NodoArbolBinario` with public integer member `valor` and public pointers `hijo_izq` and `hijo_der`.
// The solution uses recursion to traverse the tree in a depth-first manner. For each node, we sum its own value, the sum of its left subtree, and the sum of its right subtree. The base case is when the node pointer is `nullptr`, in which case we return 0. This naturally handles empty trees (root is null) and leaf nodes (both children are null). The algorithm visits each node exactly once, so the time complexity is O(n), where n is the number of nodes. The space complexity is O(h), where h is the height of the tree, due to the recursive call stack; in the worst case (a skewed tree), h = n, so O(n) space, but for balanced trees it is O(log n). Edge cases include a null root (empty tree) returning 0, a single node, and any arbitrary tree structure. The tree is not modified since we only read node values and move through pointers.
#include <cstddef>

// Definition of the binary tree node
struct NodoArbolBinario {
    int valor;
    NodoArbolBinario* hijo_izq;
    NodoArbolBinario* hijo_der;
    
    explicit NodoArbolBinario(int v) : valor(v), hijo_izq(nullptr), hijo_der(nullptr) {}
};

// Sum all integer values in the binary tree rooted at 'raiz'
// Returns 0 if the tree is empty
int suma(NodoArbolBinario* raiz) {
    if (raiz == nullptr) {
        return 0;
    }
    return raiz->valor + suma(raiz->hijo_izq) + suma(raiz->hijo_der);
}
#include <cassert>

int main() {
    // Empty tree
    NodoArbolBinario* vacio = nullptr;
    assert(suma(vacio) == 0);
    
    // Single node tree
    NodoArbolBinario* unico = new NodoArbolBinario(42);
    assert(suma(unico) == 42);
    delete unico;
    
    // Tree: root 1, left child 2, right child 3
    NodoArbolBinario* root = new NodoArbolBinario(1);
    root->hijo_izq = new NodoArbolBinario(2);
    root->hijo_der = new NodoArbolBinario(3);
    assert(suma(root) == 6);
    
    // Tree: root 10, left child 20 (which has left 30 and right 40), right child 5
    NodoArbolBinario* root2 = new NodoArbolBinario(10);
    root2->hijo_izq = new NodoArbolBinario(20);
    root2->hijo_izq->hijo_izq = new NodoArbolBinario(30);
    root2->hijo_izq->hijo_der = new NodoArbolBinario(40);
    root2->hijo_der = new NodoArbolBinario(5);
    assert(suma(root2) == 105);
    
    // Skewed left tree: 5 -> 4 -> 3 -> 2 -> 1 (total 15)
    NodoArbolBinario* skewed = new NodoArbolBinario(5);
    skewed->hijo_izq = new NodoArbolBinario(4);
    skewed->hijo_izq->hijo_izq = new NodoArbolBinario(3);
    skewed->hijo_izq->hijo_izq->hijo_izq = new NodoArbolBinario(2);
    skewed->hijo_izq->hijo_izq->hijo_izq->hijo_izq = new NodoArbolBinario(1);
    assert(suma(skewed) == 15);
    
    // Deep tree with negatives: root -5, left 10, right -2, left-left -1, left-right 7
    NodoArbolBinario* deep = new NodoArbolBinario(-5);
    deep->hijo_izq = new NodoArbolBinario(10);
    deep->hijo_der = new NodoArbolBinario(-2);
    deep->hijo_izq->hijo_izq = new NodoArbolBinario(-1);
    deep->hijo_izq->hijo_der = new NodoArbolBinario(7);
    assert(suma(deep) == 9);
    
    // Clean up all allocated nodes (simplified: traverse and delete manually for this test)
    // Note: Production code should use smart pointers or proper deletion.
    
    return 0;
}
