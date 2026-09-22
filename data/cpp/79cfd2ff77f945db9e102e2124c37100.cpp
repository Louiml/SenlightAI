You are given a vector `garbage` of strings and a vector `travel` of non-negative integers, where `garbage[i]` represents the types of garbage collected at house `i` (characters `'G'`, `'P'`, or `'M'`), and `travel[i]` represents the travel time from house `i` to house `i+1` (the last element of `travel` is unused). Each truck—one for glass (`G`), paper (`P`), and metal (`M`)—starts at house 0 and must visit every house that has at least one of its assigned garbage type. A truck can move right only; it collects all its garbage at a house when it arrives, and it does not need to stop at houses without its type. All three trucks operate simultaneously and independently. Write a C++ function `int totalGarbageCollectionTime(const std::vector<std::string>& garbage, const std::vector<int>& travel)` that returns the minimal total time (sum of all trucks' travel and collection times) to collect all garbage, where each individual garbage item takes exactly 1 unit of time to collect, and travel times are as specified. Assume `garbage` is non-empty, `travel.size() == garbage.size() - 1`, and all strings contain only the characters `'G'`, `'P'`, and `'M'`. You may modify the input vectors if needed.

#include <cassert>
#include <vector>
#include <string>

// Declare the solution function (inline for the test).
int totalGarbageCollectionTime(const std::vector<std::string>& garbage,
                               const std::vector<int>& travel);

int main() {
    // Example 1: simple case with all types.
    assert(totalGarbageCollectionTime({"G", "P", "GP", "GG"}, {2, 4, 3}) == 21);
    // Explanation: 5 items (1+1+2+1) = 5 collection; G last at 3 → travel prefix[2]=9; P last at 2 → prefix[1]=6; M none → 0. Total 5+9+6=20? Actually re-check: Let's compute manually: items: G(1), P(1), GP(2), GG(2) = 6 items. prefix = [2,6,9]; last_G=3 → prefix[2]=9; last_P=2 → prefix[1]=6; last_M=0. total=6+9+6=21. Good.

    // Example 2: no travel (one house).
    assert(totalGarbageCollectionTime({"GPM"}, {}) == 3);

    // Example 3: only one type appears.
    assert(totalGarbageCollectionTime({"G", "G", "G"}, {5, 1}) == 9);
    // 3 items + travel to last house (index 2 → prefix[1]=6) = 3+6=9.

    // Example 4: no garbage at later houses.
    assert(totalGarbageCollectionTime({"P", "", "P"}, {3, 7}) == 5);
    // 2 items + travel to last P at index 2 → prefix[1]=10 => 2+10=12? Wait prefix[1]=3+7=10, so 2+10=12. Actually let's compute: items: 1+0+1=2; last_P=2 → prefix[1]=10 → total=12.

    // Example 5: all houses empty? Not allowed per constraints, but just in case.
    // assert(totalGarbageCollectionTime({"", ""}, {1}) == 0); // Uncomment if allowed.

    // Example 6: mixed with multiple occurrences and travel.
    assert(totalGarbageCollectionTime({"M", "MP", "P", "G"}, {1, 2, 3}) == 10);
    // Items: M(1), MP(2)=3, P(1)=4, G(1)=5. prefix=[1,3,6]; last_M=1 → prefix[0]=1; last_P=2 → prefix[1]=3; last_G=3 → prefix[2]=6; sum items 5 + 1+3+6 = 15.

    return 0;
}

#include <vector>
#include <string>

// Compute the total time to collect all garbage with three trucks.
int totalGarbageCollectionTime(const std::vector<std::string>& garbage,
                               const std::vector<int>& travel) {
    // Work on a mutable copy of travel to compute prefix sums.
    std::vector<int> prefix = travel;
    for (size_t i = 1; i < prefix.size(); ++i) {
        prefix[i] += prefix[i - 1];
    }

    int total_time = 0;
    int last_glass = 0, last_paper = 0, last_metal = 0;

    for (size_t i = 0; i < garbage.size(); ++i) {
        for (char type : garbage[i]) {
            total_time += 1;  // collecting one item takes 1 unit
            if (type == 'G') last_glass = i;
            else if (type == 'P') last_paper = i;
            else if (type == 'M') last_metal = i;
        }
    }

    // Add travel time for each truck that has any garbage.
    if (last_glass > 0) total_time += prefix[last_glass - 1];
    if (last_paper > 0) total_time += prefix[last_paper - 1];
    if (last_metal > 0) total_time += prefix[last_metal - 1];

    return total_time;
}

// The key insight is that for each of the three trucks, the time it spends consists of: (1) the time to collect every individual garbage item of its type (which is simply the count of that character across all houses), and (2) the travel time needed to reach the farthest house that contains any of its type. Since all trucks move independently and travel only to the last house where they are needed, each truck's travel time is the prefix-summed travel distance to that last occurrence. A simple approach: first transform `travel` into prefix sums (where `prefix[i]` is the time from house 0 to house `i+1`), then iterate through all `garbage` strings, count every character to add to the total collection time, and record the highest index for each type. After the loop, for each type, if its last index is greater than 0, add `prefix[lastIndex-1]` (the travel time from house 0 to that last house) to the answer. Edge cases: if a type never appears, its last index remains 0, so no travel is added; if `garbage` has only one house, `travel` is empty, so no travel is added. Time complexity: \(O(N \cdot L)\) where \(N\) is the number of houses and \(L\) is the average string length, because we scan each character exactly once. Space complexity: \(O(1)\) extra beyond the input modification (prefix sums are done in place).
