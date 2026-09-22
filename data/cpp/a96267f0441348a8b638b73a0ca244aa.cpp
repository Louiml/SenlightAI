// Write a C++ function named `processCards` that simulates the card-stacking game described below. You are given an array of 52 strings, each representing a playing card with a rank (e.g., 'A', '2', ..., 'K') and a suit (e.g., 'H', 'D', 'C', 'S'), like "AH" or "10S". The cards are initially dealt face-up in a single row of 52 piles, each containing exactly one card. The game proceeds repeatedly from left to right: for each pile (starting from index 1 to 51), you check whether the top card of that pile can be moved to a pile exactly 3 positions to the left, or if not, 1 position to the left. A move is legal if the two cards share either the same rank (first character) or the same suit (second character). When moving, place the moved card on top of the target pile (increasing its count) and remove it from the source pile (decreasing its count). If the source pile becomes empty, it is removed from consideration (but the positions of piles to its right do not shift). After a successful move, restart the scanning from the target pile's index (not from the beginning). The function should return a vector of integers representing the final pile sizes (top counts) of all non-empty piles, in left-to-right order of original indices (ignoring empty piles). If a pile becomes empty, it should not appear in the output. The function must handle the input as a `const std::vector<std::string>&` and return `std::vector<int>`. Note: the input cards are guaranteed to be valid (non-empty strings with at least 2 characters, where the first character is the rank and the second is the suit). The game ends when no more moves are possible after scanning from the current position to the end. Assume the initial piles are at indices 0 to 51, each with one card.
// The problem is a simulation of a known card game (often called "Patience" or "Accordion"). The main challenge is to correctly track the state of piles after removals and to implement the scanning rule: after a move, you should resume from the pile to which the card was moved (which is always to the left of the current position), not from the beginning. The algorithm proceeds as follows:
//
// 1. Maintain an array of `std::vector<std::string>` (or a fixed-size 2D array like `std::string piles[52][52]` with a `top` array) to store stacks of cards. Initialize each pile with one card.
// 2. Use an index `i` that starts at 1 (since pile 0 cannot move anywhere). At each step, if pile `i` is empty (i.e., `top[i] < 0`), skip to the next non-empty pile.
// 3. For the current pile `i`, attempt to find a target pile `t` that is exactly 3 positions to the left, but skipping over empty piles. This means: starting from `i-1`, move left, counting non-empty piles, until you have passed exactly 3 non-empty piles. If you reach index -1 before counting 3, then no such target exists. Similarly, attempt 1 position to the left (i.e., the nearest non-empty pile to the left).
// 4. If a target is found and the top cards match (same first character or same second character), perform the move: push the card onto target, pop from source, decrement `top[source]`. Then set `i = target` and continue scanning from there (do not increment). If no move is possible for the current pile, increment `i`.
// 5. When `i` reaches 52 (end), the simulation ends.
// 6. Finally, collect all non-empty piles' sizes into a vector and return.
//
// Important edge cases: 
// - The game may require multiple moves from the same pile if a move is possible after a cascading effect.
// - Empty piles must be skipped in the step counting, but they do not shift positions.
// - The matching condition is case-sensitive as given (rank and suit are both uppercase letters/digits).
// - Since the input is fixed 52 cards, the maximum number of piles is 52, but after merges the pile sizes can grow.
//
// Time complexity: Each move reduces the total number of non-empty piles by 1. There are at most 51 moves (since initial 52 piles, final at least 1). Each scan for a target can take O(n) in the worst case due to skipping empty piles, but since n=52, it's effectively O(1). So the overall complexity is O(n^2) in the worst case with n=52, which is constant. Space complexity is O(n) for storing the stacks (each card is stored exactly once, and at most 52 cards total).
#include <vector>
#include <string>

// Simulate the card-stacking game and return the final pile sizes in left-to-right order.
// The input 'cards' must contain exactly 52 non-empty strings, each with rank and suit characters.
std::vector<int> processCards(const std::vector<std::string>& cards) {
    const int N = 52;
    std::vector<std::string> piles[N]; // Each pile as a vector of cards (stack)
    for (int i = 0; i < N; ++i) {
        piles[i].push_back(cards[i]);
    }

    // Helper lambda: check if two cards match (same rank or same suit)
    auto matches = [](const std::string& a, const std::string& b) {
        return (a[0] == b[0]) || (a[1] == b[1]);
    };

    // Helper lambda: find the target index that is 'step' positions to the left,
    // counting only non-empty piles. Returns -1 if no such pile exists.
    auto findTarget = [&](int start, int step) {
        int count = 0;
        int idx = start - 1;
        while (idx >= 0 && count < step) {
            if (!piles[idx].empty()) {
                count++;
            }
            if (count == step) {
                return idx;
            }
            idx--;
        }
        return -1;
    };

    int i = 1;
    while (i < N) {
        if (piles[i].empty()) {
            i++;
            continue;
        }
        // Try moving by 3 positions to the left
        int target3 = findTarget(i, 3);
        if (target3 >= 0 && matches(piles[i].back(), piles[target3].back())) {
            // Perform move
            piles[target3].push_back(piles[i].back());
            piles[i].pop_back();
            i = target3; // Resume scanning from the target
            continue;
        }
        // Try moving by 1 position to the left
        int target1 = findTarget(i, 1);
        if (target1 >= 0 && matches(piles[i].back(), piles[target1].back())) {
            piles[target1].push_back(piles[i].back());
            piles[i].pop_back();
            i = target1; // Resume scanning from the target
            continue;
        }
        // No move possible, advance
        i++;
    }

    // Collect non-empty pile sizes
    std::vector<int> result;
    for (int j = 0; j < N; ++j) {
        if (!piles[j].empty()) {
            result.push_back(static_cast<int>(piles[j].size()));
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Solution function declaration (as above)
std::vector<int> processCards(const std::vector<std::string>& cards);

int main() {
    // Test 1: All cards share the same rank and suit, so every move is possible.
    // Start with 52 identical cards. Each pile can move to the left by 3 or 1, merging all into one pile.
    std::vector<std::string> allSame(52, "AH");
    std::vector<int> result1 = processCards(allSame);
    assert(result1.size() == 1);
    assert(result1[0] == 52);

    // Test 2: No cards can ever match. For example, alternating suits with different ranks.
    // We need a pattern where no two cards share rank or suit. Since there are only 4 suits, use ranks '1'..'13' and suits 'H','D','C','S' but ensure no two adjacent have same suit or rank. A simple way: all cards have different suits? But only 4 suits, so duplicates will appear. Instead, use a deterministic pattern: for each i, rank = char('A' + i%26) and suit = char('1' + i%4) but that may cause matches. To guarantee no matches, use unique ranks (e.g., "1H","2D","3C","4S","5H",... but that repeats suits). Actually it's impossible to have no matches among 52 cards with only 4 suits and 13 ranks, because by pigeonhole principle some pair will share rank or suit. So we skip this; instead test a random-like input where no moves are possible: e.g., each card has a unique rank and suit such that both rank and suit differ from all others? That's impossible. So we test a case where no move is possible at start: e.g., first card "1H", second "2D" (differs), third "3C" (differs from all previous?), but third must be within 3 positions to move. For simplicity, we create a list where every card's rank and suit differ from the three immediate left neighbors. Since we have 13 ranks and 4 suits, we can create a repeating pattern that avoids immediate matches. Let's use a sequence: "1H","2D","3C","4S","5H","6D","7C","8S",... This ensures each card differs from its left neighbors (rank different, suit also different because suits cycle every 4 and ranks increment). After 52, the pattern repeats? It won't repeat exactly because ranks go to 13 then we use 'A','B',... but that would eventually match. Let's use a simpler approach: create a vector where each card is "X1","X2",... but with different suits? Not trivial. Instead, we trust the simulation with a hand-constructed small case: but the function takes 52 cards. For testing, we can use a known scenario from the original snippet: input of 52 cards from a standard shuffled deck that yields a known final pile count. However, to keep the test simple, we'll test with a trivial scenario where no moves are possible by ensuring the first card's rank and suit are unique and all others also unique? Not possible with only 4 suits. So we use an input where each card has a distinct rank (e.g., "1H","2D","3C","4S","5H",... but this repeats suits). For the first three positions, we can ensure no matches: "1H","2D","3C" – all different. But "4S" vs "1H" (distance 3) – different. So as long as we create a sequence where each card differs from the card 3 and 1 positions before, no moves will occur. We can generate such a sequence by cycling ranks 1-13 and suits H,D,C,S but with appropriate offsets. Let's define card[i] = rank_char = 'A' + (i % 13), suit_char = "HDCS"[(i/13) % 4]? That would repeat ranks, causing matches. Instead, use a fixed pattern: for i from 0 to 51, rank = "ABCDEFGHIJKLM" [i % 13] (13 different), suit = "HDCS"[(i / 13) % 4] (4 suits). This ensures that every rank appears 4 times, and every suit appears 13 times. However, some matches will occur because e.g., i=0 and i=13 have same rank? i=13 rank is 'B'? Actually i=0 rank='A', i=13 rank='A'? 13%13=0 so yes same rank, but distance 13, not within 3. So within any group of 4 consecutive cards, ranks are all different (since 4<13) and suits are different (since suits cycle every 13/4≈3.25? Actually suits repeat every 13 because (i/13)%4 changes after 13 steps, but within a block of 13, suits are all same? Wait: for i=0..12, suit='H' for all because i/13=0. So first 13 cards all have suit 'H'. That means many matches (same suit). So that's bad.

    Instead, we'll design a simple test: create a vector where cards are "1H","2D","3C","4S","5H","6D","7C","8S", ... and so on, but ensure that within 3 positions, no same rank or suit. With 52 cards, we can use ranks "A","2","3","4","5","6","7","8","9","10","J","Q","K" (13 ranks) and suits "H","D","C","S". To avoid matches, we can assign card i = rank[(i*2) % 13] + suit[(i*3) % 4]? That might give repeats. Better: Use a "Golomb ruler" type sequence? For testing, we can just trust that the simulation is correct by using a case with a known final output computed by hand for a small input? But the function requires exactly 52 cards. We can create a test where the first few cards are designed to cause a specific merge and then the rest are all distinct to stop further moves. For example, make card0="AH", card1="2D", card2="3C", card3="4S", card4="AH" (same as card0? distance 4, not within 3). Actually we want a move at start: card1? i=1, step 3 target is index -2? No. So no move. We need a move to happen. For simplicity, we can test with all same cards (as in test 1) which gives result [52]. That is a valid strong test.

    // Test 2: All cards are distinct in rank and suit within first few, but we want no moves. Since it's impossible to have all 52 distinct, we can just use a known deck and check that the result vector has size <= 52 and all numbers >0. But that's not a strong assert. Instead, we create a case where only the first two cards match and the rest are made so that no further moves happen. For example: card0="AH", card1="AD" (same rank 'A'), so card1 can move to card0? But step 1 target from index 1 is index 0, match. So after move, card1 becomes empty, card0 now has two cards. Then we continue scanning from index 0? But we start at i=1 originally. After move, i becomes 0, but then loop condition i<52, so we'll process i=0. But i=0 cannot move (no left). Then i=1 is empty, skip, i=2... and no more moves. So final piles: index0 has 2 cards, others 1 each. So result: [2,1,1,...,1] total 51 piles (since one merged). We can assert size 51 and first element 2.

    std::vector<std::string> mixed(52);
    mixed[0] = "AH";
    mixed[1] = "AD"; // same rank
    for (int i = 2; i < 52; ++i) {
        // Use unique ranks and suits that don't match any within 3 positions, simple approach: use "iH" with i as number, but ranks repeat? Let's use a pattern that ensures no matches: for i>=2, set card = std::to_string(i) + "D"? But i changes, so rank changes, but suit 'D' repeats and may match with card1 ('D')? card1 is "AD", suit 'D' matches with any other 'D' suit. To avoid, use different suits cycling but ensure no immediate matches. Since we only need to prevent further moves after the first merge, we can set from index 2 onwards all have suit 'S' and distinct ranks like "1S","2S","3S",... but that would match each other if within 3? e.g., "1S" and "3S" at distance 2? That would be a move. So we need to ensure no two within 3 positions share suit or rank. Using ranks that are consecutive numbers and all same suit would cause matches. So we switch suits every card: "1H","2D","3C","4S","5H","6D"... Let's do that. But then card1="AD" has suit 'D' and rank 'A'. The new cards have ranks numbers 1,2,3,... and suits cycling. To avoid matching with card1, we need that none of the cards at distance 1 or 3 from index 1 have rank 'A' or suit 'D'. Index 1's neighbors within 3 are indices 0,2,3,4? Actually from index 1, we look left for step 1: index0 (already handled), step3: indices -2? none. So after merging card1 into card0, we continue from index0, then i=1 empty, i=2 etc. For i>=2, we need to ensure no moves. So we need for each i>=2, no match with i-1, i-3 (the nearest non-empty? but some might be empty). To be safe, we create a pattern where every three consecutive cards have all different ranks and suits. For example, a repeating pattern of suits H,D,C,S and ranks that are unique within a window of 4. We can use ranks 1-13 and suits H,D,C,S in a cycle but shifted so that within any 3, no same. A simple way: for i from 2 to 51, set rank = (i % 13) + 1 as character? But 10 is two characters, so use "A","2",...,"K" but that's messy. Alternatively, just trust the simulation with test1 and a simpler test: Use a case where all cards are the same except the first few? Actually test1 already covers the extreme. For a second test, we can create an input where no moves are possible at all. That is impossible. But we can test with a known case from the original problem: a standard deck might produce some number. Instead, we'll test with a case where the first card "AH", second "2D", third "3C", fourth "4S" – no matches among the first four. Then all subsequent cards are set to "AH" but that would match with index0? Not within 3? For i=4, step 3 target is index1? But index1 is not empty, but card "2D" does not match "AH"? Actually card4 "AH" vs card1 "2D" – no match. So no move. Then card5 "AH" vs card2 "3C" – no match, etc. So no moves at all. So we can create a vector: first four are "AH","2D","3C","4S", then all remaining 48 are "AH"? But then card4 (index4) "AH" vs index1 "2D" (step3) no match, vs index3 "4S" (step1) no match. So no move. So entire game does nothing, result is 52 piles of size 1. That is a valid test. Let's do that.

    std::vector<std::string> noMove(52);
    noMove[0] = "AH";
    noMove[1] = "2D";
    noMove[2] = "3C";
    noMove[3] = "4S";
    for (int i = 4; i < 52; ++i) {
        noMove[i] = "5H"; // use a card that doesn't match with any of the first four within 3 positions? For index4, step3 target is index1 "2D" – no match (2 vs 5, D vs H). step1 target index3 "4S" – no match (4 vs 5, S vs H). So safe. For index5, step3 target index2 "3C" – no match; step1 target index4 "5H" – same? That would match because both "5H"! So we need to avoid that. So we cannot repeat the same card consecutively. So use alternating "5H","6D","7C","8S", etc. So we can create a pattern from index4 onward that ensures no matches with nearby. But for simplicity, we can just use a sequence of unique cards like "1H","2D","3C","4S","5H","6D"... and ensure that no two within 3 match. This sequence has the property that every card differs in rank and suit from its neighbors within distance 3? Let's check: "1H" index0, "2D" index1, "3C" index2, "4S" index3, "5H" index4 – index4 vs index1 (step3): rank 5 vs 2 diff, suit H vs D diff; vs index3 (step1): rank 5 vs 4 diff, suit H vs S diff. Good. Index5 "6D" vs index2 "3C": diff; vs index4 "5H": rank 6 vs 5 diff, suit D vs H diff. Good. So this pattern works for the whole 52 if we use ranks 1-13 and suits H,D,C,S repeatedly, but ranks will repeat after 13, and suits repeat after 4. However, within any window of 3, ranks are all different because the sequence increments by 1, and suits cycle every 4, so within 4 consecutive, suits are all different. For a window of 3, suits are all different as well. So no matches. So we can generate a full deck with a simple pattern: for i=0..51, rank = i % 13 (mapped to characters "A","2","3","4","5","6","7","8","9","T","J","Q","K"), suit = i % 4 mapped to "H","D","C","S". But then index0 "AH", index1 "2D", index2 "3C", index3 "4S", index4 "5H" – matches our pattern. This pattern ensures no two cards within 3 positions share rank or suit? Let's verify: rank i%13 repeats every 13, so within 13, ranks are distinct. Suits i%4 repeat every 4, so within 4, suits distinct. For any distance <=3, both rank and suit are different. So no moves possible. Thus we can test that result is 52 ones.

    // Generate 52 cards with pattern i%13 ranks and i%4 suits
    auto rankChar = [](int r) {
        const char* ranks = "A23456789TJQK";
        return std::string(1, ranks[r]);
    };
    auto suitChar = [](int s) {
        const char* suits = "HDCS";
        return std::string(1, suits[s]);
    };
    std::vector<std::string> noMovePattern;
    for (int i = 0; i < 52; ++i) {
        noMovePattern.push_back(rankChar(i % 13) + suitChar(i % 4));
    }
    std::vector<int> result2 = processCards(noMovePattern);
    assert(result2.size() == 52);
    for (int v : result2) assert(v == 1);

    // Test 3: A specific case where only one move happens, as described earlier.
    std::vector<std::string> oneMove(52);
    oneMove[0] = "AH";
    oneMove[1] = "AD"; // match with index0
    for (int i = 2; i < 52; ++i) {
        // Use the noMovePattern from index2 onward but ensure no collisions with index0/1? 
        // Simpler: reuse the noMovePattern but replace first two? Actually the noMovePattern has "AH" at index0? Not. To avoid complexity, we can just copy the noMovePattern and then override index1 to "AD". But then index1 "AD" might match with index4? Let's check: index1 "AD" vs index4 "5H" (step3) – rank A vs 5 diff, suit D vs H diff; vs index2 "3C" (step1) – rank A vs 3 diff, suit D vs C diff. So safe. So we can start with noMovePattern for all 52, then set oneMove[1] = "AD". Then card1 can move to card0 (same rank 'A'). After merging, we have 51 piles. The rest remain unchanged because no other matches. So result should be 51 piles, with the first pile size 2. Let's assert that.
    std::vector<std::string> oneMovePattern = noMovePattern;
    oneMovePattern[1] = "AD";
    std::vector<int> result3 = processCards(oneMovePattern);
    assert(result3.size() == 51);
    assert(result3[0] == 2);
    // All other piles should be 1
    for (size_t j = 1; j < result3.size(); ++j) {
        assert(result3[j] == 1);
    }

    return 0;
}
