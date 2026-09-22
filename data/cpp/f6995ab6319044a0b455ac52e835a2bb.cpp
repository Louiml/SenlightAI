Write a C++ function named `simulateMedicationSchedule` that takes three parameters: the number of distinct medications `k`, a positive integer `totalSlots` representing how many times a medication should be administered in total, and a vector of pairs where each pair contains the interval (in minutes) and the medication name. The function must simulate time starting at minute 1 and continuing upward; at every minute, all medications whose interval divides the current minute exactly (`currentMinute % interval == 0`) are administered once, and the administration event is recorded as a pair consisting of the current minute and the medication name. The simulation stops as soon as exactly `totalSlots` administrations have been recorded (do not continue past that count, even if more medications would be administered at that same minute). The function should return a `std::vector<std::pair<int, std::string>>` in the order the administrations occur (chronologically by minute, and for the same minute, in the order the medications appear in the input vector). Assume all intervals are positive integers, medication names are non-empty strings without spaces, and `k >= 1`. The simulation must handle cases where multiple medications have the same interval, where `totalSlots` may be less than the number of medications administered in the first minute, and where some medications may never be administered before the total is reached.
The core approach is a brute-force time simulation. Start with `currentMinute = 1` and a counter `administered` set to 0. While `administered < totalSlots`, iterate through every medication in the input vector. For each medication, check if `currentMinute % interval == 0`. If yes, push a pair `{currentMinute, name}` into the result vector and increment `administered`. After processing all medications for that minute, if `administered` has reached `totalSlots`, break out of the loop immediately; otherwise, increment `currentMinute` and continue. Important edge cases include: (1) When `totalSlots` is reached mid-minute, we must not process the remaining medications in that same minute—the loop condition `administered < totalSlots` inside the inner for-loop handles this. (2) If the first administration occurs at some minute > 1, the simulation correctly skips earlier minutes with no events. (3) If a medication has interval 1, it is administered every minute. (4) The problem guarantees the simulation will eventually reach `totalSlots` because at least one medication has a positive finite interval, so within `totalSlots * maxInterval` minutes we will always reach the count. Time complexity is O(totalSlots * k) in the worst case, because each minute we scan all k medications, and the number of minutes processed is at most `totalSlots * maxInterval` which could be large, but since each minute that has no events still costs O(k), the worst-case is O(k * totalSlots * maxInterval) if intervals are huge and sparse (but in practice bounded). A tighter bound is O(minutes * k) where minutes = totalSlots * maxInterval / (number of active medications per minute). Space complexity is O(totalSlots) for the result vector.
#include <vector>
#include <string>
#include <utility>

// Simulate a medication administration schedule. 
// Returns a vector of {minute, medication_name} pairs in chronological order.
std::vector<std::pair<int, std::string>> simulateMedicationSchedule(
    int k, int totalSlots, const std::vector<std::pair<int, std::string>>& meds) {
    std::vector<std::pair<int, std::string>> result;
    int administered = 0;
    int currentMinute = 1;

    while (administered < totalSlots) {
        for (int i = 0; i < k; ++i) {
            if (administered >= totalSlots) break;  // stop exactly at totalSlots
            const int interval = meds[i].first;
            if (currentMinute % interval == 0) {
                result.push_back({currentMinute, meds[i].second});
                ++administered;
            }
        }
        if (administered >= totalSlots) break;  // finished this minute
        ++currentMinute;
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Include the solution function here (or rely on previous definition).

int main() {
    // Test 1: Basic case with two medications, intervals 2 and 3, totalSlots 5
    {
        std::vector<std::pair<int, std::string>> meds = {{2, "A"}, {3, "B"}};
        auto result = simulateMedicationSchedule(2, 5, meds);
        std::vector<std::pair<int, std::string>> expected = {
            {2, "A"}, {3, "B"}, {4, "A"}, {6, "A"}, {6, "B"}
        };
        assert(result == expected);
    }

    // Test 2: totalSlots smaller than number of administrations in first minute
    {
        std::vector<std::pair<int, std::string>> meds = {{1, "X"}, {1, "Y"}, {2, "Z"}};
        auto result = simulateMedicationSchedule(3, 2, meds);
        std::vector<std::pair<int, std::string>> expected = {
            {1, "X"}, {1, "Y"}
        };
        assert(result == expected);
    }

    // Test 3: Single medication with interval 5, totalSlots 3
    {
        std::vector<std::pair<int, std::string>> meds = {{5, "M"}};
        auto result = simulateMedicationSchedule(1, 3, meds);
        std::vector<std::pair<int, std::string>> expected = {
            {5, "M"}, {10, "M"}, {15, "M"}
        };
        assert(result == expected);
    }

    // Test 4: Interval 1 medication appears every minute, totalSlots 4
    {
        std::vector<std::pair<int, std::string>> meds = {{1, "C"}};
        auto result = simulateMedicationSchedule(1, 4, meds);
        std::vector<std::pair<int, std::string>> expected = {
            {1, "C"}, {2, "C"}, {3, "C"}, {4, "C"}
        };
        assert(result == expected);
    }

    // Test 5: Same interval for multiple drugs, order preserved per minute
    {
        std::vector<std::pair<int, std::string>> meds = {{2, "A"}, {2, "B"}};
        auto result = simulateMedicationSchedule(2, 3, meds);
        std::vector<std::pair<int, std::string>> expected = {
            {2, "A"}, {2, "B"}, {4, "A"}
        };
        assert(result == expected);
    }

    // Test 6: Large intervals, first administration after many minutes
    {
        std::vector<std::pair<int, std::string>> meds = {{100, "Slow"}};
        auto result = simulateMedicationSchedule(1, 1, meds);
        std::vector<std::pair<int, std::string>> expected = {{100, "Slow"}};
        assert(result == expected);
    }

    // Test 7: Mixed intervals, totalSlots exactly hits at a minute boundary
    {
        std::vector<std::pair<int, std::string>> meds = {{1, "A"}, {2, "B"}};
        auto result = simulateMedicationSchedule(2, 4, meds);
        std::vector<std::pair<int, std::string>> expected = {
            {1, "A"}, {2, "A"}, {2, "B"}, {3, "A"}
        };
        assert(result == expected);
    }

    // Test 8: Empty-ish scenario with k=1 and totalSlots=1, interval 1
    {
        std::vector<std::pair<int, std::string>> meds = {{1, "Only"}};
        auto result = simulateMedicationSchedule(1, 1, meds);
        std::vector<std::pair<int, std::string>> expected = {{1, "Only"}};
        assert(result == expected);
    }

    // Test 9: Verify that no extra events are added after totalSlots reached
    {
        std::vector<std::pair<int, std::string>> meds = {{1, "A"}, {1, "B"}, {1, "C"}};
        auto result = simulateMedicationSchedule(3, 2, meds);
        assert(result.size() == 2);
        assert(result[0] == std::make_pair(1, "A"));
        assert(result[1] == std::make_pair(1, "B"));
    }

    // Test 10: Large totalSlots with interval 1, ensure correct length
    {
        std::vector<std::pair<int, std::string>> meds = {{1, "Tick"}};
        auto result = simulateMedicationSchedule(1, 10, meds);
        assert(result.size() == 10);
        for (int i = 0; i < 10; ++i) {
            assert(result[i] == std::make_pair(i + 1, std::string("Tick")));
        }
    }

    return 0;
}
