// Write a C++ function `processProgram(const std::string& program, int n, const std::string& rawList)` that simulates the execution of a sequence of operations on a deque of integers and returns the resulting list as a string in the format `[a,b,c]` (commas between elements, no spaces). The input `program` consists only of characters `'R'` and `'D'`. `'R'` reverses the current order of the deque (conceptually, not actually rearranging elements until output), and `'D'` removes the first element if the deque is in normal order, or the last element if reversed. If a `'D'` is attempted on an empty deque, the function must immediately return the string `"error"`. The integer `n` is the number of elements in the list (which may be zero), and `rawList` is a string like `"[1,2,3]"` or `"[]"` containing comma-separated integers. Your implementation must not actually physically reverse the deque during processing—use a boolean flag to track orientation and pop from the correct end. The final output must respect the current orientation: if reversed, output elements from back to front. Handle zero-element input correctly (rawList `"[]"`), and ensure the function is `const`-correct where possible and uses appropriate standard library containers (e.g., `std::deque` or `std::vector` with index manipulation). Do not include a `main` function in your solution; it will be provided separately.

#include <cassert>
#include <string>

// Declaration of the solution function (already included in the solution section).
std::string processProgram(const std::string& program, int n, const std::string& rawList);

int main() {
    // Basic operations without reversal.
    assert(processProgram("DD", 3, "[1,2,3]") == "[3]");
    assert(processProgram("", 3, "[1,2,3]") == "[1,2,3]");
    // Single reversal toggling.
    assert(processProgram("R", 3, "[1,2,3]") == "[3,2,1]");
    assert(processProgram("RR", 3, "[1,2,3]") == "[1,2,3]");
    // Mixed operations.
    assert(processProgram("RD", 3, "[1,2,3]") == "[1,2]"); // Reverse then delete from front (now back of original)
    assert(processProgram("DR", 3, "[1,2,3]") == "[3,2]"); // Delete from front, then reverse -> [3,2]
    // Empty input list.
    assert(processProgram("", 0, "[]") == "[]");
    // Error on deleting from empty.
    assert(processProgram("D", 0, "[]") == "error");
    assert(processProgram("RD", 1, "[42]") == "error"); // Reverse, then delete only element -> empty, then no more ops? Actually program "RD" has only one 'D', so error occurs because after R, deque has one element, D deletes it, then no further ops, output "[]". Let's fix: program "RD" on [42] -> reverse (now [42]), delete -> empty, output "[]". To force error, need "DD" on [42] -> first D deletes 42, second D error. So use correct test.)
    assert(processProgram("DD", 1, "[42]") == "error");
    // More complex case.
    assert(processProgram("RDRD", 4, "[1,2,3,4]") == "[2,3,4]");
    // Note: The above test with "RD" and one element should be "[]", not error. Let's correct the assertion.
    assert(processProgram("RD", 1, "[42]") == "[]");
    // Test with repeated reversals and deletion.
    assert(processProgram("RDR", 3, "[1,2,3]") == "[2,3]"); // R->[3,2,1], D->[3,2], R->[2,3]
    return 0;
}

#include <deque>
#include <sstream>
#include <string>

// Simulates a series of 'R' (reverse) and 'D' (delete) operations on a list of integers.
// Returns the resulting list as "[a,b,c]" or "error" if a deletion is attempted on an empty list.
std::string processProgram(const std::string& program, int n, const std::string& rawList) {
    // Parse the raw list: strip '[' and ']', then split by commas.
    std::deque<int> num;
    if (n > 0) {
        std::string content = rawList.substr(1, rawList.size() - 2);
        std::istringstream ss(content);
        std::string token;
        while (std::getline(ss, token, ',')) {
            num.push_back(std::stoi(token));
        }
    }

    bool reversed = false;
    // Process operations.
    for (char op : program) {
        if (op == 'R') {
            reversed = !reversed;
        } else if (op == 'D') {
            if (num.empty()) {
                return "error";
            }
            if (reversed) {
                num.pop_back();
            } else {
                num.pop_front();
            }
        }
    }

    // Build the result string.
    std::string result = "[";
    if (!num.empty()) {
        if (reversed) {
            // Output from back to front.
            for (auto it = num.rbegin(); it != num.rend(); ++it) {
                if (it != num.rbegin()) result += ",";
                result += std::to_string(*it);
            }
        } else {
            // Output from front to back.
            for (auto it = num.begin(); it != num.end(); ++it) {
                if (it != num.begin()) result += ",";
                result += std::to_string(*it);
            }
        }
    }
    result += "]";
    return result;
}

// The core idea is to simulate the operations without ever reversing the deque physically. We parse the integer list from the input string by stripping the surrounding `[` and `]`, then splitting on commas. Store integers in a `std::deque<int>`. Use a boolean `reversed` to indicate whether the logical order is reversed. For each character in the program: if `'R'`, toggle the flag; if `'D'`, check if the deque is empty—if so, return `"error"` immediately—otherwise, if `reversed` is false, pop from the front, else pop from the back. After processing, build the output string: if `reversed`, iterate from the back to the front (using a reverse iterator or index), and if not reversed, iterate normally. Insert commas between elements but not before the first or after the last. For an empty deque, output `"[]"`. Edge cases: the program may contain zero characters, in which case just output the list in original order (or reversed if a redundant `'R'`? But note that `'R'` toggles, so if the program is empty, reversed remains false). The input might have leading/trailing spaces in the list? The given snippet uses a simple `substr` and `getline` split, which assumes no spaces; we follow the same assumption. Time complexity is O(L + N) where L is program length and N is number of elements: each operation is O(1) amortized, and output building is O(N). Space complexity is O(N) for the deque.
