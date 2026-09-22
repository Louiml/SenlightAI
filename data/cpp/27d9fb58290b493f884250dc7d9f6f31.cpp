Write a C++ function named `encodeDistanceToColor` that takes a single floating-point argument `t` representing a distance value. The function converts this distance into a 32-bit unsigned integer color code in ARGB format (alpha in the most significant byte, then red, green, blue). If `t` is less than 0, return the opaque red color `0xffff0000`. If `t` is greater than 100, return the opaque black color `0xff000000`. Otherwise, compute three 8-bit channel values from `t`: red channel as the low 8 bits of `int(t * 1000)`, green channel as the low 8 bits of `int(t * 10)`, and blue channel as the low 8 bits of `int(t * 0.01)` (note integer truncation after multiplication). Combine these channels into an opaque ARGB color: alpha = 0xff, red = computed red, green = computed green, blue = computed blue, by bit-shifting and OR-ing, and return the result. Do not use any SIMD or vector extensions; the function must be pure, deterministic, and contain no side effects.

// The task is straightforward: map a float range into three separate byte values using three different scaling factors and bit-masking operations. The tricky parts are handling the edge cases (`t < 0` and `t > 100`) and ensuring the bit-masking (`&=0xff`) correctly isolates the low 8 bits after casting to `int`. Because `int` is at least 16 bits but typically 32 bits, the mask ensures only the least significant 8 bits survive. The float multiplication may produce negative values if `t` is negative, but the edge case guard catches `t < 0`. For values in the range `[0, 100]`, the products `t * 1000`, `t * 10`, and `t * 0.01` are all non-negative; note that `t * 0.01` will be in `[0, 1]` for `t <= 100`, so its integer part is 0 for most values, and masking with `0xff` gives 0—this is intended. The time complexity is O(1) and space O(1). The main pitfalls: forgetting to cast the float to `int` before masking, or using bitwise AND on a float (which is invalid). The solution must use `static_cast<int>` or C-style cast. Also, ensure the return type is `std::uint32_t` to guarantee 32-bit size.

#include <cstdint>

// Convert a distance value to an opaque ARGB color code.
// t < 0 -> opaque red; t > 100 -> opaque black; otherwise encoded channels.
std::uint32_t encodeDistanceToColor(float t) {
    if (t < 0.0f) {
        return 0xffff0000u;
    }
    if (t > 100.0f) {
        return 0xff000000u;
    }

    int a = static_cast<int>(t * 1000.0f);   // red
    a &= 0xff;
    int b = static_cast<int>(t * 10.0f);     // green
    b &= 0xff;
    int c = static_cast<int>(t * 0.01f);     // blue (almost always 0)
    c &= 0xff;

    return 0xff000000u | (static_cast<std::uint32_t>(a) << 16) |
           (static_cast<std::uint32_t>(b) << 8) | static_cast<std::uint32_t>(c);
}

#include <cassert>
#include <cstdint>

// Declaration of the function under test
std::uint32_t encodeDistanceToColor(float t);

int main() {
    // Edge cases
    assert(encodeDistanceToColor(-1.0f) == 0xffff0000u);
    assert(encodeDistanceToColor(101.0f) == 0xff000000u);
    assert(encodeDistanceToColor(100.5f) == 0xff000000u);

    // Boundary at zero
    // t=0: a=0, b=0, c=0 -> black opaque
    assert(encodeDistanceToColor(0.0f) == 0xff000000u);

    // Check a known value: t=0.5
    // a = int(500) = 500 & 0xff = 244 (0xf4)
    // b = int(5) = 5 & 0xff = 5
    // c = int(0.005) = 0
    // color = 0xff00f405
    assert(encodeDistanceToColor(0.5f) == 0xff00f405u);

    // t=1.0: a=int(1000)=1000&0xff=232 (0xe8), b=int(10)=10, c=int(0.01)=0
    assert(encodeDistanceToColor(1.0f) == 0xff00e80au);

    // t=10.0: a=int(10000)=10000&0xff=16 (0x10), b=int(100)=100 (0x64), c=int(0.1)=0
    assert(encodeDistanceToColor(10.0f) == 0xff106400u);

    // t=100.0: a=int(100000)=100000&0xff=160 (0xa0), b=int(1000)&0xff=232, c=int(1)=1
    // a = 100000 % 256 = 160, b = 1000 % 256 = 232, c = 1
    assert(encodeDistanceToColor(100.0f) == 0xffa0e801u);

    return 0;
}
