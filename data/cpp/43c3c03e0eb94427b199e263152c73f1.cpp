/*
Write a C++ function `readBinaryWatch(int turnedOn)` that takes an integer `turnedOn` (0 ≤ turnedOn ≤ 10) representing how many LEDs are on in a binary watch, and returns a vector of strings containing all valid times that can be displayed. The binary watch has 4 LEDs for hours (bits 0-3) and 6 LEDs for minutes (bits 4-9). Hours must be in the range [0, 11] and minutes in [0, 59]. Format each time as `"H:MM"` where hours have no leading zero but minutes are zero-padded to two digits (e.g., `"0:07"`, `"11:59"`). Return the times in any order. Handle edge cases: if no valid times exist, return an empty vector.
*/
#include <string>
#include <vector>

// Return all valid times for a binary watch with exactly 'turnedOn' LEDs on.
std::vector<std::string> readBinaryWatch(int turnedOn) {
    std::vector<std::string> result;
    const int totalBits = 10;

    for (int mask = 0; mask < (1 << totalBits); ++mask) {
        // Count set bits
        int bitCount = 0;
        int temp = mask;
        while (temp) {
            bitCount += temp & 1;
            temp >>= 1;
        }
        if (bitCount != turnedOn) continue;

        // Extract hour (top 4 bits) and minute (bottom 6 bits)
        int hour = mask >> 6;
        int minute = mask & 0b111111;

        if (hour < 12 && minute < 60) {
            std::string time = std::to_string(hour) + ":";
            if (minute < 10) time += "0";
            time += std::to_string(minute);
            result.push_back(time);
        }
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

int main() {
    // Test turnedOn = 0 -> only "0:00"
    assert(readBinaryWatch(0) == std::vector<std::string>{"0:00"});

    // Test turnedOn = 1 -> all single-LED times
    std::vector<std::string> one = readBinaryWatch(1);
    assert(one.size() == 10);
    assert(std::find(one.begin(), one.end(), "0:01") != one.end());
    assert(std::find(one.begin(), one.end(), "0:02") != one.end());
    assert(std::find(one.begin(), one.end(), "0:04") != one.end());
    assert(std::find(one.begin(), one.end(), "0:08") != one.end());
    assert(std::find(one.begin(), one.end(), "0:16") != one.end());
    assert(std::find(one.begin(), one.end(), "0:32") != one.end());
    assert(std::find(one.begin(), one.end(), "1:00") != one.end());
    assert(std::find(one.begin(), one.end(), "2:00") != one.end());
    assert(std::find(one.begin(), one.end(), "4:00") != one.end());
    assert(std::find(one.begin(), one.end(), "8:00") != one.end());

    // Test turnedOn = 9 -> no valid times
    assert(readBinaryWatch(9).empty());

    // Test turnedOn = 10 -> no valid times
    assert(readBinaryWatch(10).empty());

    // Test turnedOn = 8 -> possible times (e.g., "11:59" has 8 bits? 11=1011 (3), 59=111011 (5), total 8)
    std::vector<std::string> eight = readBinaryWatch(8);
    assert(!eight.empty());

    // Test specific known valid time with 2 LEDs: "3:00" (hour=3 has 2 bits) and "0:03" (minute=3 has 2 bits)
    std::vector<std::string> two = readBinaryWatch(2);
    assert(std::find(two.begin(), two.end(), "3:00") != two.end());
    assert(std::find(two.begin(), two.end(), "0:03") != two.end());

    // Test formatting: "0:07" for minute 7 (0 padded)
    std::vector<std::string> three = readBinaryWatch(3);
    assert(std::find(three.begin(), three.end(), "0:07") != three.end());

    // Test invalid time not included: "12:00" is not valid (hour >=12)
    std::vector<std::string> one2 = readBinaryWatch(1);
    assert(std::find(one2.begin(), one2.end(), "12:00") == one2.end());

    // Test duplicate checking: time count for turnedOn=6 should be >0 (e.g., "10:59" has 2+5=7 bits? Actually 10=1010 (2), 59=111011 (5) total 7, so turnedOn=6 could be "9:59" = 2+5=7 no. Let's not assert specific counts, just size consistency)
    assert(!readBinaryWatch(6).empty());
}
// The solution enumerates all 10-bit combinations (from 0 to 1023), where each bit represents an LED. The high 4 bits represent the hour (bits 6-9 if we map the total 10 bits as hour = bits 6..9, minute = bits 0..5, but the snippet uses `i >> 6` which extracts the top 4 bits, and `i & 0b111111` extracts the low 6 bits). For each combination, count the number of set bits using `__builtin_popcount` (or a custom bit-count loop for portability). If the count equals `turnedOn` and the hour is < 12 and minute < 60, format the time. Important edge cases: when `turnedOn` is 0, only "0:00" is valid; when `turnedOn` is > 8, no valid times exist (since max LEDs used in a valid time is 8: 4 for hour + 4 for minute? Actually max valid is hour 11 (3 bits) + minute 59 (6 bits) = 9 bits, but 11 has 3 bits, 59 has 5 bits, total 8? Let's verify: 11 = 1011 (3 bits), 59 = 111011 (5 bits), total 8. So for turnedOn > 8, empty vector). Complexity is O(2^10 * 1) = O(1024) time and O(number of valid times) space, which is constant.
