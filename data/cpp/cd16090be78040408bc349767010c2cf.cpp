/*
Write a C++ function that finds all times in a 24-hour period (from 00:00 to 23:59) where the sum of the squares of the hour and minute digits equals the numeric value formed by the four-digit representation. For example, if the hour is `h` and the minute is `m`, the condition is `h*h + m*m == h*100 + m`. The function should return a vector of strings, each formatted as `"HH:MM"` with leading zeros, containing all valid solutions in chronological order. The function should handle edge cases such as midnight (00:00), times with single-digit hours or minutes, and must correctly compute the four-digit value (e.g., 9:05 becomes 905, not 905). The function must be const-correct and should not modify any external state.
*/
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Finds all times (HH:MM) in a 24-hour period where h^2 + m^2 equals hhmm.
std::vector<std::string> findSpecialTimes() {
    std::vector<std::string> solutions;
    for (int h = 0; h < 24; ++h) {
        for (int m = 0; m < 60; ++m) {
            const int timeValue = h * 100 + m;
            const int sumOfSquares = h * h + m * m;
            if (sumOfSquares == timeValue) {
                std::ostringstream oss;
                oss << std::setfill('0') << std::setw(2) << h << ":"
                    << std::setfill('0') << std::setw(2) << m;
                solutions.push_back(oss.str());
            }
        }
    }
    return solutions;
}
#include <cassert>
#include <string>
#include <vector>

// Forward declaration for clarity (the actual implementation is above).
std::vector<std::string> findSpecialTimes();

int main() {
    std::vector<std::string> result = findSpecialTimes();
    // Expected solutions based on manual verification.
    std::vector<std::string> expected = {"00:00", "00:01", "12:00"};
    // Additional known solution: 20:00? Check: 20^2+0^2=400, but 2000 != 400, so no.
    // Let's assert the size and a few known entries. For full correctness, we verify all known solutions.
    // Known true solutions: 00:00, 00:01, 12:00, 20:00? No, 20^2=400, 2000 != 400.
    // Let's compute exactly: 0^2+0^2=0, 0*100+0=0 -> yes. 0^2+1^2=1, 0*100+1=1 -> yes. 
    // 1^2+0^2=1, 100 ≠ 1 -> no. 1^2+1^2=2, 101 ≠ 2 -> no. 2^2+0^2=4, 200 ≠ 4.
    // 3^2+0^2=9, 300 ≠ 9. 4^2+0^2=16, 400 ≠ 16. 5^2+0^2=25, 500 ≠ 25. 6^2=36, 600≠36.
    // 7^2=49, 700≠49. 8^2=64, 800≠64. 9^2=81, 900≠81. 10^2=100, 1000≠100. 
    // 11^2=121, 1100≠121. 12^2+0^2=144, 1200≠144. Wait, but some solutions exist beyond 00:01.
    // Let's brute-force mentally: For h=0, m^2 == m -> m=0 or 1. So 00:00, 00:01.
    // For h=1, m^2 +1 == 100+m -> m^2 - m -99=0 -> discriminant 1+396=397, not perfect square.
    // For h=2, m^2+4 == 200+m -> m^2 -m -196=0 -> disc 1+784=785, no.
    // For h=3, m^2+9==300+m -> m^2 -m -291, disc 1+1164=1165, no.
    // For h=4, m^2+16==400+m -> m^2 -m -384, disc 1+1536=1537, no.
    // For h=5, m^2+25==500+m -> m^2 -m -475, disc 1+1900=1901, no.
    // For h=6, m^2+36==600+m -> m^2 -m -564, disc 1+2256=2257, no.
    // For h=7, m^2+49==700+m -> m^2 -m -651, disc 1+2604=2605, no.
    // For h=8, m^2+64==800+m -> m^2 -m -736, disc 1+2944=2945, no.
    // For h=9, m^2+81==900+m -> m^2 -m -819, disc 1+3276=3277, no.
    // For h=10, m^2+100==1000+m -> m^2 -m -900, disc 1+3600=3601, no (sqrt 60.008).
    // For h=11, m^2+121==1100+m -> m^2 -m -979, disc 1+3916=3917, no.
    // For h=12, m^2+144==1200+m -> m^2 -m -1056, disc 1+4224=4225=65^2! -> m=(1+65)/2=33. So 12:33 works.
    // Check: 12^2+33^2=144+1089=1233, yes! So 12:33 is a solution.
    // For h=20, m^2+400==2000+m -> m^2 -m -1600, disc 1+6400=6401, no.
    // For h=21, m^2+441==2100+m -> m^2 -m -1659, disc 1+6636=6637, no.
    // For h=22, m^2+484==2200+m -> m^2 -m -1716, disc 1+6864=6865, no.
    // For h=23, m^2+529==2300+m -> m^2 -m -1771, disc 1+7084=7085, no.
    // So total solutions: 00:00, 00:01, 12:33.
    assert(result.size() == 3);
    assert(result[0] == "00:00");
    assert(result[1] == "00:01");
    assert(result[2] == "12:33");
    return 0;
}
// The solution iterates through all possible hours from 0 to 23 and minutes from 0 to 59. For each combination, it computes the four-digit numeric value as `h * 100 + m` and compares it to `h*h + m*m`. If they are equal, the time is formatted into a string using `ostringstream` with `setw(2)` and `setfill('0')` to ensure two-digit formatting for both hour and minute, separated by a colon. The algorithm checks all 24 * 60 = 1440 possible combinations, so it runs in O(1440) time, which is effectively O(1) since the input domain is fixed. Space complexity is O(k) where k is the number of solutions found, since the return vector grows linearly with the number of valid times. Edge cases include when `h` or `m` is less than 10, requiring leading zeros in the output string, and when the sum of squares equals the numeric value without leading zeros (e.g., 0:00 yields 0, but we format as "00:00").
