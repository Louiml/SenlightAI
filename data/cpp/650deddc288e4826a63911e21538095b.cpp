// Write a C++ function named `canMake24` that takes a vector of four integers (each between 1 and 13 inclusive, representing playing cards) and returns `true` if it is possible to use all four numbers exactly once, along with the binary operations addition, subtraction, multiplication, and division (with normal precedence and no extra parentheses beyond the binary pairing), to obtain the value 24. The operations are applied to the numbers in any order, and division by zero is not allowed. The function should use a tolerance of 0.1 to handle floating-point precision when comparing against 24.
The solution simulates all possible ways to combine the four numbers using a recursive backtracking approach. The core idea is: given a list of numbers, if only one remains, check whether it is within 0.1 of 24 (considering floating-point errors). Otherwise, pick any two distinct numbers from the current list, remove them, and compute all possible results from applying each allowed binary operation to those two numbers in both orders (for subtraction and division, both orders matter; for addition and multiplication, order does not matter but computing both is harmless). For each computed result, add it to the remaining numbers, recurse, and if the recursion returns true, propagate success. If no pair and operation yields success, return false. Edge cases include ensuring we never divide by zero (check both operands before division), and allowing a small epsilon in the final comparison to account for binary floating-point arithmetic (e.g., 8 / (3 - 8/3) results in a value very close to 24). Since the input size is always exactly four numbers but the recursion reduces the list size each time, the number of distinct states is limited. The time complexity is bounded by the factorial-like number of permutations of operations and pair choices: for a list of size `n`, we choose two numbers (n choose 2) and try up to 6 operations per pair, then recurse on size n-1. For n=4, the total number of recursive calls is small (on the order of a few hundred), so the algorithm runs in effectively constant time for the fixed input size. Space complexity is O(n) for the recursion stack and the temporary vectors, which for n=4 is negligible.
#include <vector>
#include <cmath>
#include <cstdlib>

// Check if a set of numbers can be combined using +, -, *, / to reach 24.
// Uses a tolerance of 0.1 for floating-point comparisons.
// All numbers are used exactly once.
bool canMake24(std::vector<double>& nums) {
    const double eps = 0.1;
    
    if (nums.size() == 1) {
        return std::abs(24.0 - nums[0]) <= eps;
    }
    
    for (size_t i = 0; i < nums.size(); ++i) {
        for (size_t j = 0; j < nums.size(); ++j) {
            if (i == j) continue;
            
            // Build the list of remaining numbers (excluding i and j)
            std::vector<double> rest;
            for (size_t k = 0; k < nums.size(); ++k) {
                if (k != i && k != j) {
                    rest.push_back(nums[k]);
                }
            }
            
            double a = nums[i];
            double b = nums[j];
            
            // All possible results from combining a and b
            std::vector<double> poss;
            poss.push_back(a + b);
            poss.push_back(a - b);
            poss.push_back(b - a);
            poss.push_back(a * b);
            if (std::abs(b) > 0.0) poss.push_back(a / b);
            if (std::abs(a) > 0.0) poss.push_back(b / a);
            
            for (double val : poss) {
                rest.push_back(val);
                if (canMake24(rest)) {
                    return true;
                }
                rest.pop_back();
            }
        }
    }
    return false;
}

// Public interface: takes a vector of four integers, converts to double, and calls solver.
bool canMake24(const std::vector<int>& cards) {
    std::vector<double> nums;
    nums.reserve(cards.size());
    for (int card : cards) {
        nums.push_back(static_cast<double>(card));
    }
    return canMake24(nums);
}
#include <cassert>
#include <vector>

int main() {
    // Standard solvable case: (8 / (3 - 8/3)) = 24
    assert(canMake24(std::vector<int>{8, 3, 8, 3}) == true);
    // Solvable: (1 * 2 * 3 * 4) = 24
    assert(canMake24(std::vector<int>{1, 2, 3, 4}) == true);
    // Solvable: (7 - 7/7) * 4 = 24
    assert(canMake24(std::vector<int>{7, 7, 7, 4}) == true);
    // Solvable: (5 - 1) * (7 - 1) = 24
    assert(canMake24(std::vector<int>{5, 1, 7, 1}) == true);
    // Solvable: (13 - 10) * (11 - 3) = 24
    assert(canMake24(std::vector<int>{13, 10, 11, 3}) == true);
    // Unsolvable: 1,1,1,1 cannot make 24 (max 1+1+1+1=4, multiplication gives 1)
    assert(canMake24(std::vector<int>{1, 1, 1, 1}) == false);
    // Unsolvable: 1,1,1,13 cannot make 24 (13+1+1+1=16, 13*1*1*1=13)
    assert(canMake24(std::vector<int>{1, 1, 1, 13}) == false);
    // Unsolvable: 9,9,9,9 (9+9+9+9=36, 9*9-9+9=81, etc., no 24)
    assert(canMake24(std::vector<int>{9, 9, 9, 9}) == false);
    // Solvable with division and parentheses: (3 - 3/7) * 7 = 18? Actually (3 - 3/7)*7 = 18, not 24. Use another: (6/(1 - 3/4)) = 24
    assert(canMake24(std::vector<int>{6, 1, 3, 4}) == true);
    // Edge: zero not allowed, only 1-13, but test that function handles duplicates
    assert(canMake24(std::vector<int>{4, 4, 4, 4}) == true); // 4*4+4+4=24
}
