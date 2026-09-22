Write a C++ function `int assignApartments(int n, int m, int k, const std::vector<int>& applicants, const std::vector<int>& apartments)` that returns the maximum number of applicants who can be assigned an apartment. Each applicant has a desired apartment size, and they will accept any apartment whose size differs from their desired size by at most `k` (inclusive). Each apartment can be assigned to at most one applicant, and each applicant receives at most one apartment. The function should handle large inputs efficiently (n and m up to 2×10^5) and large size values (up to 10^9). You may assume the input arrays are unsorted initially.

The problem is a classic greedy matching problem. Sort both the applicants' desired sizes and the apartments' sizes in ascending order. Use two pointers: one for applicants (`i`) and one for apartments (`j`). At each step, check if the current applicant's desired size `a[i]` and current apartment size `b[j]` are within the allowed difference `k` (i.e., `abs(a[i] - b[j]) <= k`). If they match, assign that apartment to the applicant, increment both pointers, and increase the answer counter. Otherwise, if the applicant's desired size is larger than the apartment size (`a[i] > b[j]`), the current apartment is too small for this applicant (and all later applicants since the list is sorted), so move to the next apartment (`j++`). If the apartment size is larger than the desired size (`a[i] < b[j]`), the current applicant is too small compared to this apartment (and all later apartments), so move to the next applicant (`i++`). This greedy choice works because sorting ensures that whenever we skip an apartment or applicant, no future match can be missed that would yield a better overall count; the two-pointer technique systematically eliminates impossible pairs. Edge cases include `k=0` (exact match required), arrays with duplicates, and cases where one list is exhausted prematurely. Time complexity is O(n log n + m log m) due to sorting, and O(1) auxiliary space aside from input storage. The two-pointer scan itself is O(n + m).

#include <vector>
#include <algorithm>
#include <cstdlib>

// Returns the maximum number of applicants that can be assigned an apartment.
int assignApartments(int n, int m, int k, const std::vector<int>& applicants, const std::vector<int>& apartments) {
    std::vector<int> a = applicants; // copy to sort without modifying input
    std::vector<int> b = apartments;
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    int ans = 0;
    int i = 0, j = 0;

    while (i < n && j < m) {
        if (std::abs(a[i] - b[j]) <= k) {
            // Match found
            ++ans;
            ++i;
            ++j;
        } else if (a[i] > b[j]) {
            // Apartment too small for current applicant, try next apartment
            ++j;
        } else {
            // Applicant too small for current apartment, try next applicant
            ++i;
        }
    }

    return ans;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
int main() {
    // Basic test
    assert(assignApartments(4, 4, 2, {1, 2, 3, 4}, {1, 3, 5, 7}) == 3);
    // All match
    assert(assignApartments(3, 3, 0, {1, 2, 3}, {1, 2, 3}) == 3);
    // No match (k=0 but sizes differ)
    assert(assignApartments(2, 2, 0, {1, 5}, {2, 6}) == 0);
    // k=0 with duplicates
    assert(assignApartments(4, 4, 0, {1, 1, 2, 2}, {1, 1, 2, 2}) == 4);
    // More applicants than apartments
    assert(assignApartments(5, 2, 3, {10, 20, 30, 40, 50}, {10, 30}) == 2);
    // More apartments than applicants
    assert(assignApartments(2, 5, 1, {5, 5}, {4, 5, 6, 7, 8}) == 2);
    // Large difference allowed
    assert(assignApartments(3, 3, 100, {1, 2, 3}, {1000, 2000, 3000}) == 0);
    // Single applicant single apartment match
    assert(assignApartments(1, 1, 0, {42}, {42}) == 1);
    // Single applicant single apartment no match
    assert(assignApartments(1, 1, 0, {42}, {43}) == 0);
    // Unsorted input arrays
    assert(assignApartments(3, 3, 2, {3, 1, 2}, {2, 3, 1}) == 3);
    // k large enough that all match
    assert(assignApartments(3, 3, 100, {1, 2, 3}, {101, 102, 103}) == 3);
    return 0;
}
