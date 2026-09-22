/*
Write a C++ function that takes a single positive integer `n` and returns a `std::vector<std::string>` where each string is a row of exactly `n` characters, all being the character `'1'`. The function should produce `n` such rows, effectively creating an `n x n` grid of ones. The function must be pure (no input/output inside the function) and should handle edge cases like `n = 1` (returns one row with one `'1'`) and `n = 0` (returns an empty vector). The implementation must not use any global variables or side effects.
*/
#include <string>
#include <vector>

// Returns a vector of n strings, each being a row of n '1' characters.
std::vector<std::string> gridOfOnes(int n) {
    std::vector<std::string> result;
    if (n <= 0) {
        return result; // empty for n <= 0
    }
    const std::string row(n, '1'); // one row of n ones
    result.assign(n, row);         // fill vector with n copies
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or via header) for the test.

int main() {
    // n = 1
    assert(gridOfOnes(1) == std::vector<std::string>{"1"});
    
    // n = 3
    assert(gridOfOnes(3) == std::vector<std::string>{"111", "111", "111"});
    
    // n = 0 returns empty
    assert(gridOfOnes(0).empty());
    
    // n = 5, check size and content of each row
    auto g5 = gridOfOnes(5);
    assert(g5.size() == 5);
    for (const auto& s : g5) {
        assert(s.size() == 5);
        assert(s == std::string(5, '1'));
    }
    
    // n = 2
    assert(gridOfOnes(2) == std::vector<std::string>{"11", "11"});
    
    // Negative n returns empty
    assert(gridOfOnes(-1).empty());
    
    // Verify each row is independent (modifying one doesn't affect others)
    auto g = gridOfOnes(2);
    g[0][0] = '0';
    assert(g[1] == "11");
}
// The core task is straightforward: build a vector of `n` identical strings, each containing `n` copies of the character `'1'`. The most direct approach is to create a base string of length `n` filled with `'1'` (using `std::string(n, '1')`) and then push that same string into a vector `n` times. Alternatively, use nested loops to construct each row individually, but using `std::string(n, '1')` is both simpler and more efficient. Edge cases: if `n == 0`, the vector is empty (no rows); if `n == 1`, the vector contains one string `"1"`. The algorithm runs in O(n^2) time because the output has `n^2` characters total, and O(n) auxiliary space for the vector of strings (each string is length `n`, but the total storage is O(n^2) since we store all characters; however, since we reuse the same string content, we could optimize but not necessary). In practice, we allocate `n` strings each of length `n`, so total space is O(n^2) for the output, plus O(n) temporary storage for the base string.
