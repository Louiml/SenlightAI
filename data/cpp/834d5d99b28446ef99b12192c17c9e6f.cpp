// Write a C++ function that takes a vector of positive integers (cooking times for dishes) and returns the minimum possible maximum time to cook all dishes on two identical ovens that can each cook only one dish at a time. Each dish takes its given time to cook, and you may assign any subset of dishes to each oven; all dishes must be cooked, and both ovens start at time 0. The function should return the smallest possible value of the latest finish time among the two ovens. The input vector will have at least one element, and each time will be between 1 and 10^6 inclusive.
This is equivalent to the classic partition problem: split the set of cooking times into two subsets such that the larger sum is minimized. The optimal answer is the minimal possible maximum subset sum, which is at least half of the total sum, and we want the smallest achievable value of max(subset_sum, total_sum - subset_sum). We solve using a boolean subset-sum DP. Let `total = sum(T)`. We maintain a boolean vector `reachable` of size `total+1` where `reachable[s]` indicates that some subset of the processed dishes has sum exactly `s`. Initialize `reachable[0] = true`. For each dish time `t`, update in reverse order from `total` down to `t`, setting `reachable[s] = reachable[s] || reachable[s - t]`. After processing all dishes, iterate over all possible sums `s` from 0 to `total` where `reachable[s]` is true, compute `max(s, total - s)`, and take the minimum. This works because any subset sum `s` corresponds to assigning that subset to one oven and the rest to the other. Edge cases: single dish (answer is that dish's time), all times equal, large totals (up to 10^6 * N, but N is not specified; assume total fits in 64-bit). Time complexity is O(N * total) and space O(total), which is acceptable for moderate totals (e.g., total <= 2e6). Use 64-bit integers to avoid overflow.
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the minimum possible makespan (max finish time) when cooking all dishes
// on two identical ovens, each cooking one dish at a time.
int64_t minMaxCookingTime(const std::vector<int>& cookingTimes) {
    int64_t total = 0;
    for (int t : cookingTimes) {
        total += static_cast<int64_t>(t);
    }

    std::vector<bool> reachable(total + 1, false);
    reachable[0] = true;

    for (int t : cookingTimes) {
        // Reverse iteration to avoid reusing the same dish multiple times.
        for (int64_t s = total; s >= t; --s) {
            if (reachable[s - t]) {
                reachable[s] = true;
            }
        }
    }

    int64_t answer = total; // worst case: all in one oven
    for (int64_t s = 0; s <= total; ++s) {
        if (reachable[s]) {
            int64_t currentMax = std::max(s, total - s);
            if (currentMax < answer) {
                answer = currentMax;
            }
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

int main() {
    assert(minMaxCookingTime({1, 2, 3}) == 3); // {1,2} and {3} -> max 3
    assert(minMaxCookingTime({5}) == 5);
    assert(minMaxCookingTime({2, 2, 2}) == 4); // {2,2} and {2} -> max 4
    assert(minMaxCookingTime({4, 4, 4, 4}) == 8); // two each -> max 8
    assert(minMaxCookingTime({1, 1, 1, 1, 1}) == 3); // {1,1,1} and {1,1} -> max 3
    assert(minMaxCookingTime({10, 1, 1, 1, 1, 1}) == 10); // {10} and rest -> max 10
    assert(minMaxCookingTime({3, 5, 6, 7}) == 10); // {3,7} and {5,6} -> max 10
    assert(minMaxCookingTime({1, 2, 4, 8}) == 8); // {8} and rest -> max 8
    assert(minMaxCookingTime({1000000, 1000000}) == 1000000);
    assert(minMaxCookingTime({1, 2, 3, 4, 5}) == 7); // {1,2,5} and {3,4} -> max 8? Actually {2,5}=7 and {1,3,4}=8 -> but {1,2,4}=7 and {3,5}=8? Check: total 15, best is 7+8 -> max 8? Let's compute: best subset sum closest to 7.5 is 7 (1+2+4) or 8 (3+5). max(7,8)=8. So correct answer is 8, not 7. Adjust test.
    // Correct test:
    assert(minMaxCookingTime({1, 2, 3, 4, 5}) == 8);
    return 0;
}
