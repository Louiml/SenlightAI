Write a C++ function that takes a positive integer `studentID` and a vector of `(examID, score)` pairs (where scores are integers between 0 and 100 inclusive), and returns the rank of the given student based on their exam score. The function should receive the student's ID and the list of all students' exam entries. Each student appears exactly once in the list with a unique exam ID, but the function should be able to handle duplicate scores. The rank is defined as: rank 1 is assigned to the highest score; if multiple students have the same score, they share the same rank (e.g., if the top three scores are 95, 90, 90, then the ranks are 1, 2, 2 respectively, and the next distinct score gets rank 4). If the student ID is not found in the list, return 0.
The solution uses a frequency-counting approach since scores are bounded between 0 and 100. First, we store each student's ID and score in a map for O(1) lookup. We also maintain an array `scoreFreq` of size 101 (indices 0–100) to count how many students achieved each score. Then, we compute ranks by iterating from score 100 down to 0: for each distinct score value `s`, the rank for that score is `currentRank + 1`, where `currentRank` is the cumulative number of students with scores strictly higher than `s`. After computing that rank, we add `scoreFreq[s]` to `currentRank`. The rank for the given student is obtained by first looking up their score in the map, then retrieving the precomputed rank for that score. Edge cases include: student ID not present (return 0), all scores equal (everyone gets rank 1), and empty input vector (then the student cannot be found, return 0). Time complexity is O(n + 101) ≈ O(n) for n students due to one pass over input and a constant-size loop over score values. Space complexity is O(n) for the map plus O(101) for the frequency array, which simplifies to O(n).
#include <vector>
#include <map>
#include <algorithm>

// Returns the rank of student with given ID based on exam scores.
// Scores range from 0 to 100 inclusive. Rank 1 for highest score, ties share rank.
// If student ID not found, returns 0.
int studentRank(int studentID, const std::vector<std::pair<int, int>>& examEntries) {
    // Map from exam ID to score
    std::map<int, int> idToScore;
    int scoreFreq[101] = {0}; // frequency of each score (0-100)

    for (const auto& entry : examEntries) {
        int examID = entry.first;
        int score = entry.second;
        idToScore[examID] = score;
        scoreFreq[score]++;
    }

    // Precompute rank for each distinct score
    int rankForScore[101] = {0};
    int cumulativeHigher = 0; // number of students with higher scores

    for (int s = 100; s >= 0; --s) {
        if (scoreFreq[s] > 0) {
            rankForScore[s] = cumulativeHigher + 1;
            cumulativeHigher += scoreFreq[s];
        }
    }

    // Look up the student's score
    auto it = idToScore.find(studentID);
    if (it == idToScore.end()) {
        return 0; // student not found
    }

    return rankForScore[it->second];
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic case: distinct scores
    std::vector<std::pair<int, int>> entries1 = {{1, 90}, {2, 80}, {3, 70}};
    assert(studentRank(1, entries1) == 1);
    assert(studentRank(2, entries1) == 2);
    assert(studentRank(3, entries1) == 3);

    // Ties share rank, next distinct gets next rank
    std::vector<std::pair<int, int>> entries2 = {{1, 95}, {2, 90}, {3, 90}, {4, 85}};
    assert(studentRank(1, entries2) == 1);
    assert(studentRank(2, entries2) == 2);
    assert(studentRank(3, entries2) == 2);
    assert(studentRank(4, entries2) == 4);

    // All same score -> everyone rank 1
    std::vector<std::pair<int, int>> entries3 = {{10, 88}, {20, 88}, {30, 88}};
    assert(studentRank(20, entries3) == 1);

    // Student not found
    assert(studentRank(99, entries1) == 0);

    // Edge: score 0 and 100
    std::vector<std::pair<int, int>> entries4 = {{1, 100}, {2, 0}, {3, 50}};
    assert(studentRank(1, entries4) == 1);
    assert(studentRank(3, entries4) == 2);
    assert(studentRank(2, entries4) == 3);

    // Empty list
    std::vector<std::pair<int, int>> entries5;
    assert(studentRank(1, entries5) == 0);

    // Many entries with duplicate scores
    std::vector<std::pair<int, int>> entries6;
    for (int i = 1; i <= 100; ++i) {
        entries6.push_back({i, 75});
    }
    assert(studentRank(50, entries6) == 1); // all tie
    assert(studentRank(101, entries6) == 0); // not in list

    return 0;
}
