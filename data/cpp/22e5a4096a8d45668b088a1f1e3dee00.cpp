Write a standalone C++ function named `productWithMask` that computes the product of selected elements from a one-dimensional array of integers. The function must accept a `std::vector<int>` representing the data, a `std::vector<bool>` mask (same length as data), and return the product as a `long long`. Only elements at positions where the mask is `true` are multiplied. If no elements are selected, return 1 (the multiplicative identity). The function must handle empty arrays, masks shorter than the data (in which case only the overlapping prefix is considered), and potential overflow (use `long long` for the result). The function must be `const`-correct and should not modify the inputs. Provide a brief analysis including time and space complexity and edge cases.
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.

int main() {
    // Basic case with mask selecting some elements
    std::vector<int> data1 = {2, 3, 4, 5};
    std::vector<bool> mask1 = {true, true, false, true};
    assert(productWithMask(data1, mask1) == 2 * 3 * 5);

    // Empty data
    std::vector<int> data2 = {};
    std::vector<bool> mask2 = {};
    assert(productWithMask(data2, mask2) == 1);

    // Empty mask
    std::vector<int> data3 = {1, 2, 3};
    std::vector<bool> mask3 = {};
    assert(productWithMask(data3, mask3) == 1);

    // Mask shorter than data (only first two selected)
    std::vector<int> data4 = {2, 3, 4, 5};
    std::vector<bool> mask4 = {true, true};
    assert(productWithMask(data4, mask4) == 2 * 3);

    // All false
    std::vector<int> data5 = {2, 3, 4};
    std::vector<bool> mask5 = {false, false, false};
    assert(productWithMask(data5, mask5) == 1);

    // All true
    std::vector<int> data6 = {1, 2, 3, 4};
    std::vector<bool> mask6 = {true, true, true, true};
    assert(productWithMask(data6, mask6) == 24);

    // Negative numbers
    std::vector<int> data7 = {-2, 3, -4};
    std::vector<bool> mask7 = {true, true, true};
    assert(productWithMask(data7, mask7) == 24);

    // Single zero selected
    std::vector<int> data8 = {0};
    std::vector<bool> mask8 = {true};
    assert(productWithMask(data8, mask8) == 0);

    // Overflow check: use large values but not beyond long long
    std::vector<int> data9 = {1000000, 1000000};
    std::vector<bool> mask9 = {true, true};
    assert(productWithMask(data9, mask9) == 1000000LL * 1000000LL);

    return 0;
}
#include <vector>

// Compute the product of elements in data where the corresponding mask is true.
// If no elements are selected, return 1.
// mask may be shorter than data; elements beyond mask.size() are not selected.
long long productWithMask(const std::vector<int>& data, const std::vector<bool>& mask) {
    long long product = 1;
    const size_t dataSize = data.size();
    const size_t maskSize = mask.size();
    for (size_t i = 0; i < dataSize; ++i) {
        if (i < maskSize && mask[i]) {
            product *= static_cast<long long>(data[i]);
        }
    }
    return product;
}
// The solution iterates over the data array once, using an index `i` from 0 to `data.size()-1`. For each index, it checks whether the mask is valid (i.e., `i < mask.size()`) and whether `mask[i] == true`. If both conditions hold, the current element is multiplied into an accumulator initialized to 1. If the mask is shorter than the data, elements beyond `mask.size()` are ignored (since they are not selected). If the data is empty or no element is selected, the accumulator remains 1, which is returned. The algorithm runs in O(n) time where n is the size of the data, and uses O(1) auxiliary space (only a `long long` accumulator). Edge cases include: empty data, empty mask, mask shorter than data, all mask true, all mask false, and values that cause overflow – the use of `long long` mitigates but does not completely avoid overflow for extreme inputs.
