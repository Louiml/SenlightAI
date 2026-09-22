/*
Write a C++ function `countDuplicateSocks(int a, int b, int c, int d)` that takes four integers representing sock colors (where each integer is a color ID) and returns the number of duplicate pairs among them. A pair is considered duplicate when two socks share the same color. The function should count each color only once per pair — for example, if colors are `(1, 1, 1, 2)`, there is exactly one pair of color 1 (two socks), so the result is 1. If `(1, 1, 1, 1)`, there are two pairs (since four socks make two pairs of color 1), so result is 2. The original snippet counts occurrences of matching conditions, but you should instead count the number of complete pairs (floor of frequency divided by 2) for each color. The function must handle any integer values, including negatives and large values, and return an integer result.
*/
#include <unordered_map>

// Count the number of duplicate pairs among four sock colors.
// Each color appears zero or more times; a pair is two socks of the same color.
// Returns the total number of complete pairs (floor(freq/2) per color).
int countDuplicateSocks(int a, int b, int c, int d) {
    std::unordered_map<int, int> freq;
    freq[a]++; freq[b]++; freq[c]++; freq[d]++;
    
    int totalPairs = 0;
    for (const auto& entry : freq) {
        totalPairs += entry.second / 2;
    }
    return totalPairs;
}
#include <cassert>

int main() {
    // No duplicates
    assert(countDuplicateSocks(1, 2, 3, 4) == 0);
    // One pair
    assert(countDuplicateSocks(1, 1, 2, 3) == 1);
    assert(countDuplicateSocks(1, 2, 1, 3) == 1);
    assert(countDuplicateSocks(1, 2, 3, 1) == 1);
    // Two pairs
    assert(countDuplicateSocks(1, 1, 2, 2) == 2);
    // Three of same color → one pair (since floor(3/2)=1)
    assert(countDuplicateSocks(1, 1, 1, 2) == 1);
    // All four same → two pairs
    assert(countDuplicateSocks(7, 7, 7, 7) == 2);
    // Negative values
    assert(countDuplicateSocks(-1, -1, 2, 3) == 1);
    assert(countDuplicateSocks(-5, -5, -5, -5) == 2);
    // Large numbers
    assert(countDuplicateSocks(1000000, 1000000, 2000000, 2000000) == 2);
    return 0;
}
// The solution builds a frequency map (or uses fixed-size counting since only 4 inputs) for the four integers. For each distinct color value, compute `frequency / 2` (integer division) and sum these values to get the total number of pairs. Since we only have 4 inputs, a simple approach is to use a `std::map<int,int>` or `std::unordered_map` to count occurrences. Then iterate through the map and add `count / 2` for each color. Edge cases include all four distinct (no pairs → 0), two equal and two distinct (one pair), two pairs (e.g., 1,1,2,2 → two pairs), all equal (two pairs), and negative values (they are just keys). Time complexity is O(4) for counting and O(k) for summing where k ≤ 4, effectively O(1). Space complexity is O(k) for the map, but since k ≤ 4 it's constant. The function should be `const`-correct (no modification of inputs) and return an `int`.
