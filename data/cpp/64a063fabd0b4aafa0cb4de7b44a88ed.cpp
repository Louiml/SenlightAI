// You are to implement a C++ function that processes a set of genomic interval overlap queries. Given a query array where each query consists of a contig identifier and two integer positions, along with per-contig data structures describing intervals (stored as flat start-end arrays), per-contig reach values, per-contig interval counts, per-contig reach lengths, per-contig shift values, and per-contig sizes, write a function that for each query returns a list of indices of all reach values (from the reach array) that fall within the specified interval after applying the shift. The output for each query should be a vector containing the query's sequential ID followed by the indices of all overlapping reach elements, terminated by a sentinel value of -1. The function must correctly navigate the flat arrays using the per-contig size and length metadata to locate the correct sub-arrays for each contig.
// The solution must first parse the query array, grouping every three integers as one query (contig ID, position A, position B). For each query, we locate the starting pointers into the start-end, reach, size, reach-length, and shift arrays by summing the sizes and reach-lengths of all preceding contigs. For a given contig, the start-end array contains `size * 2` integers representing `size` intervals (each with start and end). The reach array for that contig has `reach_length` integers. The shift is a single scalar per contig. To check overlap, for each reach value `r` (stored at index `j`), we first apply the shift by adding `shift` to `r`, then scan all intervals of the contig to see if the shifted value lies within any `[start, end]` range. If an overlap is found, we record index `j` in the result vector. After processing all reach values for a contig, we push -1 as a delimiter. Edge cases include queries where the contig ID is out of bounds, empty reach arrays (produce only sentinel), intervals with start greater than end (should be treated as no match), and shift values that could be negative, positive, or zero. Time complexity is O(Q * (R + R * I)) where Q is number of queries, R is max reach length per contig, and I is max intervals per contig; in the worst case this is O(Q * R * I). Space complexity is O(total output size) plus O(1) auxiliary.
#include <vector>
#include <cstddef>

/**
 * Processes overlap queries and returns a flattened result vector.
 * 
 * @param queries     Array of query triples: [contig_id, posA, posB] repeated N times.
 * @param query_data_len  Total number of integers in the queries array (should be 3*N).
 * @param start_end_arr   Flat array of per-contig intervals (each contig has size*2 ints).
 * @param reach_arr       Flat array of per-contig reach values.
 * @param reach_length_arr  Per-contig count of reach values.
 * @param shift_arr       Per-contig integer shift value.
 * @param vs_size_arr     Per-contig number of intervals (size of interval pairs).
 * @param num_contigs     Total number of contigs (length of the last three arrays).
 * @return Vector with for each query: query_id, then indices of overlapping reach values, then -1.
 */
std::vector<int> getOverlappingQueries(
    const int* queries,
    int query_data_len,
    const int* start_end_arr,
    const int* reach_arr,
    const int* reach_length_arr,
    const int* shift_arr,
    const int* vs_size_arr,
    int num_contigs) {
    
    std::vector<int> result;
    int num_queries = query_data_len / 3;
    
    for (int q = 0; q < num_queries; ++q) {
        const int* p_query = queries + q * 3;
        int contig_id = p_query[0];
        int posA = p_query[1];
        int posB = p_query[2];
        
        // Guard against invalid contig ID
        if (contig_id < 0 || contig_id >= num_contigs) {
            result.push_back(q);
            result.push_back(-1);
            continue;
        }
        
        // Locate sub-arrays for this contig
        const int* contig_intervals = start_end_arr;
        const int* contig_reach = reach_arr;
        for (int c = 0; c < contig_id; ++c) {
            contig_intervals += vs_size_arr[c] * 2;
            contig_reach += reach_length_arr[c];
        }
        
        int num_intervals = vs_size_arr[contig_id];
        int reach_len = reach_length_arr[contig_id];
        int shift = shift_arr[contig_id];
        
        result.push_back(q);
        
        // For each reach value, check overlap with any interval after shifting
        for (int j = 0; j < reach_len; ++j) {
            int shifted_value = contig_reach[j] + shift;
            bool overlaps = false;
            const int* interval_ptr = contig_intervals;
            for (int i = 0; i < num_intervals; ++i) {
                int start = interval_ptr[0];
                int end = interval_ptr[1];
                // Skip invalid intervals (start > end)
                if (start <= end) {
                    if (shifted_value >= start && shifted_value <= end) {
                        overlaps = true;
                        break;
                    }
                }
                interval_ptr += 2;
            }
            if (overlaps) {
                result.push_back(j);
            }
        }
        result.push_back(-1);
    }
    
    return result;
}
#include <cassert>
#include <vector>

// Function declaration (as in solution)
std::vector<int> getOverlappingQueries(
    const int* queries, int query_data_len,
    const int* start_end_arr, const int* reach_arr,
    const int* reach_length_arr, const int* shift_arr,
    const int* vs_size_arr, int num_contigs);

int main() {
    // Test 1: Two contigs, simple intervals
    int queries1[] = {0, 5, 10, 1, 0, 100};
    int start_end1[] = {0, 5, 8, 12, 10, 20}; // contig0: [0,5] [8,12]; contig1: [10,20]
    int reach1[] = {3, 6, 9, 15}; // contig0: 3,6; contig1: 9,15 (wait: need length split)
    // Actually careful: reach_length_arr tells split
    int reach_len1[] = {2, 2};
    int shift1[] = {0, 0};
    int vs_size1[] = {2, 1};
    std::vector<int> res1 = getOverlappingQueries(queries1, 6, start_end1, reach1, reach_len1, shift1, vs_size1, 2);
    std::vector<int> expected1 = {0, 0, 1, -1, 1, 1, -1}; // contig0 query [5,10] overlaps reach indices 0(3),1(6); contig1 query [0,100] overlaps reach index 1(15)
    assert(res1 == expected1);

    // Test 2: Shift positive
    int queries2[] = {0, 5, 6};
    int start_end2[] = {0, 4, 6, 8};
    int reach2[] = {3, 5};
    int reach_len2[] = {2};
    int shift2[] = {1};
    int vs_size2[] = {2};
    std::vector<int> res2 = getOverlappingQueries(queries2, 3, start_end2, reach2, reach_len2, shift2, vs_size2, 1);
    std::vector<int> expected2 = {0, 1, -1}; // shifted reach0=4 (no overlap [5,6]), reach1=6 (yes)
    assert(res2 == expected2);

    // Test 3: Invalid contig ID
    int queries3[] = {5, 1, 2};
    int start_end3[] = {1, 2};
    int reach3[] = {5};
    int reach_len3[] = {1};
    int shift3[] = {0};
    int vs_size3[] = {1};
    std::vector<int> res3 = getOverlappingQueries(queries3, 3, start_end3, reach3, reach_len3, shift3, vs_size3, 1);
    std::vector<int> expected3 = {0, -1};
    assert(res3 == expected3);

    // Test 4: Empty reach array
    int queries4[] = {0, 1, 2};
    int start_end4[] = {0, 10};
    int reach4[] = {};
    int reach_len4[] = {0};
    int shift4[] = {0};
    int vs_size4[] = {1};
    std::vector<int> res4 = getOverlappingQueries(queries4, 3, start_end4, reach4, reach_len4, shift4, vs_size4, 1);
    std::vector<int> expected4 = {0, -1};
    assert(res4 == expected4);

    // Test 5: Negative shift and multiple queries
    int queries5[] = {0, 0, 10, 0, 15, 20};
    int start_end5[] = {5, 10};
    int reach5[] = {6, 16};
    int reach_len5[] = {2};
    int shift5[] = {-1};
    int vs_size5[] = {1};
    std::vector<int> res5 = getOverlappingQueries(queries5, 6, start_end5, reach5, reach_len5, shift5, vs_size5, 1);
    std::vector<int> expected5 = {0, 0, -1, 1, -1}; // query0 shifted reach0=5 (in [0,10]), reach1=15 (no); query1 shifted reach0=5 (not in [15,20]), reach1=15 (yes)
    assert(res5 == expected5);

    // Test 6: Interval with start > end (invalid)
    int queries6[] = {0, 1, 2};
    int start_end6[] = {5, 1};
    int reach6[] = {3};
    int reach_len6[] = {1};
    int shift6[] = {0};
    int vs_size6[] = {1};
    std::vector<int> res6 = getOverlappingQueries(queries6, 3, start_end6, reach6, reach_len6, shift6, vs_size6, 1);
    std::vector<int> expected6 = {0, -1}; // invalid interval ignored
    assert(res6 == expected6);

    // Test 7: Multiple contigs with mixed sizes
    int queries7[] = {1, 0, 100, 0, 0, 1};
    int start_end7[] = {2, 3, 4, 5, 0, 1};
    int reach7[] = {3, 4, 1};
    int reach_len7[] = {1, 2};
    int shift7[] = {0, 0};
    int vs_size7[] = {1, 2};
    std::vector<int> res7 = getOverlappingQueries(queries7, 6, start_end7, reach7, reach_len7, shift7, vs_size7, 2);
    // contig1: reach[0]=3, reach[1]=4, intervals [3,3] and [4,5] => both overlap with [0,100]
    // contig0: reach[2]=1, interval [2,3] => no overlap with [0,1]
    std::vector<int> expected7 = {0, 0, 1, -1, 1, -1};
    assert(res7 == expected7);

    // Test 8: No overlap anywhere
    int queries8[] = {0, 10, 20};
    int start_end8[] = {1, 2};
    int reach8[] = {5};
    int reach_len8[] = {1};
    int shift8[] = {0};
    int vs_size8[] = {1};
    std::vector<int> res8 = getOverlappingQueries(queries8, 3, start_end8, reach8, reach_len8, shift8, vs_size8, 1);
    std::vector<int> expected8 = {0, -1};
    assert(res8 == expected8);

    // Test 9: Boundary inclusive overlap
    int queries9[] = {0, 5, 5};
    int start_end9[] = {5, 5};
    int reach9[] = {5};
    int reach_len9[] = {1};
    int shift9[] = {0};
    int vs_size9[] = {1};
    std::vector<int> res9 = getOverlappingQueries(queries9, 3, start_end9, reach9, reach_len9, shift9, vs_size9, 1);
    std::vector<int> expected9 = {0, 0, -1};
    assert(res9 == expected9);

    return 0;
}
