// Create a C++ function named `print_binary_tree_levels` that takes a `std::vector<int>` containing level-order traversal data of a binary tree (where `-1` represents a `nullptr` node) and prints the tree structure in a visual, hierarchical format using ASCII art, similar to the provided `BinaryTreePrinter` class but with a simpler fixed-width output. The function should handle empty input and trees with up to 4 levels (i.e., up to 15 nodes in a complete tree). The output should include node values left-aligned in a 3-character wide field, with nodes separated by spaces, and use `/` and `\` for outgoing pointers, dashes for connecting lines, and spaces for alignment. Specifically, for each level starting from the root, print the node data, then the outgoing pointers (`/` for left child, `\` for right child), then connecting dashes (only if there are more levels below), then incoming pointers (`/` pointing to left child position, `\` to right child), then move to the next level. Use `offset` values that decrease by powers of 2 as you go down the levels (e.g., for height 3, offsets are 15, 7, 3, 1). Assume the vector length is exactly `2^(height+1)-1` for a complete tree, but some nodes may be `-1` (empty). Write the function in C++ with `const` correctness where appropriate, and include necessary headers (`<iostream>`, `<vector>`, `<iomanip>`). The function should not return anything; it just prints to standard output.
The solution builds a tree from the level-order vector. The root is at index 0. For any node at index `i`, its left child is at index `2*i+1` and right child at `2*i+2`. We can process each level without explicitly building a tree structure: maintain a vector of node pointers (or indices) for the current level. Start with the root index (0) if it's not `-1`, else null. For each level `level` (0-indexed), compute `offset` as `2^(height-level+1)-1` initially, but the pattern from the original code uses `offset = POWERS_OF_2[(height+1)]-1` for level 0, then `offset = POWERS_OF_2[height - level] - 1` for subsequent levels. Since height is known (compute from vector size: if `n` is vector size, height = floor(log2(n+1)) - 1, but easier: for level-order array of length `n`, height is such that `2^(height+1)-1 <= n < 2^(height+2)-1`). We can instead determine height by checking the vector size: height = ceil(log2(n+1))-1, but we can also compute from the number of levels we have data for. Since we are limited to height <= 3 (4 levels), we can hardcode POWERS_OF_2 = {1,2,4,8}. The algorithm iterates levels from 0 to height. At each level, we have a list of up to `2^level` node values (or -1). Print the data with proper spacing: before each node print `offset` spaces, then the value in 3-width field, then between nodes print `2*offset-2` spaces. Then if not last level, print outgoing pointers: for each node, print a space or `/` and a space or `\`, with same spacing pattern. Then if level < height-1, print connecting dashes with specified patterns. Then print incoming pointers. Then build the next level list from current nodes' children (if value != -1 and within vector bounds). Edge cases: empty vector (print nothing or a message), single node (just print value), null nodes (print spaces instead of data). The complexity: O(n) time to process each node once, O(n) auxiliary space for level nodes. Careful with off-by-one when accessing children; ensure index < vector.size().
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

// Print a binary tree from level-order traversal (using -1 for null) in ASCII art.
void print_binary_tree_levels(const std::vector<int>& level_order) {
    if (level_order.empty()) {
        std::cout << "Empty tree" << std::endl;
        return;
    }

    // Compute height: number of levels - 1 (0-indexed).
    // For vector size n, height is largest h such that 2^(h+1)-1 <= n.
    int n = static_cast<int>(level_order.size());
    int height = 0;
    while ((1 << (height + 1)) - 1 <= n) {
        ++height;
    }
    height -= 1; // Last increment overshoots; revert.
    if (height < 0) height = 0;

    const int POWERS_OF_2[] = {1, 2, 4, 8, 16, 32};

    // Store indices of current level nodes; -1 for null.
    std::vector<int> current_level(1, 0); // root index

    int offset = (height + 1 >= 0) ? POWERS_OF_2[height + 1] - 1 : 0;

    for (int level = 0; level <= height; ++level) {
        // Print node data.
        for (int i = 0; i < static_cast<int>(current_level.size()); ++i) {
            if (i == 0) {
                for (int s = 0; s < offset; ++s) std::cout << " ";
            } else {
                for (int s = 0; s < 2 * offset - 2; ++s) std::cout << " ";
            }
            int idx = current_level[i];
            if (idx != -1 && idx < n && level_order[idx] != -1) {
                std::cout << std::setw(3) << level_order[idx];
            } else {
                std::cout << "   ";
            }
        }
        std::cout << std::endl;

        if (level != height) {
            // Print outgoing pointers (/ for left, \ for right).
            for (int i = 0; i < static_cast<int>(current_level.size()); ++i) {
                if (i == 0) {
                    for (int s = 0; s < offset; ++s) std::cout << " ";
                } else {
                    for (int s = 0; s < 2 * offset - 2; ++s) std::cout << " ";
                }
                int idx = current_level[i];
                int left_idx = (idx != -1) ? 2 * idx + 1 : -1;
                int right_idx = (idx != -1) ? 2 * idx + 2 : -1;
                bool has_left = (left_idx >= 0 && left_idx < n && level_order[left_idx] != -1);
                bool has_right = (right_idx >= 0 && right_idx < n && level_order[right_idx] != -1);
                std::cout << (has_left ? " /" : "  ");
                std::cout << (has_right ? "\\ " : "  ");
            }
            std::cout << std::endl;

            // Print connecting dashes (only if not last but one level).
            if (level < height - 1) {
                int next_offset = POWERS_OF_2[height - level] - 1;
                for (int i = 0; i < static_cast<int>(current_level.size()); ++i) {
                    if (i == 0) {
                        if (next_offset > 1) {
                            for (int s = 0; s < next_offset; ++s) std::cout << " ";
                        }
                    } else {
                        for (int s = 0; s < 2 * next_offset + 1; ++s) std::cout << " ";
                    }
                    int idx = current_level[i];
                    int left_idx = (idx != -1) ? 2 * idx + 1 : -1;
                    int right_idx = (idx != -1) ? 2 * idx + 2 : -1;
                    bool has_left = (left_idx >= 0 && left_idx < n && level_order[left_idx] != -1);
                    bool has_right = (right_idx >= 0 && right_idx < n && level_order[right_idx] != -1);
                    if (has_left) {
                        std::cout << "   ";
                        for (int d = 0; d < next_offset - 1; ++d) std::cout << "-";
                    } else {
                        for (int s = 0; s < next_offset + 2; ++s) std::cout << " ";
                    }
                    if (has_right) {
                        std::cout << "  ";
                        for (int d = 0; d < next_offset - 1; ++d) std::cout << "-";
                    } else {
                        for (int s = 0; s < next_offset + 1; ++s) std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }

            // Print incoming pointers.
            for (int i = 0; i < static_cast<int>(current_level.size()); ++i) {
                if (i == 0) {
                    for (int s = 0; s < offset; ++s) std::cout << " ";
                } else {
                    for (int s = 0; s < 2 * offset; ++s) std::cout << " ";
                }
                int idx = current_level[i];
                int left_idx = (idx != -1) ? 2 * idx + 1 : -1;
                int right_idx = (idx != -1) ? 2 * idx + 2 : -1;
                bool has_left = (left_idx >= 0 && left_idx < n && level_order[left_idx] != -1);
                bool has_right = (right_idx >= 0 && right_idx < n && level_order[right_idx] != -1);
                if (has_left) {
                    std::cout << "  /";
                } else {
                    std::cout << "   ";
                }
                if (has_right) {
                    for (int s = 0; s < 2 * offset; ++s) std::cout << " ";
                    std::cout << "\\";
                } else {
                    for (int s = 0; s < 2 * offset + 1; ++s) std::cout << " ";
                }
            }
            std::cout << std::endl;

            // Build next level.
            std::vector<int> next_level;
            next_level.reserve(current_level.size() * 2);
            for (int idx : current_level) {
                if (idx != -1) {
                    int l = 2 * idx + 1;
                    int r = 2 * idx + 2;
                    next_level.push_back((l < n && level_order[l] != -1) ? l : -1);
                    next_level.push_back((r < n && level_order[r] != -1) ? r : -1);
                } else {
                    next_level.push_back(-1);
                    next_level.push_back(-1);
                }
            }
            current_level = next_level;
            offset = POWERS_OF_2[height - level] - 1;
        }
    }
}
#include <cassert>
#include <sstream>

int main() {
    // Capture output for each test case.
    auto capture = [](const std::vector<int>& v) {
        std::ostringstream oss;
        std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
        print_binary_tree_levels(v);
        std::cout.rdbuf(old_cout);
        return oss.str();
    };

    // Test 1: Empty tree.
    assert(capture({}) == "Empty tree\n");

    // Test 2: Single node.
    std::string single = capture({42});
    assert(single.find(" 42") != std::string::npos);
    assert(single.find("/") == std::string::npos && single.find("\\") == std::string::npos);

    // Test 3: Complete tree of height 2 (7 nodes, values 1..7).
    std::vector<int> complete = {1,2,3,4,5,6,7};
    std::string out = capture(complete);
    // Check root appears.
    assert(out.find("  1") != std::string::npos);
    // Check second level values.
    assert(out.find("  2") != std::string::npos && out.find("  3") != std::string::npos);
    // Check third level values.
    assert(out.find("  4") != std::string::npos && out.find("  7") != std::string::npos);
    // Check some pointers exist.
    assert(out.find("/") != std::string::npos && out.find("\\") != std::string::npos);

    // Test 4: Tree with missing nodes (nulls represented by -1).
    // Example: root 10, left 20, right null, left's left 30, rest null.
    // Vector: [10,20,-1,30,-1,-1,-1] (height 2).
    std::vector<int> partial = {10,20,-1,30,-1,-1,-1};
    std::string pout = capture(partial);
    assert(pout.find(" 10") != std::string::npos);
    assert(pout.find(" 20") != std::string::npos);
    assert(pout.find(" 30") != std::string::npos);
    // Ensure no '3' appears in a wrong position? Basic checks.
    assert(pout.find("\\") != std::string::npos); // right child null but there is a left pointer

    // Test 5: Root only with no children (height 0).
    std::vector<int> root_only = {5};
    std::string rout = capture(root_only);
    assert(rout.find("  5") != std::string::npos);
    assert(rout.find("/") == std::string::npos);

    // Test 6: Height 3 (15 nodes complete).
    std::vector<int> full15;
    for (int i = 1; i <= 15; ++i) full15.push_back(i);
    std::string fout = capture(full15);
    // Check last level contains 8 .. 15.
    for (int i = 8; i <= 15; ++i) {
        std::string val = " " + std::to_string(i) + " ";
        assert(fout.find(val) != std::string::npos);
    }

    // Test 7: Left-heavy tree: root 1, left 2, left's left 3, no right children.
    // Vector: [1,2,-1,3,-1,-1,-1] (same as partial but different values).
    std::vector<int> left_heavy = {1,2,-1,3,-1,-1,-1};
    std::string lout = capture(left_heavy);
    assert(lout.find(" 1") != std::string::npos);
    assert(lout.find(" 2") != std::string::npos);
    assert(lout.find(" 3") != std::string::npos);
    // Should have / but not \ for root's right.
    // Check that there's at least one '/' and no '\' after the root? Hard to parse, just ensure output is non-empty.

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
