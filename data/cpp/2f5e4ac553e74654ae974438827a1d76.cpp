// Write a C++ function named `timeConversion` that takes a string `s` representing a time in 12-hour AM/PM format (e.g., `"07:05:45PM"`) and returns a string representing the same time in 24-hour format (e.g., `"19:05:45"`). The input is guaranteed to be a valid time string with exactly the format `hh:mm:ssAM` or `hh:mm:ssPM`, where `hh` is from `01` to `12`, `mm` and `ss` are from `00` to `59`. The output should maintain the same minutes and seconds as the input, with the hour converted according to the 12-hour-to-24-hour rules: 12:xx:xxAM becomes 00:xx:xx, 12:xx:xxPM stays 12:xx:xx, hours 01–11 AM remain unchanged, and hours 01–11 PM become hour + 12. The function must be `const`-correct and should not modify the input string.
// The solution extracts the AM/PM marker, hour, minutes, and seconds using substring operations. Since the input format is fixed, the positions are known: the last two characters are the period, the first two are the hour, and characters at indices 3–4 and 6–7 are minutes and seconds respectively. The main logic converts the hour: if the period is "AM" and the hour is "12", set it to "00"; if the period is "PM" and the hour is not "12", convert the hour to an integer, add 12, and convert back to a string (this is safe because hour strings like "01" become "13", and "11" becomes "23"). No other cases require changes. Edge cases include "12:00:00AM" → "00:00:00" and "12:00:00PM" → "12:00:00". The algorithm runs in O(1) time (constant number of string operations on fixed-length input) and O(1) auxiliary space, excluding the output string itself.
#include <string>

// Convert a 12-hour AM/PM time string to 24-hour format.
// Input format: hh:mm:ssAM or hh:mm:ssPM, where hh is 01-12, mm and ss are 00-59.
std::string timeConversion(const std::string& s) {
    // Extract period (AM/PM) from the last two characters.
    std::string period = s.substr(8, 2);
    
    // Extract hour, minute, and second substrings.
    std::string hour = s.substr(0, 2);
    std::string minute = s.substr(3, 2);
    std::string second = s.substr(6, 2);
    
    // Convert hour based on period.
    if (period == "AM") {
        if (hour == "12") {
            hour = "00";
        }
    } else { // PM
        if (hour != "12") {
            int hour_int = std::stoi(hour);
            hour_int += 12;
            hour = std::to_string(hour_int);
        }
    }
    
    // Return the combined 24-hour time string.
    return hour + ":" + minute + ":" + second;
}
#include <cassert>
#include <string>

// Assume the function is declared in the same translation unit.
std::string timeConversion(const std::string& s);

int main() {
    assert(timeConversion("12:00:00AM") == "00:00:00");
    assert(timeConversion("12:00:00PM") == "12:00:00");
    assert(timeConversion("01:00:00AM") == "01:00:00");
    assert(timeConversion("11:59:59PM") == "23:59:59");
    assert(timeConversion("07:05:45PM") == "19:05:45");
    assert(timeConversion("00:00:00AM") == "00:00:00");  // Note: input may still be valid per problem constraints
    assert(timeConversion("12:34:56PM") == "12:34:56");
    assert(timeConversion("01:02:03PM") == "13:02:03");
    assert(timeConversion("09:08:07AM") == "09:08:07");
    assert(timeConversion("11:11:11PM") == "23:11:11");
}
