// Write a C++ function named `isInExclusionPeriod` that accepts a `boost::gregorian::date` object and a `std::set<boost::gregorian::date_period>` representing exclusion intervals (weekends/holidays) and returns a `bool` indicating whether the given date falls within any exclusion period. Use the `contains` method of `date_period` for interval containment checks. The function must handle empty exclusion sets gracefully, and the date must be compared against all periods in the set. No `main` function should be included in the solution; only the free function with proper headers and documentation.
#include <cassert>
#include <set>
#include <boost/gregorian/gregorian.hpp>

bool isInExclusionPeriod(const boost::gregorian::date& d,
                         const std::set<boost::gregorian::date_period>& exclusions);

int main() {
    using namespace boost::gregorian;

    // Build the same exclusion set as the original snippet
    std::set<date_period> exclusions = {
        date_period(date(2002, Feb, 2), date(2002, Feb, 4)), // 2nd–3rd inclusive of begin
        date_period(date(2002, Feb, 9), date(2002, Feb, 11)),
        date_period(date(2002, Feb, 16), date(2002, Feb, 18)),
        date_period(date(2002, Feb, 23), date(2002, Feb, 25)),
        date_period(date(2002, Feb, 12), date(2002, Feb, 13))
    };

    // Dates inside the periods (begin or middle)
    assert(isInExclusionPeriod(date(2002, Feb, 2), exclusions) == true);
    assert(isInExclusionPeriod(date(2002, Feb, 3), exclusions) == true);
    assert(isInExclusionPeriod(date(2002, Feb, 12), exclusions) == true); // holiday
    assert(isInExclusionPeriod(date(2002, Feb, 16), exclusions) == true);

    // Dates equal to the end date are NOT inside (half-open interval)
    assert(isInExclusionPeriod(date(2002, Feb, 4), exclusions) == false);
    assert(isInExclusionPeriod(date(2002, Feb, 18), exclusions) == false);

    // Dates outside all periods
    assert(isInExclusionPeriod(date(2002, Feb, 5), exclusions) == false);
    assert(isInExclusionPeriod(date(2002, Feb, 14), exclusions) == false);
    assert(isInExclusionPeriod(date(2002, Feb, 27), exclusions) == false);

    // Empty set case
    std::set<date_period> empty;
    assert(isInExclusionPeriod(date(2002, Feb, 2), empty) == false);

    return 0;
}
#include <set>
#include <boost/gregorian/gregorian.hpp>

// Checks if the given date lies within any of the exclusion periods.
// The periods are half-open intervals [begin, end), so a date equal to
// the begin date is excluded, but a date equal to the end date is not.
bool isInExclusionPeriod(const boost::gregorian::date& d,
                         const std::set<boost::gregorian::date_period>& exclusions) {
    for (const auto& period : exclusions) {
        if (period.contains(d)) {
            return true;
        }
    }
    return false;
}
// The core algorithm is straightforward: iterate over every `date_period` in the provided set, and for each period, call its `contains(date)` method. The `date_period::contains` returns `true` if the interval is half‑open `[begin, end)` (i.e., includes the begin date but excludes the end date) — this matches the original behavior of the snippet. The function returns `true` as soon as any period contains the date; otherwise, after exhausting all periods, it returns `false`. Edge cases: an empty set immediately returns `false`; a date exactly equal to a period’s begin date is contained, but a date equal to the end date is not (due to half‑open semantics). Time complexity is \(O(n)\), where \(n\) is the number of periods in the set, since each period is examined once. Space complexity is \(O(1)\) auxiliary, as no additional storage is allocated beyond the loop counter and temporary references.
