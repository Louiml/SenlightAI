/*
Write a C++ function that takes a non-negative integer representing the number of seconds elapsed since midnight and returns a string in the format `"HH:MM:SS"` where HH is hours (0-23), MM is minutes (0-59), and SS is seconds (0-59). The function should handle values up to 86399 (23:59:59) correctly, but also be robust for values larger than that by wrapping around (i.e., using modulo 86400) so that the output always represents a valid time of day. For example, an input of 3661 should yield `"1:1:1"` (no leading zeros), and an input of 90061 should yield `"1:1:1"` after wrapping. Use only integer arithmetic and output the time components without leading zeros, separated by colons. The function must be pure (no input/output), and the solution must include appropriate `const` correctness where applicable.
*/
#include <string>

// Convert seconds since midnight to a "H:M:S" string, wrapping after 24 hours.
std::string secondsToHMS(int totalSeconds) {
    const int secondsPerDay = 86400;
    const int secondsPerHour = 3600;
    const int secondsPerMinute = 60;

    // Normalize to within a single day (handles values > 86399).
    totalSeconds %= secondsPerDay;

    const int hours = totalSeconds / secondsPerHour;
    const int remaining = totalSeconds % secondsPerHour;
    const int minutes = remaining / secondsPerMinute;
    const int seconds = remaining % secondsPerMinute;

    return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
}
#include <cassert>

int main() {
    // Basic cases from midnight
    assert(secondsToHMS(0) == "0:0:0");
    assert(secondsToHMS(1) == "0:0:1");
    assert(secondsToHMS(60) == "0:1:0");
    assert(secondsToHMS(3600) == "1:0:0");

    // Midday and near end of day
    assert(secondsToHMS(12 * 3600 + 34 * 60 + 56) == "12:34:56");
    assert(secondsToHMS(86399) == "23:59:59");

    // Wrapping beyond one day
    assert(secondsToHMS(86400) == "0:0:0");
    assert(secondsToHMS(90061) == "1:1:1"); // 86400 + 3661
    assert(secondsToHMS(172800 + 3661) == "1:1:1"); // two days plus 1:1:1

    // Large value
    assert(secondsToHMS(100000000) == secondsToHMS(100000000 % 86400));
}
// The main algorithm involves extracting hours, minutes, and seconds from the total seconds elapsed. First, normalize the input with the modulo operator `% 86400` (number of seconds in a day) to handle values beyond one full day, ensuring the result stays within a 24-hour range. Then compute hours using integer division by 3600 (seconds per hour). For minutes, take the remainder after removing hours (`remaining = input % 3600`), then divide that by 60 (seconds per minute). Seconds are simply the remainder after removing minutes, i.e., `remaining % 60`. Edge cases include input 0, which should produce `"0:0:0"`, and inputs that are exact multiples of 86400, which also produce midnight. Negative inputs are not expected per the spec but could be handled by taking absolute value or clamping; the task specifies non-negative, so we assume valid input. Time complexity is \(O(1)\) (constant operations), and space complexity is \(O(1)\) for auxiliary storage, excluding the returned string. The function uses `std::to_string` for conversion, which is efficient.
