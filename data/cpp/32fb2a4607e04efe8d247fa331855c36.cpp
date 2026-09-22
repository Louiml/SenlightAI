Given a positive integer `N` (1 ≤ N ≤ 10^9), write a C++ function `findSelfNumbers` that returns a `std::vector<int>` containing all numbers `x` such that `x` is greater than or equal to 0, `x ≤ 81`, and `N - x` is a positive integer whose decimal digit sum equals exactly `x`. The returned vector must be sorted in non-decreasing order. For example, if `N = 100`, the valid values of `x` are 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81 (all except those where `N - x` has a digit sum different from `x`; specifically for N=100, x=0 works because 100-0=100 digit sum=1≠0, so only x from 1 to 18? Let's verify: For x=1, N-x=99, digit sum=18≠1; continue. Correct answer for N=100 is empty? Actually according to the code, for N=100, x=1 gives N-x=99, sum=18 not 1, so none? Wait check x=18: N-x=82, sum=10≠18. So empty vector.) The function should be based on the logic in the provided snippet where it iterates x from 0 to 81 inclusive and checks if digit sum of (N - x) equals x. Edge cases: If `N - x` is 0 or negative, the digit sum is undefined, so treat as false (since the original code uses `int` and a loop that sums digits of a non‑positive value would produce incorrect results). Ensure that the returned vector is sorted. The function must not use global variables and must be self-contained.
The main idea comes from a number theory observation: For any number `N`, the sum of its digits is at most `9*10 = 90` (since N ≤ 10^9, max digit sum is 9*10=90 for 9999999999? Actually 10^9 is 1000000000, so max digit sum is 1 for 1e9, but for numbers up to 10^9-1=999999999, digit sum is 9*9=81). Thus, if we look for `x` such that `N = (N - x) + x` and the digit sum of `N - x` equals `x`, then `x` cannot exceed 81 because the digit sum of `N - x` is at most 81 (since `N - x` ≤ N ≤ 10^9, but actually `N - x` can be up to 10^9, digit sum 1, but worst case for numbers like 999999999, digit sum 81). So we only need to iterate `x` from 0 to 81 inclusive (the code uses `fe(i,81)` meaning i from 0 to 81 inclusive). For each `x`, compute `temp = N - x`. If `temp <= 0`, we skip because the digit sum is not well-defined for non-positive numbers (the original code would compute a negative digit sum, but we must handle it correctly). Otherwise, compute the sum of decimal digits of `temp`. If this sum equals `x`, then `N - x` is a valid number, and we add `N - x` to the result. Note that the original code adds `N - x`, not `x`. So the returned vector contains values of the form `N - x` for all valid `x`. After collecting all, sort the vector and return. Needs to handle edge cases: `N` small such that `N - x` becomes negative; also `x=0` may be valid if `N` has digit sum 0 (only if N=0, but N≥1, so never). Complexity: We iterate at most 82 values, each digit sum computation is O(log10(N)) = O(10), so O(82*10) = O(1) time. Space is O(number of valid x) ≤ 82 integers.
#include <vector>
#include <algorithm>
#include <numeric>

// Returns a sorted vector of all numbers of the form N - x where x in [0,81]
// and the digit sum of (N - x) equals exactly x.
std::vector<int> findSelfNumbers(int N) {
    std::vector<int> result;
    
    for (int x = 0; x <= 81; ++x) {
        int temp = N - x;
        if (temp <= 0) {
            continue;
        }
        
        // Compute digit sum of temp
        int sum = 0;
        int value = temp;
        while (value > 0) {
            sum += value % 10;
            value /= 10;
        }
        
        if (sum == x) {
            result.push_back(temp);
        }
    }
    
    std::sort(result.begin(), result.end());
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // N=1: possible x? x=0 -> temp=1 sum=1 !=0; x=1 -> temp=0 skipped; so empty
    std::vector<int> r1 = findSelfNumbers(1);
    assert(r1.empty());
    
    // N=100: check manually: for x from 0 to 81, we need N-x digit sum = x
    // Since N=100, any x makes N-x between 19 and 100. Digit sum of 100-x? 
    // Test known: x=0 -> 100 sum=1; x=1 -> 99 sum=18; x=2 -> 98 sum=17; ... 
    // No match, so empty.
    std::vector<int> r2 = findSelfNumbers(100);
    assert(r2.empty());
    
    // N=10: x=0 -> temp=10 sum=1; x=1 -> temp=9 sum=9 !=1; x=2 -> temp=8 sum=8 !=2; x=3 -> 7 sum=7 !=3; x=4 ->6 sum=6; x=5->5 sum=5; x=6->4 sum=4; x=7->3 sum=3; x=8->2 sum=2; x=9->1 sum=1 (matches? x=9, temp=1 sum=1 !=9); x=10->0 skipped. Only x=5 gives temp=5 sum=5 equals x? 5==5 yes! So result {5}.
    std::vector<int> r3 = findSelfNumbers(10);
    assert(r3.size() == 1 && r3[0] == 5);
    
    // N=0 is not allowed but test edge? Not needed.
    // N=1000000000 (1e9): x=0 -> temp=1000000000 sum=1; x=1 -> 999999999 sum=81; ... only x=81? temp=999999919 sum=81? 9+9+9+9+9+9+9+1+9 = 73? Actually 999999919 sum = 9*7 +1+9 = 63+10=73? So check: The solution should compute correctly.
    // We'll just test a smaller known pattern: N=20.
    // For N=20, valid x such that digit sum of (20-x) = x:
    // x=1 ->19 sum=10; x=2->18 sum=9; x=3->17 sum=8; x=4->16 sum=7; x=5->15 sum=6; x=6->14 sum=5; x=7->13 sum=4; x=8->12 sum=3; x=9->11 sum=2; x=10->10 sum=1; x=11->9 sum=9 !=11; x=12->8 sum=8 !=12; x=13->7 sum=7; x=14->6 sum=6; x=15->5 sum=5; x=16->4 sum=4; x=17->3 sum=3; x=18->2 sum=2; x=19->1 sum=1; x=20->0 skipped. Only x=15? temp=5 sum=5? Actually x=15 -> temp=5 sum=5, not 15. x=5 -> temp=15 sum=6, not 5. None. So empty? Wait let's compute: digit sum of (20-x) equals x. For x=1, temp=19 sum=10, not 1. x=2, temp=18 sum=9. ... For x=9, temp=11 sum=2. x=10, temp=10 sum=1. x=11, temp=9 sum=9. x=12, temp=8 sum=8. x=13, temp=7 sum=7. x=14, temp=6 sum=6. x=15, temp=5 sum=5. x=16, temp=4 sum=4. x=17, temp=3 sum=3. x=18, temp=2 sum=2. x=19, temp=1 sum=1. So matches at x=5? For x=5, temp=15 sum=6, not 5. At x=15, temp=5 sum=5, yes! So result contains 5? Wait N-x = 20-15=5, so result has 5. Also x=14 gives temp=6 sum=6, not 14. So only x=15 gives temp=5 sum=5, so result {5}. But also x=19 gives temp=1 sum=1, but x=19 !=1. So only {5}? Check x=18 gives temp=2 sum=2, not 18. So yes.
    std::vector<int> r4 = findSelfNumbers(20);
    assert(r4.size() == 1 && r4[0] == 5);
    
    // Another test: N=1000000000, known answer? Let's compute manually: For x=0, temp=1e9 sum=1; x=1, temp=999999999 sum=81; x=2, temp=999999998 sum=80; ... we need digit sum = x. For x=81, temp=999999919 sum = 9*7+1+9 = 63+10=73, not 81. For x=80, temp=999999920 sum=9*7+2+0=63+2=65. No match likely. But we can just test that the function runs without crash and returns a vector (empty or not). We'll do a simple check with a known small N.
    
    // Test N=5: possible x? x=0 -> temp=5 sum=5 !=0; x=1 -> temp=4 sum=4 !=1; x=2->temp=3 sum=3; x=3->temp=2 sum=2; x=4->temp=1 sum=1; x=5->temp=0 skipped. None match? Check x=2: temp=3 sum=3, not 2. x=3: temp=2 sum=2, not 3. x=4: temp=1 sum=1, not 4. So empty.
    std::vector<int> r5 = findSelfNumbers(5);
    assert(r5.empty());
    
    // Test N=19: From earlier, for N=20 we had {5}. For N=19, try x=1 -> temp=18 sum=9; x=9 -> temp=10 sum=1; x=10 -> temp=9 sum=9; x=14 -> temp=5 sum=5; x=15 -> temp=4 sum=4; x=16->temp=3 sum=3; x=17->temp=2 sum=2; x=18->temp=1 sum=1. Only at x=14 gives temp=5 sum=5, not 14. x=9 gives temp=10 sum=1, not 9. None? Check x=10 gives temp=9 sum=9, but x=10 !=9. So empty.
    std::vector<int> r6 = findSelfNumbers(19);
    assert(r6.empty());
    
    // Test N=1000000: We'll just verify size is non-negative and sorted.
    std::vector<int> r7 = findSelfNumbers(1000000);
    assert(std::is_sorted(r7.begin(), r7.end()));
    
    // Test N=1000000000
    std::vector<int> r8 = findSelfNumbers(1000000000);
    assert(std::is_sorted(r8.begin(), r8.end()));
    
    return 0;
}
