/*
Write a C++ function named `convertTimeZone` that takes a string representing the local time (either "noon", "midnight", or a time in 12-hour format like "3:45" with a following "a.m." or "p.m."), the source time zone abbreviation (e.g., "EST", "UTC", "BST"), and the target time zone abbreviation. The function must return a string containing the corresponding time in the target zone, formatted exactly as: "noon" for 12:00 p.m., "midnight" for 12:00 a.m., otherwise the time in "H:MM a.m." or "H:MM p.m." format (e.g., "4:30 p.m."). Use the following fixed offset table (hours relative to UTC, with half-hour zones allowed): UTC=0, GMT=0, BST=+1, IST=+1, WET=0, WEST=+1, CET=+1, CEST=+2, EET=+2, EEST=+3, MSK=+3, MSD=+4, AST=-4, ADT=-3, NST=-3.5, NDT=-2.5, EST=-5, EDT=-4, CST=-6, CDT=-5, MST=-7, MDT=-6, PST=-8, PDT=-7, HST=-10, AKST=-9, AKDT=-8, AEST=+10, AEDT=+11, ACST=+9.5, ACDT=+10.5, AWST=+8. Assume both zone abbreviations are valid keys, and the input time is in 12-hour format with "a.m." (midnight to 11:59 a.m.) or "p.m." (noon to 11:59 p.m.) correctly stated. The conversion wraps around at 24 hours, so times may cross to the previous or next day but only the time-of-day is returned. Handle leading zeros in input minutes (e.g., "5:07") correctly, and output minutes always with two digits.
*/
#include <string>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <cmath>

// Convert a time from one fixed UTC-offset zone to another.
// Input: timeStr in 12-hour format ("noon", "midnight", or "H:MM a.m./p.m."),
//        fromZone and toZone are valid abbreviations from the offset table.
// Returns: equivalent time in toZone as "noon", "midnight", or "H:MM a.m./p.m."
std::string convertTimeZone(const std::string& timeStr, const std::string& fromZone, const std::string& toZone) {
    static const std::unordered_map<std::string, double> zoneOffsets = {
        {"UTC", 0}, {"GMT", 0}, {"BST", 1}, {"IST", 1}, {"WET", 0}, {"WEST", 1},
        {"CET", 1}, {"CEST", 2}, {"EET", 2}, {"EEST", 3}, {"MSK", 3}, {"MSD", 4},
        {"AST", -4}, {"ADT", -3}, {"NST", -3.5}, {"NDT", -2.5},
        {"EST", -5}, {"EDT", -4}, {"CST", -6}, {"CDT", -5},
        {"MST", -7}, {"MDT", -6}, {"PST", -8}, {"PDT", -7},
        {"HST", -10}, {"AKST", -9}, {"AKDT", -8},
        {"AEST", 10}, {"AEDT", 11}, {"ACST", 9.5}, {"ACDT", 10.5}, {"AWST", 8}
    };

    int totalMinutes = 0;

    // Parse input time to minutes since midnight.
    if (timeStr == "noon") {
        totalMinutes = 12 * 60;
    } else if (timeStr == "midnight") {
        totalMinutes = 0;
    } else {
        size_t colonPos = timeStr.find(':');
        int hours = std::stoi(timeStr.substr(0, colonPos));
        int minutesPart = std::stoi(timeStr.substr(colonPos + 1));
        std::string ampm = timeStr.substr(timeStr.find(' ') + 1); // e.g., "a.m." or "p.m."
        if (ampm == "a.m." && hours == 12) hours = 0;
        else if (ampm == "p.m." && hours != 12) hours += 12;
        totalMinutes = hours * 60 + minutesPart;
    }

    // Compute offset difference in minutes (round to handle .5 zones accurately).
    double offsetDiffHours = zoneOffsets.at(toZone) - zoneOffsets.at(fromZone);
    int offsetDiffMinutes = static_cast<int>(std::round(offsetDiffHours * 60));

    // Apply difference and normalize to [0, 1440).
    totalMinutes += offsetDiffMinutes;
    if (totalMinutes < 0) totalMinutes += 24 * 60;
    totalMinutes %= 24 * 60;

    // Convert back to 12-hour display.
    if (totalMinutes == 12 * 60) return "noon";
    if (totalMinutes == 0) return "midnight";

    int hours = totalMinutes / 60;
    int minutesPart = totalMinutes % 60;
    bool isAm = true;
    if (hours >= 12) {
        hours -= 12;
        isAm = false;
    }
    if (hours == 0) hours = 12;

    std::ostringstream out;
    out << hours << ":" << std::setfill('0') << std::setw(2) << minutesPart
        << " " << (isAm ? "a.m." : "p.m.");
    return out.str();
}
#include <cassert>
#include <string>

// Function declaration (assume from solution).
std::string convertTimeZone(const std::string&, const std::string&, const std::string&);

int main() {
    assert(convertTimeZone("noon", "UTC", "UTC") == "noon");
    assert(convertTimeZone("midnight", "UTC", "UTC") == "midnight");
    assert(convertTimeZone("3:45 p.m.", "EST", "PST") == "12:45 p.m.");
    assert(convertTimeZone("12:00 a.m.", "UTC", "BST") == "1:00 a.m.");
    assert(convertTimeZone("11:30 p.m.", "EDT", "UTC") == "3:30 a.m.");
    assert(convertTimeZone("12:30 p.m.", "NST", "NDT") == "2:30 p.m.");
    assert(convertTimeZone("6:07 a.m.", "UTC", "ACST") == "3:37 p.m.");
    assert(convertTimeZone("10:00 p.m.", "PDT", "AEDT") == "4:00 p.m.");
    assert(convertTimeZone("1:00 a.m.", "UTC", "HST") == "3:00 p.m.");
    assert(convertTimeZone("8:00 a.m.", "AKDT", "CEST") == "6:00 p.m.");
}
// The solution parses the input time into a total minute count since midnight. For "noon", that is 12*60 = 720 minutes; for "midnight", it is 0 minutes. For a normal time string like "5:07 a.m.", split by the colon, convert hours using: if "a.m." and hours==12, set hours=0; if "p.m." and hours!=12, add 12; otherwise keep hours. Then total minutes = hours*60 + minutes_part. Next, compute the time zone offset difference by looking up both abbreviations in a static `std::unordered_map<std::string, double>` mapping each abbreviation to its UTC offset in hours. Convert the difference to minutes (since offsets like -3.5 must be handled correctly, multiply the double by 60 and round to nearest integer to avoid floating point errors). Add this difference to the total minutes, then normalize by adding 24*60 if the result is negative, and take modulo 24*60 to get the new time-of-day. Finally, convert back to a 12-hour string: if total minutes is exactly 720 and minute part is 0, output "noon"; if total minutes is exactly 0, output "midnight"; otherwise compute hours = total_minutes / 60 (which will be in 0–23 after modulo), minute = total_minutes % 60; if hours is 0, set hours=12 and am=true; else if hours >= 12, subtract 12 (if becomes 0, set to 12) and am=false; else am=true. Format hours as a plain integer and minutes with `setw(2)` and `setfill('0')` (or use `printf`). The algorithm runs in O(1) time (constant number of string operations and map lookups) and uses O(1) auxiliary space beyond the static table. Edge cases include half-hour zones like NST and NDT, inputs at "noon"/"midnight", conversions that cross into the next day (e.g., 11:00 p.m. EDT to UTC becomes 3:00 a.m. the next day), and ensuring the output format uses "a.m." or "p.m." with a space before it.
