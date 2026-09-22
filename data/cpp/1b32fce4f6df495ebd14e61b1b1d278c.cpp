Write a C++ function named `determineWinner` that takes a vector of integers `x`, a vector of integers `y`, and an integer `maxVal` (the maximum value that can appear in either vector, inclusive). The function should compute the parity (odd/even) of the number of pairs `(i, j)` such that `x[i] ^ y[j]` (bitwise XOR) equals any value that appears in the union of `x` and `y`. If the count is odd, return the string `"Koyomi"`; if even, return `"Karen"`. Both vectors have the same size `n` (1 ≤ n ≤ 2000), and all elements are non-negative integers between 0 and `maxVal` inclusive, where `maxVal` ≤ 2,777,776. Your implementation should avoid nested loops that check membership by brute force (i.e., use an efficient membership data structure). Return the winner name as a `std::string`.
The core idea is to precompute a boolean lookup table of size `maxVal+1` indicating which numbers appear in either input vector. Then, for each pair `(i, j)`, compute `v = x[i] ^ y[j]`. Using the lookup table, check if `v` exists in the union in O(1) time. Count such pairs. The total number of pairs is `n*n` (up to 4 million), which is fine for O(1) membership per pair. Important edge cases: duplicate values in the input vectors should not affect the membership lookup (the table just marks presence). The XOR result can be up to (2^22)-1 (since maxVal ≤ 2,777,776 ≈ 2^21.4), so ensure the lookup table size is at least `maxVal+1` but also consider that XOR of two numbers each ≤ maxVal can be up to `(1 << ceil(log2(maxVal+1))) - 1`. Here maxVal is given, so allocate table size `maxVal+1` but also check if XOR might exceed maxVal. Since maxVal is up to 2,777,776, the next power of two is 4,194,304. The XOR of two numbers ≤ 2,777,776 is at most 4,194,303, but if we only allocate `maxVal+1`, we might miss values above maxVal that are not in the union. However, to be correct, we need to allocate a table that covers the maximum possible XOR value, which is `(1 << (number_of_bits)) - 1` where `number_of_bits = ceil(log2(maxVal+1))`. In practice, the given constraints guarantee that all XOR results that match any member of the union must be ≤ maxVal, but to be safe, we should allocate size `(1 << (ceil(log2(maxVal+1)))` or just allocate `(1 << 22)` = 4,194,304, which is safe for the given constraints. Alternatively, we can compute the bit length from maxVal: `bitLen = 0; while ((1<<bitLen) <= maxVal) ++bitLen;` then size = 1<<bitLen. The time complexity is O(n^2) plus O(maxVal) for initialization, and space O(size).
#include <string>
#include <vector>
#include <cstddef>

// Returns "Koyomi" if the number of pairs (i,j) such that x[i]^y[j]
// appears in the union of x and y is odd, otherwise "Karen".
std::string determineWinner(const std::vector<int>& x, const std::vector<int>& y, int maxVal) {
    // Compute the smallest power of two greater than maxVal to safely include
    // all possible XOR results.
    int tableSize = 1;
    while (tableSize <= maxVal) {
        tableSize <<= 1;
    }
    // If maxVal is 0, tableSize becomes 1, but we need at least 1.
    if (tableSize < 1) tableSize = 1;

    std::vector<bool> present(tableSize, false);
    for (int val : x) {
        present[val] = true;
    }
    for (int val : y) {
        present[val] = true;
    }

    long long pairCount = 0; // Use long long to avoid overflow (n <= 2000, so max 4e6, but safe)
    const std::size_t n = x.size();
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            int xorVal = x[i] ^ y[j];
            if (present[xorVal]) {
                ++pairCount;
            }
        }
    }

    return (pairCount % 2 == 1) ? "Koyomi" : "Karen";
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is defined above or included from a header.
int main() {
    // Example 1 from typical problem: 
    // x = {1,2}, y = {2,3}, maxVal=3
    // Pairs: 1^2=3 (present), 1^3=2 (present), 2^2=0 (not present), 2^3=1 (present) => count=3 (odd) => Koyomi
    {
        std::vector<int> x = {1,2};
        std::vector<int> y = {2,3};
        assert(determineWinner(x,y,3) == "Koyomi");
    }
    // Example 2: x = {0}, y = {0}, maxVal=0
    // 0^0=0 present => count=1 (odd) => Koyomi
    {
        std::vector<int> x = {0};
        std::vector<int> y = {0};
        assert(determineWinner(x,y,0) == "Koyomi");
    }
    // Example 3: x = {1,2,3}, y = {4,5,6}, maxVal=6
    // Compute pairs: check each xor against union {1,2,3,4,5,6}
    // Let's manually count: 
    // 1^4=5 present, 1^5=4 present, 1^6=7 no
    // 2^4=6 present, 2^5=7 no, 2^6=4 present
    // 3^4=7 no, 3^5=6 present, 3^6=5 present
    // Total 5 (odd) => Koyomi
    {
        std::vector<int> x = {1,2,3};
        std::vector<int> y = {4,5,6};
        assert(determineWinner(x,y,6) == "Koyomi");
    }
    // Example 4: x = {1,2}, y = {1,2}, maxVal=2
    // Pairs: 1^1=0 no, 1^2=3 no, 2^1=3 no, 2^2=0 no => count=0 (even) => Karen
    {
        std::vector<int> x = {1,2};
        std::vector<int> y = {1,2};
        assert(determineWinner(x,y,2) == "Karen");
    }
    // Example 5: x = {5,5}, y = {5,5}, maxVal=5
    // 5^5=0 not present (only 5 in union) => count=0 => Karen
    {
        std::vector<int> x = {5,5};
        std::vector<int> y = {5,5};
        assert(determineWinner(x,y,5) == "Karen");
    }
    // Example 6: x = {1}, y = {1}, maxVal=1
    // 1^1=0 not present => count=0 => Karen
    {
        std::vector<int> x = {1};
        std::vector<int> y = {1};
        assert(determineWinner(x,y,1) == "Karen");
    }
    // Example 7: x = {2,3}, y = {0,1}, maxVal=3
    // Union {0,1,2,3}
    // 2^0=2 present, 2^1=3 present, 3^0=3 present, 3^1=2 present => count=4 even => Karen
    {
        std::vector<int> x = {2,3};
        std::vector<int> y = {0,1};
        assert(determineWinner(x,y,3) == "Karen");
    }
    // Example 8: x = {0,1}, y = {2,3}, maxVal=3
    // 0^2=2 present, 0^3=3 present, 1^2=3 present, 1^3=2 present => count=4 even => Karen
    {
        std::vector<int> x = {0,1};
        std::vector<int> y = {2,3};
        assert(determineWinner(x,y,3) == "Karen");
    }
    // Example 9: x = {7}, y = {7}, maxVal=7
    // 7^7=0 not present => count=0 => Karen
    {
        std::vector<int> x = {7};
        std::vector<int> y = {7};
        assert(determineWinner(x,y,7) == "Karen");
    }
    // Example 10: stress with larger but simple case
    // x = {0,1,2,3}, y = {0,1,2,3}, maxVal=3
    // Union {0,1,2,3}
    // Count all pairs (i,j) where x[i]^y[j] in union.
    // Since all numbers are 0..3, XOR results are 0..3. All results are in union. Count = 16 (even) => Karen
    {
        std::vector<int> x = {0,1,2,3};
        std::vector<int> y = {0,1,2,3};
        assert(determineWinner(x,y,3) == "Karen");
    }
    return 0;
}
