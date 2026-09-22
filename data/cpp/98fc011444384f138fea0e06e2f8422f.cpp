// Implement a C++ function `bucketSortFloats` that takes a `std::vector<float>&` containing values in the range `[0, 1)` and sorts it in ascending order using the bucket sort algorithm. The function should dynamically choose the number of buckets based on the input size (specifically, `n / 2` buckets, where `n` is the number of elements), distribute each element into a bucket using the mapping `index = value * bucketCount`, sort each bucket individually (using `std::sort` or any equivalent), and finally concatenate the sorted buckets back into the original vector. The function must handle empty inputs gracefully (do nothing) and ensure that the input vector is modified in place. The solution must not include a `main` function but must be self-contained with appropriate headers and comments.
The bucket sort algorithm works by distributing elements into a number of buckets based on their value, sorting each bucket independently, and then merging the buckets in order. Here, since the input values are uniformly distributed in `[0,1)`, using `k = n/2` buckets (at least 1 for small inputs) ensures each bucket receives roughly 2 elements on average. The mapping `index = value * k` converts the floating-point value into a bucket index in `[0, k-1]` because all values are less than 1. Edge cases include an empty input (immediately return), a single element (still works, but `k` would be 0, so we must clamp `k` to at least 1), and cases where all elements fall into the same bucket (still correct because the bucket is sorted). The time complexity is \(O(n + k \cdot m \log m)\) where \(m\) is the average bucket size; with roughly \(n/k\) elements per bucket and \(k = n/2\), this simplifies to approximately \(O(n)\) on average for uniform data, though the worst case (all elements in one bucket) is \(O(n \log n)\). Space complexity is \(O(n)\) for the bucket storage.
#include <vector>
#include <algorithm>
#include <cmath>

/**
 * Sorts a vector of floating-point numbers in the range [0, 1) using bucket sort.
 * The function modifies the input vector in place.
 * 
 * @param nums The vector to sort. Must contain values in [0, 1).
 */
void bucketSortFloats(std::vector<float>& nums) {
    // Handle empty or single-element vectors (already sorted)
    if (nums.size() <= 1) {
        return;
    }

    // Choose number of buckets: n/2, but at least 1
    size_t n = nums.size();
    size_t k = std::max<size_t>(1, n / 2);
    
    // Create k empty buckets
    std::vector<std::vector<float>> buckets(k);

    // Distribute elements into buckets
    for (float num : nums) {
        // Map value [0,1) to bucket index [0, k-1]
        size_t idx = static_cast<size_t>(num * k);
        // Ensure idx is within bounds (defensive for floating point edge cases)
        if (idx >= k) {
            idx = k - 1;
        }
        buckets[idx].push_back(num);
    }

    // Sort each bucket individually
    for (auto& bucket : buckets) {
        std::sort(bucket.begin(), bucket.end());
    }

    // Merge buckets back into original vector
    size_t pos = 0;
    for (const auto& bucket : buckets) {
        for (float val : bucket) {
            nums[pos++] = val;
        }
    }
}
#include <cassert>
#include <vector>
#include <algorithm>
#include <cmath>

// Forward declaration of the solution function (assumed to be defined elsewhere)
void bucketSortFloats(std::vector<float>& nums);

int main() {
    // Test 1: Example from original snippet
    std::vector<float> nums1 = {0.49f, 0.96f, 0.82f, 0.09f, 0.57f, 0.43f, 0.91f, 0.75f, 0.15f, 0.37f};
    std::vector<float> expected1 = nums1;
    std::sort(expected1.begin(), expected1.end());
    bucketSortFloats(nums1);
    assert(nums1 == expected1);

    // Test 2: Empty vector
    std::vector<float> nums2;
    bucketSortFloats(nums2);
    assert(nums2.empty());

    // Test 3: Single element
    std::vector<float> nums3 = {0.5f};
    bucketSortFloats(nums3);
    assert(nums3.size() == 1 && std::fabs(nums3[0] - 0.5f) < 1e-6);

    // Test 4: All zeros and near-one values (edge of range)
    std::vector<float> nums4 = {0.0f, 0.999f, 0.0f, 0.5f, 0.999f};
    std::vector<float> expected4 = {0.0f, 0.0f, 0.5f, 0.999f, 0.999f};
    bucketSortFloats(nums4);
    assert(nums4 == expected4);

    // Test 5: Small n with n/2 = 0, should still work with at least 1 bucket
    std::vector<float> nums5 = {0.9f, 0.1f};
    std::vector<float> expected5 = {0.1f, 0.9f};
    bucketSortFloats(nums5);
    assert(nums5 == expected5);

    // Test 6: Large uniformly distributed vector (simple stress test)
    std::vector<float> nums6;
    for (int i = 0; i < 100; ++i) {
        nums6.push_back(static_cast<float>(i % 100) / 100.0f);
    }
    std::vector<float> expected6 = nums6;
    std::sort(expected6.begin(), expected6.end());
    bucketSortFloats(nums6);
    assert(nums6 == expected6);

    // Test 7: All elements equal
    std::vector<float> nums7 = {0.42f, 0.42f, 0.42f};
    std::vector<float> expected7 = {0.42f, 0.42f, 0.42f};
    bucketSortFloats(nums7);
    assert(nums7 == expected7);

    // Test 8: Values very close to 1.0 (just below)
    std::vector<float> nums8 = {0.9999f, 0.9998f, 0.9997f};
    std::vector<float> expected8 = {0.9997f, 0.9998f, 0.9999f};
    bucketSortFloats(nums8);
    assert(nums8 == expected8);

    return 0;
}
