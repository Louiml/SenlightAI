Write a C++ function `bool canTransform(const std::string& start, const std::string& target)` that determines whether the string `start` can be transformed into the string `target` under the following rules. Both strings have the same length and consist only of characters `'L'`, `'R'`, and `'_'` (underscore representing empty space). The character `'L'` can move left any number of empty positions (i.e., swap with consecutive `'_'` characters to its left), but cannot pass through another `'L'` or `'R'`. Similarly, `'R'` can move right any number of empty positions, but cannot pass through any other piece. Non-empty characters cannot jump over one another, and the relative order of all `'L'` and `'R'` pieces must remain unchanged. The function must return `true` if such a transformation is possible, and `false` otherwise. Assume the input strings are always of equal length and contain only the allowed characters.
#include <cassert>

int main() {
    // Basic cases
    assert(canTransform("_L__R__R_", "L______RR") == true);
    assert(canTransform("R_L_", "__LR") == false);
    assert(canTransform("_R", "R_") == false);      // R cannot move left
    assert(canTransform("L_", "_L") == true);       // L moves left
    assert(canTransform("____", "____") == true);   // All underscores

    // Same order but wrong direction constraints
    assert(canTransform("R__L", "__RL") == false);  // Order changes
    assert(canTransform("_R__L_", "R____L") == false); // R moves left
    
    // Pieces already in place
    assert(canTransform("LR", "LR") == true);
    assert(canTransform("RL", "RL") == true);

    // L moves left multiple spaces
    assert(canTransform("___L", "L___") == true);
    // R moves right multiple spaces
    assert(canTransform("R___", "___R") == true);
    
    // Blocking by other pieces
    assert(canTransform("LR_", "L_R") == false);    // R cannot pass L? Actually order preserved, but R would need to move left
    assert(canTransform("_LR", "LR_") == true);     // Both move left together

    return 0;
}
#include <string>

// Determines if 'start' can be transformed into 'target' by moving
// 'L' left and 'R' right through underscores without crossing pieces.
bool canTransform(const std::string& start, const std::string& target) {
    const int n = static_cast<int>(start.length());
    int i = 0;  // Index for start
    int j = 0;  // Index for target

    while (i < n || j < n) {
        // Skip underscores in start
        while (i < n && start[i] == '_') {
            ++i;
        }
        // Skip underscores in target
        while (j < n && target[j] == '_') {
            ++j;
        }

        // If one string is exhausted, both must be exhausted
        if (i == n || j == n) {
            return i == n && j == n;
        }

        // Pieces must match in order and type
        if (start[i] != target[j]) {
            return false;
        }

        // 'L' can only move left (start index >= target index)
        if (start[i] == 'L' && i < j) {
            return false;
        }
        // 'R' can only move right (start index <= target index)
        if (start[i] == 'R' && i > j) {
            return false;
        }

        ++i;
        ++j;
    }

    return true;
}
// The solution uses a two-pointer technique that scans both strings simultaneously while skipping all underscores. The idea is to extract, in order, all the non-underscore characters from both strings and compare their sequence. If the sequences differ at any point, transformation is impossible because pieces cannot cross each other. Additionally, for each matching pair of pieces, we must verify that movement direction is feasible: an `'L'` in `start` can only move left, so its current index `i` must be greater than or equal to its target index `j` (i.e., `i >= j`). Conversely, an `'R'` in `start` can only move right, so its current index `i` must be less than or equal to its target index `j` (i.e., `i <= j`). If either condition is violated, return `false`. The loop terminates when either pointer reaches the end of its string; at that point, both must have exhausted all non-underscore characters simultaneously, otherwise one string has extra pieces. The time complexity is O(n) where n is the string length, and the auxiliary space is O(1). Edge cases include strings with no pieces (all underscores, always true), pieces already in correct positions, and pieces that need to move in the correct direction but are blocked by others—the order check catches any invalid crossing.
