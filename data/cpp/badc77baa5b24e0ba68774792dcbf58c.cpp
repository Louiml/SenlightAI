// Write a C++ function named `sumPairsInRange` that reads exactly 8 pairs of integers (each pair consisting of two integers `a` and `b`), computes the sum `a + b` for each pair, and returns a `std::vector<int>` containing all 8 sums in the order they were read. The function must take no parameters (all input is read from `std::cin` inside the function). The input is guaranteed to contain exactly 16 integers, forming 8 valid pairs, with each integer fitting in a standard `int`. There is no special formatting for output; the function only returns the sums. The solution must not print anything to the console. Handle potential input failures gracefully by treating any unreadable value as 0, but since the input is guaranteed valid, this is just a safety measure.

#include <cassert>
#include <sstream>
#include <vector>

// Forward declaration of the function under test
std::vector<int> sumPairsInRange();

int main() {
    // Redirect stdin to test data
    std::istringstream input("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");
    std::cin.rdbuf(input.rdbuf());
    std::vector<int> result = sumPairsInRange();
    assert(result.size() == 8);
    assert(result[0] == 3);   // 1+2
    assert(result[1] == 7);   // 3+4
    assert(result[2] == 11);  // 5+6
    assert(result[3] == 15);  // 7+8
    assert(result[4] == 19);  // 9+10
    assert(result[5] == 23);  // 11+12
    assert(result[6] == 27);  // 13+14
    assert(result[7] == 31);  // 15+16

    // Test with negative numbers
    std::istringstream input2("-5 5 -10 10 -100 100 -1 1 -2 2 -3 3 -4 4 -6 6");
    std::cin.rdbuf(input2.rdbuf());
    result = sumPairsInRange();
    assert(result[0] == 0);
    assert(result[1] == 0);
    assert(result[2] == 0);
    assert(result[3] == 0);
    assert(result[4] == 0);
    assert(result[5] == 0);
    assert(result[6] == 0);
    assert(result[7] == 0);

    // Test with fewer than 16 integers (should still return 8 sums, missing values as 0)
    std::istringstream input3("1 2 3 4 5 6 7");
    std::cin.rdbuf(input3.rdbuf());
    result = sumPairsInRange();
    assert(result[0] == 3);
    assert(result[1] == 7);
    assert(result[2] == 11);
    assert(result[3] == 0); // missing 4th pair: both zero
    assert(result[4] == 0);
    assert(result[5] == 0);
    assert(result[6] == 0);
    assert(result[7] == 0);

    return 0;
}

#include <vector>
#include <iostream>

// Reads exactly 8 pairs of integers from std::cin and returns their sums in order.
std::vector<int> sumPairsInRange() {
    std::vector<int> sums;
    sums.reserve(8);
    
    for (int i = 0; i < 8; ++i) {
        int a = 0, b = 0;
        if (!(std::cin >> a)) {
            a = 0; // Fallback if reading fails
        }
        if (!(std::cin >> b)) {
            b = 0; // Fallback if reading fails
        }
        sums.push_back(a + b);
    }
    return sums;
}

// The main algorithm is straightforward: use a loop that runs exactly 8 times. In each iteration, read two integers `a` and `b` from `std::cin` using the extraction operator. If reading fails (e.g., due to end-of-file or invalid tokens), set the failed value to 0 to keep the program stable. Compute `a + b` and push the result into a `std::vector<int>`. After the loop, return the vector. Edge cases include: the input may contain extra spaces or newlines (handled automatically by extraction), but the count of integers is fixed; if fewer than 16 integers are provided, the function still processes up to 8 successful pairs and fills missing values with 0 via error handling. Time complexity is O(1) because the number of operations is constant (8 iterations). Space complexity is O(1) for the loop variables plus O(8) for the returned vector, which is effectively constant as well.
