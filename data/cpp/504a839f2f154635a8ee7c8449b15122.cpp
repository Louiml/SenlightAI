// Create a C++ function that calculates the total cost of a sequence of actions, where each action is represented by a character in a string (either '1', '2', '3', or '4'), and each action type has its own per-use cost (given as four integers). The function should take four integer costs (a, b, c, d) and a string `s` containing the action sequence (composed only of characters '1', '2', '3', '4'), and return the total cost as an integer. The string may be empty, in which case the total cost is 0. Assume the costs and the length of the string fit within the range of a 64-bit signed integer.
The solution is straightforward: iterate over each character in the input string once. For each character, check its value and add the corresponding cost to a running total. Since the string is guaranteed to contain only the characters '1' through '4', a simple chain of if-else or a switch statement suffices. Edge cases include an empty string (return 0) and very large costs or very long strings (use `long long` to avoid overflow). The algorithm runs in O(n) time where n is the length of the string, and uses O(1) auxiliary space (only the running total and loop index). The use of `const std::string&` prevents unnecessary copying and supports const correctness.
#include <string>

// Calculate total cost of an action sequence.
// '1' costs a, '2' costs b, '3' costs c, '4' costs d.
long long totalCost(long long a, long long b, long long c, long long d, const std::string& s) {
    long long result = 0;
    for (char ch : s) {
        if (ch == '1') {
            result += a;
        } else if (ch == '2') {
            result += b;
        } else if (ch == '3') {
            result += c;
        } else if (ch == '4') {
            result += d;
        }
    }
    return result;
}
#include <cassert>
#include <string>

// Include the function declaration here or via header
long long totalCost(long long a, long long b, long long c, long long d, const std::string& s);

int main() {
    // Basic test from the snippet
    assert(totalCost(1, 2, 3, 4, "1234") == 10);
    // Empty string returns 0
    assert(totalCost(5, 6, 7, 8, "") == 0);
    // All same character
    assert(totalCost(10, 20, 30, 40, "111") == 30);
    // Mixed characters
    assert(totalCost(1, 100, 3, 40, "4321") == 144);
    // Large costs and long string
    assert(totalCost(1000000000LL, 2000000000LL, 3000000000LL, 4000000000LL, "1234") == 10000000000LL);
    // Repeated pattern
    assert(totalCost(1, 2, 3, 4, "1414") == 10);
    return 0;
}
