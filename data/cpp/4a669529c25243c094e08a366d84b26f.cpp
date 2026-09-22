// Given a list of students where each student has a unique 19-digit examination ID, an assigned machine seat number (from 1 to 1000), and a corresponding exam location seat number, write a C++ function `lookupStudentSeat` that takes the number of students `N`, an array structured as described (machine seat index → student ID and location), and a list of `M` machine seat numbers to query, and returns a `std::vector<std::pair<long long, int>>` where each pair contains the student's exam ID and location seat number for each queried machine seat. The input may contain duplicate machine seat numbers in the query list, and the function must handle queries for machine seats that were never assigned (in which case return `{-1, -1}` for that query). The original student data is not necessarily sorted by machine seat, and the machine seat numbers are within the range `[1, 1000]`.
#include <cassert>
#include <vector>
#include <tuple>
#include <utility>

// The solution function is assumed to be defined above
// (including the StudentInfo struct and MAX_MACHINE_SEATS constant)
// Here we only provide the test main.

int main() {
    // Test case 1: Basic lookup with one student
    std::vector<std::tuple<long long, int, int>> students1 = {
        {1234567890123456789LL, 1, 101}
    };
    std::vector<int> queries1 = {1};
    auto res1 = lookupStudentSeat(1, students1, queries1);
    assert(res1.size() == 1);
    assert(res1[0].first == 1234567890123456789LL);
    assert(res1[0].second == 101);

    // Test case 2: Multiple students and multiple queries
    std::vector<std::tuple<long long, int, int>> students2 = {
        {1111111111111111111LL, 5, 55},
        {2222222222222222222LL, 2, 22},
        {3333333333333333333LL, 1000, 999}
    };
    std::vector<int> queries2 = {5, 2, 1000};
    auto res2 = lookupStudentSeat(3, students2, queries2);
    assert(res2.size() == 3);
    assert(res2[0].first == 1111111111111111111LL && res2[0].second == 55);
    assert(res2[1].first == 2222222222222222222LL && res2[1].second == 22);
    assert(res2[2].first == 3333333333333333333LL && res2[2].second == 999);

    // Test case 3: Query for unassigned machine seat returns {-1, -1}
    std::vector<std::tuple<long long, int, int>> students3 = {
        {4444444444444444444LL, 10, 10}
    };
    std::vector<int> queries3 = {10, 20, 1};
    auto res3 = lookupStudentSeat(1, students3, queries3);
    assert(res3.size() == 3);
    assert(res3[0].first == 4444444444444444444LL && res3[0].second == 10);
    assert(res3[1].first == -1 && res3[1].second == -1);
    assert(res3[2].first == -1 && res3[2].second == -1);

    // Test case 4: Duplicate queries return correct repeated results
    std::vector<std::tuple<long long, int, int>> students4 = {
        {5555555555555555555LL, 3, 33}
    };
    std::vector<int> queries4 = {3, 3, 3};
    auto res4 = lookupStudentSeat(1, students4, queries4);
    assert(res4.size() == 3);
    for (const auto& p : res4) {
        assert(p.first == 5555555555555555555LL);
        assert(p.second == 33);
    }

    // Test case 5: Machine seat number out of range in query
    std::vector<std::tuple<long long, int, int>> students5 = {
        {6666666666666666666LL, 1, 1}
    };
    std::vector<int> queries5 = {0, 1001, 1};
    auto res5 = lookupStudentSeat(1, students5, queries5);
    assert(res5.size() == 3);
    assert(res5[0].first == -1 && res5[0].second == -1);
    assert(res5[1].first == -1 && res5[1].second == -1);
    assert(res5[2].first == 6666666666666666666LL && res5[2].second == 1);

    return 0;
}
#include <vector>
#include <utility>
#include <cstdint>

// Struct to hold student info at a given machine seat
struct StudentInfo {
    long long testId;
    int location;
};

// Predefined maximum machine seat number (based on problem constraints)
const int MAX_MACHINE_SEATS = 1000;

// Lookup function: given N students (each described by test ID, machine seat, location),
// and a list of M machine seats to query, return vector of {testId, location} for each query.
// For unassigned machine seats, returns {-1, -1}.
std::vector<std::pair<long long, int>> lookupStudentSeat(
    int N,
    const std::vector<std::tuple<long long, int, int>>& studentRecords,
    const std::vector<int>& querySeats) {

    // Initialize direct-address table with sentinel values
    std::vector<StudentInfo> table(MAX_MACHINE_SEATS + 1, StudentInfo{-1, -1});

    // Store each student's info
    for (const auto& rec : studentRecords) {
        long long testId = std::get<0>(rec);
        int machineSeat = std::get<1>(rec);
        int location = std::get<2>(rec);
        if (machineSeat >= 1 && machineSeat <= MAX_MACHINE_SEATS) {
            table[machineSeat] = StudentInfo{testId, location};
        }
    }

    // Build result for each query
    std::vector<std::pair<long long, int>> result;
    result.reserve(querySeats.size());
    for (int seat : querySeats) {
        if (seat >= 1 && seat <= MAX_MACHINE_SEATS) {
            result.emplace_back(table[seat].testId, table[seat].location);
        } else {
            result.emplace_back(-1, -1);
        }
    }
    return result;
}
// The core idea is to use a direct-address table (an array of size 1001) indexed by machine seat number. Since the machine seat range is small and fixed, we can preallocate an array of structs (or a vector) where each element stores the test ID and location. We initialize every slot to a sentinel value (e.g., test ID = -1, location = -1) to indicate "not assigned". Then, for each student record, we store their data at the index equal to their machine seat. For each query, we simply access that index and return the stored pair (or the sentinel if the slot was never set). This approach runs in O(N + M) time because each insertion and query is O(1). Space complexity is O(1001) which is constant, independent of N and M. Edge cases: duplicate machine seat assignments should not occur per problem statement, but if they do, the later entry overwrites the earlier one; queries for unassigned seats return the sentinel; and the query list may contain duplicates, which is handled naturally.
