/*
Write a C++ function named `distanceTable` that takes two integer parameters: `speed` (in miles per hour) and `hours` (the number of hours traveled). The function must validate that `speed` is non-negative and `hours` is at least 1; if either is invalid, throw an `std::invalid_argument` exception with a descriptive error message. The function must return a `std::string` containing a formatted table with a header line "Hour\tDistance Traveled" (without quotes), a separator line of 37 hyphen characters, and then for each hour from 1 to `hours`, a line with that hour and the corresponding distance (`speed * hour`), separated by a tab. Each line must end with a newline character. For example, if `speed = 40` and `hours = 3`, the returned string should be exactly:
```
Hour	Distance Traveled
-------------------------------------
1	40
2	80
3	120
```
(Note: the exact number of hyphens in the separator should be 37 as shown. No extra spaces, no leading/trailing whitespace beyond line newlines. Use integer arithmetic; distances are always integers.)
*/
#include <string>
#include <stdexcept>

// Build a distance-vs-time table for a vehicle given speed in mph and hours traveled.
// Throws std::invalid_argument if speed < 0 or hours < 1.
std::string distanceTable(const int speed, const int hours) {
    if (speed < 0) {
        throw std::invalid_argument("speed must be non-negative");
    }
    if (hours < 1) {
        throw std::invalid_argument("hours must be at least 1");
    }

    const std::string separator(37, '-');

    std::string result;
    result += "Hour\tDistance Traveled\n";
    result += separator + "\n";

    for (int hour = 1; hour <= hours; ++hour) {
        result += std::to_string(hour) + "\t" + std::to_string(speed * hour) + "\n";
    }

    return result;
}
#include <cassert>
#include <string>
#include <stdexcept>

// Declaration of the function under test (assumed in the same translation unit).
std::string distanceTable(const int speed, const int hours);

int main() {
    // Normal case
    std::string expected = "Hour\tDistance Traveled\n"
                           "-------------------------------------\n"
                           "1\t40\n"
                           "2\t80\n"
                           "3\t120\n";
    assert(distanceTable(40, 3) == expected);

    // Speed zero produces all zero distances
    expected = "Hour\tDistance Traveled\n"
               "-------------------------------------\n"
               "1\t0\n"
               "2\t0\n";
    assert(distanceTable(0, 2) == expected);

    // Only one hour
    expected = "Hour\tDistance Traveled\n"
               "-------------------------------------\n"
               "1\t75\n";
    assert(distanceTable(75, 1) == expected);

    // Check exact count of hyphens (37)
    std::string result = distanceTable(1, 1);
    size_t pos = result.find('\n') + 1;
    size_t end = result.find('\n', pos);
    assert(end - pos == 37);

    // Invalid speed throws
    bool threw = false;
    try {
        distanceTable(-1, 2);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Invalid hours throws
    threw = false;
    try {
        distanceTable(10, 0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Larger hours produce correct last line
    result = distanceTable(10, 5);
    assert(result.find("5\t50\n") != std::string::npos);

    return 0;
}
// The solution builds the output line by line. First, validate inputs: speed must be >= 0, and hours must be >= 1. If not, throw `std::invalid_argument`. Then create a `std::string` variable and append the header, separator, and each row using a loop from 1 to `hours`. Each row uses `std::to_string(count)` and `std::to_string(speed * count)` separated by a tab, followed by a newline. The loop runs exactly `hours` times, so time complexity is O(hours). Space complexity is O(hours) because the returned string is proportional to the number of rows. Edge cases include `hours = 1` (only one row), `speed = 0` (all distances are zero), and invalid inputs that must trigger exceptions. The solution uses `const` parameters and a `const std::string` for the separator.
