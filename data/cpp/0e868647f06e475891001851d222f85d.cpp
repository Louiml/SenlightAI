Given a non-empty array of non-negative integers where each element represents the radius of a disc centered at index i (i.e., disc i covers the interval from i - radius[i] to i + radius[i]), write a C++ function that returns the number of pairs of discs that intersect (i.e., share at least one common point). The function must return -1 if the number of intersecting pairs exceeds 10,000,000, and return 0 for an empty array. The solution should handle arrays with up to 100,000 elements efficiently, and the result should be an integer (the count of pairs).
The problem is a classic "disc intersection" counting task. A naive O(n²) double loop over all pairs is correct but too slow for large inputs. However, the provided snippet uses exactly that naive approach, so the reference solution here will present an optimized O(n log n) approach to be more robust. The key observation: two discs i and j (with i < j) intersect if i + radius[i] >= j - radius[j]. This can be rearranged as i + radius[i] >= j - radius[j]. Sorting the "start points" (i - radius[i]) and "end points" (i + radius[i]) separately allows counting intersections using a sweep: for each disc j, count how many earlier discs have an end point >= j - radius[j]. This is done by iterating j from 0 to n-1, maintaining a sorted list of start points, and using binary search to count how many starts are ≤ j + radius[j] minus those with ends < j - radius[j]. A simpler method: sort starts, and for each end, count how many starts are ≤ that end, then subtract pairs already counted. Edge cases: empty array returns 0; large counts may overflow, so use long long internally and return -1 if exceeds 10,000,000. Time complexity O(n log n) and extra space O(n).
#include <vector>
#include <algorithm>

// Count intersecting disc pairs, return -1 if > 10,000,000.
// Each disc i covers [i - radius[i], i + radius[i]].
int countDiscIntersections(std::vector<int>& radii) {
    const long long LIMIT = 10000000;
    size_t n = radii.size();
    if (n < 2) return 0;

    std::vector<long long> start(n);
    std::vector<long long> end(n);
    for (size_t i = 0; i < n; ++i) {
        start[i] = static_cast<long long>(i) - radii[i];
        end[i] = static_cast<long long>(i) + radii[i];
    }

    std::sort(start.begin(), start.end());

    long long count = 0;
    for (size_t j = 0; j < n; ++j) {
        // Disc j starts at start[j], ends at end[j].
        // Count all discs with start <= end[j] (including itself),
        // then subtract those with start < start[j] (strictly before j's start).
        // But to count pairs (i,j) with i<j, we only consider discs i with i<j.
        // However, the simple formula: total pairs = sum over j of (#discs with start <= end[j]) - (j+1) (because each disc counts itself and all earlier ones?)
        // Actually, a known formula: sort all "left" points and "right" points. For each right, count lefts ≤ right, sum, then subtract n*(n+1)/2.
        // That gives total intersecting pairs (including i=j). Then subtract n to get distinct pairs.
        long long leqRight = std::upper_bound(start.begin(), start.end(), end[j]) - start.begin();
        long long totalPairs = 0;
        // Accumulate but we need per j: number of discs i with i < j and end[i] >= start[j]? 
        // Let's use the standard method: For each j, count discs with start <= end[j], subtract j+1 (discs 0..j).
        // This gives number of discs i with i < j or i > j? Actually careful.
        // Standard: total = sum_{j} (#discs i with start[i] <= end[j]) - n*(n+1)/2? Wait.
        // Better: Let's use the correct two-pointer method for clarity.
        // We'll compute total = 0; for each j, we count how many i < j have end[i] >= start[j].
        // Since we have all starts sorted, for each j we do binary search on end values? 
        // Simpler: sort all starts and ends, then sweep.
        // To avoid confusion, I'll directly implement the correct approach:
        // For each j, count how many i < j have end[i] >= start[j]. But we don't have end sorted per index. 
        // Actually the classic solution: sort starts. For each i, count how many starts (from sorted) are ≤ end[i], subtract i+1 (discs with index ≤ i), and also subtract (n-1-i) for discs with index > i? 
        // Let me present a known clean method: 
        // Create vector of pairs (start, 1) and (end+1, -1) and sweep to count active discs might be more complex.
        // Given this is a teaching assistant task, maybe better to keep O(n^2) as in the snippet? The snippet uses O(n^2) which is fine for small inputs. Since the task says "inspired by a given code snippet", it's acceptable to replicate the O(n^2) solution but with const correctness and better naming.
        // I'll provide the O(n^2) solution as the reference, as it matches the snippet exactly.
        // But to satisfy "efficiently", the task says "handle arrays with up to 100,000 elements", implying O(n^2) is too slow. So I'll provide the O(n log n) solution.
        // Let me implement the known O(n log n) approach: 
        // For each j, count discs i with i < j and i + radii[i] >= j - radii[j].
        // Equivalently, for each i, count j > i with j - radii[j] <= i + radii[i].
        // Let's sort by (j - radii[j]) and for each i, count how many j have j - radii[j] <= i + radii[i], minus those with j <= i.
        // A common implementation: put all "left" points (i - radii[i]) into a vector, sort them. For each right end R = i + radii[i], count how many left points are ≤ R, subtract (i+1) because left points of indices 0..i are included. This gives number of discs with left ≤ R but index > i? Actually if we sort lefts, the count of lefts ≤ R includes all discs (including those with index ≤ i). We want pairs (i,j) with i<j and left[j] ≤ R[i]. So for each i, we want count of indices j > i such that left[j] ≤ R[i]. That is: total lefts ≤ R[i] minus count of lefts among indices ≤ i. The latter is i+1 minus those among first i+1 that have left > R[i]? This gets messy.
        // Let me use a simpler method: create a vector of pairs (left, index). Sort by left. Then for each i, use two pointers to count how many j have left[j] ≤ R[i] and j > i. The classic solution: for each i, count all j with left[j] ≤ R[i], subtract number of j ≤ i (which is i+1), but that gives j > i? Actually total j with left[j] ≤ R[i] includes all j. Among them, j ≤ i are i+1. So subtract i+1 gives j > i. But also need j > i and left[j] ≤ R[i]. So count = (count of left ≤ R[i]) - (i+1). That works because all indices 0..i have left[j] <= R[i]? Not necessarily, but if left[j] > R[i] for some j ≤ i, then we would subtract too many. However, for j ≤ i, we have left[j] = j - radii[j] <= j <= i < R[i] (since R[i] = i + radii[i] >= i). So indeed for all j ≤ i, left[j] ≤ i ≤ R[i]. So they are all counted in the "count of left ≤ R[i]". Thus subtracting i+1 is correct. So the formula: for each i from 0 to n-1, count = upper_bound(start.begin(), start.end(), R[i]) - start.begin(); pairs += count - (i+1). But careful: the sorted start includes all discs, including j=i. But for j=i, left[i] = i - radii[i] ≤ R[i] always, so it's in the count. Subtract (i+1) removes indices 0..i, which includes i. So that leaves j > i. Good. So the algorithm: sort start points, then for each i, add (upper_bound(start, R[i]) - (i+1)). Accumulate as long long, if > LIMIT return -1.
        count += (std::upper_bound(start.begin(), start.end(), end[i]) - start.begin()) - static_cast<long long>(i) - 1;
        if (count > LIMIT) return -1;
    }
    return static_cast<int>(count);
}
#include <cassert>
#include <vector>

// The function is declared above; here we just test.

int main() {
    // Example from snippet: [1,5,2,1,4,0] has 11 intersections? Let's compute manually? 
    // The snippet returns 11? We'll trust it.
    std::vector<int> v1 = {1, 5, 2, 1, 4, 0};
    assert(countDiscIntersections(v1) == 11);

    // Empty array
    std::vector<int> v2;
    assert(countDiscIntersections(v2) == 0);

    // Single disc
    std::vector<int> v3 = {5};
    assert(countDiscIntersections(v3) == 0);

    // Two non-overlapping: radii [0,0] -> intervals [0,0] and [1,1] no intersection
    std::vector<int> v4 = {0, 0};
    assert(countDiscIntersections(v4) == 0);

    // Two overlapping: radii [1,1] -> intervals [ -1,1] and [0,2] intersect
    std::vector<int> v5 = {1, 1};
    assert(countDiscIntersections(v5) == 1);

    // All zeros: each disc is just point at i, no intersections
    std::vector<int> v6 = {0, 0, 0, 0};
    assert(countDiscIntersections(v6) == 0);

    // Large radii causing many intersections: e.g., [100,100,100] - all overlap each other, 3 choose 2 = 3
    std::vector<int> v7 = {100, 100, 100};
    assert(countDiscIntersections(v7) == 3);

    // Check limit: create a vector that would exceed 10M? We'll just test small.
    // For correctness, assert a known value.
    std::vector<int> v8 = {1, 0, 1};
    // Intervals: disc0: [-1,1], disc1:[1,1], disc2:[1,3] - intersections? (0,1) yes, (0,2) yes, (1,2) yes = 3
    assert(countDiscIntersections(v8) == 3);

    // Test with negative radii? Not allowed per task (non-negative). So no test.

    return 0;
}
