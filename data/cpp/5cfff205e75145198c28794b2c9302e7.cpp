// Write a C++ function named `processBookQueries` that takes a vector of query strings, where each query is either `"1 k"` (push integer k onto a stack), `"2"` (pop the top element if the stack is not empty), or `"3"` (record the top element if the stack is not empty). The function should return a vector of integers containing the results of all `"3"` queries in order. If a `"3"` query is issued while the stack is empty, skip that query and do not add anything to the result. The queries are given as a vector of strings, each already trimmed with no extra spaces (e.g., `"1 5"`, `"2"`, `"3"`). Assume all input values fit in a 32-bit signed integer.

// The solution simulates a stack using `std::stack<int>`. We iterate through each query string:
// - If the query starts with `'1'`, parse the integer after the space (using `stoi(query.substr(2))`) and push it onto the stack.
// - If the query is `"2"`, pop the top element only if the stack is not empty. Ignore the pop if empty.
// - If the query is `"3"`, check if the stack is not empty; if so, push the top value into the result vector.
//
// Edge cases: 
// - Query `"3"` on an empty stack → skip (do not add to result).
// - Query `"2"` on an empty stack → do nothing.
// - Integer values may be negative, positive, or zero — parse with `stoi` handles negatives.
// - There may be multiple `"3"` queries; each must appear in result in the order they appear in input.
//
// Time complexity: O(Q), where Q is the number of queries, since each string operation is O(1) (substr and stoi are proportional to the string length, which is small constant on average). Space complexity: O(Q) in the worst case for the stack and result vector (if all pushes and no pops).

#include <vector>
#include <string>
#include <stack>
#include <cstdlib>

// Process a list of stack queries. Returns the results of all "3" queries.
std::vector<int> processBookQueries(const std::vector<std::string>& queries) {
    std::stack<int> books;
    std::vector<int> results;

    for (const std::string& query : queries) {
        if (query[0] == '1') {
            // Parse the integer after "1 "
            int value = std::stoi(query.substr(2));
            books.push(value);
        } else if (query[0] == '2') {
            if (!books.empty()) {
                books.pop();
            }
        } else if (query[0] == '3') {
            if (!books.empty()) {
                results.push_back(books.top());
            }
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is included above.

int main() {
    // Test 1: Basic push and top queries
    std::vector<std::string> q1 = {"1 5", "3", "1 10", "3", "2", "3"};
    assert(processBookQueries(q1) == std::vector<int>({5, 10, 5}));

    // Test 2: Pop on empty stack does nothing
    std::vector<std::string> q2 = {"2", "3", "1 7", "3"};
    assert(processBookQueries(q2) == std::vector<int>({7}));

    // Test 3: Top on empty stack is skipped
    std::vector<std::string> q3 = {"3", "3"};
    assert(processBookQueries(q3) == std::vector<int>({}));

    // Test 4: Negative and zero values
    std::vector<std::string> q4 = {"1 -3", "3", "1 0", "3", "2", "3"};
    assert(processBookQueries(q4) == std::vector<int>({-3, 0, -3}));

    // Test 5: Multiple pushes and pops, result order preserved
    std::vector<std::string> q5 = {"1 1", "1 2", "3", "2", "3", "2", "3"};
    assert(processBookQueries(q5) == std::vector<int>({2, 1}));

    // Test 6: Large sequence with no final tops
    std::vector<std::string> q6 = {"1 100", "1 200", "2", "2", "3"};
    assert(processBookQueries(q6) == std::vector<int>({}));

    // Test 7: Single query
    std::vector<std::string> q7 = {"1 42", "3"};
    assert(processBookQueries(q7) == std::vector<int>({42}));

    // Test 8: Pop after all pushes
    std::vector<std::string> q8 = {"1 9", "1 8", "2", "3"};
    assert(processBookQueries(q8) == std::vector<int>({9}));

    return 0;
}
