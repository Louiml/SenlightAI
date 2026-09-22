// Write a C++ function that processes multiple rounds of triples of integers (each either 0 or 1), where the number of rounds is provided as a positive integer. For each round, the function must determine if the majority of the three values equals 1 (i.e., at least two of the three are 1), and count how many rounds meet that condition. The function should read the total number of rounds from standard input, then for each round read three integers, and finally output the count of rounds where the condition holds. The input format is: first an integer `n` (number of rounds), followed by `3*n` integers (each 0 or 1) where each round consists of three consecutive integers. The function must handle edge cases such as `n = 0` (output 0), invalid values outside {0,1} (you may assume input is valid), and should only count rounds where at least two of the three values are exactly 1.
// The solution reads an integer `n` for the number of rounds. For each round, we read a block of three integers. The condition for counting a round is that the sum of the three integers is at least 2, since only values 0 and 1 are allowed (so sum >= 2 means at least two 1s). This avoids explicit logical disjunctions and works for any order of values. We initialize a counter to 0, then loop exactly `n` times, reading three ints per iteration. For each triple, if the sum is >= 2, increment the counter. After the loop, output the counter. Edge cases: `n = 0` will output 0 with no iterations; if the input contains values outside {0,1}, the sum condition still gives a meaningful result but such cases are unlikely. Time complexity is O(n), since we process each of the 3n integers exactly once, and space complexity is O(1) (only a fixed-size array of 3 elements and a few integer variables). The solution uses `const` where possible, and the core logic can be separated into a free function `countMajorityRounds` that reads from standard input and returns the count, or alternatively the function takes a vector of ints and n as parameters. For the reference solution, we will design a function `countMajorityRounds(int n, const std::vector<int>& values)` that assumes values has size 3*n and returns the count, making it testable.
#include <vector>
#include <cassert>

// Counts the number of rounds (each of three integers) where at least two are 1.
// `n` is the number of rounds; `values` must have size exactly 3*n.
int countMajorityRounds(int n, const std::vector<int>& values) {
    int count = 0;
    for (int round = 0; round < n; ++round) {
        // Sum of the three values in this round.
        int sum = values[round * 3] + values[round * 3 + 1] + values[round * 3 + 2];
        if (sum >= 2) {
            ++count;
        }
    }
    return count;
}
#include <vector>
#include <cassert>

int countMajorityRounds(int n, const std::vector<int>& values);

int main() {
    // n=0 -> no rounds, count=0
    assert(countMajorityRounds(0, {}) == 0);
    // One round: 1 1 0 -> sum=2 -> count
    assert(countMajorityRounds(1, {1, 1, 0}) == 1);
    // One round: 1 0 0 -> sum=1 -> not count
    assert(countMajorityRounds(1, {1, 0, 0}) == 0);
    // Multiple rounds with mixed results
    assert(countMajorityRounds(3, {1, 1, 0, 0, 1, 0, 1, 1, 1}) == 2); // first and third count
    // All zeros
    assert(countMajorityRounds(2, {0, 0, 0, 0, 0, 0}) == 0);
    // All ones
    assert(countMajorityRounds(2, {1, 1, 1, 1, 1, 1}) == 2);
    // Check three rounds where the middle one is exactly sum=2
    assert(countMajorityRounds(3, {1, 1, 1, 0, 1, 1, 1, 0, 0}) == 2); // first and second count
    return 0;
}
