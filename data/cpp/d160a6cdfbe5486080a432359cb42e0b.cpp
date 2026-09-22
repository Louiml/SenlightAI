Write a C++ function `chickenPieces(int has, int needs)` that takes two non-negative integers: the number of chicken pieces Dr. Chaz currently has and the number he either needs or will have extra, depending on the scenario. The function must return a string exactly matching the format of the given snippet: if `has > needs`, return `"Dr. Chaz needs X piece(s) of chicken!"` where X is the difference and the word "piece" is plural only when X > 1. If `has <= needs`, return `"Dr. Chaz will have Y piece(s) of chicken left over!"` where Y is `needs - has` and pluralization applies the same way. The output must have correct singular/plural forms, and the message must match the exact wording and punctuation from the snippet, including the exclamation mark and the correct use of "needs" vs "will have".
#include <cassert>

int main() {
    // Basic cases from the snippet
    assert(chickenPieces(5, 3) == "Dr. Chaz needs 2 more pieces of chicken!");
    assert(chickenPieces(1, 4) == "Dr. Chaz will have 3 pieces of chicken left over!");
    
    // Singular and plural boundaries
    assert(chickenPieces(2, 1) == "Dr. Chaz needs 1 more piece of chicken!");
    assert(chickenPieces(1, 2) == "Dr. Chaz will have 1 piece of chicken left over!");
    
    // Equal values: leftover zero is plural
    assert(chickenPieces(3, 3) == "Dr. Chaz will have 0 pieces of chicken left over!");
    
    // Larger differences
    assert(chickenPieces(10, 0) == "Dr. Chaz needs 10 more pieces of chicken!");
    assert(chickenPieces(0, 10) == "Dr. Chaz will have 10 pieces of chicken left over!");
    
    // Boundary with zero and one
    assert(chickenPieces(1, 0) == "Dr. Chaz needs 1 more piece of chicken!");
    assert(chickenPieces(0, 1) == "Dr. Chaz will have 1 piece of chicken left over!");
    
    // Maximum values (no overflow)
    assert(chickenPieces(1000000, 999999) == "Dr. Chaz needs 1 more piece of chicken!");
    assert(chickenPieces(999999, 1000000) == "Dr. Chaz will have 1 piece of chicken left over!");
}
#include <string>

// Return a message describing the chicken piece situation.
std::string chickenPieces(int has, int needs) {
    if (has > needs) {
        int deficit = has - needs;
        std::string plural = (deficit == 1) ? "" : "s";
        return "Dr. Chaz needs " + std::to_string(deficit) + " more piece" + plural + " of chicken!";
    } else {
        int leftover = needs - has;
        std::string plural = (leftover == 1) ? "" : "s";
        return "Dr. Chaz will have " + std::to_string(leftover) + " piece" + plural + " of chicken left over!";
    }
}
// The core logic is a simple conditional comparison between the two input numbers. If the current pieces (`has`) exceed what is needed (`needs`), compute the difference `has - needs` and produce the "needs ... more piece(s)" message. Otherwise, compute `needs - has` and produce the "will have ... piece(s) left over" message. The pluralization rule is: use "s" only when the count is not exactly 1 (i.e., count > 1). Edge cases: when the counts are equal, the difference is 0, which requires plural "pieces" (since 0 is not 1). Also, if the difference is 1, use singular "piece". The function must handle any non-negative integers without overflow concerns (differences fit in normal `int`). Time complexity is O(1) and space complexity is O(1) aside from the returned string.
