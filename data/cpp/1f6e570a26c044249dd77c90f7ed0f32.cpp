Write a C++ function `colorPattern(int n)` that, given a positive integer `n`, returns a string of length `n` formed by repeating the seven-character sequence `"ROYGBIV"` as many times as needed, then appending a specific prefix (from the start of `"ROYGBIV"`) to fill the remaining positions. The appended prefix must be chosen so that the resulting pattern has no two adjacent characters equal (cyclic adjacency: the last character is adjacent to the first). Specifically, for a remainder `r = n % 7`, the suffix must be exactly: if `r == 0` → append nothing; if `r == 1` → `"G"`; `r == 2` → `"GB"`; `r == 3` → `"GBI"`; `r == 4` → `"GBIV"`; `r == 5` → `"GBIVG"`; `r == 6` → `"GBIVGB"`. The function should handle any positive `n` (including values less than 7) and return a `std::string`. The solution must be self-contained, with no reliance on global variables, and should not be an exact copy of the given snippet but a proper function implementing the same logic.
#include <cassert>
#include <string>

// Declare the function (or include the header where it is defined)
std::string colorPattern(int n);

int main() {
    assert(colorPattern(7) == "ROYGBIV");
    assert(colorPattern(1) == "G");
    assert(colorPattern(2) == "GB");
    assert(colorPattern(3) == "GBI");
    assert(colorPattern(4) == "GBIV");
    assert(colorPattern(5) == "GBIVG");
    assert(colorPattern(6) == "GBIVGB");
    assert(colorPattern(8) == "ROYGBIVG");
    assert(colorPattern(14) == "ROYGBIVROYGBIV");
    assert(colorPattern(17) == "ROYGBIVROYGBIVGBI");
    return 0;
}
#include <string>

// Return a string of length n following the "ROYGBIV" repeating pattern
// with a special suffix to ensure no adjacent equal characters cyclically.
std::string colorPattern(int n) {
    std::string result;
    result.reserve(n);

    // Append full cycles of "ROYGBIV"
    const std::string block = "ROYGBIV";
    int fullCycles = n / 7;
    for (int i = 0; i < fullCycles; ++i) {
        result += block;
    }

    // Append the required suffix based on the remainder
    int r = n % 7;
    if (r == 1) {
        result += "G";
    } else if (r == 2) {
        result += "GB";
    } else if (r == 3) {
        result += "GBI";
    } else if (r == 4) {
        result += "GBIV";
    } else if (r == 5) {
        result += "GBIVG";
    } else if (r == 6) {
        result += "GBIVGB";
    }
    // r == 0 adds nothing

    return result;
}
// The core idea is to first append the full block `"ROYGBIV"` repeated `n / 7` times to cover as many complete cycles as possible. Since the full block is internally valid (adjacent characters differ, and the cyclic adjacency from end 'V' to start 'R' is valid), appending full blocks preserves validity across block boundaries. After that, the remaining `r = n % 7` positions must be filled. The given suffix patterns are chosen such that the last character of the suffix does not equal 'R' (the first character of the pattern) to maintain cyclic adjacency, and also the first character of the suffix does not equal 'V' (the last character of the last full block) when a full block exists. For `n < 7`, the suffix alone is the entire pattern, and the chosen suffixes are guaranteed to have no adjacent duplicates and also meet the cyclic condition. The algorithm runs in O(n) time because it constructs a string of length `n` (concatenations are linear in the output size) and uses O(1) auxiliary space beyond the output string. Edge cases include `n` being a multiple of 7 (just full blocks, no suffix), and `n` less than 7 (only the suffix, no full blocks). The mapping from remainder to suffix is fixed and must be implemented exactly as specified.
