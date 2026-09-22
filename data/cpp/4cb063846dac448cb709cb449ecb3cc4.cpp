/*
Write a C++ function `registerNames` that takes a positive integer `n` and an input stream, reads `n` strings (names), and returns a `std::vector<std::string>` containing the output lines exactly as produced by the given snippet: for each name, if it is seen for the first time, output "OK"; if it has been seen before, output the name followed by the number of times it has been seen so far (not counting the current occurrence, i.e., the count before incrementing). The function must preserve the exact formatting (no leading/trailing spaces) and handle names that are arbitrary non-whitespace strings. The function should not print to `std::cout`; it should return the lines in order.
*/
#include <string>
#include <vector>
#include <unordered_map>
#include <istream>

// Reads n names from the input stream and returns the output lines as a vector.
// For each name, if it's the first occurrence, returns "OK"; otherwise returns
// the name followed by the number of previous occurrences (count before increment).
std::vector<std::string> registerNames(int n, std::istream& input) {
    std::vector<std::string> result;
    result.reserve(n);
    std::unordered_map<std::string, int> count;
    
    for (int i = 0; i < n; ++i) {
        std::string name;
        input >> name;
        int previousCount = count[name]; // 0 if not present
        if (previousCount == 0) {
            result.push_back("OK");
        } else {
            result.push_back(name + std::to_string(previousCount));
        }
        ++count[name];
    }
    return result;
}
#include <cassert>
#include <sstream>
#include <vector>
#include <string>

// Declaration from solution
std::vector<std::string> registerNames(int n, std::istream& input);

int main() {
    // Test 1: Basic example from snippet
    std::istringstream ss1("5\nabc\nabc\nxyz\nabc\ndef");
    std::vector<std::string> r1 = registerNames(5, ss1);
    std::vector<std::string> e1 = {"OK", "abc1", "OK", "abc2", "OK"};
    assert(r1 == e1);

    // Test 2: All unique names
    std::istringstream ss2("3\na\nb\nc");
    std::vector<std::string> r2 = registerNames(3, ss2);
    std::vector<std::string> e2 = {"OK", "OK", "OK"};
    assert(r2 == e2);

    // Test 3: Same name repeated many times
    std::istringstream ss3("4\nx\nx\nx\nx");
    std::vector<std::string> r3 = registerNames(4, ss3);
    std::vector<std::string> e3 = {"OK", "x1", "x2", "x3"};
    assert(r3 == e3);

    // Test 4: Names that are numeric strings
    std::istringstream ss4("4\n123\n123\n456\n123");
    std::vector<std::string> r4 = registerNames(4, ss4);
    std::vector<std::string> e4 = {"OK", "1231", "OK", "1232"};
    assert(r4 == e4);

    // Test 5: Single name only
    std::istringstream ss5("1\nhello");
    std::vector<std::string> r5 = registerNames(1, ss5);
    std::vector<std::string> e5 = {"OK"};
    assert(r5 == e5);

    return 0;
}
// The core idea is to maintain a frequency map (`std::map<std::string, int>` or `std::unordered_map`) for each name. As we read each string, we query its current count. If the count is 0 (i.e., first occurrence), we append `"OK"` to the result vector. If the count is greater than 0, we append the name concatenated with the current count (which is the number of previous occurrences, because the current occurrence hasn't been counted yet). After producing the output line, we increment the count for that name. This matches the snippet's behavior exactly (note the snippet's commented-out increments and the effective count logic). Edge cases: empty input (but task guarantees n positive), duplicate names many times, names that are purely numeric (they are strings, so concatenation works). Time complexity is O(n log k) where k is the number of distinct names if using `std::map`, or O(n) average with `std::unordered_map`; space is O(k). We'll implement with `std::unordered_map` for average O(1) per operation for clarity, but mention both.
