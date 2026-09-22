Write a C++ function that takes a positive integer `k` and an array `S` of `k` distinct integers (already sorted in increasing order) as input, and returns a vector of strings, where each string contains exactly six integers separated by spaces representing one combination of six numbers chosen from `S`. The combinations must be generated in lexicographic order based on the indices in the original array (i.e., the same order produced by six nested loops). If `k` is less than 6, the function should return an empty vector. The function should be named `generateLottoCombinations`, take `const std::vector<int>& S` as its parameter, and return `std::vector<std::string>`.

#include <cassert>
#include <vector>
#include <string>

// (Include the solution function here in a real setting)

int main() {
    // Test case 1: Basic example k=6
    std::vector<int> S1 = {1, 2, 3, 4, 5, 6};
    auto res1 = generateLottoCombinations(S1);
    assert(res1.size() == 1);
    assert(res1[0] == "1 2 3 4 5 6");

    // Test case 2: k=7 produces 7 combinations
    std::vector<int> S2 = {1, 2, 3, 4, 5, 6, 7};
    auto res2 = generateLottoCombinations(S2);
    assert(res2.size() == 7);
    assert(res2[0] == "1 2 3 4 5 6");
    assert(res2[1] == "1 2 3 4 5 7");
    assert(res2[2] == "1 2 3 4 6 7");
    assert(res2[3] == "1 2 3 5 6 7");
    assert(res2[4] == "1 2 4 5 6 7");
    assert(res2[5] == "1 3 4 5 6 7");
    assert(res2[6] == "2 3 4 5 6 7");

    // Test case 3: k=5 returns empty
    std::vector<int> S3 = {10, 20, 30, 40, 50};
    auto res3 = generateLottoCombinations(S3);
    assert(res3.empty());

    // Test case 4: Larger values, k=8 -> C(8,6)=28
    std::vector<int> S4 = {2, 4, 6, 8, 10, 12, 14, 16};
    auto res4 = generateLottoCombinations(S4);
    assert(res4.size() == 28);
    assert(res4.front() == "2 4 6 8 10 12");
    assert(res4.back() == "8 10 12 14 16 2" ? false : res4.back() == "8 10 12 14 16 2"); // just check first and count

    // Test case 5: All numbers negative
    std::vector<int> S5 = {-5, -4, -3, -2, -1, 0};
    auto res5 = generateLottoCombinations(S5);
    assert(res5.size() == 1);
    assert(res5[0] == "-5 -4 -3 -2 -1 0");

    // Test case 6: Check uniqueness of combinations for k=10 (should be 210)
    std::vector<int> S6 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto res6 = generateLottoCombinations(S6);
    assert(res6.size() == 210);
    // Ensure first and last are correct
    assert(res6[0] == "1 2 3 4 5 6");
    assert(res6.back() == "5 6 7 8 9 10");

    // Test case 7: k=12 -> 924 combinations
    std::vector<int> S7 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    auto res7 = generateLottoCombinations(S7);
    assert(res7.size() == 924);
}

#include <vector>
#include <string>
#include <sstream>

// Generates all 6-number combinations from a sorted array S (size k).
// Returns a vector of strings, each "a b c d e f" in increasing index order.
std::vector<std::string> generateLottoCombinations(const std::vector<int>& S) {
    std::vector<std::string> result;
    const int k = static_cast<int>(S.size());
    if (k < 6) {
        return result; // Not enough numbers
    }
    for (int a = 0; a <= k - 6; ++a) {
        for (int b = a + 1; b <= k - 5; ++b) {
            for (int c = b + 1; c <= k - 4; ++c) {
                for (int d = c + 1; d <= k - 3; ++d) {
                    for (int e = d + 1; e <= k - 2; ++e) {
                        for (int f = e + 1; f <= k - 1; ++f) {
                            std::ostringstream oss;
                            oss << S[a] << ' ' << S[b] << ' ' << S[c] << ' '
                                << S[d] << ' ' << S[e] << ' ' << S[f];
                            result.push_back(oss.str());
                        }
                    }
                }
            }
        }
    }
    return result;
}

// The problem is a classic combination generation problem: choose all subsets of size 6 from a set of `k` distinct elements, where the input array is already sorted. The standard approach is to use six nested loops with carefully chosen bounds to ensure indices increase strictly: the first index `a` ranges from 0 to `k-6` (because we need at least 5 more elements after it), the second `b` from `a+1` to `k-5`, and so on until the sixth `f` from `e+1` to `k-1`. This guarantees that every combination is generated exactly once and in lexicographic order. Edge case: if `k < 6`, the loops would not execute (or we can check upfront) and we return an empty vector. Time complexity is \(O(\binom{k}{6} \cdot 6)\), which is proportional to the number of combinations times the constant cost of formatting each string. Space complexity is \(O(\binom{k}{6})\) to store all output strings, plus the temporary string for each combination.
