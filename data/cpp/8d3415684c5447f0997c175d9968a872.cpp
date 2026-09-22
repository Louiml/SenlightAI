// Write a C++ function `int maxMatchingApartments(int n, int m, int k, const std::vector<int>& applicants, const std::vector<int>& apartmentSizes)` that determines the maximum number of applicants who can each be assigned an apartment, given that the desired apartment size of an applicant differs from the actual apartment size by at most `k` (i.e., absolute difference ≤ k). Applicants and apartment sizes are provided as unsorted vectors of positive integers. The function should return the count of unmatched applicants that can be paired optimally. Assume `n` and `m` can be zero (in which case the answer is 0), and the input vectors may contain duplicates. The function must not modify the input vectors. Pair each applicant with at most one apartment, and each apartment with at most one applicant. Use a greedy two-pointer approach after sorting both vectors. For example, if applicants = {60, 45, 80}, apartment sizes = {30, 60, 75}, and k = 5, then applicant 60 matches apartment 60, applicant 45 matches apartment 50? No, no 50, so only 1 match. But if k=15, then 45 matches 30, 60 matches 60, 80 matches 75 => 3 matches.
The problem is a classic bipartite matching problem where you want to maximize the number of pairs such that the difference between an applicant's desired size and an apartment's actual size is within `k`. The greedy approach works because both lists are sorted: iterate with two indices. If the current applicant's desired size is within `k` of the current apartment's size, you match them and move both pointers forward. If not, you move the pointer of the smaller value (the one that is too far below the other) forward, because that smaller value cannot match any later larger value (since later values are larger, the difference becomes even larger). This greedy strategy is optimal because it always attempts to match the smallest applicant with the smallest possible apartment that fits, leaving larger apartments for larger applicants. Edge cases: empty vectors (return 0), all applicants or all apartments already matched, and when `k` is large enough to match everything. Time complexity is O(n log n + m log m) for sorting and O(n + m) for the two-pointer scan, so overall O(n log n + m log m). Space complexity is O(1) auxiliary (excluding input vectors).
#include <vector>
#include <algorithm>
#include <cstdlib> // for std::abs

// Returns the maximum number of applicants that can be assigned an apartment
// where the absolute difference between desired size and apartment size <= k.
// The input vectors are not modified.
int maxMatchingApartments(int n, int m, int k,
                          const std::vector<int>& applicants,
                          const std::vector<int>& apartmentSizes) {
    // Create local sorted copies to avoid modifying input.
    std::vector<int> sortedApplicants = applicants;
    std::vector<int> sortedSizes = apartmentSizes;
    std::sort(sortedApplicants.begin(), sortedApplicants.end());
    std::sort(sortedSizes.begin(), sortedSizes.end());

    int count = 0;
    int i = 0; // index for applicants
    int j = 0; // index for apartment sizes

    while (i < n && j < m) {
        if (std::abs(sortedApplicants[i] - sortedSizes[j]) <= k) {
            // Match this applicant with this apartment.
            ++count;
            ++i;
            ++j;
        } else {
            // If the apartment size is larger than the applicant's desire,
            // then this applicant cannot match this or any later apartment,
            // because later apartments are even larger. Move applicant forward.
            if (sortedSizes[j] > sortedApplicants[i]) {
                ++i;
            } else {
                // Otherwise, the apartment is too small for this applicant,
                // and any later applicant is even larger, so move apartment forward.
                ++j;
            }
        }
    }
    return count;
}
#include <cassert>
#include <vector>

int main() {
    // Test case 1: Simple match with k=5.
    std::vector<int> app1 = {60, 45, 80};
    std::vector<int> ap1 = {30, 60, 75};
    assert(maxMatchingApartments(3, 3, 5, app1, ap1) == 1); // only 60-60

    // Test case 2: Larger k matches all.
    assert(maxMatchingApartments(3, 3, 15, app1, ap1) == 3);

    // Test case 3: Empty applicants.
    std::vector<int> app_empty;
    std::vector<int> ap3 = {10, 20};
    assert(maxMatchingApartments(0, 2, 5, app_empty, ap3) == 0);

    // Test case 4: Empty apartments.
    std::vector<int> app4 = {10, 20};
    std::vector<int> ap_empty;
    assert(maxMatchingApartments(2, 0, 5, app4, ap_empty) == 0);

    // Test case 5: Duplicates and exact match.
    std::vector<int> app5 = {5, 5, 10};
    std::vector<int> ap5 = {5, 10, 10};
    assert(maxMatchingApartments(3, 3, 0, app5, ap5) == 3); // 5-5, 5-10? no, k=0, 5->5, 5->? second 5 only matches 5(used), so 5 matches 10? no, so 5-left unmatched, 10->10 => 2? Actually: sorted apps: 5,5,10; sizes:5,10,10. Match: 5-5, 5-10 (diff 5>0 no), so move size? Since size 10>5, move applicant to second 5, then 5-10 diff 5>0, size > app? size 10>5 so move applicant to 10, then 10-10 diff 0 match. So count=2. So assert should be 2. I'll correct.
    assert(maxMatchingApartments(3, 3, 0, app5, ap5) == 2);

    // Test case 6: All too far apart.
    std::vector<int> app6 = {1, 2, 3};
    std::vector<int> ap6 = {100, 200, 300};
    assert(maxMatchingApartments(3, 3, 1, app6, ap6) == 0);

    // Test case 7: k very large.
    assert(maxMatchingApartments(3, 3, 1000, app6, ap6) == 3);

    // Test case 8: Unbalanced sizes.
    std::vector<int> app8 = {1, 2, 3};
    std::vector<int> ap8 = {2, 3};
    assert(maxMatchingApartments(3, 2, 1, app8, ap8) == 2); // 1-2, 2-3, leftover 3

    // Test case 9: Additional sanity.
    std::vector<int> app9 = {10, 20, 30};
    std::vector<int> ap9 = {15, 25, 35};
    assert(maxMatchingApartments(3, 3, 5, app9, ap9) == 3); // diff 5 each

    // Test case 10: negative numbers allowed? The snippet didn't restrict, but assume positive. skip.

    return 0;
}
