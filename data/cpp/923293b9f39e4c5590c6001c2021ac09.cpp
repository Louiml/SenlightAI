Write a standalone C++ function `findClosestPair` that takes two sorted vectors of `double` values (`std::vector<double>`), each sorted in strictly ascending order, and a target `double` value. The function must return a `std::pair<double, double>` containing the pair of numbers (one from each vector) whose sum is closest to the target. If multiple pairs have the same distance, return the pair with the smaller first element (from the first vector); if still tied, return the pair with the smaller second element. The vectors are guaranteed to be non-empty, and the function must not modify the inputs. Use a two-pointer technique for efficiency.

// The task requires finding the pair (a from vector `A`, b from vector `B`) that minimizes `|a + b - target|`. Since both vectors are sorted ascending, we can use a two-pointer approach: one index starting at the beginning of `A` (pointing to the smallest element) and one index starting at the end of `B` (pointing to the largest element). At each step, compute the current sum and the absolute difference from the target. Update the best pair if the current difference is strictly smaller, or if equal and the new pair is lexicographically smaller (i.e., `a < best.first` or `a == best.first && b < best.second`). Then, if the current sum is less than the target, move the `A` pointer forward (increase the sum); if greater, move the `B` pointer backward (decrease the sum); if equal, that is the perfect match and can return immediately (since no closer pair exists). 
//
// Edge cases: Both vectors have at least one element. The vectors may contain negative numbers, so the sum can be anywhere in the real range. Duplicate values are allowed within each vector, but the vectors are strictly ascending, so no duplicates within a vector (strictly ascending implies unique). The function must handle cases where the closest sum is before the first element or after the last element; the two-pointer method inherently covers all possibilities. Time complexity is `O(n + m)` where `n` and `m` are the sizes of the two vectors. Space complexity is `O(1)` aside from the returned pair.

#include <vector>
#include <utility>
#include <cmath>
#include <limits>

// Find the pair (one from each sorted vector) whose sum is closest to target.
// Returns {a, b} where a is from A, b is from B.
// If ties, chooses smaller a, then smaller b.
std::pair<double, double> findClosestPair(
    const std::vector<double>& A,
    const std::vector<double>& B,
    double target
) {
    int left = 0;                  // index in A (smallest)
    int right = static_cast<int>(B.size()) - 1; // index in B (largest)
    
    double best_a = A[0];
    double best_b = B[0];
    double best_diff = std::abs(A[0] + B[0] - target);
    
    while (left < static_cast<int>(A.size()) && right >= 0) {
        double a = A[left];
        double b = B[right];
        double sum = a + b;
        double diff = std::abs(sum - target);
        
        // Update best if strictly better, or tie-breaking with smaller a then smaller b
        if (diff < best_diff ||
            (diff == best_diff && (a < best_a || (a == best_a && b < best_b)))) {
            best_a = a;
            best_b = b;
            best_diff = diff;
        }
        
        // If perfect match, can't do better; break
        if (sum == target) break;
        
        if (sum < target) {
            ++left; // need larger sum
        } else {
            --right; // need smaller sum
        }
    }
    
    return {best_a, best_b};
}

#include <cassert>
#include <cmath>

int main() {
    // Basic positive case
    std::vector<double> A1 = {1.0, 3.0, 5.0};
    std::vector<double> B1 = {2.0, 4.0, 6.0};
    auto r1 = findClosestPair(A1, B1, 7.0);
    assert(r1.first == 1.0 && r1.second == 6.0);
    
    // Perfect match
    std::vector<double> A2 = {-10.0, 0.0, 10.0};
    std::vector<double> B2 = {-5.0, 5.0, 20.0};
    auto r2 = findClosestPair(A2, B2, 5.0);
    assert(r2.first == 0.0 && r2.second == 5.0);
    
    // Negative values, target below all sums
    std::vector<double> A3 = {-20.0, -10.0};
    std::vector<double> B3 = {-15.0, -5.0};
    auto r3 = findClosestPair(A3, B3, 0.0);
    // sums: -35, -25, -15, -15 -> closest is -15 (either {-10,-5} or {-10,-5}? actually -10 + -5 = -15, diff=15; -20+-5=-25 diff=25; -10+-15=-25 diff=25; -20+-15=-35 diff=35)
    // Best is -10 + -5 = -15 diff 15
    assert(r3.first == -10.0 && r3.second == -5.0);
    
    // Tie-break case: sums equal distance
    std::vector<double> A4 = {0.0, 4.0};
    std::vector<double> B4 = {0.0, 4.0};
    // target=4 -> pairs: (0,4)=4 diff0, (4,0)=4 diff0 -> tie, smaller first a=0
    auto r4 = findClosestPair(A4, B4, 4.0);
    assert(r4.first == 0.0 && r4.second == 4.0);
    
    // Larger target than any sum
    std::vector<double> A5 = {1.0, 2.0};
    std::vector<double> B5 = {3.0, 4.0};
    auto r5 = findClosestPair(A5, B5, 100.0);
    // biggest sum is 2+4=6, diff=94
    assert(r5.first == 2.0 && r5.second == 4.0);
    
    // Single-element vectors
    std::vector<double> A6 = {7.5};
    std::vector<double> B6 = {-2.5};
    auto r6 = findClosestPair(A6, B6, 5.0);
    assert(r6.first == 7.5 && r6.second == -2.5);
    
    // Duplicate values across vectors but strictly ascending within each
    std::vector<double> A7 = {1.0, 2.0, 2.0}; // Actually not strictly; but spec says strictly ascending, so use {1.0,2.0}
    // Instead test with mixed signs
    std::vector<double> A8 = {-3.0, 0.0, 4.0};
    std::vector<double> B8 = {-2.0, 1.0, 5.0};
    // target=1 -> sums: -3-2=-5 diff6; -3+1=-2 diff3; -3+5=2 diff1; 0-2=-2 diff3; 0+1=1 diff0 -> perfect
    auto r8 = findClosestPair(A8, B8, 1.0);
    assert(r8.first == 0.0 && r8.second == 1.0);
    
    // Floating point near-equality: ensure not using == for double in test? Use tolerance
    // But our function uses exact diff; for demo we use simple cases
    return 0;
}
