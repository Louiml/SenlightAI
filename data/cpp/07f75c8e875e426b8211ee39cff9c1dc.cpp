/*
Implement a standalone C++ function named `heapSimulation` that processes a sequence of heap commands and returns a string capturing the state of a max-heap after each command. The commands are: `+ n` (push integer `n`), `-` (pop the top), and `!` (stop processing). After every command (including the initial empty state before any command), append a line containing the heap's top value (or `"NONE"` if empty), its size, emptiness flag (`"1"` for empty, `"0"` otherwise), and the heap's contents in non-increasing order, separated by spaces. Each block of four outputs (top, size, empty, contents) should be separated by a line containing `"------"`. The function should take no input parameters; instead, it should read commands from standard input until `!` is encountered. The heap is a max-heap, meaning the largest value is always at the top, and when popping, the largest value is removed. Duplicate values are allowed. The output string must be exactly as specified, with each command block ending with a newline. If a `-` command is given when the heap is empty, the pop should be ignored (i.e., no change to the heap). The input is guaranteed to be well-formed (valid commands with appropriate arguments). For example, if the input is `+ 5\n+ 3\n-\n!`, the output should have two blocks: one for the initial empty state, and one after each command, totaling four blocks (initial, after first push, after second push, after pop), each with the appropriate state.
*/

#include <bits/stdc++.h>

// Simulates a max-heap interactive session, returning the full output string.
std::string heapSimulation() {
    std::priority_queue<int> heap;
    std::ostringstream out;

    while (true) {
        // Output current state
        if (heap.empty()) {
            out << "top:   NONE\n";
            out << "size:  0\n";
            out << "empty: 1\n";
            out << "heap:\n";
        } else {
            out << "top:   " << heap.top() << "\n";
            out << "size:  " << heap.size() << "\n";
            out << "empty: 0\n";
            out << "heap:";
            // Copy and pop to get ordered elements
            std::priority_queue<int> temp = heap;
            while (!temp.empty()) {
                out << " " << temp.top();
                temp.pop();
            }
            out << "\n";
        }
        out << "------\n";

        // Read command
        char t;
        std::cin >> t;
        if (t == '!') {
            break;
        } else if (t == '+') {
            int n;
            std::cin >> n;
            heap.push(n);
        } else if (t == '-') {
            if (!heap.empty()) {
                heap.pop();
            }
        }
    }

    return out.str();
}

#include <bits/stdc++.h>

std::string heapSimulation(); // declaration

int main() {
    // Test 1: Simple push and pop
    {
        std::istringstream input("+ 5\n+ 3\n-\n!");
        std::cin.rdbuf(input.rdbuf());
        std::string result = heapSimulation();
        std::string expected = "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n"
                               "top:   5\nsize:  1\nempty: 0\nheap: 5\n------\n"
                               "top:   5\nsize:  2\nempty: 0\nheap: 5 3\n------\n"
                               "top:   3\nsize:  1\nempty: 0\nheap: 3\n------\n";
        assert(result == expected);
    }

    // Test 2: Empty pop ignored
    {
        std::istringstream input("-\n!\n");
        std::cin.rdbuf(input.rdbuf());
        std::string result = heapSimulation();
        std::string expected = "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n"
                               "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n";
        assert(result == expected);
    }

    // Test 3: Duplicates and larger values
    {
        std::istringstream input("+ 10\n+ 10\n+ 5\n\n!\n");
        std::cin.rdbuf(input.rdbuf());
        std::string result = heapSimulation();
        std::string expected = "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n"
                               "top:   10\nsize:  1\nempty: 0\nheap: 10\n------\n"
                               "top:   10\nsize:  2\nempty: 0\nheap: 10 10\n------\n"
                               "top:   10\nsize:  3\nempty: 0\nheap: 10 10 5\n------\n";
        assert(result == expected);
    }

    // Test 4: Negative and zero values
    {
        std::istringstream input("+ -1\n+ 0\n+ -5\n!\n");
        std::cin.rdbuf(input.rdbuf());
        std::string result = heapSimulation();
        std::string expected = "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n"
                               "top:   -1\nsize:  1\nempty: 0\nheap: -1\n------\n"
                               "top:   0\nsize:  2\nempty: 0\nheap: 0 -1\n------\n"
                               "top:   0\nsize:  3\nempty: 0\nheap: 0 -1 -5\n------\n";
        assert(result == expected);
    }

    // Test 5: Alternating pops and pushes
    {
        std::istringstream input("+ 7\n-\n+ 2\n-\n-\n!\n");
        std::cin.rdbuf(input.rdbuf());
        std::string result = heapSimulation();
        std::string expected = "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n"
                               "top:   7\nsize:  1\nempty: 0\nheap: 7\n------\n"
                               "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n"
                               "top:   2\nsize:  1\nempty: 0\nheap: 2\n------\n"
                               "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n"
                               "top:   NONE\nsize:  0\nempty: 1\nheap:\n------\n";
        assert(result == expected);
    }

    return 0;
}

// The core of the solution is to maintain a max-heap data structure. In C++, the standard library provides `std::priority_queue` which implements a max-heap by default. We can use that directly. The function reads from `std::cin` character by character or using a loop that reads a character token and possibly an integer. For each command, we first output the current state (before processing the command) to match the typical interactive behavior displayed in the snippet: the state is printed before reading the next command. So we need a loop that prints the current heap state, then reads a command character. If the command is `+`, we read an integer and push it. If `-`, we pop if not empty. If `!`, we break. Each printed state includes the top (using `empty()` to check), size, empty flag as `1` or `0`, and the contents in non-increasing order. To get the contents in order, we can copy the heap into a temporary vector, then pop all elements to retrieve them in descending order, or we can use `std::priority_queue` doesn't allow iteration, so we need to pop all elements to reconstruct the order. The complexity for printing is O(n log n) for that copy, but since we process all commands, the total time is O(m * n log n) where m is number of commands and n is heap size; but typical competitive problems accept this. Alternatively, we could use a multiset or ordered multiset to get ordered iteration, but priority_queue is fine. Important edge cases: empty heap pop ignored; initial state printed before any command; after `!` we must not print another state (the loop breaks before printing). The output string should accumulate all blocks in order. We'll use a `std::ostringstream` to build the result. Time complexity per command is O(n log n) for printing, but overall we can say O(k * n log n) where k is number of commands and n is maximum heap size; space complexity O(n) for the heap plus O(k * n) for the output string.
