/*
Write a C++ function `findLotoCombination` that takes a vector of distinct positive integers `numbers` and a target sum `target`, and returns a `vector<int>` containing exactly six integers from `numbers` (repetitions allowed) whose sum equals `target`. The six integers must be arranged as two groups of three: the first three are the first found triple in ascending index order (with indices `i <= j <= k`), and the last three are the triple stored for the complementary sum. If no such combination exists, return an empty vector. The function must run efficiently for up to 100 numbers and target up to 1,000,000, and must handle cases where multiples of the same number can be used. The order of the first triple must follow the original input order when scanning triples in nested loops (i from 0 to n-1, j from i to n-1, k from j to n-1). For the second triple, output the stored order from the map. If multiple solutions exist, only the first one found is returned.
*/
#include <vector>
#include <unordered_map>
using namespace std;

// Given a vector of positive integers and a target sum, return six integers
// (as two triples) whose sum equals target, or an empty vector if none.
// Repetition of numbers is allowed. The first triple is the first found in
// index order (i <= j <= k), and the second triple is the stored complement.
vector<int> findLotoCombination(const vector<int>& numbers, int target) {
    int n = static_cast<int>(numbers.size());
    unordered_map<int, vector<int>> sumToTriple;

    // First pass: store all possible sums of three numbers (with repetition
    // allowed via indices i <= j <= k) and the corresponding triple.
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            for (int k = j; k < n; ++k) {
                int sum = numbers[i] + numbers[j] + numbers[k];
                // Only store the first occurrence for each sum to preserve order.
                if (sumToTriple.find(sum) == sumToTriple.end()) {
                    sumToTriple[sum] = {numbers[i], numbers[j], numbers[k]};
                }
            }
        }
    }

    // Second pass: for each triple, check if complement exists in the map.
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            for (int k = j; k < n; ++k) {
                int firstSum = numbers[i] + numbers[j] + numbers[k];
                int complement = target - firstSum;
                auto it = sumToTriple.find(complement);
                if (it != sumToTriple.end()) {
                    // Build result: first triple + stored complement triple.
                    vector<int> result = {numbers[i], numbers[j], numbers[k]};
                    result.insert(result.end(), it->second.begin(), it->second.end());
                    return result;
                }
            }
        }
    }

    return {};
}
#include <cassert>
#include <vector>
using namespace std;

// Declare the function to test (actual definition would be linked separately)
vector<int> findLotoCombination(const vector<int>& numbers, int target);

int main() {
    // Basic case: 1+2+3 = 6 and complement 1+2+3=6 gives total 12
    vector<int> res1 = findLotoCombination({1,2,3}, 12);
    assert(res1.size() == 6);
    int sum1 = 0;
    for (int x : res1) sum1 += x;
    assert(sum1 == 12);

    // Case with repetition: 2+2+2=6 and 2+2+2=6 total 12
    vector<int> res2 = findLotoCombination({2}, 12);
    assert(res2.size() == 6);
    assert(res2 == vector<int>({2,2,2,2,2,2}));

    // No solution: target too small
    vector<int> res3 = findLotoCombination({1,5,10}, 2);
    assert(res3.empty());

    // Larger test with distinct numbers
    vector<int> res4 = findLotoCombination({1,2,3,4,5}, 18);
    assert(res4.size() == 6);
    int sum4 = 0;
    for (int x : res4) sum4 += x;
    assert(sum4 == 18);

    // First triple must respect order: for target 6 with numbers {1,2,3},
    // 1+1+1=3 and 1+1+1=3 total 6, but target 6, check another.
    // Here target 9: 1+2+3=6 complement 1+1+1=3 gives 9
    vector<int> res5 = findLotoCombination({1,2,3}, 9);
    assert(res5.size() == 6);
    // First triple should be 1,1,1 (indices 0,0,0) then complement triple 1,2,3
    // Actually first triple loop: i=0,j=0,k=0 sum=3 complement=6 -> exists 1,2,3
    assert(res5[0] == 1 && res5[1] == 1 && res5[2] == 1);
    assert(res5[3] == 1 && res5[4] == 2 && res5[5] == 3);

    return 0;
}
// The solution uses a hash map to store every possible sum of three input numbers (with indices `i <= j <= k`) as key, and the actual triple as value. Then, for each triple again in the same order, compute the complement `s2 = target - (nr[i]+nr[j]+nr[k])` and check if `s2` exists in the map. If found, return the current triple followed by the stored triple. This ensures that the first solution found (in lexicographic order of indices of the first triple) is returned. Important edge cases: all numbers are positive, so sums grow; if target is smaller than the smallest triple sum, no solution; if the same number repeated is used, indices are not tied to uniqueness, only values. Because the map stores only the first triple for each sum (due to overwriting but with same iteration order), the stored triple is always the earliest encountered for that sum. Time complexity is O(n^3) for the triple loops, with each map operation O(1) average, so O(n^3) overall. Space complexity is O(n^3) in worst case for the map storing up to that many distinct sums (though in practice bounded by distinct sums). For n=100, worst-case triple count is about 171,700, which is acceptable.
