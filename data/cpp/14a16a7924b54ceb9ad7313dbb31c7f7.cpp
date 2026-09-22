Write a standalone C++ function that groups class labels from a training dataset and returns them sorted in ascending order along with their counts. The function should accept an array of class labels (stored as `double` values, as in the LIBLINEAR library) and the number of training samples, then produce a sorted list of unique class labels and the number of times each label appears. The function must handle arbitrary integer-valued labels (including negative and non-consecutive values), dynamically grow internal storage when more unique classes are encountered than initially expected, and return the results through output parameters. This task is inspired by the `group_classes` function in the provided LIBLINEAR code, which sorts labels for consistent class ordering regardless of their first-appearance order in the data.

The solution mirrors the behavior of the `group_classes` function in the snippet, but simplified to operate on a flat array of labels rather than the full `problem` structure. The approach involves two passes over the data. In the first pass, we build an unsorted list of unique labels and their counts by scanning the input array; for each label, we check if it already exists in the current list, and if not, we append it (growing a dynamically allocated array if necessary, similar to the original's `realloc` logic). In the second pass, we sort the label-count pairs by label value in ascending order using an insertion sort—matching the `/* START MOD: Sort labels */` section of the snippet, which uses a simple insertion sort for clarity and memory efficiency. Finally, we copy the sorted results into caller-provided output pointers (allocating new arrays for the caller to free) and assign the number of unique classes. Edge cases include an empty input (no labels), single unique label (output should be that label with its count), labels appearing out of order, and duplicate labels; all are handled naturally by the two-pass approach. Time complexity is \(O(n \cdot k + k^2)\) where \(n\) is the number of samples and \(k\) is the number of unique classes, due to the linear scan plus the insertion sort (which is \(O(k^2)\) in the worst case but efficient for typical small \(k\)); space complexity is \(O(k)\) for the temporary and output arrays.

#include <cstddef>
#include <cstdlib>

/**
 * Groups class labels from an array and returns them sorted in ascending order.
 *
 * @param labels    Input array of class labels (each as a double, typically integer-valued).
 * @param l         Number of samples in the labels array.
 * @param nr_class  Output parameter: number of unique classes.
 * @param label_out Output parameter: pointer to an allocated array of size nr_class containing sorted labels (caller must free).
 * @param count_out Output parameter: pointer to an allocated array of size nr_class containing counts per label (caller must free).
 */
void group_unique_labels(const double* labels, int l, int* nr_class, int** label_out, int** count_out) {
    if (l < 0) {
        *nr_class = 0;
        *label_out = nullptr;
        *count_out = nullptr;
        return;
    }

    // Dynamic initial capacity, similar to the original max_nr_class = 16
    int max_nr_class = 16;
    int nr_class = 0;
    int* label_list = (int*)malloc(max_nr_class * sizeof(int));
    int* count_list = (int*)malloc(max_nr_class * sizeof(int));
    if (!label_list || !count_list) {
        // Handle allocation failure gracefully
        free(label_list);
        free(count_list);
        *nr_class = 0;
        *label_out = nullptr;
        *count_out = nullptr;
        return;
    }

    // First pass: collect unique labels and counts (unsorted)
    for (int i = 0; i < l; ++i) {
        int this_label = (int)labels[i];
        int j = 0;
        while (j < nr_class && label_list[j] != this_label) {
            ++j;
        }
        if (j == nr_class) {
            // New label found; grow if needed
            if (nr_class == max_nr_class) {
                max_nr_class *= 2;
                int* new_labels = (int*)realloc(label_list, max_nr_class * sizeof(int));
                int* new_counts = (int*)realloc(count_list, max_nr_class * sizeof(int));
                if (!new_labels || !new_counts) {
                    // Allocation failure; clean up and return empty
                    free(label_list);
                    free(count_list);
                    *nr_class = 0;
                    *label_out = nullptr;
                    *count_out = nullptr;
                    return;
                }
                label_list = new_labels;
                count_list = new_counts;
            }
            label_list[nr_class] = this_label;
            count_list[nr_class] = 1;
            ++nr_class;
        } else {
            ++count_list[j];
        }
    }

    // Second pass: sort labels and counts by label value (ascending) using insertion sort
    for (int j = 1; j < nr_class; ++j) {
        int i = j - 1;
        int this_label = label_list[j];
        int this_count = count_list[j];
        while (i >= 0 && label_list[i] > this_label) {
            label_list[i + 1] = label_list[i];
            count_list[i + 1] = count_list[i];
            --i;
        }
        label_list[i + 1] = this_label;
        count_list[i + 1] = this_count;
    }

    // Transfer to output parameters (allocate fresh arrays for caller)
    *nr_class = nr_class;
    *label_out = (int*)malloc(nr_class * sizeof(int));
    *count_out = (int*)malloc(nr_class * sizeof(int));
    if (*label_out && *count_out) {
        for (int i = 0; i < nr_class; ++i) {
            (*label_out)[i] = label_list[i];
            (*count_out)[i] = count_list[i];
        }
    } else {
        // Clean up on failure
        free(*label_out);
        free(*count_out);
        *label_out = nullptr;
        *count_out = nullptr;
    }

    free(label_list);
    free(count_list);
}

#include <cassert>
#include <cstdlib>

// Declare the function under test (from the solution)
void group_unique_labels(const double* labels, int l, int* nr_class, int** label_out, int** count_out);

// Helper to verify sorted labels and counts
static void check_grouping(const double* labels, int l, int expected_nr, const int* expected_labels, const int* expected_counts) {
    int nr;
    int* got_labels;
    int* got_counts;
    group_unique_labels(labels, l, &nr, &got_labels, &got_counts);

    assert(nr == expected_nr);
    for (int i = 0; i < nr; ++i) {
        assert(got_labels[i] == expected_labels[i]);
        assert(got_counts[i] == expected_counts[i]);
    }

    free(got_labels);
    free(got_counts);
}

int main() {
    // Case 1: Basic unsorted labels with duplicates
    double labels1[] = {3.0, 1.0, 2.0, 1.0, 3.0, 2.0, 2.0};
    int exp_labels1[] = {1, 2, 3};
    int exp_counts1[] = {2, 3, 2};
    check_grouping(labels1, 7, 3, exp_labels1, exp_counts1);

    // Case 2: Single unique label
    double labels2[] = {5.0, 5.0, 5.0};
    int exp_labels2[] = {5};
    int exp_counts2[] = {3};
    check_grouping(labels2, 3, 1, exp_labels2, exp_counts2);

    // Case 3: Empty input
    double* labels3 = nullptr;
    int exp_labels3[] = {};
    int exp_counts3[] = {};
    check_grouping(labels3, 0, 0, exp_labels3, exp_counts3);

    // Case 4: Negative and non-consecutive labels
    double labels4[] = {-1.0, 10.0, -1.0, 7.0, 10.0, 7.0, -1.0, 0.0};
    int exp_labels4[] = {-1, 0, 7, 10};
    int exp_counts4[] = {3, 1, 2, 2};
    check_grouping(labels4, 8, 4, exp_labels4, exp_counts4);

    // Case 5: Already sorted with many duplicates
    double labels5[] = {1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0};
    int exp_labels5[] = {1, 2, 3, 4};
    int exp_counts5[] = {2, 1, 3, 2};
    check_grouping(labels5, 8, 4, exp_labels5, exp_counts5);

    // Case 6: All identical values in a larger array (stress count correctness)
    double labels6[100];
    for (int i = 0; i < 100; ++i) labels6[i] = 42.0;
    int exp_labels6[] = {42};
    int exp_counts6[] = {100};
    check_grouping(labels6, 100, 1, exp_labels6, exp_counts6);

    // Case 7: More than 16 unique labels to test dynamic growth
    double labels7[20];
    int expected_labels7[20];
    int expected_counts7[20];
    for (int i = 0; i < 20; ++i) {
        labels7[i] = (double)(20 - i);  // Reverse order to force sorting
        expected_labels7[i] = i + 1;
        expected_counts7[i] = 1;
    }
    check_grouping(labels7, 20, 20, expected_labels7, expected_counts7);

    return 0;
}
