// Write a C++ function `vector<string> removeSubfolders(vector<string>& folder)` that takes a list of absolute Unix-style folder paths (each starting with `/`, no trailing slash, and no duplicate paths) and returns the list of folders after removing all subfolders. A folder is considered a subfolder if it is a prefix of another folder immediately followed by a `/`. For example, given `["/a","/a/b","/c/d","/c/d/e","/c/f"]`, the output should be `["/a","/c/d","/c/f"]` because `/a/b` is a subfolder of `/a`, and `/c/d/e` is a subfolder of `/c/d`. The input vector may be modified during processing (sorting is allowed). The order of the output should be sorted lexicographically, which naturally results from the algorithm. The function should handle edge cases such as empty input, folders with common prefixes that are not subfolders (e.g., `/a/b` and `/a/bc`), and folders where one is exactly equal to another (no duplicates in input). Assume the input is valid (all strings start with `/`, no trailing `/`, length >= 1). The function should be efficient for large inputs.

// The core idea is to sort the folder paths lexicographically. After sorting, any subfolder will appear immediately after its parent folder in the sorted order because the parent is a prefix and the next character after the parent in the subfolder is `/`, which sorts before any other character (like letters) that would appear in a sibling folder. We iterate through the sorted list and maintain a result vector. For each folder, we compare it with the last folder added to the result. If the current folder starts with the previous folder, has a length greater than the previous, and the character at the index equal to the previous's length is a `/`, then the current folder is a subfolder and should be skipped. Otherwise, it is not a subfolder of the last kept folder, so we add it to the result. This works because sorting ensures that if a folder is a subfolder of some earlier folder, that earlier folder (or the most recently kept folder) will be the immediate predecessor that is its parent. Edge cases: (1) Empty input returns empty vector; (2) A folder that shares a prefix but not followed by `/` (e.g., `/a/b` and `/a/bc`) – the condition `f[prev.size()] == '/'` correctly distinguishes; (3) Identical strings are not allowed in input, but if they were, sorting would put them adjacent, and the condition `f.size() > prev.size()` would skip duplicates (though not needed). Time complexity: sorting takes `O(N log N * L)` where `L` is the average string length, and the single pass is `O(N * L)` for string comparisons, so overall `O(N log N * L)`. Space complexity: `O(N)` for the result vector, plus `O(1)` extra auxiliary space (ignoring the sorting which uses `O(log N)` stack space).

#include <vector>
#include <string>
#include <algorithm>

// Given a vector of absolute folder paths, remove all subfolders and return
// the remaining folders sorted lexicographically.
std::vector<std::string> removeSubfolders(std::vector<std::string>& folder) {
    std::sort(folder.begin(), folder.end());
    std::vector<std::string> result;
    for (const std::string& current : folder) {
        if (result.empty()) {
            result.push_back(current);
        } else {
            const std::string& previous = result.back();
            // Check if 'current' is a subfolder of 'previous'
            if (current.size() > previous.size() &&
                current.compare(0, previous.size(), previous) == 0 &&
                current[previous.size()] == '/') {
                continue; // 'current' is a subfolder, skip
            } else {
                result.push_back(current);
            }
        }
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

// Function declaration (the above code is assumed to be included here)
std::vector<std::string> removeSubfolders(std::vector<std::string>& folder);

int main() {
    // Basic example
    std::vector<std::string> input1 = {"/a", "/a/b", "/c/d", "/c/d/e", "/c/f"};
    std::vector<std::string> expected1 = {"/a", "/c/d", "/c/f"};
    assert(removeSubfolders(input1) == expected1);

    // Empty input
    std::vector<std::string> input2 = {};
    std::vector<std::string> expected2 = {};
    assert(removeSubfolders(input2) == expected2);

    // No subfolders, only siblings
    std::vector<std::string> input3 = {"/a/b", "/a/c", "/a/d"};
    std::vector<std::string> expected3 = {"/a/b", "/a/c", "/a/d"};
    assert(removeSubfolders(input3) == expected3);

    // Nested deep subfolders
    std::vector<std::string> input4 = {"/a/b/c/d", "/a", "/a/b"};
    std::vector<std::string> expected4 = {"/a"};
    assert(removeSubfolders(input4) == expected4);

    // Prefix that is not a separator (e.g., /a/b vs /a/bc)
    std::vector<std::string> input5 = {"/a/b", "/a/bc", "/a/bc/d"};
    std::vector<std::string> expected5 = {"/a/b", "/a/bc"};
    assert(removeSubfolders(input5) == expected5);

    // Same length but different (no duplicates in input, but check)
    std::vector<std::string> input6 = {"/x", "/y", "/z"};
    std::vector<std::string> expected6 = {"/x", "/y", "/z"};
    assert(removeSubfolders(input6) == expected6);

    // Single folder
    std::vector<std::string> input7 = {"/root"};
    std::vector<std::string> expected7 = {"/root"};
    assert(removeSubfolders(input7) == expected7);

    // Nested with multiple parents and siblings
    std::vector<std::string> input8 = {"/a/b/c", "/a/b", "/a", "/b/c", "/b"};
    std::vector<std::string> expected8 = {"/a", "/b"};
    assert(removeSubfolders(input8) == expected8);

    // Already sorted input
    std::vector<std::string> input9 = {"/1", "/1/2", "/1/2/3"};
    std::vector<std::string> expected9 = {"/1"};
    assert(removeSubfolders(input9) == expected9);

    return 0;
}
