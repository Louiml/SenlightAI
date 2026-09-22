Write a C++ function named `printAscendingAndDescending` that takes a single positive integer `n` and returns a `std::string` containing two lines: the first line lists the integers from 1 to `n` in ascending order, each followed by a tab character (`'\t'`), and the second line lists them in descending order, also each followed by a tab. The two lines must be separated by a newline character (`'\n'`). For example, if `n = 3`, the returned string should be `"1\t2\t3\t\n3\t2\t1\t\n"` (note the trailing tab after the last number on each line). The function must use recursion (no loops) for both sequences. You may assume `n ≥ 1`. The function should be declared in a header‑only style with appropriate `const` correctness where applicable.
// The solution requires constructing a string recursively without using iterative loops. For the ascending sequence, we can define a helper that recursively builds the string from 1 to `n`: the base case is when the current number equals `n`, in which case we append that number and a tab. For a general call with value `k`, we first recursively call with `k+1` (or build from `k+1` to `n`), then append `k` and a tab — but this would produce descending order. Instead, we need to build ascending order: we can either pass a starting value and use a helper that appends the current value, then recurses with `current+1` until `n`. Alternatively, we can first recursively obtain the string for `n-1` and then append `n` to the end. Similarly, for descending, we can use a helper that appends the current value, then recurses with `current-1` until 1. The main function `printAscendingAndDescending(n)` calls two recursive helpers and concatenates their results with a newline. Edge cases: `n=1` produces the string `"1\t\n1\t\n"`. Time complexity is O(n) because each number from 1 to `n` is processed exactly once in each helper. Space complexity is O(n) due to the recursive call stack depth (n) plus the storage of the growing string (which is also O(n) in total, but the string construction uses additional memory for intermediate results; however, we can build it in a way that accumulates efficiently). We must ensure the function is self-contained, uses no loops, and returns the exact format with trailing tabs.
#include <string>

// Recursive helper that appends numbers from current up to limit in ascending order.
void buildAscending(std::string& result, int current, const int limit) {
    result += std::to_string(current);
    result += '\t';
    if (current < limit) {
        buildAscending(result, current + 1, limit);
    }
}

// Recursive helper that appends numbers from current down to 1 in descending order.
void buildDescending(std::string& result, int current) {
    result += std::to_string(current);
    result += '\t';
    if (current > 1) {
        buildDescending(result, current - 1);
    }
}

// Returns two lines: 1..n (ascending, tab-separated) and n..1 (descending, tab-separated).
std::string printAscendingAndDescending(const int n) {
    std::string result;
    buildAscending(result, 1, n);
    result += '\n';
    buildDescending(result, n);
    return result;
}
#include <cassert>
#include <string>

// Include the solution function here or via header.
// For brevity, the function definitions are assumed above.

int main() {
    assert(printAscendingAndDescending(1) == "1\t\n1\t\n");
    assert(printAscendingAndDescending(2) == "1\t2\t\n2\t1\t\n");
    assert(printAscendingAndDescending(3) == "1\t2\t3\t\n3\t2\t1\t\n");
    assert(printAscendingAndDescending(4) == "1\t2\t3\t4\t\n4\t3\t2\t1\t\n");
    assert(printAscendingAndDescending(5) == "1\t2\t3\t4\t5\t\n5\t4\t3\t2\t1\t\n");
    
    // Check that the first line is ascending and second descending for n=10.
    std::string s = printAscendingAndDescending(10);
    std::string expected = "1\t2\t3\t4\t5\t6\t7\t8\t9\t10\t\n10\t9\t8\t7\t6\t5\t4\t3\t2\t1\t\n";
    assert(s == expected);
    
    // Large value to ensure recursion works (depth ~50 is fine).
    std::string large = printAscendingAndDescending(50);
    assert(large.substr(0, 3) == "1\t2");
    assert(large.find("50\t\n50\t49") != std::string::npos);
    
    // Verify no loops are used: we can't test that directly, but we can ensure the format is correct.
    // Check that newline occurs exactly once.
    int newlines = 0;
    for (char c : large) {
        if (c == '\n') ++newlines;
    }
    assert(newlines == 1);
    
    return 0;
}
