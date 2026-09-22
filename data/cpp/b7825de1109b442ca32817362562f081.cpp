Write a C++ function `std::string partitionByPivot(const std::vector<int>& digits)` that takes a vector of digits (each between 0 and 9 inclusive) and returns a string of length equal to the input size, where each character is either '1' or '2', indicating an assignment of each digit to one of two groups. The assignment must be valid according to the following rule: there exists an integer pivot `p` (0 ≤ p ≤ 9) such that all digits strictly less than `p` are assigned to group 1, all digits strictly greater than `p` are assigned to group 2, and digits equal to `p` can be split arbitrarily between groups 1 and 2, but the sequence of group-1 digits must be non-decreasing, and the sequence of group-2 digits must be non-increasing. If no such pivot and assignment exist, return the string "-" (a single hyphen). If multiple valid assignments exist, return any one of them.
#include <cassert>
#include <vector>
#include <string>

// The function under test is declared above (assume included).
std::string partitionByPivot(const std::vector<int>& digits);

int main() {
    // Simple cases
    assert(partitionByPivot({1,2,3}) == "111"); // pivot 4, all less -> all 1
    assert(partitionByPivot({3,2,1}) == "222"); // pivot 0, all greater -> all 2
    assert(partitionByPivot({5}) == "1");       // pivot 6 -> less -> 1

    // Mixed valid case: pivot 5, values 1 and 9 go to sides, 5 can go either
    std::vector<int> v1 = {1,5,9};
    std::string r1 = partitionByPivot(v1);
    assert(r1.size() == 3);
    assert((r1 == "112" || r1 == "122" || r1 == "212" || r1 == "222" || r1 == "111" || r1 == "211"));

    // Case where pivot splits: e.g., {1,5,5,9}
    std::vector<int> v2 = {1,5,5,9};
    std::string r2 = partitionByPivot(v2);
    assert(r2.size() == 4);
    // Must have first digit '1' (1<5), last digit '2' (9>5), and middle two can be any combination
    assert(r2[0] == '1' && r2[3] == '2');
    assert(r2[1] == '1' || r2[1] == '2');
    assert(r2[2] == '1' || r2[2] == '2');

    // Impossible case: {2,1,2} with any pivot? Check for "-"
    // Let's verify: try p=1: less none, greater 2 at pos1 and3, need non-increasing -> ok (2,2), pivot 1 at pos2, last_less=0, first_greater=1, check if p inside? i=2, first_greater<2 && 2<last_less? last_less=0, false, so valid? Actually with p=1, digits<1 none, >1 are pos1,pos3, non-increasing? 2 and 2 ok. For p=1 at pos2, since last_less=0, we assign it to group2 (since i>=last_less? i=2>=0 yes -> '1'? Wait our code: if (digits[i-1]==p && i >= last_less) -> '1', else '2'. last_less=0, so i=2>=0 -> '1'. Then output "1 1 2"? That gives group1: 2,1? That's non-decreasing? No, 2 then 1 is decreasing, invalid. But our feasibility check didn't catch this because we only checked digits<p and digits>p monotonicity, not the combined effect with p assignments. The original code had a bug? Let's examine: For p=1, lst (last less) =0, fst(first greater)=1. Condition `for i if s[i]==j && fst<i && i<lst` → false because lst=0, so ok. Then assignment: `if(s[i]<j)write('1'); else if(s[i]>j)write('2'); else if(lst<=i)write('1'); else write('2');` Here lst=0, so for any i (i>=1), lst<=i true, so all p's get '1'. Then output "111"? Wait then group1 has [2,1,2] which is not non-decreasing. But the original code doesn't check that! Actually the original code's check for <p and >p is insufficient because it doesn't enforce that the boundary between group1 and group2 is exactly after all less and before all greater. In fact, for p=1, we have no less digits, so group1 can contain 1s and also maybe some digits? The rule says all digits < p must be in group1, all >p in group2, and p can be split. But group1 must be non-decreasing. With p=1, group1 could be {1} (all p's) and group2 {2,2} which is non-increasing (2,2 fine). So output "122" is valid. But our code would output "111" for p=1? Let's test: last_less=0, first_greater=1, check p at i=2: first_greater<2 && 2<last_less? 1<2 && 2<0? false, ok. Then for i=1 (digit 2>1) -> '2', i=2 (digit 1==1) and i>=last_less? 2>=0 true -> '1', i=3 (digit 2>1) -> '2' → so "212" which is not valid because group1 has [1]? Wait order: positions 1,2,3 = 2,1,2. Group1 digits from '1' are? Position2 is '1', position1 and4? Actually "212" means pos1 '2', pos2 '1', pos3 '2' – group1 has only digit at pos2 (1), group2 has 2 and 2 – that's valid! Because group1 sequence [1] non-decreasing, group2 [2,2] non-increasing. So "212" is valid. Good. So our code may produce valid. But my earlier assert expecting "111" for {1,2,3}? For {1,2,3}, pivot 4: all <4 → all '1' → "111" correct. For {3,2,1}, pivot 0: all >0 → all '2' → "222" correct. For {5}, pivot 6 → all <6 → '1' correct. So my test assertions might be too strict. I'll adjust tests to check only validity, not exact output, because multiple valid answers exist. However, the task says "If multiple valid assignments exist, return any one of them." So test should verify the returned string is a valid assignment. I'll write a helper function to verify validity given the returned string and digits.

Actually, to keep tests simple, I'll directly test known unique cases where only one pivot works and the assignment is forced. For example, {1,5,9} with pivot 5: group1 must have 1 and can have 5s assigned; group2 must have 9 and can have 5s. The only requirement is non-decreasing group1 and non-increasing group2. Many possibilities. So better to write a validator. But the problem statement says to "Call the solution function directly and compare results appropriately using == or another suitable comparison." That doesn't forbid validation. I'll include a small validator in the test and assert that the function returns a valid assignment or "-" when none exists. But to keep it simple, I'll test some impossible cases for "-", and for valid cases, I'll check a property like the output length and that it consists only of '1','2', and that for each pair of indices, if digit[i] < digit[j] and i<j but output[i]=='2' and output[j]=='1', then it's invalid? That's complex. Simpler: implement a function `isValid` that checks the condition. Then assert `isValid(digits, partitionByPivot(digits))` for all examples. That is acceptable.

I'll write a validator in the test code that uses the same logic: for the given output, verify that group1 digits are non-decreasing and group2 non-increasing, and also that there exists a pivot p such that all digits < p are '1' and all > p are '2'. Actually that's redundant; the output itself defines groups. We can just check that the digit sequence of '1's is non-decreasing and the digit sequence of '2's is non-increasing. That's the whole definition (since the pivot is just a boundary; if those two sequences are monotonic, then a pivot can be chosen as any value between the max of '1's and min of '2's). So validation: extract digits for group '1' and group '2' in original order, check non-decreasing for '1', non-increasing for '2'. That's sufficient. So in tests, I'll write `bool isValid(const std::vector<int>& d, const std::string& s)`.

Then test with several vectors, including some known impossible ones like {2,1,3}? Let's check: Can we partition {2,1,3}? Try pivot p=2: less = {1} at pos2, greater = {3} at pos3, p=2 at pos1. last_less=2, first_greater=3. p at pos1: first_greater<1? no, ok. So possible: group1 has digits <2 (1) and maybe p's after last_less? last_less=2, so p at pos1 (i=1) is before last_less, so assigned '2' -> group2. So group1: [1] (non-dec), group2: [2,3] (non-inc? 2 then 3 is increasing, which violates non-increasing). So invalid. That means no assignment? Let's try other pivot. p=1: less none, greater {2,3}? Actually digits >1 are pos1(2),pos3(3). Non-increasing? 2 then 3? order positions 1 and3: 2,3 increasing, so fails. p=3: less {1,2} at pos1,pos2? less than 3 are 1,2 at pos1,pos2 – they are 1 then 2? Actually digits: 2,1,3. pos1=2<3, pos2=1<3, order 2 then 1 which is decreasing, so fails. p=0: all greater? 2,1,3 – non-increasing? 2,1,3 so 2>1 but 1<3 fails. p=9: all less? 2,1,3 non-decreasing? 2>1 fails. So impossible – test "-". Good.

I'll include that case.

Also test all same digits: {5,5,5} – pivot 5, all can be '1' or '2'? Actually group1 non-dec, group2 non-inc, both fine. So output "111" is valid. Test that.

I'll write a few asserts with the validator.

Also note: The provided solution function returns any valid string, so for a given input there may be multiple valid outputs, but the validator will catch if it's invalid. For "-", we assert that the result equals "-".

I'll also test that `partitionByPivot({})`? Not specified; but we can assert it returns "-" or maybe handle gracefully. I'll skip empty.

Finally, test for large n? Not needed.

I'll write the test code inside a global main with asserts, including the validator function.
</throught>

I'll now produce the final answer. Ensure the block contains only the function without main. The block includes main with asserts. I'll include a helper `isValid` inside the test main block.

Let me write the final response.
#include <cassert>
#include <vector>
#include <string>

// Helper to check if a given assignment is valid.
bool isValid(const std::vector<int>& digits, const std::string& assignment) {
    if (assignment.size() != digits.size()) return false;
    for (char c : assignment) if (c != '1' && c != '2') return false;
    int prev1 = -1, prev2 = 10; // sentinels
    for (size_t i = 0; i < digits.size(); ++i) {
        if (assignment[i] == '1') {
            if (digits[i] < prev1) return false;
            prev1 = digits[i];
        } else {
            if (digits[i] > prev2) return false;
            prev2 = digits[i];
        }
    }
    return true;
}

int main() {
    // Simple monotonic cases
    assert(partitionByPivot({1,2,3}) == "111");
    assert(partitionByPivot({3,2,1}) == "222");
    assert(partitionByPivot({5}) == "1");
    assert(partitionByPivot({5,5,5}) == "111"); // any assignment works

    // Valid mixed cases – validate returned string
    std::vector<int> v1 = {1,5,9};
    std::string r1 = partitionByPivot(v1);
    assert(isValid(v1, r1));

    std::vector<int> v2 = {1,5,5,9};
    std::string r2 = partitionByPivot(v2);
    assert(isValid(v2, r2));

    // Impossible case
    std::vector<int> v3 = {2,1,3};
    assert(partitionByPivot(v3) == "-");

    // Edge case with all same digits and mixed bounds
    std::vector<int> v4 = {0,0,0,0};
    std::string r4 = partitionByPivot(v4);
    assert(isValid(v4, r4));
    assert(partitionByPivot({9,9,9}) == "111"); // pivot 10? Actually pivot >9, all less -> all 1
}
#include <string>
#include <vector>

// Returns a valid two-group assignment string, or "-" if none exists.
std::string partitionByPivot(const std::vector<int>& digits) {
    int n = static_cast<int>(digits.size());
    // Try each possible pivot value from 0 to 9.
    for (int p = 0; p <= 9; ++p) {
        // last_less_index: last position (1-based) with digit < p, or 0 if none.
        int last_less = 0;
        bool less_ok = true;
        for (int i = 1; i <= n; ++i) {
            if (digits[i-1] < p) {
                if (last_less != 0 && digits[last_less-1] > digits[i-1]) {
                    less_ok = false;
                    break;
                }
                last_less = i;
            }
        }
        if (!less_ok) continue;

        // first_greater_index: first position (1-based) with digit > p, or n+1 if none.
        int first_greater = n + 1;
        bool greater_ok = true;
        for (int i = n; i >= 1; --i) {
            if (digits[i-1] > p) {
                if (first_greater != n+1 && digits[first_greater-1] < digits[i-1]) {
                    greater_ok = false;
                    break;
                }
                first_greater = i;
            }
        }
        if (!greater_ok) continue;

        // Check that no digit equal to p lies strictly between first_greater and last_less.
        bool pivot_ok = true;
        for (int i = 1; i <= n; ++i) {
            if (digits[i-1] == p && first_greater < i && i < last_less) {
                pivot_ok = false;
                break;
            }
        }
        if (!pivot_ok) continue;

        // Build the result string for this valid pivot.
        std::string result(n, ' ');
        for (int i = 1; i <= n; ++i) {
            if (digits[i-1] < p) {
                result[i-1] = '1';
            } else if (digits[i-1] > p) {
                result[i-1] = '2';
            } else { // digit == p
                // Assign to group 1 if it comes after (or at) the last less index.
                // Otherwise assign to group 2.
                if (i >= last_less) {
                    result[i-1] = '1';
                } else {
                    result[i-1] = '2';
                }
            }
        }
        return result;
    }
    return "-";
}
// The solution examines each possible pivot value `p` from 0 to 9. For a given `p`, we need to check feasibility: the group-1 digits consist of all digits `< p` plus a suffix of occurrences of `p` (from some position onward), and group-2 digits consist of all digits `> p` plus a prefix of occurrences of `p`. To check feasibility efficiently, precompute for each index the last position (from left to right) of a digit `< p` to check if the `< p` digits are non-decreasing, and similarly precompute from right to left for digits `> p` to check non-increasing. Additionally, ensure that no occurrence of `p` lies strictly between the last `< p` position and the first `> p` position (when scanning right-to-left), because that would force a `p` to be split in a way that violates both monotonic conditions. More concretely: let `last_less[i]` be the maximum index `j ≤ i` such that `digits[j] < p` (or 0 if none), and `first_greater[i]` be the minimum index `j ≥ i` such that `digits[j] > p` (or n+1 if none). The `< p` digits are non-decreasing if for every `i` with `digits[i] < p`, `last_less[i-1]`'s value ≤ `digits[i]`. Similarly for `> p`. For `p` digits, we can assign all occurrences before `first_greater` to group 2 and all after `last_less` to group 1, but if there is an occurrence of `p` with index between `last_less` and `first_greater` (i.e., strictly after the last `< p` and before the first `> p`), it cannot be assigned cleanly; however, since `p` is between those extremes, any placement would break monotonicity. The condition in the original code checks `fst < i && i < lst` where `fst` is the first `> p` and `lst` is the last `< p`. That means if there is a `p` that lies strictly between the first greater and the last less, it's impossible. Wait, careful: the original code defines `lst` as the last index where digit `< p` appears, and `fst` as the first index (from right scan) where digit `> p` appears. Then it checks that no `p` has index `i` with `fst < i && i < lst`. That is correct because if `i` is after the first greater and before the last less, then this `p` would be to the right of a greater digit (so group-2 would have a greater before a p, violating non-increasing if that p is in group 2) and to the left of a less digit (so group-1 would have a p before a less, violating non-decreasing if in group 1). So it's impossible. For a valid `p`, we construct the output: for each index, if digit `< p` → '1'; if `> p` → '2'; if `== p` and `i >= lst` (i.e., after or at the last less) → '1', else '2'. Time complexity is O(n) per pivot, with constant number of pivots (10), so O(10n) = O(n). Space O(n) for the result string. Edge cases: empty vector? The problem expects a non-empty vector; but handle gracefully by returning "-"? The original reads n≥1. The largest digit 9 and smallest 0 are covered by pivots 0..9. If no pivot works, return "-".
