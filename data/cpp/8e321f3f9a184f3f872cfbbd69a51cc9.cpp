/*
Given a vector of integers representing daily temperatures, write a C++ function `dailyTemperatureWaitDays` that returns a vector of integers of the same length, where each element at index `i` is the number of days you have to wait after day `i` to get a warmer temperature. If there is no future day with a warmer temperature, that element should be `0`. The input vector is non-empty and contains only positive integer values.
*/

#include <vector>
#include <stack>

// For each day, return the number of days until a warmer temperature,
// or 0 if no warmer day exists in the future.
std::vector<int> dailyTemperatureWaitDays(const std::vector<int>& temperatures) {
    int n = static_cast<int>(temperatures.size());
    std::vector<int> result(n, 0);
    std::stack<int> st; // stores indices of days with no warmer temperature found yet

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && temperatures[st.top()] < temperatures[i]) {
            int prevIndex = st.top();
            st.pop();
            result[prevIndex] = i - prevIndex;
        }
        st.push(i);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Example from classic problem
    std::vector<int> temps1 = {73, 74, 75, 71, 69, 72, 76, 73};
    std::vector<int> expected1 = {1, 1, 4, 2, 1, 1, 0, 0};
    assert(dailyTemperatureWaitDays(temps1) == expected1);

    // single element
    std::vector<int> temps2 = {30};
    assert(dailyTemperatureWaitDays(temps2) == std::vector<int>{0});

    // all equal
    std::vector<int> temps3 = {50, 50, 50};
    assert(dailyTemperatureWaitDays(temps3) == std::vector<int>{0, 0, 0});

    // strictly decreasing
    std::vector<int> temps4 = {90, 80, 70, 60};
    assert(dailyTemperatureWaitDays(temps4) == std::vector<int>{0, 0, 0, 0});

    // strictly increasing
    std::vector<int> temps5 = {60, 70, 80, 90};
    assert(dailyTemperatureWaitDays(temps5) == std::vector<int>{1, 1, 1, 0});

    // mixed with gaps
    std::vector<int> temps6 = {70, 60, 80, 75, 85};
    // Day0 waits 2 days to 80, Day1 waits 1 day to 80, Day2 waits 2 days to 85, Day3 waits 1 day to 85, Day4 0
    assert(dailyTemperatureWaitDays(temps6) == std::vector<int>{2, 1, 2, 1, 0});
}

// The problem is a classic "next greater element" variant where we need the distance to the next larger value. A brute-force approach for each day scanning forward would be O(n²) and too slow for large inputs. Instead, we use a monotonic decreasing stack that stores indices (not values) of temperatures that have not yet found a warmer day. We iterate through the array from left to right. For each current temperature, while the stack is not empty and the temperature at the stack’s top index is strictly less than the current temperature, we pop that index. The difference between the current index and the popped index gives the number of days until a warmer temperature for that popped day. We then push the current index onto the stack. At the end, any indices still in the stack have no warmer future day and remain `0` (they were initialized to zero). Edge cases: a single element returns `[0]`; all equal temperatures return all zeros; a strictly decreasing sequence returns all zeros except the last (which is also zero). Time complexity is O(n) because each index is pushed and popped at most once. Space complexity is O(n) for the stack and the answer vector.
