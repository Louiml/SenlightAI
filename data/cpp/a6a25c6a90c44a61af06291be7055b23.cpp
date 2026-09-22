// Write a C++ function that, given a vector of integers where each element is a digit from 0 to 9, returns a vector of all unique 3-digit even numbers that can be formed using the digits exactly as many times as they appear in the input. The 3-digit number must not have a leading zero. The returned vector must be sorted in ascending order. The input vector may contain duplicates, and the function must respect the frequency of each digit (i.e., you cannot use a digit more times than it appears). If no valid 3-digit even number can be formed, return an empty vector.
#include <cassert>
#include <vector>

// The solution function declaration is above, included here for completeness.
std::vector<int> findEvenDigits(const std::vector<int>& digits); // declared in solution

int main() {
    // Basic example from the problem statement
    std::vector<int> digits1 = {2, 1, 4};
    std::vector<int> expected1 = {124, 142, 214, 412};
    assert(findEvenDigits(digits1) == expected1);

    // Duplicate digits allow repeated use up to their count
    std::vector<int> digits2 = {1, 1, 2, 2, 3};
    std::vector<int> expected2 = {112, 122, 132, 212, 222, 232, 312, 322};
    assert(findEvenDigits(digits2) == expected2);

    // No even digit available -> empty result
    std::vector<int> digits3 = {1, 3, 5};
    assert(findEvenDigits(digits3).empty());

    // Leading zero not allowed
    std::vector<int> digits4 = {0, 0, 1};
    // Only possible is 100? No, because need 3 digits: 1,0,0 -> 100 is valid (even, no leading zero)
    std::vector<int> expected4 = {100};
    assert(findEvenDigits(digits4) == expected4);

    // All digits are the same even digit, but need three copies
    std::vector<int> digits5 = {2, 2, 2};
    std::vector<int> expected5 = {222};
    assert(findEvenDigits(digits5) == expected5);

    // More than three copies of a digit, still only one combination per number
    std::vector<int> digits6 = {2, 2, 2, 2};
    std::vector<int> expected6 = {222};
    assert(findEvenDigits(digits6) == expected6);

    // Mixed with zeros and even digits
    std::vector<int> digits7 = {0, 2, 4, 6};
    std::vector<int> expected7 = {204, 206, 240, 246, 260, 264, 402, 406, 420, 426, 460, 462, 602, 604, 620, 624, 640, 642};
    assert(findEvenDigits(digits7) == expected7);

    // Only one digit (cannot form 3-digit)
    std::vector<int> digits8 = {8};
    assert(findEvenDigits(digits8).empty());

    // Large input with many duplicates but limited combinations
    std::vector<int> digits9(10, 0); // ten zeros
    digits9.push_back(5); // one 5, but no even unit? Actually 5 is odd, so no even number possible
    assert(findEvenDigits(digits9).empty());

    // Ensure output is sorted ascending
    std::vector<int> digits10 = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    std::vector<int> result10 = findEvenDigits(digits10);
    for (size_t i = 1; i < result10.size(); ++i) {
        assert(result10[i - 1] < result10[i]);
    }

    return 0;
}
#include <vector>

// Returns all unique 3-digit even numbers that can be formed from the given digits,
// respecting the frequency of each digit. The result is sorted in ascending order.
std::vector<int> findEvenDigits(const std::vector<int>& digits) {
    std::vector<int> freq(10, 0);
    for (int d : digits) {
        if (d >= 0 && d <= 9) {
            ++freq[d];
        }
    }

    std::vector<int> result;
    for (int hundreds = 1; hundreds <= 9; ++hundreds) {
        if (freq[hundreds] == 0) continue;
        --freq[hundreds]; // use one occurrence of hundreds digit

        for (int tens = 0; tens <= 9; ++tens) {
            if (freq[tens] == 0) continue;
            --freq[tens]; // use one occurrence of tens digit

            for (int units = 0; units <= 8; units += 2) {
                if (freq[units] > 0) {
                    result.push_back(hundreds * 100 + tens * 10 + units);
                }
            }

            ++freq[tens]; // restore tens digit
        }

        ++freq[hundreds]; // restore hundreds digit
    }

    return result;
}
// The problem requires generating all possible 3-digit even numbers from a multiset of digits, respecting frequency constraints. The main algorithm uses a frequency array of size 10 to count digit occurrences. Then, iterate over the hundreds digit from 1 to 9 (to avoid leading zeros), the tens digit from 0 to 9, and the units digit from the set {0,2,4,6,8} (to ensure evenness). For each candidate combination, temporarily decrement the frequency counts of the chosen digits to ensure they are available; if all three counts are positive, construct the number and add it to the result. Since we iterate in increasing order of hundreds, tens, then units, the result is naturally sorted. Important edge cases: if a digit appears multiple times, it can be used for multiple positions as long as each position consumes one occurrence; if the same even number can be formed from different digit placements, only one copy should appear in the result, so uniqueness is guaranteed because the iteration only adds each combination once. Time complexity is O(9 * 10 * 5) = O(1) since the loops are fixed at 450 maximum iterations, and space complexity is O(10 + number of results) for the frequency array and output vector, but the output can be up to 450 numbers, so space is O(1) auxiliary plus output size.
