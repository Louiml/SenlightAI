// Write a C++ function that simulates a simple browser history navigation system. The function takes a vector of strings representing the initial browsing history (in order from oldest to newest, where the first element is the current page), followed by a vector of command strings, each of which is one of "visit <page>", "next", or "prev". The "visit" command changes the current page to the given page if it exists in the history; otherwise, it is ignored (and the current page remains unchanged). The "next" command moves to the next page in history if one exists, and "prev" moves to the previous page if one exists. The function should return a vector of strings containing the results of each command: for a successful "visit", "next", or "prev", output the page name that becomes the new current page; for an unsuccessful command (i.e., a "visit" to a non-existent page, or a "next" when already at the last page, or "prev" when already at the first page), output the string "Not Available". The initial current page is always the first element of the history vector. The history is immutable — you are only moving a pointer/iterator within it, not adding or removing pages.

The solution is a direct translation of the provided snippet into a standalone function. We maintain an iterator (or index) pointing to the current page in the history. For each command, we parse the command string: if it starts with "visit ", extract the page name and use `std::find` to locate it in the vector. If found, set the current iterator to that position and output the page; if not found, output "Not Available". For "next", check if the iterator is at the last element (using `std::next` or comparing to `history.end()-1`); if so, output "Not Available", otherwise advance it and output the new page. For "prev", check if the iterator is at the beginning; if so, output "Not Available", otherwise move it back and output the new page. Edge cases include empty history (though the problem guarantees at least one element), commands that are malformed (we assume they are well-formed as per the specification, but we can safely ignore unknown commands), and repeated visits to the same page. Time complexity is O(C * N) in the worst case for `visit` commands (each `find` is O(N), where N is the number of pages) and O(C) for `next`/`prev`, where C is the number of commands. Space complexity is O(C) for the result vector, plus O(1) auxiliary.

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>

// Simulate a browser history navigation system.
// history: list of pages from oldest to newest; index 0 is the current page initially.
// commands: list of commands, each is "visit <page>", "next", or "prev".
// Returns a vector of results for each command.
std::vector<std::string> simulateBrowserHistory(const std::vector<std::string>& history,
                                                const std::vector<std::string>& commands) {
    std::vector<std::string> results;
    if (history.empty()) {
        // If no pages, all commands fail.
        for (const auto& cmd : commands) {
            results.push_back("Not Available");
        }
        return results;
    }

    // Current position pointer into the history vector.
    size_t current = 0;

    for (const auto& cmd : commands) {
        if (cmd.rfind("visit ", 0) == 0) {
            // Extract page name after "visit ".
            std::string page = cmd.substr(6); // length of "visit " is 6
            auto it = std::find(history.begin(), history.end(), page);
            if (it != history.end()) {
                current = static_cast<size_t>(std::distance(history.begin(), it));
                results.push_back(history[current]);
            } else {
                results.push_back("Not Available");
            }
        } else if (cmd == "next") {
            if (current + 1 < history.size()) {
                ++current;
                results.push_back(history[current]);
            } else {
                results.push_back("Not Available");
            }
        } else if (cmd == "prev") {
            if (current > 0) {
                --current;
                results.push_back(history[current]);
            } else {
                results.push_back("Not Available");
            }
        } else {
            // Unknown command (shouldn't happen per spec); ignore or push "Not Available".
            results.push_back("Not Available");
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: Basic navigation
    std::vector<std::string> h1 = {"A", "B", "C"};
    std::vector<std::string> c1 = {"next", "next", "next", "prev", "prev", "prev"};
    std::vector<std::string> r1 = simulateBrowserHistory(h1, c1);
    assert(r1 == std::vector<std::string>({"B", "C", "Not Available", "B", "A", "Not Available"}));

    // Test 2: Visit existing and non-existing pages
    std::vector<std::string> h2 = {"Home", "About", "Contact"};
    std::vector<std::string> c2 = {"visit About", "visit Nonexistent", "visit Home", "next"};
    std::vector<std::string> r2 = simulateBrowserHistory(h2, c2);
    assert(r2 == std::vector<std::string>({"About", "Not Available", "Home", "About"}));

    // Test 3: Single page history
    std::vector<std::string> h3 = {"Only"};
    std::vector<std::string> c3 = {"next", "prev", "visit Only", "visit Other"};
    std::vector<std::string> r3 = simulateBrowserHistory(h3, c3);
    assert(r3 == std::vector<std::string>({"Not Available", "Not Available", "Only", "Not Available"}));

    // Test 4: Duplicate pages in history — visit picks first occurrence
    std::vector<std::string> h4 = {"X", "Y", "X"};
    std::vector<std::string> c4 = {"visit X", "next", "prev"};
    std::vector<std::string> r4 = simulateBrowserHistory(h4, c4);
    assert(r4 == std::vector<std::string>({"X", "Y", "X"}));

    // Test 5: Empty history (edge case)
    std::vector<std::string> h5;
    std::vector<std::string> c5 = {"next", "prev", "visit A"};
    std::vector<std::string> r5 = simulateBrowserHistory(h5, c5);
    assert(r5 == std::vector<std::string>({"Not Available", "Not Available", "Not Available"}));

    // Test 6: Commands after visiting a middle page (nav from there)
    std::vector<std::string> h6 = {"P1", "P2", "P3", "P4"};
    std::vector<std::string> c6 = {"visit P3", "prev", "next", "next", "prev"};
    std::vector<std::string> r6 = simulateBrowserHistory(h6, c6);
    assert(r6 == std::vector<std::string>({"P3", "P2", "P3", "P4", "P3"}));

    return 0;
}
