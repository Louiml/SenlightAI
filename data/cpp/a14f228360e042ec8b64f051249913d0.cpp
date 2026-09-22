// Write a C++ function named `processSwitchManipulation` that simulates a set of `n` light switches (each initially either 0 for off or 1 for on) and a sequence of student actions. The function must take as parameters: an integer `n` (the number of switches, 1 ≤ n ≤ 100), a `std::vector<int>` of length `n` representing the initial switch states (each 0 or 1), and a `std::vector<std::pair<int,int>>` of student actions where each action has a gender (1 = male, 2 = female, as defined by the task) and a switch number `num` (1-based). For a male student, toggle all switches whose position (1-based) is a multiple of `num`. For a female student, let `num` be the center; starting with `s = num-1` and `e = num+1`, while both indices are within [1, n] and the switches at `s` and `e` are equal, toggle both switches, then move `s` down and `e` up. After the loop, toggle the center switch `num` as well. The function must return the final switch states as a `std::vector<int>` (each 0 or 1) after all actions are applied in the given order. Maintain 1-based indexing internally for clarity, but the input vector is 0-based (index 0 corresponds to switch 1). The function must be robust for all valid inputs, including when `n` is 1 and when `num` is at the boundary. Time complexity must be O(n * number_of_actions) worst-case, and space complexity O(n). The function should be a free function with a descriptive name, not part of a class, and must not use global variables.
#include <cassert>
#include <vector>
#include <utility>

// prototype
std::vector<int> processSwitchManipulation(
    int n,
    const std::vector<int>& initialStates,
    const std::vector<std::pair<int,int>>& actions
);

int main() {
    // Basic single toggle
    {
        std::vector<int> init = {0, 1, 0};
        std::vector<std::pair<int,int>> actions = {{1, 2}}; // male toggles switch 2
        std::vector<int> result = processSwitchManipulation(3, init, actions);
        assert((result == std::vector<int>{0, 0, 0}));
    }
    // Female with symmetric expansion
    {
        std::vector<int> init = {1, 0, 1, 0, 1};
        std::vector<std::pair<int,int>> actions = {{2, 3}}; // center at 3
        // s=2 (0), e=4 (0) match -> toggle both -> 1,1; then s=1 (1), e=5 (1) match -> toggle -> 0,0; then stops
        // toggle center 3 from 1 to 0
        std::vector<int> result = processSwitchManipulation(5, init, actions);
        assert((result == std::vector<int>{0, 1, 0, 1, 0}));
    }
    // Edge case: n=1, female center
    {
        std::vector<int> init = {1};
        std::vector<std::pair<int,int>> actions = {{2, 1}}; // only center toggle
        std::vector<int> result = processSwitchManipulation(1, init, actions);
        assert((result == std::vector<int>{0}));
    }
    // Male multiples when num=1 toggles all
    {
        std::vector<int> init = {0, 0, 0, 0};
        std::vector<std::pair<int,int>> actions = {{1, 1}};
        std::vector<int> result = processSwitchManipulation(4, init, actions);
        assert((result == std::vector<int>{1, 1, 1, 1}));
    }
    // Multiple actions in sequence
    {
        std::vector<int> init = {1, 1, 1};
        std::vector<std::pair<int,int>> actions = {{1, 2}, {2, 2}};
        // after male 2: toggles switch 2 -> {1,0,1}
        // female center 2: s=1 (1), e=3(1) match -> toggle both -> {0,?,0}; toggle center 2 from 0->1 -> {0,1,0}
        std::vector<int> result = processSwitchManipulation(3, init, actions);
        assert((result == std::vector<int>{0, 1, 0}));
    }
    // Female at boundary with no expansion
    {
        std::vector<int> init = {1, 0, 1};
        std::vector<std::pair<int,int>> actions = {{2, 1}}; // center 1, s=0 fails immediately
        // only toggle center 1 -> 0
        std::vector<int> result = processSwitchManipulation(3, init, actions);
        assert((result == std::vector<int>{0, 0, 1}));
    }
    return 0;
}
#include <vector>
#include <utility>

// Simulates switch toggling by male and female students.
// genders: 1 = male, 2 = female. Each action pair: (gender, switch_number)
// Returns final switch states as vector<int> of 0/1 in original order.
std::vector<int> processSwitchManipulation(
    int n,
    const std::vector<int>& initialStates,
    const std::vector<std::pair<int,int>>& actions
) {
    // Internal 1-based container, index 0 unused
    std::vector<int> work(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        work[i + 1] = initialStates[i];
    }

    for (const auto& action : actions) {
        int gender = action.first;
        int num = action.second; // 1-based

        if (gender == 1) { // male
            for (int i = num; i <= n; i += num) {
                work[i] = 1 - work[i];
            }
        } else if (gender == 2) { // female
            int s = num - 1;
            int e = num + 1;
            while (s >= 1 && e <= n && work[s] == work[e]) {
                work[s] = 1 - work[s];
                work[e] = 1 - work[e];
                --s;
                ++e;
            }
            work[num] = 1 - work[num];
        }
    }

    std::vector<int> result(n);
    for (int i = 1; i <= n; ++i) {
        result[i - 1] = work[i];
    }
    return result;
}
// The solution mimics the exact logic from the code snippet, but we adapt it to avoid vector padding by shifting to 1-based indexing internally. We create a local `std::vector<int> work` of size `n+1` where `work[0]` is unused and `work[i]` corresponds to the i-th switch (1-based). Initialize `work[1..n]` from the input vector. For each action, if gender == 1 (male), iterate with a for loop from `num` to `n` stepping by `num` and flip each bit using `work[i] = 1 - work[i]` (or XOR with 1). If gender == 2 (female), set `s = num-1` and `e = num+1`, then while `s >= 1 && e <= n && work[s] == work[e]`, toggle both `work[s]` and `work[e]` and decrement `s` and increment `e`. After the loop, toggle `work[num]`. After processing all actions, construct the result vector of size `n` by copying `work[1]` through `work[n]`. Edge cases: when `num` is 1 for male, all switches toggle; when `n=1`, female's while loop won't execute (since s=0, fails s>=1), so only the center toggles; when `num` is at the end, the while loop may run zero times if neighbors don't match. Time complexity per action is O(n) worst-case (male) or O(n) for the while loop (female, but it can expand to at most n/2 steps), so total O(actions * n). Space is O(n) for the working vector and output. The function is `const`-correct on the input parameters (they are passed by const reference to avoid copying).
