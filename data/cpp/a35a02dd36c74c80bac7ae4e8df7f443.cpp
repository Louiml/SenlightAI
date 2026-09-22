// Write a C++ function that takes a non-negative integer representing a duration in seconds and returns a formatted string in the 24-hour clock format `HH:MM:SS`, where each part is zero-padded to exactly two digits (e.g., input `3661` returns `"01:01:01"`, input `59` returns `"00:00:59"`, and input `86399` returns `"23:59:59"`). The function should handle any valid input from `0` up to `86399` (one full day minus one second) and must not assume the input is already within normal clock bounds (i.e., it must correctly process values up to the given maximum).

#include <cassert>
#include <string>

// Declare the function (or include the header where it is defined)
std::string formatDuration(int totalSeconds);

int main() {
    assert(formatDuration(0) == "00:00:00");
    assert(formatDuration(59) == "00:00:59");
    assert(formatDuration(60) == "00:01:00");
    assert(formatDuration(3600) == "01:00:00");
    assert(formatDuration(3661) == "01:01:01");
    assert(formatDuration(86399) == "23:59:59");
    assert(formatDuration(125) == "00:02:05");
    return 0;
}

#include <string>
#include <sstream>
#include <iomanip>

// Convert a non-negative number of seconds into a zero-padded HH:MM:SS string.
std::string formatDuration(int totalSeconds) {
    const int hours = totalSeconds / 3600;
    const int minutes = (totalSeconds % 3600) / 60;
    const int seconds = (totalSeconds % 3600) % 60;

    std::ostringstream output;
    output << std::setw(2) << std::setfill('0') << hours << ":"
           << std::setw(2) << std::setfill('0') << minutes << ":"
           << std::setw(2) << std::setfill('0') << seconds;
    return output.str();
}

// The core algorithm divides the total seconds into hours, minutes, and remaining seconds using integer division and modulo operations. First, compute `hours = total_seconds / 3600` (since 3600 seconds make an hour). Then, use the remainder `total_seconds % 3600` to compute `minutes = remainder / 60` and `seconds = remainder % 60`. This works because the modulo operation isolates the leftover seconds after removing full hours, and then dividing that remainder by 60 gives the complete minutes; the final modulo extracts leftover seconds.
//
// Edge cases: input `0` yields `"00:00:00"`; values less than 60 produce `"00:00:XX"`; values just below 3600 produce `"00:MM:SS"`; and the maximum `86399` yields `"23:59:59"`. No negative inputs are handled as the spec restricts to non-negative. The formatting requires each component to be zero-padded to two digits, which is done using `std::ostringstream` with `std::setw(2)` and `std::setfill('0')`. 
//
// Time complexity is \(O(1)\) because only constant arithmetic and string formatting operations are performed. Space complexity is \(O(1)\) for auxiliary storage (the output string itself is length 8, which is constant).
