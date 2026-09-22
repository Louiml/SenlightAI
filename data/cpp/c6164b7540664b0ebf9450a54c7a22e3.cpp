/*
Write a C++ function `std::string playAlignmentGame()` that simulates an interactive zero-sum game on a 4×4 board. The function reads, in a loop, three integers `r`, `c`, `k` from standard input (each in the range 1..4 for `r,c` and 1..4 for `k`), representing the opponent's move: placing value `k` at cell `(r,c)`. The board starts empty. After each opponent move, your function must decide a response and print it to standard output, then verify the board state. The logic: First, after reading a move, if placing a value `x` (1..4) in the unique empty cell of any row or column would make that row/column sum equal to 10, then output that move as `"x y z WIN"` (row, column, value) and terminate the function returning that string (e.g., `"1 2 3 WIN"`). Otherwise, select the column with the fewest filled cells (excluding the column `c` of the opponent's last move; if multiple, the smallest index), place a value equal to `5 - k` (where `k` is the opponent's last move) at row `r` in that chosen column, and output `"r chosenColumn value"` (without WIN). Continue the loop indefinitely (in practice, the test harness will terminate). The function must ensure all outputs are flushed. The board coordinates are 1-indexed. The board initially has no values; the opponent will never place more than 4 moves before a win scenario occurs. Return the last output string (including WIN if any). If the game never ends (unlikely in tests), return an empty string.
*/
#include <bits/stdc++.h>

// Simulate the interactive alignment game on a 4x4 board.
// Reads moves from stdin, prints responses, returns the last printed output string.
std::string playAlignmentGame() {
    int board[5][5] = {{0}}; // 1-indexed, initialized to 0
    std::string lastOutput;
    while (true) {
        int r, c, k;
        if (std::scanf("%d%d%d", &r, &c, &k) != 3) break;
        board[r][c] = k;

        // Check all rows
        for (int i = 1; i <= 4; ++i) {
            int sum = 0, zeroCount = 0, emptyR = -1, emptyC = -1;
            for (int j = 1; j <= 4; ++j) {
                if (board[i][j] != 0) sum += board[i][j];
                else { zeroCount++; emptyR = i; emptyC = j; }
            }
            if (zeroCount == 1 && sum >= 6 && sum < 10) {
                int value = 10 - sum;
                char buf[64];
                std::snprintf(buf, sizeof(buf), "%d %d %d WIN", emptyR, emptyC, value);
                lastOutput = buf;
                std::printf("%s\n", buf);
                std::fflush(stdout);
                return lastOutput;
            }
        }

        // Check all columns
        for (int j = 1; j <= 4; ++j) {
            int sum = 0, zeroCount = 0, emptyR = -1, emptyC = -1;
            for (int i = 1; i <= 4; ++i) {
                if (board[i][j] != 0) sum += board[i][j];
                else { zeroCount++; emptyR = i; emptyC = j; }
            }
            if (zeroCount == 1 && sum >= 6 && sum < 10) {
                int value = 10 - sum;
                char buf[64];
                std::snprintf(buf, sizeof(buf), "%d %d %d WIN", emptyR, emptyC, value);
                lastOutput = buf;
                std::printf("%s\n", buf);
                std::fflush(stdout);
                return lastOutput;
            }
        }

        // No win: choose column with fewest filled cells (excluding column c)
        int minCount = 5; // max 4 filled cells
        int chosenColumn = -1;
        for (int j = 1; j <= 4; ++j) {
            if (j == c) continue;
            int count = 0;
            for (int i = 1; i <= 4; ++i) {
                if (board[i][j] != 0) count++;
            }
            if (count < minCount) {
                minCount = count;
                chosenColumn = j;
            }
        }

        int ans = 5 - k;
        board[r][chosenColumn] = ans;
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%d %d %d", r, chosenColumn, ans);
        lastOutput = buf;
        std::printf("%s\n", buf);
        std::fflush(stdout);
    }
    return lastOutput;
}
#include <bits/stdc++.h>
// The solution function is defined above; for testing, we inline it here.
// The test simulates three example game sequences by temporarily redirecting stdin/stdout.

// To keep the test self-contained, we copy the solution function into this scope.
std::string playAlignmentGame() {
    int board[5][5] = {{0}};
    std::string lastOutput;
    while (true) {
        int r, c, k;
        if (std::scanf("%d%d%d", &r, &c, &k) != 3) break;
        board[r][c] = k;
        for (int i = 1; i <= 4; ++i) {
            int sum = 0, zeroCount = 0, emptyR = -1, emptyC = -1;
            for (int j = 1; j <= 4; ++j) {
                if (board[i][j] != 0) sum += board[i][j];
                else { zeroCount++; emptyR = i; emptyC = j; }
            }
            if (zeroCount == 1 && sum >= 6 && sum < 10) {
                int value = 10 - sum;
                char buf[64];
                std::snprintf(buf, sizeof(buf), "%d %d %d WIN", emptyR, emptyC, value);
                lastOutput = buf;
                std::printf("%s\n", buf);
                std::fflush(stdout);
                return lastOutput;
            }
        }
        for (int j = 1; j <= 4; ++j) {
            int sum = 0, zeroCount = 0, emptyR = -1, emptyC = -1;
            for (int i = 1; i <= 4; ++i) {
                if (board[i][j] != 0) sum += board[i][j];
                else { zeroCount++; emptyR = i; emptyC = j; }
            }
            if (zeroCount == 1 && sum >= 6 && sum < 10) {
                int value = 10 - sum;
                char buf[64];
                std::snprintf(buf, sizeof(buf), "%d %d %d WIN", emptyR, emptyC, value);
                lastOutput = buf;
                std::printf("%s\n", buf);
                std::fflush(stdout);
                return lastOutput;
            }
        }
        int minCount = 5;
        int chosenColumn = -1;
        for (int j = 1; j <= 4; ++j) {
            if (j == c) continue;
            int count = 0;
            for (int i = 1; i <= 4; ++i) {
                if (board[i][j] != 0) count++;
            }
            if (count < minCount) {
                minCount = count;
                chosenColumn = j;
            }
        }
        int ans = 5 - k;
        board[r][chosenColumn] = ans;
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%d %d %d", r, chosenColumn, ans);
        lastOutput = buf;
        std::printf("%s\n", buf);
        std::fflush(stdout);
    }
    return lastOutput;
}

int main() {
    // Test 1: opponent places 1 at (1,1) then 2 at (1,2) -> row 1 has sum 3, two zeros, no win, respond.
    {
        std::string input = "1 1 1\n1 2 2\n";
        // We'll redirect stdin/stdout manually using tmpfile() and freopen.
        FILE* in = tmpfile();
        FILE* out = tmpfile();
        fputs(input.c_str(), in);
        rewind(in);
        // Save original file descriptors
        int stdin_fd = dup(fileno(stdin));
        int stdout_fd = dup(fileno(stdout));
        dup2(fileno(in), fileno(stdin));
        dup2(fileno(out), fileno(stdout));
        std::string result = playAlignmentGame();
        fflush(stdout);
        // Restore
        dup2(stdin_fd, fileno(stdin));
        dup2(stdout_fd, fileno(stdout));
        close(stdin_fd);
        close(stdout_fd);
        // The last output from the first iteration: column 2 has fewest? Actually after first move column 2 has 0, chosen column 2 (since c=1), ans=4, output "1 2 4"
        // But our function continues until no input, so it reads second move and then checks row 1: sum=1+2=3, no win, then choose column 3? Actually minus column c=2, choose column 3 (0 filled) ans=3, output "1 3 3" as last before loop ends.
        // We don't have a clean assert for this, so just verify the function returns something non-empty.
        assert(!result.empty());
        // The exact last output is "1 3 3" because after reading second move, no win, choose column 3 (fewest filled: col1 has 2, col3 has 0, col4 has 0, tie col3 vs col4, smallest is 3)
        assert(result == "1 3 3");
        fclose(in);
        fclose(out);
    }

    // Test 2: opponent makes row 1 sum = 7 with one empty cell -> win
    {
        std::string input = "1 1 1\n1 2 2\n1 3 4\n"; // sum row1 = 7, empty cell (1,4) filling 3 gives win
        FILE* in = tmpfile();
        FILE* out = tmpfile();
        fputs(input.c_str(), in);
        rewind(in);
        int stdin_fd = dup(fileno(stdin));
        int stdout_fd = dup(fileno(stdout));
        dup2(fileno(in), fileno(stdin));
        dup2(fileno(out), fileno(stdout));
        std::string result = playAlignmentGame();
        dup2(stdin_fd, fileno(stdin));
        dup2(stdout_fd, fileno(stdout));
        close(stdin_fd);
        close(stdout_fd);
        // After moves 1,2,3: row1 sum=7, empty at (1,4), value 3, output "1 4 3 WIN"
        assert(result == "1 4 3 WIN");
        fclose(in);
        fclose(out);
    }

    // Test 3: opponent places a column with sum 8 and one empty -> win
    {
        std::string input = "1 1 1\n2 1 3\n3 1 4\n"; // column1 sum=8, empty (4,1) fill 2 -> WIN
        FILE* in = tmpfile();
        FILE* out = tmpfile();
        fputs(input.c_str(), in);
        rewind(in);
        int stdin_fd = dup(fileno(stdin));
        int stdout_fd = dup(fileno(stdout));
        dup2(fileno(in), fileno(stdin));
        dup2(fileno(out), fileno(stdout));
        std::string result = playAlignmentGame();
        dup2(stdin_fd, fileno(stdin));
        dup2(stdout_fd, fileno(stdout));
        close(stdin_fd);
        close(stdout_fd);
        assert(result == "4 1 2 WIN");
        fclose(in);
        fclose(out);
    }

    // Test 4: no win, multiple columns empty, choose smallest index with fewest filled
    {
        std::string input = "2 2 1\n"; // opponent places at (2,2) value 1
        FILE* in = tmpfile();
        FILE* out = tmpfile();
        fputs(input.c_str(), in);
        rewind(in);
        int stdin_fd = dup(fileno(stdin));
        int stdout_fd = dup(fileno(stdout));
        dup2(fileno(in), fileno(stdin));
        dup2(fileno(out), fileno(stdout));
        std::string result = playAlignmentGame();
        dup2(stdin_fd, fileno(stdin));
        dup2(stdout_fd, fileno(stdout));
        close(stdin_fd);
        close(stdout_fd);
        // After one move, no win. Excluding column c=2, columns 1,3,4 all have 0 filled, choose smallest = 1. ans=5-1=4, output "2 1 4"
        assert(result == "2 1 4");
        fclose(in);
        fclose(out);
    }

    return 0;
}
// The solution simulates the game exactly as described. Maintain a 4×4 integer board initialized to 0. For each iteration, read three integers `r`, `c`, `k` from input. Set `board[r][c] = k`. Then check each of the 4 rows: for each row, count non-zero values (`sum`) and the sum of those values (`sum_s`), and the coordinates of the single zero cell. If exactly one zero and `sum` (sum of non-zero values) is between 6 and 9 inclusive, then filling the zero with `10 - sum` wins (since sum + (10-sum) = 10), and that move is valid only if `10-sum` is between 1 and 4 (which is guaranteed because sum between 6 and 9). Output that move with " WIN" and return that string. If no row wins, do the same for each column. If still no win, find the column with the fewest filled cells (count non-zero entries) among columns except `c`; if tie, smallest index. Then compute `ans = 5 - k`, set `board[r][wh_y] = ans`, output that move without WIN, and continue. The function returns the last printed string. Edge cases: The opponent's move may place a value that already fills a cell (in tests, assume not). The board may have values from previous turns; sums must consider only non-zero cells. The unique empty cell in a winning row/column must be found correctly. Complexity: each iteration is O(4*4) = O(16) constant time, space O(16). The function typically loops a few times (maximum 4 opponent moves) before a win.
