// Implement a C++ function `simulateBrowser` that takes a vector of commands as input, where each command is a string representing a user action in a web browser simulation. The valid commands are: `"visit <url>"` (visit a new URL, clearing forward history), `"back"` (navigate to the previous page if one exists; otherwise do nothing), `"forward"` (navigate to the next page in forward history if one exists; otherwise do nothing), and `"current"` (return the URL of the current web page). The function should process all commands sequentially and return a vector of strings containing the outputs of all `"current"` commands executed. The browser starts at a blank page represented by the string `"start"`. If a `"back"` or `"forward"` command is impossible (no history in that direction), the current page remains unchanged. The function should handle an arbitrary number of commands, including zero, and must not use any standard stack/container types; instead, use two fixed-size arrays (or dynamic arrays) of strings to implement the back and forward stacks manually, with a maximum history depth of 100 pages. Ensure the function is `const`-correct for its input and that all passed strings are properly handled.
// The solution simulates a browser's history using two stacks: a "back" stack (containing pages visited before the current one, with the immediately previous page on top) and a "forward" stack (containing pages that would be visited on "forward", with the next page on top). The current page is stored separately. Initially, back and forward stacks are empty, and the current page is `"start"`. When a `"visit <url>"` command arrives, we push the current page onto the back stack, set the current page to the new URL, and clear the forward stack (all its items are discarded). When a `"back"` command arrives, if the back stack is not empty, we pop its top into a temporary variable, push the current page onto the forward stack, and set the current page to the popped value. If it is empty, we leave the current page as is. A `"forward"` command does the reverse: if the forward stack is not empty, pop its top, push the current page onto the back stack, and update current. Otherwise, do nothing. A `"current"` command collects the current page into the result vector. Edge cases include visiting a new URL immediately after going back (which clears forward history), attempting to go back or forward repeatedly until reaching boundaries, and sequences with zero commands. The algorithm processes each command in O(1) time (stack operations are constant time) and uses O(H) auxiliary space, where H is the maximum history depth (here capped at 100, but the implementation can use fixed arrays of size 100). The overall time complexity is O(N) where N is the number of commands, and space complexity is O(1) beyond the input and output.
#include <string>
#include <vector>

// Simulate a web browser with back/forward navigation, returning outputs of "current" commands.
// History depth is capped at 100 pages. The browser starts at "start".
std::vector<std::string> simulateBrowser(const std::vector<std::string>& commands) {
    const int MAX_HISTORY = 100;
    std::string backStack[MAX_HISTORY];
    std::string forwardStack[MAX_HISTORY];
    int backSize = 0;
    int forwardSize = 0;

    std::string currentPage = "start";
    std::vector<std::string> results;

    for (const auto& command : commands) {
        if (command == "current") {
            results.push_back(currentPage);
        } else if (command.rfind("visit ", 0) == 0) {
            // Extract URL after "visit "
            std::string newUrl = command.substr(6);
            // Push current page onto back stack
            if (backSize < MAX_HISTORY) {
                backStack[backSize++] = currentPage;
            }
            // Clear forward history
            forwardSize = 0;
            currentPage = newUrl;
        } else if (command == "back") {
            if (backSize > 0) {
                // Move current page to forward stack
                if (forwardSize < MAX_HISTORY) {
                    forwardStack[forwardSize++] = currentPage;
                }
                // Pop from back stack
                currentPage = backStack[--backSize];
            }
            // else do nothing
        } else if (command == "forward") {
            if (forwardSize > 0) {
                // Move current page to back stack
                if (backSize < MAX_HISTORY) {
                    backStack[backSize++] = currentPage;
                }
                // Pop from forward stack
                currentPage = forwardStack[--forwardSize];
            }
            // else do nothing
        }
        // Ignore any other commands (e.g., empty strings)
    }

    return results;
}
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Test 1: Basic navigation
    {
        std::vector<std::string> cmds = {
            "visit a.com", "current", "visit b.com", "current",
            "back", "current", "forward", "current"
        };
        std::vector<std::string> expected = {"a.com", "b.com", "a.com", "b.com"};
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 2: Back and forward boundaries
    {
        std::vector<std::string> cmds = {"back", "current", "forward", "current"};
        std::vector<std::string> expected = {"start", "start"};
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 3: New visit clears forward history
    {
        std::vector<std::string> cmds = {
            "visit a", "visit b", "back", "current",
            "visit c", "current", "forward", "current"
        };
        std::vector<std::string> expected = {"a", "c", "c"};
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 4: Multiple backs and forwards
    {
        std::vector<std::string> cmds = {
            "visit a", "visit b", "visit c",
            "back", "back", "forward", "back",
            "forward", "forward", "back", "current"
        };
        std::vector<std::string> expected = {"c"};
        // After: visit a (current=a, back=[start])
        // visit b (current=b, back=[start,a])
        // visit c (current=c, back=[start,a,b])
        // back -> current=b, forward=[c]
        // back -> current=a, forward=[c,b]
        // forward -> current=b, back=[start,a], forward=[c]
        // back -> current=a, back=[start], forward=[c,b]
        // forward -> current=b, back=[start,a], forward=[c]
        // forward -> current=c, back=[start,a,b], forward=[]
        // back -> current=b, back=[start,a], forward=[c]
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 5: No commands
    {
        std::vector<std::string> cmds;
        assert(simulateBrowser(cmds).empty());
    }

    // Test 6: Visit with spaces in URL
    {
        std::vector<std::string> cmds = {"visit https://example.com/page", "current"};
        std::vector<std::string> expected = {"https://example.com/page"};
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 7: Back after new visit does not clear back stack
    {
        std::vector<std::string> cmds = {
            "visit a", "visit b", "back", "visit c",
            "back", "current", "forward", "current"
        };
        std::vector<std::string> expected = {"a", "c"};
        // After: visit a (a, back=[start])
        // visit b (b, back=[start,a])
        // back (a, back=[start], forward=[b])
        // visit c (c, back=[start,a], forward=[])
        // back (a, back=[start], forward=[c])
        // forward (c, back=[start,a], forward=[])
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 8: Repeated back at start
    {
        std::vector<std::string> cmds = {"back", "back", "current"};
        std::vector<std::string> expected = {"start"};
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 9: Forward without back
    {
        std::vector<std::string> cmds = {"visit a", "forward", "current"};
        std::vector<std::string> expected = {"a"};
        assert(simulateBrowser(cmds) == expected);
    }

    // Test 10: Long sequence
    {
        std::vector<std::string> cmds = {
            "visit x", "visit y", "back", "forward", "back",
            "visit z", "back", "current", "forward", "current"
        };
        std::vector<std::string> expected = {"y", "z"};
        // visit x (x, back=[start])
        // visit y (y, back=[start,x])
        // back (x, back=[start], forward=[y])
        // forward (y, back=[start,x], forward=[])
        // back (x, back=[start], forward=[y])
        // visit z (z, back=[start,x], forward=[])
        // back (x, back=[start], forward=[z])
        // current -> x
        // forward (z, back=[start,x], forward=[])
        // current -> z
        assert(simulateBrowser(cmds) == expected);
    }

    return 0;
}
