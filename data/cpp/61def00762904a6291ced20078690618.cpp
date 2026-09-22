// You are given an even-length sequence of integers stored in a `std::deque<int>`. You must process the deque by repeatedly removing one element from the front and one from the back simultaneously. For each removal step, you must record the pair of moves (one character for the front removal, one for the back removal) based on a rule that alternates with each step. Write a C++ function `std::string processDeque(const std::deque<int>& input)` that, given the initial deque, returns a string of characters `'L'` (meaning remove from left/front) and `'R'` (meaning remove from right/back) representing the sequence of removals. The rule for each step is: on the first step (step index 0), if the front element is greater than the back element, append `"LR"` (front then back), otherwise append `"RL"` (front then back). On the second step (step index 1), the rule flips: if the front is greater, append `"RL"`, otherwise append `"LR"`. The rule continues to alternate every subsequent step. The deque will always have an even number of elements initially, so after processing all full pairs, no elements remain. The function should only compare the current front and back values before removal. The input deque is passed by constant reference and should not be modified. If the deque is empty, return an empty string.
// The main idea is to simulate the process exactly as described: while there are at least two elements, compare `front()` and `back()`. Use a boolean flag to track the current orientation (true or false) that toggles after each pair removal. When the flag is true, the sequence is `LR` if front > back, else `RL`. When false, it is the opposite. After appending two characters, remove the front and back elements using `pop_front()` and `pop_back()`, then flip the flag. Since the input length is even, the loop always ends with zero elements. Edge cases: an empty deque returns an empty string; there is no need to handle odd lengths because the problem guarantees even. The algorithm runs in \(O(n)\) time because each element is accessed and removed exactly once, and uses \(O(1)\) auxiliary space aside from the output string (which holds \(n\) characters).
#include <deque>
#include <string>

// Process a deque of integers by removing front and back pairs,
// alternating the comparison rule each step, and return the move sequence.
// 'L' indicates a front removal, 'R' indicates a back removal.
std::string processDeque(const std::deque<int>& input) {
    std::string ans;
    if (input.empty()) return ans;

    std::deque<int> temp = input;  // local copy to avoid modifying input
    bool flag = false;             // starts with rule for step 0: false means front>back -> "LR"

    while (temp.size() > 1) {
        if (flag) {
            // flag true: if front > back, output "RL", else "LR"
            if (temp.front() > temp.back()) {
                ans.push_back('R');
                ans.push_back('L');
            } else {
                ans.push_back('L');
                ans.push_back('R');
            }
        } else {
            // flag false: if front > back, output "LR", else "RL"
            if (temp.front() > temp.back()) {
                ans.push_back('L');
                ans.push_back('R');
            } else {
                ans.push_back('R');
                ans.push_back('L');
            }
        }
        flag = !flag;  // alternate rule for next step
        temp.pop_front();
        temp.pop_back();
    }
    // Even length guarantee: no leftover element.
    return ans;
}
#include <cassert>
#include <deque>
#include <string>

// prototype
std::string processDeque(const std::deque<int>& input);

int main() {
    // Basic example: front > back -> "LR", then flag flips, next step: front > back -> "RL"
    assert(processDeque({5, 1, 2, 3}) == "LRRL");

    // Front < back initially -> "RL", then front < back (with flag flipped) -> "LR"
    assert(processDeque({1, 2, 3, 4}) == "RLLR");

    // Equal values: front not > back, so treat as "RL" on first step, then flag flips
    // 2,2,2,2 -> first "RL", second step front==back -> with flag true, front>back false -> "LR"
    assert(processDeque({2, 2, 2, 2}) == "RLLR");

    // Larger even sequence with alternating comparisons
    // Step0: 9>1 -> "LR"; Step1: 8>2 -> flag true -> "RL"; Step2: 7>3 -> flag false -> "LR"; Step3: 6>4 -> flag true -> "RL"
    assert(processDeque({9, 8, 7, 6, 4, 3, 2, 1}) == "LRRLRLRL");

    // Edge case: empty deque
    assert(processDeque({}) == "");

    // Even length with only one pair, front > back
    assert(processDeque({10, 5}) == "LR");

    // Even length with only one pair, front < back
    assert(processDeque({5, 10}) == "RL");

    // Random small test: front > back, then after flipping, new front < new back
    // [4,1,2,3] -> step0: 4>3 -> "LR", remove 4 and 3 -> [1,2]; step1: flag true, 1>2? no -> "LR" -> total "LRLR"
    assert(processDeque({4, 1, 2, 3}) == "LRLR");

    // Another random: [3,1,4,2] -> step0: 3>2 -> "LR", remove 3 and 2 -> [1,4]; step1: flag true, 1>4? no -> "LR" -> total "LRLR"
    assert(processDeque({3, 1, 4, 2}) == "LRLR");

    return 0;
}
