Write a C++ function named `topFiveSorted` that reads a sequence of integers from standard input until the special sentinel value `-2` is entered. The function must keep exactly the five smallest distinct values encountered among all integers entered before the sentinel, ignoring all other values. A second sentinel `-1`, when entered, triggers the output of the current fifth-smallest value (if five values have been collected) followed by the five stored values in ascending order, formatted as `{ a, b, c, d, e }` (with no trailing comma). After outputting, the function continues accepting input until `-2` is entered. The function should use robust input validation to reject non-integer input or extra characters, and must return nothing (void). If fewer than five valid distinct integers are collected when `-2` is entered, no output is produced and the function simply returns.

#include <cassert>
#include <vector>
#include <algorithm>

// Helper to call insertIfDistinctAndSmall and return the vector for testing.
// This is a test-only wrapper; the actual solution function is topFiveSorted.
std::vector<int> simulateInsert(const std::vector<int>& input) {
    std::vector<int> vec;
    for (int v : input) {
        if (v == -2) break;
        if (v == -1) continue; // output not tested here
        // Duplicate logic from insertIfDistinctAndSmall
        if (std::find(vec.begin(), vec.end(), v) != vec.end()) continue;
        if (vec.size() < 5) {
            auto pos = std::lower_bound(vec.begin(), vec.end(), v);
            vec.insert(pos, v);
        } else if (v < vec.back()) {
            vec.pop_back();
            auto pos = std::lower_bound(vec.begin(), vec.end(), v);
            vec.insert(pos, v);
        }
    }
    return vec;
}

int main() {
    // Case 1: Fewer than 5 distinct values
    std::vector<int> res1 = simulateInsert({5, 3, 8, -1, -2});
    assert(res1.size() == 3 && res1[0] == 3 && res1[1] == 5 && res1[2] == 8);

    // Case 2: Exactly 5 distinct values in random order
    std::vector<int> res2 = simulateInsert({10, 2, 7, 4, 9, -2});
    assert(res2.size() == 5 && res2[0] == 2 && res2[1] == 4 && res2[2] == 7 && res2[3] == 9 && res2[4] == 10);

    // Case 3: More than 5 distinct values, keeps smallest 5
    std::vector<int> res3 = simulateInsert({5, 1, 3, 2, 4, 8, 0, -2});
    assert(res3.size() == 5 && res3[0] == 0 && res3[1] == 1 && res3[2] == 2 && res3[3] == 3 && res3[4] == 4);

    // Case 4: Duplicates are ignored
    std::vector<int> res4 = simulateInsert({3, 3, 1, 1, 5, -2});
    assert(res4.size() == 3 && res4[0] == 1 && res4[1] == 3 && res4[2] == 5);

    // Case 5: Values larger than existing top five are ignored
    std::vector<int> res5 = simulateInsert({1, 2, 3, 4, 5, 6, 7, -2});
    assert(res5.size() == 5 && res5[0] == 1 && res5[1] == 2 && res5[2] == 3 && res5[3] == 4 && res5[4] == 5);

    // Case 6: Negative numbers and mixed order
    std::vector<int> res6 = simulateInsert({-5, 10, -1, 0, -10, 3, -2});
    assert(res6.size() == 5 && res6[0] == -10 && res6[1] == -5 && res6[2] == -1 && res6[3] == 0 && res6[4] == 3);

    // Case 7: Sentinel -1 does not affect stored values (output not tested)
    std::vector<int> res7 = simulateInsert({4, -1, 2, -1, 9, -2});
    assert(res7.size() == 3 && res7[0] == 2 && res7[1] == 4 && res7[2] == 9);

    // Case 8: All numbers large, only five smallest kept
    std::vector<int> res8 = simulateInsert({100, 50, 75, 25, 60, 90, 30, -2});
    assert(res8.size() == 5 && res8[0] == 25 && res8[1] == 30 && res8[2] == 50 && res8[3] == 60 && res8[4] == 75);

    // Case 9: Empty input (sentinel immediately)
    std::vector<int> res9 = simulateInsert({-2});
    assert(res9.empty());

    // Case 10: Exactly one distinct value
    std::vector<int> res10 = simulateInsert({42, -2});
    assert(res10.size() == 1 && res10[0] == 42);

    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>

// Reads and validates an integer from standard input.
// Rejects invalid input (non-integer or trailing characters) and prompts again.
int readValidatedInt() {
    while (true) {
        std::cout << "Enter an integer: ";
        int value;
        std::cin >> value;

        if (std::cin.fail() || std::cin.peek() != '\n') {
            std::cerr << "Invalid input. Please enter an integer.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            return value;
        }
    }
}

// Inserts value into sorted vector v if it is not already present and
// keeps the vector size at most 5 by discarding the largest element when necessary.
void insertIfDistinctAndSmall(std::vector<int>& v, int value) {
    // Check if value already exists
    if (std::find(v.begin(), v.end(), value) != v.end()) {
        return;
    }

    if (v.size() < 5) {
        // Insert in sorted position
        auto pos = std::lower_bound(v.begin(), v.end(), value);
        v.insert(pos, value);
    } else {
        // Only insert if value is smaller than the current largest (v.back())
        if (value < v.back()) {
            v.pop_back();
            auto pos = std::lower_bound(v.begin(), v.end(), value);
            v.insert(pos, value);
        }
    }
}

// Main function: reads integers until -2, tracks five smallest distinct values,
// and outputs when -1 is entered.
void topFiveSorted() {
    std::vector<int> vec; // Sorted vector of at most 5 distinct integers
    int n = 0;

    while (true) {
        n = readValidatedInt();

        if (n == -2) {
            break; // Terminate processing
        }

        if (n == -1) {
            // Output if five distinct values have been collected
            if (vec.size() == 5) {
                std::cout << "\nFifth smallest: " << vec[4] << "\n";
                std::cout << "Sorted distinct values: { ";
                for (size_t i = 0; i < vec.size(); ++i) {
                    if (i > 0) std::cout << ", ";
                    std::cout << vec[i];
                }
                std::cout << " }\n";
            }
        } else {
            // Process regular integer
            insertIfDistinctAndSmall(vec, n);
        }
    }
}

// The core algorithm maintains a sorted vector of at most five integers. Each new valid integer is checked for duplication against the current vector (to ensure only distinct values are stored). If the vector has fewer than five elements, the new value is inserted in sorted order (using binary search or linear insertion). If the vector already has five elements, the new value is inserted only if it is smaller than the current largest element; in that case, the largest element is removed and the new value is inserted in sorted position. This ensures the vector always holds the five smallest distinct integers seen so far. When `-1` is entered, if the vector size equals five, the function prints the fifth element (the largest stored) and the entire sorted vector in the specified format. Edge cases include: repeated values (ignored), non-numeric input (rejected with an error message and re-prompted), values larger than the current top five (ignored), and sentinel `-2` terminating without output if fewer than five distinct numbers were collected. Time complexity is O(k * n) where n is the number of input integers and k = 5 (constant insertion cost for at most 5 elements), effectively O(n). Space complexity is O(1) since the vector never exceeds 5 elements.
