Given a list of integers where each number is in the range [0, 2^18) and a sentinel value -1 terminates input, write a C++ function that, for each valid input number, counts how many other numbers in the set (input list) are numerically greater than it AND differ from it by exactly one or exactly two bits in their binary representation (using 18-bit representation, i.e., bits indexed 0 through 17). The function should take a vector of integers (the numbers before the -1 sentinel) and return a vector of strings in the format "num:count" in the same order as the input numbers, where count is the number of such strictly greater numbers reachable by flipping exactly one bit or exactly two bits.

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (countBitFlips). This is just the test main.

int main() {
    // Example: numbers 0, 1, 3, 7
    // 0 diff by 1 bit: 1 (diff at bit0) -> yes. diff by 2 bits: from 0, candidates 3 (bits0,1) -> 3>0 yes, also 5,6 etc but only 3 is in set. So ways=2.
    // 1 diff by 1 bit: 3 (bit1) -> yes. 1 diff by 2 bits: from 1, candidates: 7 (bits1,2) -> 7>1 yes; also 5,2 etc not in set. So ways=2.
    // 3 diff by 1 bit: 7 (bit2) -> yes. 3 diff by 2 bits: from 3, candidates: 0 (bits0,1) but 0<3; 1 (bits0,1?) actually 3^1^2? Let's compute: 3^1=2, ^2=0; but we need candidate>3, so none. So ways=1.
    // 7 diff by 1 bit: any candidate would be <7 or not in set, none. diff by 2 bits: all candidates are less than 7, so ways=0.

    std::vector<int> input = {0, 1, 3, 7};
    std::vector<std::string> res = countBitFlips(input);
    assert(res.size() == 4);
    assert(res[0] == "0:2");
    assert(res[1] == "1:2");
    assert(res[2] == "3:1");
    assert(res[3] == "7:0");

    // Duplicates: same number appears twice, should be counted independently.
    std::vector<int> dup = {2, 2, 6};
    // 2 (binary 10): 1-bit flip: 0 (not in set), 3 (not), 6 (bit2? 2^4=6) -> 6 in set and >2, so +1. 2-bit flips: from 2, candidates: 2^1^2=1 (not), 2^1^4=7 (not), 2^2^4=0 (not). So ways=1. Same for second 2.
    // 6 (110): 1-bit flip: 2 (bit2 flip) -> 2<6 so ignore; 4 (bit1) not in set; 7 (bit0) not. 2-bit flips: 6^4^2=0 (not), 6^4^1=3 (not), 6^2^1=5 (not). So ways=0.
    std::vector<std::string> res2 = countBitFlips(dup);
    assert(res2 == std::vector<std::string>({"2:1", "2:1", "6:0"}));

    // Empty input
    std::vector<int> empty;
    assert(countBitFlips(empty).empty());

    // Large set: only one number, should return 0.
    std::vector<int> single = {5};
    assert(countBitFlips(single) == std::vector<std::string>({"5:0"}));

    // All numbers 0..7 (8 numbers), check a known case: 0 should have 1-bit flips: 1,2,4 (3) plus 2-bit flips: 3,5,6 (3) total 6. But 7 is 3-bit flips, so not counted. So ways=6.
    std::vector<int> allSmall;
    for (int i = 0; i < 8; ++i) allSmall.push_back(i);
    auto res3 = countBitFlips(allSmall);
    assert(res3[0] == "0:6");
    assert(res3[7] == "7:0");

    return 0;
}

#include <vector>
#include <string>
#include <bitset>

// Count, for each number, how many strictly greater numbers in the set differ by exactly 1 or 2 bits.
std::vector<std::string> countBitFlips(const std::vector<int>& numbers) {
    constexpr int MAX_BITS = 18;
    constexpr int TABLE_SIZE = 1 << MAX_BITS; // 262144

    std::vector<bool> seen(TABLE_SIZE, false);
    for (int num : numbers) {
        seen[num] = true;
    }

    std::vector<std::string> result;
    result.reserve(numbers.size());

    for (int num : numbers) {
        int ways = 0;

        // Flip exactly one bit
        for (int i = 0; i < MAX_BITS; ++i) {
            int candidate = num ^ (1 << i);
            if (candidate > num && seen[candidate]) {
                ++ways;
            }
        }

        // Flip exactly two bits (unordered pairs)
        for (int i = 0; i < MAX_BITS; ++i) {
            for (int j = 0; j < i; ++j) {
                int candidate = num ^ (1 << i) ^ (1 << j);
                if (candidate > num && seen[candidate]) {
                    ++ways;
                }
            }
        }

        result.push_back(std::to_string(num) + ":" + std::to_string(ways));
    }

    return result;
}

// The core idea is to precompute a boolean lookup table (`seen`) of size 2^18 (since numbers are up to 2^18 - 1) indicating which numbers appear in the input set. For each input number `num`, we need to count how many numbers `val` in the set satisfy: `val > num` and `val` differs from `num` in exactly 1 or exactly 2 bit positions.  
// - For one-bit flips: iterate over all 18 bit positions, compute `candidate = num ^ (1 << i)`. If `candidate > num` and `seen[candidate]`, increment the count.  
// - For two-bit flips: iterate over all unordered pairs of bit positions (i > j), compute `candidate = num ^ (1 << i) ^ (1 << j)`. If `candidate > num` and `seen[candidate]`, increment the count.  
// Edge case: duplicates in input are fine; they are independent entries, and each will be processed separately. The sentinel -1 is not included. Empty input (no numbers) should return an empty vector.  
// Time complexity: There are at most 18 + C(18,2) = 18 + 153 = 171 candidates checked per number. If there are `n` input numbers, total time is O(n * 171) = O(n), constant factor. Space complexity is O(2^18) for the lookup table plus O(n) for the output, which is acceptable.
