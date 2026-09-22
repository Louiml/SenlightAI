// Write a C++ function that processes a list of operations for a special "double-ended priority queue." Each operation is a string starting with either `'I'` (insert) followed by a space and an integer (e.g., `"I 42"`), or `'D'` (delete) followed by a space and either `"1"` to delete the maximum value or `"-1"` to delete the minimum value. If the queue is empty, delete operations are ignored. The function must return a vector of two integers: the first is the maximum value remaining in the queue, and the second is the minimum value, in that order; if the queue ends up empty, return `{0, 0}`. The input may contain duplicate values, negative numbers, and the operations are processed in order. Provide a function signature: `std::vector<int> processPriorityOperations(const std::vector<std::string>& operations);`
// The core requirement is to maintain a multiset of values that supports efficient insertion, and removal of the smallest or largest element. A `std::multiset` stores elements in sorted order and provides `begin()` and `rbegin()` (or `--end()`) for constant-time access to the smallest and largest elements. For each operation string, check the first character: if it's `'I'`, extract the integer after the space using `std::stoi` on a substring and insert it into the multiset. If it's `'D'`, inspect the character after the space: if it's `'1'` (maximum), erase the last element when the multiset is non-empty; if it's `'-'` (minimum, since the string will be `"D -1"`), erase the first element when non-empty. The `multiset` automatically handles duplicates. Edge cases include empty multiset operations (ignore deletions), and at the end if the multiset is empty, return `{0, 0}`; otherwise, set `answer[0]` to the largest (`*ms.rbegin()` or `*(--ms.end())`) and `answer[1]` to the smallest (`*ms.begin()`). Time complexity is `O(n log m)` where `n` is the number of operations and `m` is the current size of the multiset (insert and erase take logarithmic time). Space complexity is `O(m)` for storing the elements.
#include <string>
#include <vector>
#include <set>

// Processes insert/delete-max/min operations on a double-ended priority queue.
// Returns {max_remaining, min_remaining}, or {0,0} if empty after all ops.
std::vector<int> processPriorityOperations(const std::vector<std::string>& operations) {
    std::multiset<int> queue;
    
    for (const std::string& op : operations) {
        if (op.empty()) continue; // defensive, though operations are well-formed
        if (op[0] == 'I') {
            // Format: "I <number>"
            int value = std::stoi(op.substr(2));
            queue.insert(value);
        } else if (op[0] == 'D') {
            // Format: "D 1" (delete max) or "D -1" (delete min)
            if (op[2] == '1') { // delete max
                if (!queue.empty()) {
                    auto it = queue.end();
                    --it; // points to last (largest) element
                    queue.erase(it);
                }
            } else if (op[2] == '-') { // delete min
                if (!queue.empty()) {
                    queue.erase(queue.begin());
                }
            }
        }
    }
    
    if (queue.empty()) {
        return {0, 0};
    }
    int max_val = *queue.rbegin(); // largest
    int min_val = *queue.begin();  // smallest
    return {max_val, min_val};
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic insert and delete min/max
    assert(processPriorityOperations({"I 5", "I 3", "I 8", "D 1", "D -1"}) == std::vector<int>({5, 5}));
    
    // Empty queue deletions ignored, final empty -> {0,0}
    assert(processPriorityOperations({"D 1", "I -2", "D -1", "D -1"}) == std::vector<int>({0, 0}));
    
    // Duplicates handled correctly
    assert(processPriorityOperations({"I 7", "I 7", "I 7", "D 1", "D 1"}) == std::vector<int>({7, 7}));
    
    // Negative numbers
    assert(processPriorityOperations({"I -10", "I -1", "D 1", "I -5"}) == std::vector<int>({-5, -10}));
    
    // Single element
    assert(processPriorityOperations({"I 42"}) == std::vector<int>({42, 42}));
    
    // Interleaved operations
    assert(processPriorityOperations({"I 1", "I 2", "D -1", "I 3", "D 1", "I 0"}) == std::vector<int>({0, 0}));
    
    // Larger sequence with all max deletes
    assert(processPriorityOperations({"I 10", "I 20", "I 30", "D 1", "D 1"}) == std::vector<int>({10, 10}));
    
    // All min deletes
    assert(processPriorityOperations({"I 10", "I 20", "D -1", "D -1"}) == std::vector<int>({0, 0}));
    
    // Negative and positive mix
    assert(processPriorityOperations({"I 1", "I -1", "I 0", "D -1", "D 1"}) == std::vector<int>({0, 0}));
    
    // Empty input
    assert(processPriorityOperations({}) == std::vector<int>({0, 0}));
    
    return 0;
}
