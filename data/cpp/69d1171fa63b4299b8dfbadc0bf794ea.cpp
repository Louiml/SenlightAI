// Write a C++ function that takes an array of records, where each record has a string field `name` (representing a person’s name, consisting of lowercase letters only) and an integer field `priority` (representing a numeric priority level), along with the array size `n`. The function must sort the records in ascending order by `priority`; if two records have the same `priority`, they must be sorted alphabetically by `name` (case-sensitive, but all names will be lowercase). The sorting must be stable in the sense that after the primary and secondary sorts, records with identical `name` and `priority` retain their original relative order relative to each other. The function should modify the array in place and return void. You are not allowed to use `std::sort` or any container from the standard library for sorting; implement the sorting algorithm yourself using bubble sort or selection sort.
The problem requires a two-key sorting. The simplest approach is to first sort by the primary key (`priority`) using bubble sort, then perform a stable sort by the secondary key (`name`) — but that is tricky because bubble sort on the secondary key may break the primary order. A safer method is to sort by both keys in a single comparison during a bubble sort pass: compare record `j` and `j+1`; swap them if `arr[j].priority > arr[j+1].priority` OR if priorities are equal and `arr[j].name > arr[j+1].name`. This ensures the array is sorted lexicographically by (priority, name). Because bubble sort only swaps adjacent elements when the comparator says they are out of order, equal records (same priority and same name) are never swapped, preserving their original order — making the sort stable. The main edge case is when records have identical priority and identical name: they must remain in the original order, which the adjacent-comparison approach guarantees. For `n` records, bubble sort runs in O(n²) time and O(1) auxiliary space (only a single temporary record for swapping). We must be careful to use `strcmp` or string comparison for names.
#include <cstring>

struct Record {
    char name[100];
    int priority;
};

// Sort an array of records by (priority ascending, then name ascending).
// Uses a stable bubble sort with a combined comparator.
void sortRecords(Record arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            bool outOfOrder = false;
            if (arr[j].priority > arr[j + 1].priority) {
                outOfOrder = true;
            } else if (arr[j].priority == arr[j + 1].priority) {
                if (std::strcmp(arr[j].name, arr[j + 1].name) > 0) {
                    outOfOrder = true;
                }
            }
            if (outOfOrder) {
                Record temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}
#include <cassert>
#include <cstring>
#include <iostream>

// Assume the function and struct are defined above (or included).
// For the test, we define them here to make it self-contained.

int main() {
    // Test 1: Basic sorting by priority, with tie-breaking by name.
    Record r1[] = {
        {"bob", 2},
        {"alice", 1},
        {"carol", 2}
    };
    sortRecords(r1, 3);
    assert(std::strcmp(r1[0].name, "alice") == 0 && r1[0].priority == 1);
    assert(std::strcmp(r1[1].name, "bob") == 0 && r1[1].priority == 2);
    assert(std::strcmp(r1[2].name, "carol") == 0 && r1[2].priority == 2);

    // Test 2: All same priority, names sorted alphabetically.
    Record r2[] = {
        {"zoe", 5},
        {"amy", 5},
        {"tom", 5}
    };
    sortRecords(r2, 3);
    assert(std::strcmp(r2[0].name, "amy") == 0);
    assert(std::strcmp(r2[1].name, "tom") == 0);
    assert(std::strcmp(r2[2].name, "zoe") == 0);

    // Test 3: All same priority and name, order preserved (stable).
    Record r3[] = {
        {"same", 1},
        {"same", 1},
        {"same", 1}
    };
    sortRecords(r3, 3);
    assert(std::strcmp(r3[0].name, "same") == 0);
    assert(std::strcmp(r3[1].name, "same") == 0);
    assert(std::strcmp(r3[2].name, "same") == 0);

    // Test 4: Single element.
    Record r4[] = {{"only", 9}};
    sortRecords(r4, 1);
    assert(std::strcmp(r4[0].name, "only") == 0 && r4[0].priority == 9);

    // Test 5: Already sorted, should not change.
    Record r5[] = {
        {"a", 1},
        {"b", 2},
        {"c", 3}
    };
    sortRecords(r5, 3);
    assert(std::strcmp(r5[0].name, "a") == 0 && r5[0].priority == 1);
    assert(std::strcmp(r5[1].name, "b") == 0 && r5[1].priority == 2);
    assert(std::strcmp(r5[2].name, "c") == 0 && r5[2].priority == 3);

    // Test 6: Reverse order.
    Record r6[] = {
        {"d", 4},
        {"c", 3},
        {"b", 2},
        {"a", 1}
    };
    sortRecords(r6, 4);
    assert(std::strcmp(r6[0].name, "a") == 0 && r6[0].priority == 1);
    assert(std::strcmp(r6[1].name, "b") == 0 && r6[1].priority == 2);
    assert(std::strcmp(r6[2].name, "c") == 0 && r6[2].priority == 3);
    assert(std::strcmp(r6[3].name, "d") == 0 && r6[3].priority == 4);

    // Test 7: Mixed priorities with tie-breaking.
    Record r7[] = {
        {"x", 2},
        {"y", 1},
        {"x", 1},
        {"y", 2}
    };
    sortRecords(r7, 4);
    assert(std::strcmp(r7[0].name, "x") == 0 && r7[0].priority == 1);
    assert(std::strcmp(r7[1].name, "y") == 0 && r7[1].priority == 1);
    assert(std::strcmp(r7[2].name, "x") == 0 && r7[2].priority == 2);
    assert(std::strcmp(r7[3].name, "y") == 0 && r7[3].priority == 2);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
