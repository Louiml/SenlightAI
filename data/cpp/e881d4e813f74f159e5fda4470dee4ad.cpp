/*
Write a C++ function `simulate_worm` that simulates a worm moving on a rectangular board. The function takes six parameters: the board width `x_max`, board height `y_max`, the worm's initial length `edzio_size`, the board contents as a `std::vector<std::string>` (each string a row of characters, with `'T'` denoting a wall, `'G'` a growth pellet, `'K'` a shrink pellet, lowercase letters as colour-changing cells, and `'.'` empty), and a string `moves` containing movement commands (`'p'`=right, `'l'`=left, `'d'`=down, `'g'`=up) possibly with repetitions like `"3d"` meaning three moves down, and `"2p"` meaning two moves right. The worm starts at position (0,0) with length `edzio_size`, all segments initially coloured `'.'`. On each move, if the target cell is a wall (`'T'`) or out of bounds, the move is ignored. If the target is `'G'`, the worm grows by one segment before moving, and the cell becomes empty. If the target is `'K'`, the worm shrinks by one segment (the tail segment, i.e., the one behind the head) after moving, and the cell becomes empty. If the target is a lowercase letter, the head's colour changes to that letter. If the target is empty, nothing special happens. After all moves (or if the worm dies by shrinking when it has only one segment left), the function should return a struct containing: `alive` (bool), `head_x`, `head_y` (final head position), and `board` (a `std::vector<std::string>` representing the final board after all moves, where the worm's head leaves a trail of its current colour on every cell it visits, and any cells previously coloured by the worm remain coloured unless overwritten). The worm's body is not drawn on the board; only the head's path is drawn (each visited cell gets the head's current colour at the time of that visit). The board is initially given; the function modifies it in-place. The input moves string may contain digits before a letter to repeat that move that many times; if the worm dies mid-repeat, stop processing further moves. The function should handle edge cases: moves that would take the worm out of bounds are ignored, the worm starts at (0,0) and initially has length `edzio_size` (all segments exist but do not affect the board), and the board dimensions are at least 1x1.
*/
#include <vector>
#include <string>
#include <cctype>

struct WormResult {
    bool alive;
    int head_x;
    int head_y;
    std::vector<std::string> board;
};

// Simulates worm movement on a rectangular board.
// 'p'=right, 'l'=left, 'd'=down, 'g'=up.
// Digits before a letter repeat that move that many times.
// Returns final state after all moves (or until worm dies).
WormResult simulate_worm(int x_max, int y_max, int edzio_size,
                         std::vector<std::string> board,
                         const std::string& moves) {
    int x = 0, y = 0;
    int len = edzio_size;
    char head_colour = '.';
    bool alive = true;

    size_t i = 0;
    while (i < moves.size() && alive) {
        int repeat = 0;
        if (std::isdigit(static_cast<unsigned char>(moves[i]))) {
            while (i < moves.size() && std::isdigit(static_cast<unsigned char>(moves[i]))) {
                repeat = repeat * 10 + (moves[i] - '0');
                ++i;
            }
        } else {
            repeat = 1;
        }
        if (i >= moves.size()) break;
        char dir = moves[i++];

        for (int r = 0; r < repeat && alive; ++r) {
            int next_x = x, next_y = y;
            switch (dir) {
                case 'p': next_x++; break;
                case 'l': next_x--; break;
                case 'd': next_y++; break;
                case 'g': next_y--; break;
                default: break; // invalid, ignore
            }
            // out of bounds or wall
            if (next_x < 0 || next_x >= x_max || next_y < 0 || next_y >= y_max)
                continue;
            if (board[next_y][next_x] == 'T')
                continue;

            // Process the cell
            char cell = board[next_y][next_x];
            if (cell == 'G') {
                ++len; // grow
            }
            // Move head
            x = next_x;
            y = next_y;

            if (cell == 'K') {
                --len; // shrink
            }
            if (len <= 0) {
                alive = false;
                break;
            }

            // Update head colour if cell is a lowercase letter
            if (cell >= 'a' && cell <= 'z') {
                head_colour = cell;
            }
            // Paint the current cell with head colour
            board[y][x] = head_colour;
        }
    }

    return {alive, x, y, board};
}
#include <cassert>
#include <vector>
#include <string>

// (Include the solution code above here)

int main() {
    // Basic movement: move right twice, down once, left once
    {
        std::vector<std::string> board = {"....", "...."};
        auto res = simulate_worm(4, 2, 1, board, "2pdl");
        assert(res.alive);
        assert(res.head_x == 1 && res.head_y == 1);
        assert(res.board[0] == ".pp.");
        assert(res.board[1] == ".d..");
    }

    // Wall stops movement
    {
        std::vector<std::string> board = {".T..", "...."};
        auto res = simulate_worm(4, 2, 1, board, "p");
        assert(res.alive);
        assert(res.head_x == 0 && res.head_y == 0);
        assert(res.board[0] == ".T..");
    }

    // Growth pellet increases length but still moves and paints
    {
        std::vector<std::string> board = {".G..", "...."};
        auto res = simulate_worm(4, 1, 1, board, "p");
        assert(res.alive);
        assert(res.head_x == 1 && res.head_y == 0);
        // After eating G, length becomes 2, cell painted with head colour '.'
        assert(res.board[0] == "...."); // board[0][1] becomes '.', but originally 'G' so it's same '.'? Wait originally 'G' becomes '.' after painting, so board[0] = "...." ? Actually board[0][1] was 'G', but after painting with '.', it becomes '.'. So board[0] = "...." but that's same as original except 'G' changed to '.'. So assert board[0] == "...." works.
    }

    // Shrink pellet can kill worm of length 1
    {
        std::vector<std::string> board = {".K..", "...."};
        auto res = simulate_worm(4, 1, 1, board, "p");
        assert(!res.alive);
        assert(res.head_x == 1 && res.head_y == 0);
        // Length becomes 0, worm dies, cell not painted (since check after shrink)
        assert(res.board[0][1] == 'K'); // remains 'K' because we didn't paint after death
    }

    // Lowercase changes head colour and paints
    {
        std::vector<std::string> board = {".r..", "...."};
        auto res = simulate_worm(4, 1, 1, board, "p");
        assert(res.alive);
        assert(res.head_colour_not_needed); // not in struct
        assert(res.board[0] == ".r.."); // head moves to cell, colour becomes 'r', paints 'r'
        assert(res.head_x == 1 && res.head_y == 0);
    }

    // Repetition: "3p" moves three times right
    {
        std::vector<std::string> board = {".....", "....."};
        auto res = simulate_worm(5, 1, 1, board, "3p");
        assert(res.alive);
        assert(res.head_x == 3 && res.head_y == 0);
        assert(res.board[0] == ".ppp.");
    }

    // Out of bounds ignored
    {
        std::vector<std::string> board = {".."};
        auto res = simulate_worm(2, 1, 1, board, "ll");
        assert(res.alive);
        assert(res.head_x == 0 && res.head_y == 0);
        assert(res.board[0] == "..");
    }

    // Worm dies mid-repeat stops processing
    {
        std::vector<std::string> board = {".K..", "...."};
        auto res = simulate_worm(4, 1, 1, board, "2p");
        assert(!res.alive);
        assert(res.head_x == 1 && res.head_y == 0);
        // Second move not executed because dead
        assert(res.board[0][2] == '.'); // unchanged from initial
    }
}
// The solution needs to model the worm as a circular doubly-linked list of segments, similar to the original code, but we can simplify by using a `std::list<char>` or just a deque of colours representing the body segments. However, the original code uses a circular linked list where `current` points to the head, and `current->next` is the segment behind the head (the tail direction). For movement, we need to:
// - Parse the moves string, handling repetitions like `"3d"` meaning move down three times.
// - For each move, compute the target cell based on direction, check bounds and wall.
// - If the target is `'G'`, grow the body (add a segment) before moving.
// - Then advance the head by moving `current = current->next`, effectively moving the head one step forward; the old head's position becomes part of the body but we don't explicitly track body positions since we only care about the head's path for drawing.
// - If the target was `'K'`, shrink by removing the tail segment (which is `current->next` after the head advance, actually the segment that was previously ahead of the head? Need to be careful: In the original code, after moving, `current` points to the new head. If `next_field == 'K'`, they call `shrink()` which removes the segment that is `current->next` (which is the tail relative to the new head). But wait, in a circular list, `current->next` is the segment behind the head (the one that was just left behind). Actually in a circular list where `current` is head, `current->next` is the next segment in the circle, which is the one that follows the head around the circle, i.e., the tail. So after advancing `current = current->next`, the old head becomes the new tail? Hmm, let's think: Initially head is current. When we move, we want the head to move to a new segment. The original code does `current = current->next`, which means the new head is the segment that was previously the tail (the one farthest from the old head). This is a clever way to simulate movement: the body is a circle, and moving just advances the "head" pointer one step along the circle. The segment that was the tail becomes the new head, and the old head becomes the new tail. So the body effectively moves without needing to shift all segments. That's the key.
//
// So we can replicate that: Maintain a circular doubly-linked list of segments. Growing adds a new segment right after the head (so the circle gets bigger). Shrinking removes one segment (the one that is `current->next`? Actually in `shrink()` they remove the segment that is `current->next`? Let's re-read: `shrink()` does: `current->next->prev = current->prev; current->prev->next = current->next; Segment* temp = current->next; delete current; current = temp;` Wait, that removes `current`, not `current->next`! That seems odd. Let's look again: In the original `shrink()`:
// ```
// void shrink() {
//     if (current != current->next) {
//         current->next->prev = current->prev;
//         current->prev->next = current->next;
//         Segment* temp = current->next;
//         delete current;
//         current = temp;
//     }
//     ...
// }
// ```
// So it deletes `current` and sets `current = current->next`. That removes the head segment, not the tail. That would make the worm shorter by removing the head, which is weird. But in the context of the original game, when the worm eats a 'K', it shrinks, meaning it loses a segment. Which segment? Probably the tail, not the head. But the original code removes `current` (the head) after moving? Wait, in `move()`, they do:
// ```
// current = current->next;  // move head one step
// if (next_field == 'K') {
//     shrink();
// }
// ```
// So after moving, `current` is the new head. Then `shrink()` deletes `current` (the new head) and sets `current` to `current->next` (which is the tail of the new configuration). That effectively removes the segment that was the tail before the move? Let's simulate: Suppose we have segments A(head), B, C in a circle. `current=A`. Moving: `current = current->next` => current=B. Now B is head. Then shrink deletes B and sets current = B->next = C. So now C is head, and the circle is A-C (since B is deleted). The worm length decreased by one, and the head moved forward (from A to C) but skipped B? This is confusing.
//
// Actually, let's think differently. The original code is not necessarily logically perfect, but we need to design a correct simulation. For simplicity, we can model the worm as a list of segment colours, but since we only need to know the length (for alive) and the head's position (for drawing), we don't need to track the body positions. The worm's body length affects only whether it's alive (length > 0) and whether growing adds a segment (increasing length) and shrinking removes a segment (decreasing length). The body's positions don't need to be tracked because the board only gets painted by the head's path. The original code never draws the body; it only draws the head's current colour at the new position when the cell is empty. So we can simply track the worm's length as an integer, and ignore the body's actual positions. That simplifies greatly.
//
// But wait, the original code uses the circular list to manage the head's colour change: when you eat a lowercase letter, you change the head's colour, which then persists for future moves. Since the head is just a segment, we can store the head colour separately. Growing and shrinking only affect length. However, note that when you shrink, the head colour might change? In the original code, after moving, the head is a different segment (previously the tail). That tail segment had its own colour (which was set when it was head earlier). So the colour of the new head is the colour of the segment that is now head (which was previously the tail). So the worm's head colour is not simply a single variable; it depends on the sequence of segments and their individual colours. Because each segment has a colour, and when the head moves, the old head becomes tail, and the new head takes the colour of the segment that was previously the tail. So the colours propagate along the worm.
//
// Thus we need to model the worm as a sequence of segments, each with a colour. Growing adds a new segment with colour `'.'` (or perhaps the colour of the newly eaten cell? In original code, when eating 'G', they `grow()` then set `next_field='.'`, so the new segment is added before moving, and the new segment's colour is `'.'`. Then moving advances to that new segment, which will be the new head with colour `'.'`. Then if the target cell is empty, they poke the head's colour (which is `'.'`). So yes.
//
// Shrinking: in original code, after moving, if next_field=='K', they call shrink() which deletes the current head (the new head) and sets current to the next segment. That effectively removes the segment that just became head, meaning the worm loses the segment that was just moved into? That doesn't make sense. Let's re-examine the original code more carefully. Perhaps the circular list's orientation is different. Let's trace a simple example: initial worm size 2, segments S1, S2 in a circle, current=S1. `current->next` is S2, `current->prev` is S2 (since it's circular). Move right: `current = current->next` => current=S2. Now S2 is head. Then if we eat 'K', shrink() deletes S2 and sets current=S2->next=S1. So now head is S1. The worm length becomes 1. The head position? In the original code, after move, they set x=next_x, y=next_y, so the position is the new cell. Then later they might shrink, but they don't change x,y because shrink only changes the current segment pointer. So the head position remains at the new cell after moving, even if the head segment was removed. That seems like the head "dies" but actually the worm loses the segment that was at the head, so the new head is the segment behind it, but the physical position remains? This is inconsistent.
//
// Given the complexity and potential bugs in the original, for the task we should design a clear, correct simulation. Since the task is to create an independent programming exercise, we can define the behavior explicitly: The worm has a length L. It moves step by step. Each segment has a colour. When the head moves to a new cell, the head's colour is updated based on the cell: if cell is a lowercase letter, head colour becomes that letter. If cell is 'G', grow: add a new segment at the tail? Or at the head? In typical snake games, growing adds a segment at the tail, not changing the head. But in the original code, grow adds a segment right after the head in the circle, and then moving advances the head to that new segment, so effectively the head moves into a new segment of colour '.', and the old head becomes the tail. So growth happens at the head. For simplicity, we'll adopt the original's approach: grow adds a new segment adjacent to the head (in the tail direction? actually in the circle it's inserted between head and its next). Then moving advances the head pointer to that new segment. So the head colour becomes '.'. Then if the target cell was 'G', the cell becomes empty, and the head colour remains '.' for that step. But then if the new cell is a lowercase letter, the head colour changes to that letter. If it's empty, head colour stays whatever it was before (which was the colour of the segment that became head).
//
// Let's define a simpler model: We'll maintain a circular doubly-linked list of segments, exactly as in the original, and replicate the logic for movement, grow, shrink. But we need to ensure shrink removes the tail, not the head. The original's shrink seems to remove the head, which might be intentional if the head is the "old" head after moving? Let's re-interpret: In the circular list, `current` always points to the head. After `current = current->next`, the head is now the segment that was previously the tail (since tail is one step behind head in the circle). So the old head becomes the tail. Then if we want to shrink (remove tail), we should remove `current->prev` (the old head, now tail). But the original removes `current` (the new head) which is wrong. So we'll fix that in our reference solution: shrink should remove the tail, which is `current->prev`. That way the worm loses its tail, and the head remains the same. However, the original's shrink deletes the head, which would make the worm's position jump? Actually the position is stored separately in x,y, so it doesn't matter. But for correct gameplay, losing the tail is standard.
//
// Given the task is to write a function that simulates the worm according to a clear spec we define, we can specify: "Grow adds a new segment with colour '.' at the tail (so the head remains unchanged). Shrink removes the tail segment." But the original code adds at the head? Actually growing in the original inserts a new segment right after the head in the circle, but since the circle is symmetric, it also inserts before the tail? It's ambiguous. To keep the task simpler and avoid confusion, we'll define: The worm has a length; each segment has a colour. Initially all segments are '.'. The head's colour is the colour of the first segment. When moving, we create a new head segment with the colour determined by the target cell: if cell is '.', colour becomes the previous head's colour; if cell is a lowercase letter, that letter; if 'G', then we grow first (length+1) and the new head colour is '.' (the cell becomes empty); if 'K', then we shrink (length-1) and the new head colour is the previous head's colour? Actually to avoid complexity, we can mimic the original behavior but fix the shrink bug. But for a standalone task, we can simplify: treat the worm as just a length and a head colour. Since the body segments' colours don't affect the board (only the head's path is drawn), we don't need to propagate colours through the body. The only place body colours matter is when the worm shrinks, the new head colour might be different from the old head colour. But we can ignore that by specifying that the head colour remains the same when eating 'K' (shrink just reduces length). That makes the task much simpler and still interesting. The original code does propagate colours, but for an exercise, we can define: "The head's colour is a property that changes only when the worm eats a lowercase letter. Growing and shrinking do not change the head colour." That is a reasonable simplification and clearly specified.
//
// Let's do that: The worm's head colour is initially `'.'`. When moving to a cell containing a lowercase letter, the head colour becomes that letter. When moving to a cell containing 'G', the worm's length increases by 1 (but does not affect colour). When moving to 'K', length decreases by 1 (if length becomes 0, the worm dies and stops moving). Otherwise, colour stays the same. After moving, if the cell was not 'T' and not 'K' (and not 'G'? Actually after eating 'G', the cell becomes empty and the head moves there, painting it with the current head colour (which may be '.' or previous colour). For 'K', the cell becomes empty and the head moves there, painting it with the head colour, but then the worm shrinks and might die. So the order matters: move, then shrink, then if alive, paint. We'll specify: On each move, first determine target. If out of bounds or 'T', skip. Otherwise, if target is 'G', increase length by 1. Then move head to target. Then if target is 'K', decrease length by 1. Then if length > 0, paint the target cell with the current head colour, and if target was a lowercase letter, update head colour before painting (so the cell gets painted with the new colour). Actually the original paints after updating colour? In original, for lowercase letter, they set `current->colour = next_field` and then later when they poke, they use `current->colour` which is the new colour. So the cell gets painted with the new colour. For 'G', they set next_field='.', then later if next_field=='.', they poke with current->colour (which is likely '.' because grow added a new segment with colour '.'). So the cell gets painted with '.'.
//
// To avoid confusion, we'll define the painting rule: After successfully moving to a cell (and after any grow/shrink effects), if the worm is still alive, the cell is set to the head's colour. The head's colour is updated before painting if the cell initially contained a lowercase letter. For 'G' and 'K', the head colour does not change, but the cell becomes empty ('.') before moving? Actually 'G' and 'K' are replaced by empty after the worm consumes them, but the worm still moves onto that cell and paints it. The painting sets the cell to the head colour, overwriting the 'G' or 'K'. So effectively, 'G' and 'K' are converted to the head colour after being consumed. That matches the original: for 'G', next_field becomes '.', then since next_field=='.', they poke with current->colour. So the cell becomes current->colour (which might be '.'). For 'K', next_field is 'K', they shrink, then if next_field=='.'? Actually after shrink, they check if alive, then if next_field=='.' they poke, but next_field was 'K', not '.', so they don't poke? Wait in original, for 'K', they set next_field? No, they don't change next_field. Then after shrink, they check `if (next_field == '.')` but next_field is 'K', so they don't poke. That means the head moves onto the 'K' cell but doesn't paint it? That seems like a bug. Actually after eating 'K', the cell becomes empty? In typical snake, the pellet disappears but the snake occupies the cell, so the cell should become snake colour. But original doesn't paint it. For our task, we'll define consistently: All non-wall cells that the head moves onto are painted with the head's current colour (after any colour change from lowercase letters). So 'G', 'K', '.', and lowercase letters all become the head colour (after the colour change for lowercase). That is simpler.
//
// Given the complexity, for the task we will implement a clean simulation with explicit rules:
// - The worm has a length `len` and a head colour `col` (initial `'.'`).
// - Board is given as a vector of strings.
// - Directions: 'p' (right, x+1), 'l' (left, x-1), 'd' (down, y+1), 'g' (up, y-1). Note: original uses y as vertical, x as horizontal.
// - Movement: compute target (nx,ny). If out of bounds (0 <= nx < x_max, 0 <= ny < y_max) and board[ny][nx] != 'T', then:
//    - If board[ny][nx] == 'G': len++.
//    - Move head to (nx,ny).
//    - If board[ny][nx] == 'K': len--.
//    - If len <= 0: set alive=false, stop processing further moves (but still update final position? The worm is dead, so we might not paint further cells).
//    - If alive: update board[ny][nx] to `col` (the head colour). If cell was a lowercase letter (a-z), set `col` to that letter before painting? Actually "the head's colour changes to that letter" and then the cell is painted with that new colour. So we should: if cell is lowercase, set col = that letter. Then paint board[ny][nx] = col. For 'G' and 'K', we don't change col, but we paint col.
//    - Also, if the cell was '.' nothing special.
// - If the move is skipped (out of bounds or wall), the worm stays, no painting.
// - The head position starts at (0,0). The initial cells are not painted by the worm (only moves paint).
// - The function should return a struct with fields `alive`, `head_x`, `head_y`, and `board` (final board after all moves). 
// - The moves string may have repetitions like "3d" meaning three moves down, "2p" meaning two right, etc. Also single letters without digits. Parsing: read characters; if digit, read the full number (may be multiple digits) then the letter. Repeat that many times. Stop if worm dies.
//
// Edge cases: initial len could be 1; eating 'K' with len=1 makes len=0, worm dies. The board dimensions at least 1. The moves string may be empty. If worm dies, stop processing and return current state (position at death, board with any paints up to that point). Complexity: O(R * B) where R is total number of executed moves, B is board size? Actually each move is O(1) for painting and length change. Total moves ≤ sum of numbers in moves string, at most maybe input size. So O(total moves). Storing and copying board in return struct is O(x_max*y_max). Space: O(x_max*y_max) for final board (plus input).
//
// We will write a function `simulate_worm(int x_max, int y_max, int edzio_size, std::vector<std::string> board, const std::string& moves)` returning a struct `WormResult`.
