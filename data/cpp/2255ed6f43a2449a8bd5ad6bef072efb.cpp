/*
Write a standalone C++ function that takes a `const std::vector<int>&` as input and searches for a user-specified target value, returning a `std::pair<bool, int>` where the first element indicates whether the target was found at least once and the second element is the count of all occurrences of that target in the vector. The function must print each index where the target appears (in ascending order) to standard output, with one index per line, and then either print `"Jumlah data : X"` (where X is the total count) if found, or `"Bilangan tidak ditemukan !"` if not found. The vector may be empty, and the target can appear zero or multiple times. The function must not modify the input vector and should be efficient for large inputs.
*/

#include <vector>
#include <iostream>

// Searches for target in data, prints indices of all matches, and prints summary.
// Returns a pair: {found_any, count_of_occurrences}.
std::pair<bool, int> findAndCountOccurrences(const std::vector<int>& data, int target) {
    bool found = false;
    int count = 0;

    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] == target) {
            found = true;
            std::cout << "Bilangan di temukan di elemen : " << i << std::endl;
            ++count;
        }
    }

    if (found) {
        std::cout << "Jumlah data : " << count << std::endl;
    } else {
        std::cout << "Bilangan tidak ditemukan !" << std::endl;
    }

    return {found, count};
}

#include <cassert>
#include <sstream>
#include <iostream>
#include <vector>

// To test the function, we capture standard output and verify the pair result.
// Helper to capture cout output from a given call.
std::string captureOutput(const std::vector<int>& data, int target, std::pair<bool, int>& result) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    result = findAndCountOccurrences(data, target);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    std::vector<int> v1 = {45, 34, 23, 34, 32, 12, 65, 76, 34, 23};
    std::pair<bool, int> result;

    // Test 1: target found multiple times (34 appears at indices 1,3,8)
    captureOutput(v1, 34, result);
    assert(result.first == true);
    assert(result.second == 3);

    // Test 2: target found once (12 at index 5)
    captureOutput(v1, 12, result);
    assert(result.first == true);
    assert(result.second == 1);

    // Test 3: target not found (99)
    captureOutput(v1, 99, result);
    assert(result.first == false);
    assert(result.second == 0);

    // Test 4: empty vector (nothing found)
    std::vector<int> empty;
    captureOutput(empty, 10, result);
    assert(result.first == false);
    assert(result.second == 0);

    // Test 5: all elements match (vector of 5, target=7)
    std::vector<int> allSame(5, 7);
    captureOutput(allSame, 7, result);
    assert(result.first == true);
    assert(result.second == 5);

    // Test 6: negative numbers
    std::vector<int> neg = {-1, -2, -1, 3};
    captureOutput(neg, -1, result);
    assert(result.first == true);
    assert(result.second == 2);

    // Test 7: large vector with a single match at the end
    std::vector<int> big(1000, 0);
    big[999] = 42;
    captureOutput(big, 42, result);
    assert(result.first == true);
    assert(result.second == 1);

    // Test 8: large vector with no match
    captureOutput(big, 1, result);
    assert(result.first == false);
    assert(result.second == 0);

    // Test 9: single element vector, not found
    captureOutput({5}, 6, result);
    assert(result.first == false);
    assert(result.second == 0);

    // Test 10: duplicate target values but only one appears
    captureOutput({1, 2, 3, 4}, 2, result);
    assert(result.first == true);
    assert(result.second == 1);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution iterates through the input vector exactly once, comparing each element to the target value. For each match, it prints the current index and increments a counter. A boolean flag is set to `true` when the first match occurs. After the loop, the function prints the appropriate final message: if the flag is true, it prints the count; otherwise, it prints the not-found message. The algorithm handles edge cases: an empty vector (automatically not found, prints not-found message), all elements matching, or no matches. Time complexity is O(n) where n is the vector size because we traverse each element once. Space complexity is O(1) auxiliary, as only a few local variables are used, and the input vector is read-only via `const&`.
