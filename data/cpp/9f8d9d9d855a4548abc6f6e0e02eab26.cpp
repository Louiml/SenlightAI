Write a C++ function named `processSetOperations` that takes a vector of strings representing a series of set operations on integers and returns a vector of strings containing the output of all "query" operations (type 3). Each operation string follows the same format as the provided code: first character is the operation type (`'1'` = insert, `'2'` = erase, `'3'` = query), followed by a single space, then the integer value. For type 3 queries (whether the value exists in the set), you must append `"Yes"` if the integer is present, otherwise `"No"`. The function should process operations in order, maintaining a `std::set<int>`, and must handle duplicate insertions gracefully (they do nothing) and erasing a value that is not present (it should do nothing). You may assume all input strings are well-formed (non-empty, correct format). The function should return the collected query results in the order they were processed.

// The solution uses a `std::set<int>` to store the integers because it automatically maintains sorted order and provides logarithmic-time operations for insertion, deletion, and lookup. We iterate through each operation string, parse the operation type from the first character and the integer value from the substring after position 2 (skipping the space). For type `'1'`, we call `insert` on the set—duplicates are ignored by the set. For type `'2'`, we find the value; if found, we erase it; otherwise, nothing happens. For type `'3'`, we find the value and append `"Yes"` if found, `"No"` otherwise to the result vector. Important edge cases: the input may contain negative numbers, which `atoi` or `stoi` handles; the set should be initially empty; and the number of operations could be large, so we must avoid unnecessary copies or re-parsing. Time complexity is \(O(n \log m)\) where \(n\) is the number of operations and \(m\) is the number of distinct integers in the set (since each set operation is \(O(\log m)\)). Space complexity is \(O(m + k)\) where \(m\) is the set size and \(k\) is the number of query results. Parsing the integer can be done with `std::stoi(line.substr(2))` which is safe and clean.

#include <string>
#include <vector>
#include <set>
#include <sstream>

// Process a series of integer set operations and return the results of all queries.
std::vector<std::string> processSetOperations(const std::vector<std::string>& operations) {
    std::set<int> values;
    std::vector<std::string> results;
    
    for (const auto& op : operations) {
        // Parse operation type from first character
        char operationType = op[0];
        // Parse the integer after "X " (position 2)
        int number = std::stoi(op.substr(2));
        
        switch (operationType) {
            case '1': // insert
                values.insert(number);
                break;
            case '2': { // erase
                auto it = values.find(number);
                if (it != values.end()) {
                    values.erase(it);
                }
                break;
            }
            case '3': { // query
                auto it = values.find(number);
                if (it != values.end()) {
                    results.push_back("Yes");
                } else {
                    results.push_back("No");
                }
                break;
            }
            default:
                break;
        }
    }
    
    return results;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is already defined above; here is the test harness.
// (In a real solution file, include the function definitions.)
int main() {
    // Basic insert, erase, query
    std::vector<std::string> ops1 = {"1 5", "3 5", "2 5", "3 5"};
    assert(processSetOperations(ops1) == std::vector<std::string>({"Yes", "No"}));
    
    // Duplicate insert does nothing
    std::vector<std::string> ops2 = {"1 10", "1 10", "3 10"};
    assert(processSetOperations(ops2) == std::vector<std::string>({"Yes"}));
    
    // Erase non-existent value does nothing
    std::vector<std::string> ops3 = {"2 7", "3 7"};
    assert(processSetOperations(ops3) == std::vector<std::string>({"No"}));
    
    // Negative numbers
    std::vector<std::string> ops4 = {"1 -3", "3 -3", "1 2", "3 2"};
    assert(processSetOperations(ops4) == std::vector<std::string>({"Yes", "Yes"}));
    
    // Multiple queries with mixed operations
    std::vector<std::string> ops5 = {"1 4", "1 9", "3 4", "3 5", "2 4", "3 4", "3 9"};
    assert(processSetOperations(ops5) == std::vector<std::string>({"Yes", "No", "No", "Yes"}));
    
    // Empty operation list
    std::vector<std::string> ops6;
    assert(processSetOperations(ops6).empty());
    
    // Large values
    std::vector<std::string> ops7 = {"1 1000000", "3 1000000", "3 -1000000"};
    assert(processSetOperations(ops7) == std::vector<std::string>({"Yes", "No"}));
    
    // Sequential insert then erase then insert again
    std::vector<std::string> ops8 = {"1 42", "2 42", "3 42", "1 42", "3 42"};
    assert(processSetOperations(ops8) == std::vector<std::string>({"No", "Yes"}));
    
    return 0;
}
