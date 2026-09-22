Write a C++ function named `compareVersions` that takes two software version strings, `ver1` and `ver2`, and returns an integer indicating their relative order: `1` if `ver1` is newer (greater) than `ver2`, `-1` if `ver1` is older (less) than `ver2`, and `0` if they are equal. Version strings consist of one or more numeric components separated by dots (e.g., `"1.0"`, `"0.1.1"`, `"3"`). The comparison must be done numerically component by component, so `"1.10"` is greater than `"1.9"`. Missing trailing components are treated as zero: for example, `"1.1"` equals `"1.1.0"`, and `"1"` equals `"1.0.0"`. Leading dots are allowed (e.g., `".1"` is equivalent to `"0.1"`). Empty strings are not allowed, but components can be any non-negative integer (including zero). The function must handle arbitrary-length version strings with any number of components, not just the typical two or three.

// The solution approach is to parse each version string into an array of integers representing the numeric value of each dot-separated component. To handle differing numbers of components, we first count the number of dots in each string, use the maximum count to determine the array size (which equals the maximum number of components), and pad the shorter version with zeros to that size. Then we iterate component-wise from left to right, comparing the corresponding numeric values. As soon as one is greater than the other, we return `1` or `-1` accordingly. If we finish all components without finding a difference, we return `0`. Important edge cases include: leading dots (which we treat as a leading zero), missing trailing components (treated as zero), and very long version strings. The time complexity is \(O(n + m)\), where \(n\) and \(m\) are the total number of characters (or components) in the two input strings, and the space complexity is \(O(\max(n,m))\) for the integer arrays. Since we are handling each component numerically, `"1.10"` correctly compares greater than `"1.9"`. The implementation uses dynamic allocation with `delete[]` as is common in such exercises, but a `std::vector` would also be acceptable. We’ll note the use of `const` references for the input strings to avoid unnecessary copies, and ensure no overflow because the problem does not specify component size limits (it could be arbitrarily large, but for practical purposes `int` is fine; if needed, we could use `long long`, but the problem does not require it).

#include <string>

/**
 * Compare two software version strings numerically component by component.
 * @param ver1 first version string (e.g., "1.2.3")
 * @param ver2 second version string
 * @return 1 if ver1 > ver2, -1 if ver1 < ver2, 0 if equal
 */
int compareVersions(const std::string& ver1, const std::string& ver2) {
    const char delimiter = '.';
    
    // Handle leading dots: insert a leading zero for consistency.
    // Use a copy because we may modify the string.
    std::string v1 = ver1;
    std::string v2 = ver2;
    if (!v1.empty() && v1[0] == delimiter) {
        v1.insert(0, "0");
    }
    if (!v2.empty() && v2[0] == delimiter) {
        v2.insert(0, "0");
    }
    
    // Count number of delimiters in each string.
    int dots1 = 0, dots2 = 0;
    for (char c : v1) {
        if (c == delimiter) dots1++;
    }
    for (char c : v2) {
        if (c == delimiter) dots2++;
    }
    
    // Determine the maximum number of components.
    int maxComponents = (dots1 > dots2 ? dots1 : dots2) + 1;
    
    // Dynamically allocate arrays to store numeric components.
    int* nums1 = new int[maxComponents]();
    int* nums2 = new int[maxComponents]();
    
    // Parse components of v1.
    int partIndex = 0;
    for (char c : v1) {
        if (c == delimiter) {
            partIndex++;
        } else {
            nums1[partIndex] = nums1[partIndex] * 10 + (c - '0');
        }
    }
    
    // Parse components of v2.
    partIndex = 0;
    for (char c : v2) {
        if (c == delimiter) {
            partIndex++;
        } else {
            nums2[partIndex] = nums2[partIndex] * 10 + (c - '0');
        }
    }
    
    // Compare component by component.
    for (int i = 0; i < maxComponents; ++i) {
        if (nums1[i] < nums2[i]) {
            delete[] nums1;
            delete[] nums2;
            return -1;
        } else if (nums1[i] > nums2[i]) {
            delete[] nums1;
            delete[] nums2;
            return 1;
        }
    }
    
    delete[] nums1;
    delete[] nums2;
    return 0;
}

#include <cassert>

int main() {
    // Basic comparisons
    assert(compareVersions("0.1", "0.2") == -1);
    assert(compareVersions("1.0", "1.0") == 0);
    assert(compareVersions("2.0", "1.9") == 1);
    
    // Different number of components, missing components treated as zero
    assert(compareVersions("1", "1.1") == -1);
    assert(compareVersions("1.1", "1.1.0") == 0);
    assert(compareVersions("1.0", "1") == 0);
    assert(compareVersions("1.2.0", "1.2") == 0);
    
    // Leading dot handled
    assert(compareVersions(".1", "0.1") == 0);
    assert(compareVersions(".2", ".1.1") == 1);
    
    // Large component numbers compare numerically, not lexicographically
    assert(compareVersions("1.10", "1.9") == 1);
    assert(compareVersions("256", "300.2.1") == -1);
    
    // Long version strings
    assert(compareVersions("2.3.2.2.3.1.1.5.3.5.6.2", "1.1.1.1.1.1.1") == 1);
    
    return 0;
}
