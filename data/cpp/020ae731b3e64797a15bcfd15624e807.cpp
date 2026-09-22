// Implement a C++ function `build_splay_tree_from_initializer_list` that takes a `std::initializer_list<int>` of values (possibly unsorted, with duplicates) and returns a pointer to the root node of a splay tree built from scratch using only the provided node structure and the splay tree algorithms from the Boost.Intrusive library, as exemplified in the snippet. The function must use `boost::intrusive::splaytree_algorithms<my_splaytree_node_traits>` and the `header` node technique to insert each value using `insert_equal_lower_bound` (or `insert_equal_upper_bound`) with a comparison functor that orders by the `int_` member. The returned root must be the header node's left child (or `nullptr` if the tree is empty), and the tree must satisfy the splay tree invariants (i.e., binary search tree ordering, no parent/child inconsistencies). The function must allocate each node dynamically using `new my_node(value)`, and you must ensure no memory leaks by properly cleaning up all nodes at the end (the test will erase all nodes using `algo::erase`). The function signature should be `my_node* build_splay_tree_from_initializer_list(std::initializer_list<int> values)`. You may include the necessary Boost headers and the node/traits definitions inside your solution (since the task is standalone), but the solution should only output the function definition, not a main function.
#include <cassert>
#include <vector>

// Include the solution code above (for test purposes, but here we assume it's already defined)

// Helper to count nodes in the tree using splay algorithms (for verification)
int count_nodes(my_node* root) {
    if (!root) return 0;
    return 1 + count_nodes(root->left_) + count_nodes(root->right_);
}

int main() {
    // Test empty list
    my_node* empty = build_splay_tree_from_initializer_list({});
    assert(empty == nullptr);

    // Test single element
    my_node* single = build_splay_tree_from_initializer_list({42});
    assert(single != nullptr);
    assert(single->int_ == 42);
    assert(single->left_ == nullptr && single->right_ == nullptr);
    // Cleanup
    using algo = boost::intrusive::splaytree_algorithms<my_splaytree_node_traits>;
    my_node header;
    algo::init_header(&header);
    // Re-attach single to a header for erasure
    algo::set_parent(single, &header);
    algo::set_left(&header, single);
    algo::erase(&header, single);

    // Test multiple distinct values
    my_node* tree = build_splay_tree_from_initializer_list({3, 1, 2});
    assert(tree != nullptr);
    // Check in-order traversal yields 1,2,3
    std::vector<int> in_order;
    // Traverse manually (left-root-right)
    std::function<void(my_node*)> traverse = [&](my_node* n) {
        if (!n) return;
        traverse(n->left_);
        in_order.push_back(n->int_);
        traverse(n->right_);
    };
    traverse(tree);
    assert((in_order == std::vector<int>{1, 2, 3}));
    assert(count_nodes(tree) == 3);
    // Cleanup all nodes (erase from root)
    my_node* cur = tree;
    // Simpler: erase each node via algo::erase with header, but need a header
    algo::init_header(&header);
    // Rebuild header's left to point to tree root
    algo::set_parent(tree, &header);
    algo::set_left(&header, tree);
    // Erase all nodes in order (we can erase root repeatedly)
    while (header.left_ != &header) {
        algo::erase(&header, header.left_);
    }

    // Test duplicates
    my_node* dup = build_splay_tree_from_initializer_list({2, 2, 1});
    assert(dup != nullptr);
    std::vector<int> dup_order;
    std::function<void(my_node*)> trav_dup = [&](my_node* n) {
        if (!n) return;
        trav_dup(n->left_);
        dup_order.push_back(n->int_);
        trav_dup(n->right_);
    };
    trav_dup(dup);
    assert((dup_order == std::vector<int>{1, 2, 2}));
    // Cleanup
    algo::init_header(&header);
    algo::set_parent(dup, &header);
    algo::set_left(&header, dup);
    while (header.left_ != &header) {
        algo::erase(&header, header.left_);
    }

    // Test many elements (including negative)
    my_node* many = build_splay_tree_from_initializer_list({5, -1, 3, 5, 0, -2});
    assert(many != nullptr);
    std::vector<int> many_order;
    std::function<void(my_node*)> trav_many = [&](my_node* n) {
        if (!n) return;
        trav_many(n->left_);
        many_order.push_back(n->int_);
        trav_many(n->right_);
    };
    trav_many(many);
    assert((many_order == std::vector<int>{-2, -1, 0, 3, 5, 5}));
    // Cleanup
    algo::init_header(&header);
    algo::set_parent(many, &header);
    algo::set_left(&header, many);
    while (header.left_ != &header) {
        algo::erase(&header, header.left_);
    }

    return 0;
}
#include <boost/intrusive/splaytree_algorithms.hpp>
#include <initializer_list>

// Node structure as per the snippet
struct my_node {
    my_node(int i = 0) : parent_(nullptr), left_(nullptr), right_(nullptr), color_(0), int_(i) {}
    my_node *parent_, *left_, *right_;
    int color_;
    int int_;
};

// Node traits as per the snippet
struct my_splaytree_node_traits {
    typedef my_node node;
    typedef my_node *node_ptr;
    typedef const my_node *const_node_ptr;

    static node_ptr get_parent(const_node_ptr n)       { return n->parent_; }
    static void set_parent(node_ptr n, node_ptr parent){ n->parent_ = parent; }
    static node_ptr get_left(const_node_ptr n)         { return n->left_; }
    static void set_left(node_ptr n, node_ptr left)    { n->left_ = left; }
    static node_ptr get_right(const_node_ptr n)        { return n->right_; }
    static void set_right(node_ptr n, node_ptr right)  { n->right_ = right; }
};

// Comparator for node pointers by int_
struct node_ptr_compare {
    bool operator()(const my_node *a, const my_node *b) const {
        return a->int_ < b->int_;
    }
};

// Build a splay tree from an initializer list and return the root pointer (nullptr if empty)
my_node* build_splay_tree_from_initializer_list(std::initializer_list<int> values) {
    using algo = boost::intrusive::splaytree_algorithms<my_splaytree_node_traits>;

    my_node header;  // header node, not dynamically allocated
    algo::init_header(&header);

    for (int val : values) {
        my_node* new_node = new my_node(val);
        algo::insert_equal_lower_bound(&header, new_node, node_ptr_compare());
    }

    // If the tree is empty, header.left_ points to &header itself; we return nullptr
    if (header.left_ == &header) {
        return nullptr;
    }
    return header.left_;  // root of the tree
}
// The solution must replicate the pattern shown in the snippet: create a header node, initialize it with `algo::init_header`, then for each value in the initializer list, allocate a new `my_node` with that value and insert it using `algo::insert_equal_lower_bound(&header, new_node, comparator)`. The comparator is a functor that compares two `my_node*` by their `int_` member, as defined in the snippet. After all insertions, the root of the tree is stored in `header.left_` (since the header is the sentinel and its left child points to the root of the tree). The function returns that pointer. Important edge cases: an empty initializer list should return `nullptr` (after `init_header`, `header.left_` will be `&header` itself, so you must handle that: if the list is empty, the returned root should be `nullptr`). Duplicate values are allowed and will be inserted as equal elements; the splay tree algorithms handle duplicates correctly, placing them in the left or right subtree depending on the insertion function used. The time complexity is O(n log n) on average for n insertions (since each insertion into a splay tree has amortized O(log n) cost), and O(n) worst-case for a degenerate sequence, but that's acceptable. Space complexity is O(n) for the nodes. The function must not leak memory: it is the caller's responsibility to erase all nodes; however, to be safe, the test will erase all nodes reachable from the returned root using the provided `algo::erase` and `algo::unlink` pattern, so the function must ensure the tree is properly linked.
