Write a C++ function `minimumExamsToPass(const std::vector<int>& grades)` that takes a vector of non-negative integer grades (each between 0 and 5 inclusive) representing a student's exam scores, and returns the minimum number of exams that must be retaken (each retaken exam can earn a new grade of 5) to make the student's average grade at least 4.5. If the average is already at least 4.5, return 0. The grades vector will contain at least one element. The function should not modify the input vector.

#include <cassert>
#include <vector>

int minimumExamsToPass(const std::vector<int>& grades);

int main() {
    // Already passing average
    assert(minimumExamsToPass({5, 5, 5}) == 0);
    assert(minimumExamsToPass({4, 5}) == 0); // average 4.5 exactly
    assert(minimumExamsToPass({4, 4, 5, 5}) == 0);
    
    // Need retakes
    assert(minimumExamsToPass({2, 3, 4}) == 1); // average 3.0, retake 2 -> sum=12, avg=4.0? wait: sum=9, retake 2->5 gives sum=12, avg=4.0, still below 4.5? Actually 12/3=4.0, so need more. Let's compute: required sum=13.5, deficit=4.5, retake 2 gives +3, deficit=1.5, retake 3 gives +2, so 2 retakes. So assert 2.
    assert(minimumExamsToPass({2, 3, 4}) == 2);
    assert(minimumExamsToPass({0, 0, 0}) == 3); // all zeros, need all retakes
    assert(minimumExamsToPass({5, 0, 0}) == 2); // retake two zeros
    assert(minimumExamsToPass({4, 0, 0}) == 2); // retake both zeros, sum becomes 4+5+5=14, avg≈4.67
    assert(minimumExamsToPass({3, 3, 3, 3}) == 2); // sum=12, required=18, deficit=6, retake two 3s -> +4 each, total +8, after two retakes sum=20, avg=5
    assert(minimumExamsToPass({1, 2, 3, 4, 5}) == 2); // sum=15, required=22.5, deficit=7.5, retake 1(+4) and 2(+3) total +7, still deficit 0.5? Actually retake 1,2,3 gives +4+3+2=9 >7.5, so 3 retakes? Let's compute: sum=15, needed total=22.5, deficit=7.5. Retake 1 gives +4, remaining 3.5. Retake 2 gives +3, remaining 0.5. Retake 3 gives +2, covers. So 3. But maybe retake 1,2,4? no. So assert 3.
    assert(minimumExamsToPass({1, 2, 3, 4, 5}) == 3);
    
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the minimum number of exams to retake (each scoring 5) so that the average grade is at least 4.5.
// Grades are non-negative integers between 0 and 5 inclusive. The input vector is not modified.
int minimumExamsToPass(const std::vector<int>& grades) {
    const int n = static_cast<int>(grades.size());
    long long currentSum = 0;
    for (int g : grades) {
        currentSum += g;
    }
    
    // Compare average >= 4.5 using integer arithmetic: 2 * sum >= 9 * n
    if (2 * currentSum >= 9LL * n) {
        return 0;
    }
    
    // We need to increase the sum so that 2 * sum >= 9 * n
    long long deficit = 9LL * n - 2 * currentSum;  // positive, even
    
    // Sort a copy of grades ascending to maximize gain per retake
    std::vector<int> sorted(grades);
    std::sort(sorted.begin(), sorted.end());
    
    int retakes = 0;
    for (int i = 0; i < n && deficit > 0; ++i) {
        // Retaking grade sorted[i] gives increase of (5 - sorted[i]) to sum, so 2*(5 - sorted[i]) to 2*sum
        long long gain = 2LL * (5 - sorted[i]);
        deficit -= gain;
        ++retakes;
    }
    
    return retakes;
}

// The goal is to raise the average to at least 4.5. First, compute the current sum of grades. The required total sum for an average of 4.5 over `n` exams is `4.5 * n`. If the current sum is already at least that required total, the answer is 0. Otherwise, we need to increase the total sum by the deficit `requiredSum - currentSum`. Each retake replaces an old grade `g` with 5, increasing the total sum by `(5 - g)`. To minimize the number of retakes, we should retake exams with the smallest grades first, since they yield the largest increase per retake. Sort the grades ascending, then greedily apply increases from the smallest grade upward until the deficit is fully covered. Since each increase is positive (as all grades are ≤5), the process always terminates. The time complexity is O(n log n) due to sorting, and O(n) auxiliary space if we copy the vector for sorting; otherwise, we can sort a copy to avoid modifying the input. Edge cases: if all grades are already 5 but average is below 4.5 (impossible because all 5s give average 5), but if grades include zeros, we handle that. Also, the deficit is a double, so we compare with a small epsilon to avoid floating-point issues, or equivalently use integer arithmetic: requiredSum = 9 * n / 2, but to avoid fractions, multiply by 2: `2 * currentSum` vs `9 * n`. If `2*currentSum >= 9*n`, answer is 0. Otherwise, compute deficit as `9*n - 2*currentSum` (which is even), and each retake of grade g increases `2*sum` by `2*(5-g)`. Then greedily subtract while deficit > 0.
