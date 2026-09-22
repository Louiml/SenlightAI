/*
Write a C++ function that takes two integers `b` and `w` (both non-negative, and at least one is positive) representing the counts of black and white nodes in a connected undirected tree. The function must return a vector of strings, where the first element is a string of length `b + w` consisting of 'B' and 'W' characters representing the color of each node (nodes indexed 1 to b+w), and each subsequent element is a string in the format "1 i" (for i from 2 to b+w) representing an edge connecting node 1 to node i. The construction must satisfy: (1) The number of black nodes equals `b` and white nodes equals `w`. (2) The tree is connected and has exactly `b + w` nodes. (3) The color of each node is either 'B' or 'W'. If it is impossible to construct such a tree (e.g., when one color count is 0 and the other is greater than 1), return an empty vector. Note that a single-node tree (b=1,w=0 or b=0,w=1) is valid. The function should be deterministic and follow the same pattern as the given snippet: if possible, the tree should be a star centered at node 1, and the coloring should be arranged so that all nodes of one color (except possibly the center) are connected directly to the center.
*/
#include <vector>
#include <string>

// Construct a valid tree with given black (b) and white (w) node counts.
// Returns a vector of strings: first element is the color string of all nodes,
// subsequent elements are edges "1 i". Returns empty vector if impossible.
std::vector<std::string> buildTree(int b, int w) {
    // Impossible cases: one color zero and the other > 1
    if ((b == 0 && w > 1) || (w == 0 && b > 1)) {
        return {};
    }

    // Single node cases
    if (b == 1 && w == 0) {
        return {"B"};
    }
    if (w == 1 && b == 0) {
        return {"W"};
    }

    std::vector<std::string> result;
    int total = b + w;

    // Special case: exactly one white but multiple blacks -> center white
    if (w == 1) {
        std::string colors = "W" + std::string(b, 'B');
        result.push_back(colors);
        for (int i = 2; i <= total; ++i) {
            result.push_back("1 " + std::to_string(i));
        }
        return result;
    }

    // General case: w >= 2, b >= 1 (also covers b==1, w>=2)
    // Center black, then w whites, then b-1 extra blacks
    std::string colors = "B" + std::string(w, 'W') + std::string(b - 1, 'B');
    result.push_back(colors);
    for (int i = 2; i <= total; ++i) {
        result.push_back("1 " + std::to_string(i));
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Solution function declared above
std::vector<std::string> buildTree(int b, int w);

int main() {
    // Impossible cases
    assert(buildTree(0, 3).empty());
    assert(buildTree(5, 0).empty());

    // Single node
    auto t1 = buildTree(1, 0);
    assert(t1.size() == 1 && t1[0] == "B");
    auto t2 = buildTree(0, 1);
    assert(t2.size() == 1 && t2[0] == "W");

    // w==1, b>=1
    auto t3 = buildTree(3, 1);
    assert(t3.size() == 4); // 1 color string + 3 edges
    assert(t3[0] == "WBBB"); // center white, then 3 blacks
    assert(t3[1] == "1 2");
    assert(t3[2] == "1 3");
    assert(t3[3] == "1 4");

    // General case b=1,w=2
    auto t4 = buildTree(1, 2);
    assert(t4.size() == 3); // 1 + 2 edges
    assert(t4[0] == "BWW"); // center black, then 2 whites
    assert(t4[1] == "1 2");
    assert(t4[2] == "1 3");

    // General case b=2,w=3
    auto t5 = buildTree(2, 3);
    assert(t5.size() == 6); // 1 + 5 edges
    assert(t5[0] == "BWWWB"); // center black, 3 whites, 1 extra black
    for (int i = 1; i <= 5; ++i) {
        assert(t5[i] == "1 " + std::to_string(i + 1));
    }

    // b=1,w=1 (also general case covers b=1,w>=2? No, w=1 special case handles this)
    auto t6 = buildTree(1, 1);
    assert(t6.size() == 2); // 1 + 1 edge
    assert(t6[0] == "WB"); // special case w==1 gives center white
    assert(t6[1] == "1 2");

    return 0;
}
// The problem is to construct a valid connected tree with given black and white counts. The only impossible cases are when one count is zero and the other is greater than one, because a connected tree with more than one node cannot have all nodes the same color while maintaining connectivity (there would be no node of the other color to connect between). For the valid cases, we handle:  
// - If b==1 and w==0: single black node. Return "B" and no edges.  
// - If w==1 and b==0: single white node. Return "W" and no edges.  
// - If w==1 and b>=1: We place a white node as center (node 1), then connect all black nodes to it, and the remaining 0 white nodes (w-1=0) are none. So string = "W" + b times "B". Edges: connect center to nodes 2..b+1.  
// - If b>=1 and w>=1 but w>1: The general case from the snippet: make node 1 black, then connect `w` white nodes to it, and the remaining `b-1` black nodes to it. So string = "B" + w times "W" + (b-1) times "B". Edges: connect center to nodes 2..b+w.  
// But careful: if w==1 and b>=1, the snippet uses a special case to avoid the general pattern that would produce "B" + "W" + (b-1) "B" which is also valid. However the snippet special-cases w==1 to make the pattern "W" + b "B" (center white). Both are valid, but we must follow the snippet's exact output pattern for consistency. Actually the snippet handles w==1 and b>1 by making center white. For w>1 and b>=1, it uses center black. For b==1 and w>1, the snippet uses the general case: center black, then w whites, then 0 extra blacks. That works. For w==0 and b>1, it returns -1. For b==0 and w>1, it returns -1.  
// So the solution:  
// - If (b==0 && w>1) or (w==0 && b>1) → return empty vector.  
// - If b==1 && w==0 → return vector with one string "B".  
// - If w==1 && b==0 → return vector with one string "W".  
// - If w==1 && b>=1 → center white, string = "W" + "B"*b, edges from 1 to 2..b+1.  
// - Otherwise (b>=1, w>=2) → center black, string = "B" + "W"*w + "B"*(b-1), edges from 1 to 2..b+w.  
// Time complexity: O(b+w) to build the string and edges. Space complexity: O(b+w) for the output vector.  
// Edge cases: b=0,w=0 is not allowed since at least one positive; we can assume input given is valid (at least one positive).
