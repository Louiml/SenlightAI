// Write a C++ function `std::vector<int> fridayThe13ths(int firstDayOfYear)` that, given the day of the week on which January 1 falls (where 0 = Sunday, 1 = Monday, ..., 6 = Saturday), returns a list of the month numbers (1–12) in which the 13th day of the month falls on a Friday. The function should consider a non‑leap year (February has 28 days). The month lengths are standard: Jan 31, Feb 28, Mar 31, Apr 30, May 31, Jun 30, Jul 31, Aug 31, Sep 30, Oct 31, Nov 30, Dec 31. The returned vector should contain the month numbers in increasing order from 1 to 12 that satisfy the condition. If no month has a Friday the 13th, return an empty vector. The input is guaranteed to be an integer between 0 and 6 inclusive.
#include <cassert>
#include <vector>

// Solution function is assumed to be defined above (not reproduced here).

int main() {
    // January 1 = Sunday (0). Then Jan 13 = Sunday+12 = 12%7=5 (Friday) -> Jan.
    // Feb 13: add 31 days -> (5+31)%7 = (5+3)=8%7=1 (Monday). Mar: add 28 -> (1+0)=1.
    // Apr: add 31 -> (1+3)=4. May: add 30 -> (4+2)=6. Jun: add 31 -> (6+3)=2.
    // Jul: add 30 -> (2+2)=4. Aug: add 31 -> (4+3)=0. Sep: add 31 -> (0+3)=3.
    // Oct: add 30 -> (3+2)=5 -> Oct. Nov: add 31 -> (5+3)=1. Dec: add 30 -> (1+2)=3.
    std::vector<int> r0 = fridayThe13ths(0);
    assert((r0 == std::vector<int>{1, 10}));
    
    // January 1 = Monday (1). Jan 13 = (1+12)%7=13%7=6 (Saturday). None?
    // Feb: add 31 -> (6+3)=2. Mar: add28 ->2. Apr: +31=5 (Friday) -> Apr.
    // May: +30=0. Jun: +31=3. Jul: +30=5 -> Jul. Aug: +31=1. Sep: +31=4.
    // Oct: +30=6. Nov: +31=2. Dec: +30=4.
    std::vector<int> r1 = fridayThe13ths(1);
    assert((r1 == std::vector<int>{4, 7}));
    
    // January 1 = Tuesday (2). Jan 13 = (2+12)%7=0 (Sunday). Feb: +31=3.
    // Mar: +28=3. Apr: +31=6. May: +30=1. Jun: +31=4. Jul: +30=6.
    // Aug: +31=2. Sep: +31=5 (Friday) -> Sep. Oct: +30=0. Nov: +31=3. Dec: +30=5 (Friday) -> Dec.
    std::vector<int> r2 = fridayThe13ths(2);
    assert((r2 == std::vector<int>{9, 12}));
    
    // January 1 = Wednesday (3). Jan 13 = (3+12)%7=1 (Monday). Feb: +31=4.
    // Mar: +28=4. Apr: +31=0. May: +30=2. Jun: +31=5 (Friday) -> Jun.
    // Jul: +30=0. Aug: +31=3. Sep: +31=6. Oct: +30=1. Nov: +31=4. Dec: +30=6.
    std::vector<int> r3 = fridayThe13ths(3);
    assert((r3 == std::vector<int>{6}));
    
    // January 1 = Thursday (4). Jan 13 = (4+12)%7=2 (Tuesday). Feb: +31=5 (Friday) -> Feb.
    // Mar: +28=5 (Friday) -> Mar. Apr: +31=1. May: +30=3. Jun: +31=6.
    // Jul: +30=1. Aug: +31=4. Sep: +31=0. Oct: +30=2. Nov: +31=5 (Friday) -> Nov.
    // Dec: +30=0.
    std::vector<int> r4 = fridayThe13ths(4);
    assert((r4 == std::vector<int>{2, 3, 11}));
    
    // January 1 = Friday (5). Jan 13 = (5+12)%7=3 (Wednesday). Feb: +31=6.
    // Mar: +28=6. Apr: +31=2. May: +30=4. Jun: +31=0. Jul: +30=2.
    // Aug: +31=5 (Friday) -> Aug. Sep: +31=1. Oct: +30=3. Nov: +31=6. Dec: +30=1.
    std::vector<int> r5 = fridayThe13ths(5);
    assert((r5 == std::vector<int>{8}));
    
    // January 1 = Saturday (6). Jan 13 = (6+12)%7=4 (Thursday). Feb: +31=0.
    // Mar: +28=0. Apr: +31=3. May: +30=5 (Friday) -> May. Jun: +31=1.
    // Jul: +30=3. Aug: +31=6. Sep: +31=2. Oct: +30=4. Nov: +31=0. Dec: +30=2.
    std::vector<int> r6 = fridayThe13ths(6);
    assert((r6 == std::vector<int>{5}));
    
    return 0;
}
#include <vector>

// Return month numbers (1-12) where the 13th falls on a Friday.
// firstDayOfYear: 0=Sunday, 1=Monday, ..., 6=Saturday.
std::vector<int> fridayThe13ths(int firstDayOfYear) {
    // Days in each month for a non-leap year (index 0 = January).
    const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    std::vector<int> result;
    
    // Weekday of January 13th.
    int weekdayOf13 = (firstDayOfYear + 12) % 7; // 0=Sunday ... 6=Saturday, Friday=5
    
    for (int month = 1; month <= 12; ++month) {
        if (weekdayOf13 == 5) {
            result.push_back(month);
        }
        // Shift to next month: add days of current month (not used after December).
        if (month < 12) {
            weekdayOf13 = (weekdayOf13 + daysInMonth[month - 1]) % 7;
        }
    }
    
    return result;
}
// The approach simulates the day of the week for the 13th day of each month. Start by determining the weekday of January 13th: since January 1st is `firstDayOfYear`, January 13th is `(firstDayOfYear + 12) % 7`. If that equals 5 (Friday), add month 1 to the result. Then for each subsequent month, add the number of days in the previous month to the current weekday offset, then check the 13th. More precisely, maintain a variable `weekdayOf13` that holds the day of the week for the 13th of the current month. Initially set it to `(firstDayOfYear + 12) % 7`. For month i (from 2 to 12), update `weekdayOf13 = (weekdayOf13 + daysInPreviousMonth) % 7` because adding the previous month's length shifts the weekday of the same day number (13) by that many days mod 7. Then check if it equals 5. This avoids recomputing from scratch for each month. Edge cases: when firstDayOfYear is any valid value; February always has 28 days (non‑leap). Complexity: O(12) time, O(1) extra space (plus the result vector which may hold up to 12 integers).
