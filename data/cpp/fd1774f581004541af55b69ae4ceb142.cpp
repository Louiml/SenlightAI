// Write a C++ function named `debugString` that takes a vector of vectors of integers and returns a single std::string representation of that data in the exact format produced by the custom `to_string`/`debug_out` macros from the provided snippet. Specifically, the outer vector should be enclosed in curly braces `{}`, each inner vector also enclosed in curly braces, elements separated by `", "` (comma-space), and there should be no space after the opening brace nor before the closing brace. For example, `{{1,2},{3}}` becomes `"{{1, 2}, {3}}"`. Handle empty vectors correctly: an empty inner vector becomes `{}` and an empty outer vector becomes `{}`. No other leading/trailing whitespace or newlines should be present. The input vector is guaranteed to be non-const but you must not modify it.

// The task requires replicating the recursive string-building logic shown in the snippet's `to_string` for containers, but specialized for `vector<vector<int>>`. The algorithm is straightforward: first handle the base case where the outer vector is empty, returning `"{}"`. Otherwise, iterate over the outer vector using an index, and for each inner vector, build its representation in a similar nested manner: if empty, return `"{}"`; otherwise, concatenate elements with `", "` separators. Use `std::to_string` for individual integers (which handles negative numbers correctly). The main complexity is ensuring correct formatting: no extra spaces at the container boundaries, and the comma-space separator appears only between elements. The time complexity is linear in the total number of integers, since each integer is converted exactly once. Space complexity is linear in the total number of output characters, which is also linear in the number of integers plus the number of containers (since each container adds two curly braces). There are no tricky edge cases other than empty containers, which must be handled explicitly to avoid off-by-one errors in the separator logic.

#include <string>
#include <vector>

// Replicate the custom debug-output formatting for a vector of vectors of ints.
std::string debugString(const std::vector<std::vector<int>>& data) {
    if (data.empty()) {
        return "{}";
    }

    std::string result = "{";
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (i > 0) {
            result += ", ";
        }

        const auto& inner = data[i];
        if (inner.empty()) {
            result += "{}";
        } else {
            result += "{";
            for (std::size_t j = 0; j < inner.size(); ++j) {
                if (j > 0) {
                    result += ", ";
                }
                result += std::to_string(inner[j]);
            }
            result += "}";
        }
    }
    result += "}";

    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test (already defined above).
std::string debugString(const std::vector<std::vector<int>>& data);

int main() {
    // Basic non-empty case
    assert(debugString({{1, 2}, {3}}) == "{{1, 2}, {3}}");
    
    // Single inner vector with one element
    assert(debugString({{42}}) == "{{42}}");
    
    // Empty outer vector
    assert(debugString({}) == "{}");
    
    // Empty inner vectors
    assert(debugString({{}, {1}, {}}) == "{{}, {1}, {}}");
    
    // Multiple inner vectors with various sizes and negative numbers
    assert(debugString({{-1}, {}, {2, 3, -4}, {5}}) == "{{-1}, {}, {2, 3, -4}, {5}}");
    
    // Larger nested data
    assert(debugString({{1, 2, 3}, {4, 5}, {6}}) == "{{1, 2, 3}, {4, 5}, {6}}");
    
    // All inner vectors empty
    assert(debugString({{}, {}}) == "{{}, {}}");
    
    // Single element inner vectors only
    assert(debugString({{7}, {8}, {9}}) == "{{7}, {8}, {9}}");
    
    // Zero values and duplicates
    assert(debugString({{0, 0}, {0}}) == "{{0, 0}, {0}}");
    
    // Large numbers to ensure no overflow in string conversion
    assert(debugString({{2147483647, -2147483648}}) == "{{2147483647, -2147483648}}");
    
    return 0;
}
