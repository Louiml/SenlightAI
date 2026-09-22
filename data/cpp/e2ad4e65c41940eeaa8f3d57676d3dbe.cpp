// Write a standalone C++ function named `printNumbersUpTo` that takes a positive integer `N` and prints the numbers from 1 to N, each on a new line, using recursion only (no loops allowed). The function must not return any value, but it must be safe for large N (e.g., N up to 1000) in terms of not causing stack overflow due to excessive recursion depth—this implies you must implement the recursion in a tail-recursive-like manner (though C++ doesn't guarantee tail-call optimization, you should structure the recursive call as the last operation and pass the current state as parameters). The function must not use any global or static variables; all state must be passed as parameters. Your solution must also demonstrate proper handling of the base case and ensure that the output exactly matches the expected sequence.

The core idea is to replace the iterative loop in the original snippet with a recursive approach that passes the current number and the limit as parameters. The function `printNumbersUpTo(current, N)` prints `current`, then if `current < N`, it calls itself with `current + 1`. The base case is when `current > N` (or `current == N+1`), at which point the recursion stops. Since the recursive call is the last action (after printing), it's tail recursion, which modern compilers may optimize to a loop, but even without optimization, the recursion depth is O(N), which is acceptable for N ≤ 1000. Edge cases: N must be positive; if N=0, the function should do nothing (though the task specifies positive integer, we can handle 0 gracefully). Time complexity is O(N) because we do one print per number. Space complexity is O(N) in the worst case for the call stack if not optimized, but conceptually O(1) auxiliary if tail-call eliminated. Important: no global variables, no static counters—all state is passed via parameters.

#include <iostream>

// Recursively prints numbers from current up to N, each on a new line.
// Uses tail recursion by passing the current state as parameters.
void printNumbersUpTo(int current, int N) {
    if (current > N) {
        return; // base case: all numbers printed
    }
    std::cout << current << '\n';
    printNumbersUpTo(current + 1, N); // tail-recursive call
}
(Note: The function is free and does not include main.)

#include <cassert>
#include <sstream>
#include <iostream>

// Redirect cout to test output
void test_printNumbersUpTo() {
    // Test N=0 (should print nothing)
    std::ostringstream oss;
    std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
    printNumbersUpTo(1, 0);
    std::cout.rdbuf(old_cout);
    assert(oss.str() == "");

    // Test N=1
    oss.str("");
    oss.clear();
    old_cout = std::cout.rdbuf(oss.rdbuf());
    printNumbersUpTo(1, 1);
    std::cout.rdbuf(old_cout);
    assert(oss.str() == "1\n");

    // Test N=5
    oss.str("");
    oss.clear();
    old_cout = std::cout.rdbuf(oss.rdbuf());
    printNumbersUpTo(1, 5);
    std::cout.rdbuf(old_cout);
    assert(oss.str() == "1\n2\n3\n4\n5\n");

    // Test N=10
    oss.str("");
    oss.clear();
    old_cout = std::cout.rdbuf(oss.rdbuf());
    printNumbersUpTo(1, 10);
    std::cout.rdbuf(old_cout);
    std::string expected;
    for (int i = 1; i <= 10; ++i) expected += std::to_string(i) + "\n";
    assert(oss.str() == expected);

    // Test large N=1000 (basic check that it doesn't crash; we won't compare full string)
    oss.str("");
    oss.clear();
    old_cout = std::cout.rdbuf(oss.rdbuf());
    printNumbersUpTo(1, 1000);
    std::cout.rdbuf(old_cout);
    // Count newlines: should be 1000
    int count = 0;
    for (char c : oss.str()) if (c == '\n') ++count;
    assert(count == 1000);
}

int main() {
    test_printNumbersUpTo();
    return 0;
}
