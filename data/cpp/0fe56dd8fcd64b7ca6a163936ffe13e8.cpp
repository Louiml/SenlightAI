// Implement a C++ function `firstUniqueInStream` that takes an initial vector of integers and a sequence of operations (each operation is either a query asking for the first unique number in the current multiset, or an addition of a new number). More specifically, write a function `std::vector<int> processOperations(const std::vector<int>& initial, const std::vector<std::string>& ops)` where each string in `ops` is either `"show"` (meaning return the first unique number in the current multiset, or -1 if none exists) or `"add <value>"` (meaning insert that integer into the multiset). The function should return a vector of results in order for every `"show"` operation. Assumptions: values are within 32-bit signed integer range, initially all numbers are given, and operations are valid (well‑formed, with proper spacing). The order of equal values does not matter; uniqueness is based on count (exactly 1 occurrence).

// We maintain a count map (`unordered_map<int,int>`) for the number of occurrences of each integer, and a queue (`std::deque<int>`) that stores every integer ever added (including the initial ones) in the order they were inserted. The queue may contain stale entries (numbers whose count is no longer 1). For `show`, we pop from the front of the queue while the front element’s count in the map is not 1; once we find a front with count 1, we return it. If the queue becomes empty, we return -1. For `add`, we increment its count in the map and push it to the back of the queue. This works because an element becomes “non‑unique” only when its count goes above 1, and once that happens, it will never become unique again (count only increases). Thus, stale front elements can be safely removed. Edge cases: initial empty input (then show returns -1 immediately), repeated additions of the same value, and many `show` operations without additions. Time complexity: each number is pushed once and popped at most once, so amortized O(1) per operation, and building the initial structure is O(N) where N is the size of the initial vector. Space complexity: O(N + M) where M is the number of added elements over all operations, because the queue stores all ever‑seen values.

#include <vector>
#include <string>
#include <unordered_map>
#include <deque>
#include <sstream>

// Processes a stream of operations on a multiset of integers.
// "show" returns the first value that appears exactly once, or -1 if none.
// "add <value>" inserts a new integer.
// Returns the results of all "show" operations in order.
std::vector<int> processOperations(const std::vector<int>& initial,
                                   const std::vector<std::string>& ops) {
    std::unordered_map<int, int> count;
    std::deque<int> order;

    for (int value : initial) {
        ++count[value];
        order.push_back(value);
    }

    std::vector<int> results;

    for (const std::string& op : ops) {
        if (op == "show") {
            while (!order.empty() && count[order.front()] != 1) {
                order.pop_front();
            }
            results.push_back(order.empty() ? -1 : order.front());
        } else {
            // op starts with "add "
            std::istringstream iss(op);
            std::string command;
            int value;
            iss >> command >> value;
            ++count[value];
            order.push_back(value);
        }
    }

    return results;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above.
// Here we test it with various scenarios.

int main() {
    // Basic initial unique check
    {
        std::vector<int> init = {2, 3, 5};
        std::vector<std::string> ops = {"show"};
        assert((processOperations(init, ops) == std::vector<int>{2}));
    }

    // No unique initially -> -1
    {
        std::vector<int> init = {1, 1, 2, 2};
        std::vector<std::string> ops = {"show"};
        assert((processOperations(init, ops) == std::vector<int>{-1}));
    }

    // Adding a new unique makes it first
    {
        std::vector<int> init = {1, 1};
        std::vector<std::string> ops = {"add 5", "show"};
        assert((processOperations(init, ops) == std::vector<int>{5}));
    }

    // Adding a duplicate removes it from unique
    {
        std::vector<int> init = {3, 4};
        std::vector<std::string> ops = {"add 3", "show"};
        assert((processOperations(init, ops) == std::vector<int>{4}));
    }

    // Multiple queries and adds
    {
        std::vector<int> init = {7, 7, 8, 9};
        std::vector<std::string> ops = {"show", "add 10", "show", "add 8", "show"};
        assert((processOperations(init, ops) == std::vector<int>{8, 8, 9}));
    }

    // Empty initial
    {
        std::vector<int> init = {};
        std::vector<std::string> ops = {"show", "add 1", "show"};
        assert((processOperations(init, ops) == std::vector<int>{-1, 1}));
    }

    // All duplicates after adds
    {
        std::vector<int> init = {1, 2};
        std::vector<std::string> ops = {"add 1", "add 2", "show"};
        assert((processOperations(init, ops) == std::vector<int>{-1}));
    }

    // Large sequence: first unique remains after many adds
    {
        std::vector<int> init = {5};
        std::vector<std::string> ops = {"add 6", "add 7", "show", "add 5", "show"};
        assert((processOperations(init, ops) == std::vector<int>{6, 6}));
    }

    // Negative numbers
    {
        std::vector<int> init = {-1, -1, -2};
        std::vector<std::string> ops = {"show"};
        assert((processOperations(init, ops) == std::vector<int>{-2}));
    }

    // Zero handling
    {
        std::vector<int> init = {0, 1, 0};
        std::vector<std::string> ops = {"show"};
        assert((processOperations(init, ops) == std::vector<int>{1}));
    }

    return 0;
}
