Given a Newick-format phylogenetic tree string (e.g., `"((A:1,B:2):3,(C:4,D:5):6);"`), write a C++ function that parses the tree and returns a string containing the number of leaves (OTUs), the maximum number of nodes along any root-to-leaf path (the tree depth, counting leaves and internal nodes), and the total number of internal nodes, formatted as `"leaves=X depth=Y internal=Z"`. The input is guaranteed to be a valid fully bifurcating rooted tree with branch lengths separated by `:` after node labels, and leaf labels are non-empty strings containing only letters. Do not use any external tree libraries; implement the parser manually using only the standard library. The function should ignore branch length values entirely.
// The approach is to parse the Newick string using a single left-to-right scan while maintaining a stack of open parentheses. Each time we encounter `'('`, we push a marker onto a stack representing a new internal node. Each time we see a leaf label (a sequence of letters), we increment the leaf count. When we encounter `')'`, we pop the stack (indicating completion of an internal node) and increment the internal node count. To compute the depth, we track the current depth as we traverse: depth increases when we see `'('` and decreases when we see `')'`; we update the maximum depth seen. Because the tree is fully bifurcating, every `')'` corresponds to exactly one internal node, so counting closing parentheses works. Edge cases include a single leaf tree (e.g., `"A;"`), which has zero internal nodes and depth 1. The algorithm processes each character once, so time complexity is O(n) for string length n, and space complexity is O(d) where d is the maximum nesting depth (for the stack). We ignore all characters after `:` and before the next delimiter (`,` `)` `;`) when parsing a label.
#include <string>
#include <cctype>

// Parse a Newick tree string and return summary statistics as a formatted string.
std::string tree_summary(const std::string& newick) {
    int leaves = 0, internal = 0, depth = 0, max_depth = 0;
    int pos = 0;
    const int n = newick.size();
    while (pos < n) {
        char c = newick[pos];
        if (c == '(') {
            depth++;
            if (depth > max_depth) max_depth = depth;
            pos++;
        } else if (c == ')') {
            depth--;
            internal++; // every closing parenthesis closes an internal node
            pos++;
        } else if (c == ',') {
            pos++;
        } else if (c == ';') {
            break;
        } else if (c == ':') {
            // skip branch length until delimiter
            while (pos < n && newick[pos] != ',' && newick[pos] != ')' && newick[pos] != ';') {
                pos++;
            }
        } else if (std::isalpha(c)) {
            // leaf label: read letters until delimiter
            while (pos < n && std::isalpha(newick[pos])) {
                pos++;
            }
            leaves++;
        } else {
            pos++; // skip unknown punctuation
        }
    }
    return "leaves=" + std::to_string(leaves) + " depth=" + std::to_string(max_depth) + " internal=" + std::to_string(internal);
}
#include <cassert>
#include <string>

// (function declaration from solution is assumed to be visible)

int main() {
    // Standard tree with 2 leaves, one internal node, depth 3 (root + leaf)
    assert(tree_summary("((A:1,B:2):3);") == "leaves=2 depth=3 internal=1");
    // Tree with 4 leaves, 3 internal nodes, depth 4
    assert(tree_summary("((A:1,B:2):3,(C:4,D:5):6);") == "leaves=4 depth=4 internal=3");
    // Single leaf tree
    assert(tree_summary("X:10;") == "leaves=1 depth=1 internal=0");
    // Deep unbalanced tree
    assert(tree_summary("((((A:1,B:2):3,C:4):5,D:6):7);") == "leaves=4 depth=5 internal=3");
    // Labels with no branch lengths
    assert(tree_summary("((A,B),(C,D));") == "leaves=4 depth=4 internal=3");
    // Branch lengths with decimals
    assert(tree_summary("((A:0.5,B:1.25):2.5,(C:3,D:0.75):1.4);") == "leaves=4 depth=4 internal=3");
    // Larger tree: 6 leaves
    assert(tree_summary("((A:1,B:2):3,((C:4,D:5):6,(E:7,F:8):9):10);") == "leaves=6 depth=5 internal=5");
    // Tree with only two leaves and root
    assert(tree_summary("(A:1,B:2);") == "leaves=2 depth=2 internal=1");
    // Empty internal node? Not fully bifurcating, but test robustness: single leaf with colon
    assert(tree_summary("Z;") == "leaves=1 depth=1 internal=0");
    // Deep balanced tree: 8 leaves, 7 internal, depth 5 (root counts as depth 1? Actually root plus 4 levels)
    assert(tree_summary("(((A:1,B:2):3,(C:4,D:5):6):7,((E:8,F:9):10,(G:11,H:12):13):14);") == "leaves=8 depth=5 internal=7");
    return 0;
}
