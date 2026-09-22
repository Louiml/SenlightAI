/*
Write a C++ function `bool isValidPreorderSerialization(const std::string& preorder)` that determines whether a given comma-separated string is a valid preorder traversal serialization of a binary tree, without reconstructing the tree. The string contains only integers (possibly multiple digits) and the character `'#'` representing null nodes, separated by single commas. The input format is guaranteed valid (no consecutive commas, no empty fields). A valid serialization must form exactly one binary tree: every non-null node has exactly two children (either actual nodes or nulls), the traversal ends exactly after consuming all nodes, and no extra or missing nodes exist. For example, `"9,3,4,#,#,1,#,#,2,#,6,#,#"` should return `true`, while `"1,#"` and `"9,#,#,1"` should return `false`. The function must handle edge cases like a single `"#"` (valid empty tree) and sequences that overshoot or undershoot node counts.
*/

#include <string>
#include <sstream>

bool isValidPreorderSerialization(const std::string& preorder) {
    int slots = 1;  // start with one slot for the root
    std::stringstream ss(preorder);
    std::string token;

    while (std::getline(ss, token, ',')) {
        // A token consumes one slot (whether '#' or integer)
        --slots;
        if (slots < 0) {
            return false;  // more nodes than available parent slots
        }
        if (token != "#") {
            slots += 2;  // non-null node creates two child slots
        }
    }

    return slots == 0;
}

#include <cassert>

int main() {
    // Valid serialization from the problem statement
    assert(isValidPreorderSerialization("9,3,4,#,#,1,#,#,2,#,6,#,#") == true);

    // Invalid: only root and one null; missing left child of root
    assert(isValidPreorderSerialization("1,#") == false);

    // Invalid: root has two null children, but extra node follows
    assert(isValidPreorderSerialization("9,#,#,1") == false);

    // A single null node represents an empty tree -> valid
    assert(isValidPreorderSerialization("#") == true);

    // Root with two null children -> valid
    assert(isValidPreorderSerialization("9,#,#") == true);

    // More nulls than needed -> invalid (slots go negative)
    assert(isValidPreorderSerialization("9,#,#,#") == false);

    // More nodes than slots -> invalid
    assert(isValidPreorderSerialization("1,2,3") == false);

    // Deep valid tree
    assert(isValidPreorderSerialization("1,2,#,#,3,#,#") == true);

    // Valid tree with multi-digit values
    assert(isValidPreorderSerialization("12,34,#,#,56,#,#") == true);

    // Invalid: extra comma at the end is not allowed by format, but test with valid format only
    // Empty string is not a valid format (should have at least one token); but ensure no crash
    // (We can skip empty input as the problem guarantees valid input format)
}

// The problem can be solved by simulating a preorder traversal while tracking the number of "available slots" for nodes. Initially, one slot exists for the root. As we process each comma-separated token from left to right:
// - If the token is `"#"`, it consumes one slot (fill a null node), so slot count decreases by 1.
// - If the token is an integer, it also consumes one slot (fill a non-null node), but then creates two new child slots, so net slot count increases by 1.
// If at any point the slot count becomes negative, the serialization is invalid because a node appears where no parent expects a child. After processing all tokens, the slot count must be exactly zero; if it is positive, there are leftover missing children, and if negative (already caught), there are too many nodes. Key edge cases: a single `"#"` starts with slot=1, consumes it, ends with 0 → valid. `"1,#"` starts with slot=1, token "1" makes slot=2, then "#" makes slot=1 → positive leftover → invalid. `"9,#,#,1"` starts with 1, "9" → 2, "#" → 1, "#" → 0, then "1" → -1 → invalid (negative at the end). Time complexity is O(n) where n is the length of the string (one pass over tokens), and space complexity is O(1) besides the input string itself.
