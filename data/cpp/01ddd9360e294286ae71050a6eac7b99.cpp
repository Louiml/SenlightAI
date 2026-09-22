Write a C++ function that takes a non-empty string (containing only printable ASCII characters, possibly with spaces) and returns a `std::vector<std::string>` containing all distinct rotations of the original string. A rotation is obtained by moving the first character to the end repeatedly. The returned vector must contain each rotation exactly once, in the order they are generated (i.e., the original string first, then the rotation after one left shift, etc.). The input string length is at least 1. For a string of length `n`, there are exactly `n` rotations; if the string consists of a repeated pattern (e.g., "abab"), some rotations may be identical, but the function must still return all `n` rotations including duplicates, because they are generated sequentially. The function must not modify the input string.
// The solution is straightforward: repeatedly take the first character and append it to the end of the string, each time storing the resulting string in a vector. Start with the original string as the first element. For a string of length `n`, we perform `n-1` left shifts after the initial copy, for a total of `n` rotations. Edge cases: an empty string should not occur by specification, but if it did, the function would return an empty vector; however, the task guarantees non-empty. For length 1, there is exactly one rotation (the string itself), and the loop runs zero times for the shifts. Since the input is passed by `const std::string&`, we make a mutable copy inside the function to avoid altering the caller's data. Complexity: each rotation involves string concatenation of length `n`, and we do this `n` times, so the time complexity is `O(n^2)` due to repeated copying; space complexity is `O(n^2)` in total because we store `n` strings each of length `n` (output size), plus the internal copy.
#include <string>
#include <vector>

// Return all left rotations of the given non-empty string, including duplicates.
std::vector<std::string> allRotations(const std::string& input) {
    std::vector<std::string> rotations;
    if (input.empty()) {
        return rotations;
    }

    std::string current = input; // mutable copy
    const size_t len = input.length();

    for (size_t i = 0; i < len; ++i) {
        rotations.push_back(current);
        // Perform one left rotation: move first char to end
        if (len > 1) {
            char first = current[0];
            current = current.substr(1) + first;
        }
    }

    return rotations;
}
#include <cassert>
#include <vector>
#include <string>
using namespace std;

// The solution function is declared above.
// Test code:
int main() {
    // Basic test: length 3
    vector<string> r1 = allRotations("abc");
    assert(r1.size() == 3);
    assert(r1[0] == "abc");
    assert(r1[1] == "bca");
    assert(r1[2] == "cab");

    // Length 1
    assert(allRotations("x") == vector<string>({"x"}));

    // Repeated pattern – duplicates must be included
    vector<string> r2 = allRotations("abab");
    assert(r2.size() == 4);
    assert(r2[0] == "abab");
    assert(r2[1] == "baba");
    assert(r2[2] == "abab");
    assert(r2[3] == "baba");

    // String with spaces
    vector<string> r3 = allRotations("a b");
    assert(r3.size() == 3);
    assert(r3[0] == "a b");
    assert(r3[1] == " ba");
    assert(r3[2] == "ba ");

    // Longer string: "hello" – check first and last
    vector<string> r4 = allRotations("hello");
    assert(r4.size() == 5);
    assert(r4[0] == "hello");
    assert(r4[4] == "ohell");

    // Ensure input unchanged
    string input = "test";
    allRotations(input);
    assert(input == "test");

    // Mixed characters
    vector<string> r5 = allRotations("A1!");
    assert(r5 == vector<string>({"A1!", "1!A", "!A1"}));
}
