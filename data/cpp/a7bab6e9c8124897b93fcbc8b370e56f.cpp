Write a C++ function `findPairWithSum` that takes a constant reference to a vector of integers and a target key, and prints every distinct index pair `(i, j)` with `i < j` such that `arr[i] + arr[j] == key`, each pair in the format `[i,j]` separated by a space. If no such pair exists, print the exact message `Can't find key`. The function must handle arrays with duplicate values, negative numbers, and at least two elements. The output order must follow the nested-loop order: for each `i` from 0 to n-2, for each `j` from i+1 to n-1, printing pairs as they are found. The function returns nothing (`void`) and performs output via `std::cout`.
// The solution uses a brute-force double loop to examine all possible pairs. For each element at index `i`, iterate over every subsequent element at index `j`. If the sum equals the target, print the pair in the required format and set a `found` flag. After all pairs are checked, if the flag remains false, output `Can't find key`. This guarantees the correct order and handles duplicates naturally (each pair of indices is considered exactly once). Edge cases: arrays with fewer than two elements will never produce a pair (though the problem implies at least two elements, the loop handles it safely). Negative numbers and zero are processed correctly by simple addition. Time complexity is O(n²) due to the nested loops, where n is the size of the array. Space complexity is O(1) auxiliary, excluding the input vector itself, as we only use a handful of scalar variables.
#include <vector>
#include <iostream>

// Prints all index pairs where the sum equals the key, or "Can't find key".
void findPairWithSum(const std::vector<int>& arr, int key) {
    bool found = false;
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (arr[i] + arr[j] == key) {
                std::cout << "[" << i << "," << j << "] ";
                found = true;
            }
        }
    }
    if (!found) {
        std::cout << "Can't find key";
    }
}
#include <cassert>
#include <sstream>
#include <vector>

// Declare the function from the solution (included above for test).
void findPairWithSum(const std::vector<int>& arr, int key);

// Helper to capture output of findPairWithSum as a string.
std::string captureOutput(const std::vector<int>& arr, int key) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    findPairWithSum(arr, key);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Basic case with one pair.
    assert(captureOutput({1, 2, 3, 4, 5}, 5) == "[0,3] [1,2] ");
    // Multiple pairs with duplicates.
    assert(captureOutput({2, 2, 2, 2}, 4) == "[0,1] [0,2] [0,3] [1,2] [1,3] [2,3] ");
    // No pair exists.
    assert(captureOutput({1, 2, 3}, 100) == "Can't find key");
    // Negative numbers.
    assert(captureOutput({-1, 0, 1, 2}, 1) == "[0,2] [1,2] ");
    // Two elements only.
    assert(captureOutput({5, 7}, 12) == "[0,1] ");
    assert(captureOutput({5, 7}, 13) == "Can't find key");
    // Key zero with negative and positive.
    assert(captureOutput({-3, 1, 3, -1, 5}, 0) == "[0,2] [1,3] ");
    // Large vector, check first and last pair.
    std::vector<int> big(100, 1);
    // 100 ones, sum 2 → pairs = 100*99/2 = 4950 pairs.
    std::string bigOut = captureOutput(big, 2);
    // Verify it starts with [0,1] and ends with [98,99] plus trailing space.
    assert(bigOut.substr(0, 5) == "[0,1]");
    assert(bigOut.substr(bigOut.size() - 6) == "[98,99] ");
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
