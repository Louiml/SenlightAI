Write a C++ function `prefixesDivBy5` that takes a vector of integers `nums` where every element is either `0` or `1`, representing a binary number bit by bit from most significant to least significant. The function must return a vector of booleans of the same length where the `i`-th boolean is `true` if the binary number formed by the first `i+1` bits is divisible by 5, and `false` otherwise. For example, given `nums = {1, 0, 1}`, the prefixes are `1`, `2` (binary `10`), and `5` (binary `101`); the result should be `{false, false, true}`. The input vector will have at least one element. Handle potentially large inputs correctly without overflowing the integer type.

// The key observation is that we do not need to store the full binary number, which could be huge. Instead, we only need its remainder when divided by 5, because divisibility by 5 depends solely on this remainder. Iterate through the bits from left to right. For each new bit, the current number becomes `previous * 2 + bit`. We compute this modulo 5 using `(previous * 2 + bit) % 5`. Since `previous` is always kept in `[0,4]`, the multiplication `previous * 2` is at most `8`, and adding the bit gives at most `9`, so no overflow occurs. After updating the remainder, check if it is `0` and push `true` or `false` accordingly. Edge cases: the first bit alone can be `0` (remainder `0`, divisible by 5) or `1` (remainder `1`, not divisible). Also, when the binary number is `0` at any prefix, it is divisible by 5. Time complexity is `O(n)` where `n` is the input size, and auxiliary space is `O(1)` besides the output vector of size `n`.

#include <vector>

// For each prefix of the binary number represented by nums, return whether
// that prefix is divisible by 5.
std::vector<bool> prefixesDivBy5(const std::vector<int>& nums) {
    std::vector<bool> result;
    result.reserve(nums.size());
    int remainder = 0;
    for (int bit : nums) {
        remainder = (remainder * 2 + bit) % 5;
        result.push_back(remainder == 0);
    }
    return result;
}

#include <cassert>
#include <vector>

std::vector<bool> prefixesDivBy5(const std::vector<int>& nums);

int main() {
    std::vector<int> nums1 = {0};
    std::vector<bool> expected1 = {true};
    assert(prefixesDivBy5(nums1) == expected1);

    std::vector<int> nums2 = {1};
    std::vector<bool> expected2 = {false};
    assert(prefixesDivBy5(nums2) == expected2);

    std::vector<int> nums3 = {0, 1, 1, 0, 1};
    // Prefixes: 0, 1, 3, 6, 13 -> divisible: true, false, false, false, false
    std::vector<bool> expected3 = {true, false, false, false, false};
    assert(prefixesDivBy5(nums3) == expected3);

    std::vector<int> nums4 = {1, 0, 1, 1, 1, 0, 1};
    // Prefixes (decimal): 1,2,5,11,23,46,93
    // Divisible by 5: 5 only -> {false,false,true,false,false,false,false}
    std::vector<bool> expected4 = {false, false, true, false, false, false, false};
    assert(prefixesDivBy5(nums4) == expected4);

    std::vector<int> nums5 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    // Decimal values: 1,3,7,15,31,63,127,255,511,1023
    // Only 255 is divisible by 5? 255%5==0, and 1023%5==3, etc. Actually 1,3,7,15,31,63,127,255,511,1023; 255%5=0, 1%5=1,3%5=3,7%5=2,15%5=0? 15 div by 5=3, yes. So 15 and 255 are divisible. Let's enumerate: i=0:1->F, i=1:3->F, i=2:7->F, i=3:15->T, i=4:31->F, i=5:63->F, i=6:127->F, i=7:255->T, i=8:511->F, i=9:1023->F
    std::vector<bool> expected5 = {false, false, false, true, false, false, false, true, false, false};
    assert(prefixesDivBy5(nums5) == expected5);

    std::vector<int> nums6 = {1, 0, 0, 0, 0, 0, 0, 0};
    // Decimal: 1,2,4,8,16,32,64,128 -> none divisible by 5
    std::vector<bool> expected6 = {false, false, false, false, false, false, false, false};
    assert(prefixesDivBy5(nums6) == expected6);

    std::vector<int> nums7 = {}; // Will not be used per spec, but test empty vector
    std::vector<bool> expected7 = {};
    assert(prefixesDivBy5(nums7) == expected7);

    std::vector<int> nums8 = {0, 0, 0, 0};
    std::vector<bool> expected8 = {true, true, true, true};
    assert(prefixesDivBy5(nums8) == expected8);

    return 0;
}
