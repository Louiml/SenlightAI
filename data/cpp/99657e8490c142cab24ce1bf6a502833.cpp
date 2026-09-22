Write a C++ function `int countCaughtCriminals(int n, int startCity, const std::vector<int>& crimeFlags)` that simulates the process described in the original snippet. The function receives the number of cities `n` (cities are indexed `0` to `n-1`), the index `startCity` where the police chief is located, and a vector `crimeFlags` where `crimeFlags[i]` is `1` if a criminal is in city `i` and `0` otherwise. The chief can search cities at increasing distances from `startCity`. For each distance `d` (starting from 1 upward), he checks both `startCity - d` and `startCity + d` if they exist. If exactly one of those two cities exists, he catches the criminal in that city (if any). If both cities exist, he only catches criminals if both cities have criminals (because if one is safe and the other has a criminal, they escape together); in that case, he catches criminals in both. The chief always catches a criminal in `startCity` itself at the end. Return the total number of criminals caught. You must handle cases where `startCity` is at the edge, `n` up to 1000, and use `const` references appropriately.
The solution simulates the described process directly. For each distance `d` from 1 up to the maximum possible distance (`max(startCity, n-1-startCity)`), we check if the left city `startCity - d` exists (i.e., `d <= startCity`) and if the right city `startCity + d` exists (i.e., `startCity + d < n`). We count how many of these two cities exist (`cities`) and how many of them have criminals (sum of `crimeFlags` at those indices). If `cities == criminals`, then either both cities exist and both have criminals, or only one city exists (so its criminal count equals the count of cities, meaning that one city has a criminal) — in either case, all criminals at those positions are caught and added to the total. If `cities != criminals`, no criminals are added for that distance. After the loop, add the criminal flag at `startCity` itself. Key edge cases: when `startCity` is at one end, for many distances only one city exists, so if that city has a criminal, it is caught; when both cities exist but one has no criminal, neither is caught (because the criminal escapes with the safe person). Time complexity is O(n) because the loop runs for up to `n` distances and each iteration does O(1) work. Space complexity is O(1) auxiliary (apart from the input vector). The function uses `const` for the vector to avoid modification.
#include <vector>
#include <algorithm>

// Counts criminals caught by searching outward from startCity.
// crimeFlags[i] is 1 if a criminal is in city i, 0 otherwise.
// Returns total criminals caught.
int countCaughtCriminals(int n, int startCity, const std::vector<int>& crimeFlags) {
    int caught = 0;
    int maxDist = std::max(startCity, n - 1 - startCity);
    
    for (int d = 1; d <= maxDist; ++d) {
        int cities = 0;
        int criminals = 0;
        
        if (startCity - d >= 0) {
            ++cities;
            criminals += crimeFlags[startCity - d];
        }
        if (startCity + d < n) {
            ++cities;
            criminals += crimeFlags[startCity + d];
        }
        
        if (cities == criminals) {
            caught += criminals;
        }
    }
    
    caught += crimeFlags[startCity];
    return caught;
}
#include <cassert>
#include <vector>

// Assume countCaughtCriminals is defined above.

int main() {
    // Example from original snippet: n=6, startCity=2, arr=[1,1,1,0,1,0]
    std::vector<int> v1 = {1,1,1,0,1,0};
    assert(countCaughtCriminals(6, 2, v1) == 3);

    // All criminals, start in middle
    std::vector<int> v2 = {1,1,1};
    assert(countCaughtCriminals(3, 1, v2) == 3);

    // No criminals anywhere
    std::vector<int> v3 = {0,0,0,0};
    assert(countCaughtCriminals(4, 1, v3) == 0);

    // Start at edge, only one city to the right
    std::vector<int> v4 = {0,1,0};
    assert(countCaughtCriminals(3, 0, v4) == 1); // only city 1 caught, cities 0 and 2 are safe

    // Both sides exist but one safe: criminal escapes
    std::vector<int> v5 = {1,0,1};
    assert(countCaughtCriminals(3, 1, v5) == 0); // at d=1, both exist but only one criminal -> none caught

    // Both sides exist and both criminals: caught both
    std::vector<int> v6 = {1,0,1,0,1};
    assert(countCaughtCriminals(5, 2, v6) == 3); // d=1 catches 2 criminals, plus start has 1

    // Start at last index
    std::vector<int> v7 = {0,1,1};
    assert(countCaughtCriminals(3, 2, v7) == 2); // d=1: only left exists, has criminal -> caught, plus start

    // Single city
    std::vector<int> v8 = {1};
    assert(countCaughtCriminals(1, 0, v8) == 1);

    // Larger test: n=7, start=3, flags [0,1,0,1,1,0,0]
    std::vector<int> v9 = {0,1,0,1,1,0,0};
    // d=1: left (2) safe, right (4) criminal -> cities=2 criminals=1 -> none
    // d=2: left (1) criminal, right (5) safe -> none
    // d=3: left (0) safe, right (6) safe -> none
    // start: 1 -> total 1
    assert(countCaughtCriminals(7, 3, v9) == 1);

    return 0;
}
