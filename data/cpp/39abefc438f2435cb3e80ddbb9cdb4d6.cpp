// Write a C++ function named `canAllCarsPark` that determines whether a parking garage can accommodate all the cars that have entered on a given day. The garage is a 4x4 grid where each cell is either an empty spot (`.`) or occupied by one of 9 car types, each labeled with a digit from `1` to `9`. Each car type must be parked in pairs, so every digit that appears on the grid must appear an **even number of times** (and specifically exactly 2, 4, 6, ..., up to 16) in the grid. Additionally, each pair of cars of the same type requires 2 units of parking space per car, meaning that for each digit, the total count of that digit across the grid must be **less than or equal to 2 * n**, where `n` is the number of arrival waves (each wave brings one car of each type, so `2*n` is the maximum allowed spots per type). The function takes an integer `n` (the number of waves) and a 4x4 character matrix as input, and returns `true` if the configuration is valid (i.e., no type exceeds its quota) and `false` otherwise. The grid is guaranteed to be exactly 4 rows and 4 columns, and only contain characters `.` and digits `'1'` to `'9'`.

The core idea is to count the frequency of each digit (`'1'` to `'9'`) in the 4x4 grid. Since the grid has 16 cells, and each cell can hold at most one car, the maximum count for any digit is 16. For each digit from 1 to 9, after scanning the entire grid, if its count exceeds `2 * n`, then that type has more cars than the parking capacity allows, and the configuration is invalid. The function should return `false` immediately if any digit exceeds the limit; otherwise, if all digits are within the limit, return `true`. Edge cases include: if `n` is large enough (e.g., `n >= 8`), then `2*n >= 16`, so any count up to 16 is fine, and the answer is always `true` unless there is some other constraint (none here). If `n` is small, some counts may exceed the limit. Note that the function does not need to check that counts are even or that exactly pairs exist—only that each type does not exceed its allowed quota. The main loop runs over the 16 cells of the grid (constant size), and then a second loop over the 9 digits, so the time complexity is O(16 + 9) = O(1) per call, and space complexity is O(1) for the frequency array.

#include <array>
#include <cstddef>

// Determines if the given 4x4 parking grid can accommodate all car types
// given n arrival waves, where each type may occupy at most 2*n spots.
// Returns true if no car type exceeds its quota, false otherwise.
bool canAllCarsPark(int n, const std::array<std::array<char, 4>, 4>& grid) {
    std::array<int, 10> freq = {0}; // indices 0..9, we use 1..9

    // Count occurrences of each digit in the grid
    for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            char c = grid[i][j];
            if (c != '.') {
                freq[c - '0']++;
            }
        }
    }

    // Check quota for each car type
    for (int digit = 1; digit <= 9; ++digit) {
        if (freq[digit] > 2 * n) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <array>

// The function is declared above; here is the test driver.
int main() {
    // Test 1: n=1, each type at most 2 spots, it's empty so fine
    std::array<std::array<char, 4>, 4> empty_grid = {{
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}}
    }};
    assert(canAllCarsPark(1, empty_grid) == true);

    // Test 2: n=1, one type appears 3 times (more than 2*n=2) -> false
    std::array<std::array<char, 4>, 4> grid3 = {{
        {{'1', '1', '1', '.'}},
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}}
    }};
    assert(canAllCarsPark(1, grid3) == false);

    // Test 3: n=2, one type appears 4 times, equal to 2*n=4 -> true
    std::array<std::array<char, 4>, 4> grid4 = {{
        {{'2', '2', '2', '2'}},
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}}
    }};
    assert(canAllCarsPark(2, grid4) == true);

    // Test 4: n=2, one type appears 5 times (exceeds 4) -> false
    std::array<std::array<char, 4>, 4> grid5 = {{
        {{'3', '3', '3', '3'}},
        {{'3', '.', '.', '.'}},
        {{'.', '.', '.', '.'}},
        {{'.', '.', '.', '.'}}
    }};
    assert(canAllCarsPark(2, grid5) == false);

    // Test 5: n=8 (max waves), any count up to 16 is fine -> true
    std::array<std::array<char, 4>, 4> full_grid = {{
        {{'9', '9', '9', '9'}},
        {{'9', '9', '9', '9'}},
        {{'9', '9', '9', '9'}},
        {{'9', '9', '9', '9'}}
    }};
    assert(canAllCarsPark(8, full_grid) == true);

    // Test 6: n=8, but a type has 16 (max) and n=8 gives 16 allowed -> true
    assert(canAllCarsPark(8, full_grid) == true);

    // Test 7: n=0 (no waves) then 2*n=0, any car present -> false
    assert(canAllCarsPark(0, full_grid) == false);

    // Test 8: n=3, mixed types all within 6 each -> true
    std::array<std::array<char, 4>, 4> mixed = {{
        {{'1', '1', '2', '2'}},
        {{'3', '3', '4', '4'}},
        {{'5', '5', '6', '6'}},
        {{'7', '7', '8', '8'}}
    }};
    assert(canAllCarsPark(3, mixed) == true);

    // Test 9: n=2, all 16 cells filled with '1' exceeds 4 -> false
    std::array<std::array<char, 4>, 4> all_ones = {{
        {{'1', '1', '1', '1'}},
        {{'1', '1', '1', '1'}},
        {{'1', '1', '1', '1'}},
        {{'1', '1', '1', '1'}}
    }};
    assert(canAllCarsPark(2, all_ones) == false);

    // Test 10: n=1, exactly 2 of each type (max allowed) -> true
    std::array<std::array<char, 4>, 4> pairs = {{
        {{'1', '2', '3', '4'}},
        {{'1', '2', '3', '4'}},
        {{'5', '6', '7', '8'}},
        {{'5', '6', '7', '8'}}
    }};
    assert(canAllCarsPark(1, pairs) == true);

    return 0;
}
