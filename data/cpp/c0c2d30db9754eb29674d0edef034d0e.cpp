// Write a C++ function named `classifyNumbers` that takes a single integer parameter `count` (the number of values to process) and reads exactly `count` integers from standard input (one per line or space-separated). The function should return a `std::tuple<int, int, int>` where the three integers represent the counts of positive, negative, and zero (neutral) numbers, respectively, in the order (positive, negative, zero). Assume the input is well-formed and contains at least one integer. The function must handle large counts efficiently and should not modify any external state. The function signature should be `std::tuple<int, int, int> classifyNumbers(int count);`. Note: The original snippet contains a bug (`else if(num=0)` uses assignment, not comparison) – your implementation must correctly count zeros using `==`.

#include <cassert>
#include <tuple>
#include <sstream>
#include <iostream>

// The function under test (included here for self-containment)
std::tuple<int, int, int> classifyNumbers(int count) {
    int positiveCount = 0;
    int negativeCount = 0;
    int zeroCount = 0;

    for (int i = 0; i < count; ++i) {
        int num;
        std::cin >> num;

        if (num > 0) {
            ++positiveCount;
        } else if (num < 0) {
            ++negativeCount;
        } else {
            ++zeroCount;
        }
    }

    return std::make_tuple(positiveCount, negativeCount, zeroCount);
}

int main() {
    // Test 1: Mixed values
    {
        std::istringstream input("5 -3 0 7 0 2");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(6);
        assert(std::get<0>(result) == 3);  // positives: 5, 7, 2
        assert(std::get<1>(result) == 1);  // negatives: -3
        assert(std::get<2>(result) == 2);  // zeros: 0, 0
    }

    // Test 2: All positives
    {
        std::istringstream input("1 2 3 4");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(4);
        assert(std::get<0>(result) == 4);
        assert(std::get<1>(result) == 0);
        assert(std::get<2>(result) == 0);
    }

    // Test 3: All negatives
    {
        std::istringstream input("-1 -2 -3");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(3);
        assert(std::get<0>(result) == 0);
        assert(std::get<1>(result) == 3);
        assert(std::get<2>(result) == 0);
    }

    // Test 4: All zeros
    {
        std::istringstream input("0 0 0 0");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(4);
        assert(std::get<0>(result) == 0);
        assert(std::get<1>(result) == 0);
        assert(std::get<2>(result) == 4);
    }

    // Test 5: Single positive
    {
        std::istringstream input("42");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(1);
        assert(std::get<0>(result) == 1);
        assert(std::get<1>(result) == 0);
        assert(std::get<2>(result) == 0);
    }

    // Test 6: Single negative
    {
        std::istringstream input("-7");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(1);
        assert(std::get<0>(result) == 0);
        assert(std::get<1>(result) == 1);
        assert(std::get<2>(result) == 0);
    }

    // Test 7: Single zero
    {
        std::istringstream input("0");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(1);
        assert(std::get<0>(result) == 0);
        assert(std::get<1>(result) == 0);
        assert(std::get<2>(result) == 1);
    }

    // Test 8: Boundary values (INT_MAX, INT_MIN, 0)
    {
        std::istringstream input("2147483647 -2147483648 0");
        std::cin.rdbuf(input.rdbuf());
        auto result = classifyNumbers(3);
        assert(std::get<0>(result) == 1); // INT_MAX
        assert(std::get<1>(result) == 1); // INT_MIN
        assert(std::get<2>(result) == 1); // 0
    }

    std::cout << "All tests passed.\n";
    return 0;
}

#include <tuple>
#include <iostream>

// Reads 'count' integers from standard input and returns a tuple of
// (positive count, negative count, zero count).
std::tuple<int, int, int> classifyNumbers(int count) {
    int positiveCount = 0;
    int negativeCount = 0;
    int zeroCount = 0;

    for (int i = 0; i < count; ++i) {
        int num;
        std::cin >> num;

        if (num > 0) {
            ++positiveCount;
        } else if (num < 0) {
            ++negativeCount;
        } else {
            ++zeroCount;
        }
    }

    return std::make_tuple(positiveCount, negativeCount, zeroCount);
}

// The approach is straightforward: initialize three counters (`positiveCount`, `negativeCount`, `zeroCount`) to zero. Loop exactly `count` times, reading an integer from `std::cin` into a temporary variable each iteration. For each integer, use an `if-else if-else` chain to compare the value against zero: if greater than zero, increment positive counter; else if less than zero, increment negative counter; else (which only happens when the value is exactly zero) increment the zero counter. Key edge cases: when `count` is zero (though the task says at least one integer, still safe to handle), the loop runs zero times and returns all zeros. Also, integers can be as large as the `int` range, but the counters themselves could theoretically overflow if `count` is extremely large; in practice, for a reasonable `count`, `int` suffices, but for robustness one could use `long long` – however, the task asks for `int` return type, so we assume `count` is modest. Time complexity is O(count) because we process each input exactly once. Space complexity is O(1) since we only use a few scalar variables.
