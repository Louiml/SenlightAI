Given two singly linked lists, each representing a table where every node stores fixed-size integer arrays of differing lengths (`col1` and `col2`), write a standalone C++ function `listNaturalJoin` that performs an inner equi-join on a specified column index from each input list (1-based indexing, as the user would specify). The function takes pointers to the two head nodes (each containing `row`, `col`, `next`), two column numbers, and returns a dynamically allocated head node of a new list where each row is the concatenation of a row from table1 and a row from table2 whenever the selected values are equal. The function must handle empty tables, produce zero rows when no matches occur, and correctly set the new row count. Do not modify the input lists. Assume input lists are well-formed with all data integers and column indices valid for their respective lists.

// The solution mimics the classic nested-loop join algorithm: for each row in table1, iterate through all rows in table2, and when the key value at the specified column (after converting 1-based index to 0-based) matches, allocate a new node of size `col1+col2` integers, copy table1’s row followed by table2’s row into it, and append this node to the result list using a tail pointer. Initialize result head with `col = col1+col2`, `row = 0`, `next = nullptr`. Handle the edge case where no matches occur: the tail pointer remains `nullptr`, so after the loops, if the tail is non-null, set its `next = nullptr`; if null, ensure result head’s next stays null. Because the input lists are not modified, we iterate using temporary pointers. Time complexity is O(row1 * row2) in the worst case (unless columns are sorted), and space complexity is O(k) where k is the number of matched rows, because we allocate one node per match. The algorithm uses only constant extra memory beyond the output.

#include <cstddef> // for nullptr

// Node structure for table-linked list
struct DList {
    int data[100]; // assume max 100 columns for simplicity; task fixed size
    DList* next;
};

struct HList {
    int row;
    int col;
    DList* next;
};

// Perform natural inner equi-join between two tables on given columns (1-based).
HList* listNaturalJoin(const HList* h1, const HList* h2, int col1, int col2) {
    // Create result head
    HList* result = new HList;
    result->col = h1->col + h2->col;
    result->row = 0;
    result->next = nullptr;

    DList* r = nullptr; // tail of result list

    // Iterate over table1 rows
    for (const DList* p = h1->next; p != nullptr; p = p->next) {
        // Iterate over table2 rows
        for (const DList* q = h2->next; q != nullptr; q = q->next) {
            // 1-based column indices converted to 0-based
            if (p->data[col1 - 1] == q->data[col2 - 1]) {
                // Create new node
                DList* s = new DList;
                // Copy table1 columns
                for (int k = 0; k < h1->col; ++k) {
                    s->data[k] = p->data[k];
                }
                // Copy table2 columns
                for (int k = 0; k < h2->col; ++k) {
                    s->data[h1->col + k] = q->data[k];
                }
                s->next = nullptr;

                // Append to result
                if (result->next == nullptr) {
                    result->next = s;
                } else {
                    r->next = s;
                }
                r = s;
                result->row++;
            }
        }
    }

    // If any row was added, tail already has next = nullptr; else result->next is null
    return result;
}

#include <cassert>
#include <iostream>

// Include the solution function and structures here (or assume they are already defined)

// Helper to build a table from a given set of rows and column count
HList* createTable(int col, int row, int data[][100]) {
    HList* h = new HList;
    h->row = row;
    h->col = col;
    h->next = nullptr;
    DList* tail = nullptr;
    for (int i = 0; i < row; ++i) {
        DList* p = new DList;
        for (int j = 0; j < col; ++j) {
            p->data[j] = data[i][j];
        }
        p->next = nullptr;
        if (tail == nullptr) h->next = p;
        else tail->next = p;
        tail = p;
    }
    if (tail != nullptr) tail->next = nullptr;
    return h;
}

// Helper to check if result matches expected rows
bool checkResult(const HList* result, int expectedRow, int expectedCol, int expected[][100]) {
    if (result->row != expectedRow) return false;
    if (result->col != expectedCol) return false;
    const DList* p = result->next;
    for (int i = 0; i < expectedRow; ++i) {
        for (int j = 0; j < expectedCol; ++j) {
            if (p->data[j] != expected[i][j]) return false;
        }
        p = p->next;
    }
    return p == nullptr;
}

int main() {
    // Test 1: Basic join with matching keys
    int t1[2][100] = {{1, 10}, {2, 20}};
    int t2[3][100] = {{1, 100}, {3, 300}, {1, 1000}};
    HList* h1 = createTable(2, 2, t1);
    HList* h2 = createTable(2, 3, t2);
    HList* result = listNaturalJoin(h1, h2, 1, 1);
    int expected[2][100] = {{1, 10, 1, 100}, {1, 10, 1, 1000}};
    assert(checkResult(result, 2, 4, expected));

    // Test 2: No matches
    int t3[1][100] = {{5, 50}};
    HList* h3 = createTable(2, 1, t3);
    result = listNaturalJoin(h1, h3, 1, 1);
    assert(result->row == 0);
    assert(result->col == 4);
    assert(result->next == nullptr);

    // Test 3: Empty first table
    HList* hEmpty = new HList;
    hEmpty->row = 0;
    hEmpty->col = 2;
    hEmpty->next = nullptr;
    result = listNaturalJoin(hEmpty, h2, 1, 1);
    assert(result->row == 0);
    assert(result->col == 4);
    assert(result->next == nullptr);

    // Test 4: Multiple matches and different join columns
    int t4[2][100] = {{10, 1}, {20, 2}};
    int t5[2][100] = {{1, 99}, {2, 100}};
    HList* h4 = createTable(2, 2, t4);
    HList* h5 = createTable(2, 2, t5);
    result = listNaturalJoin(h4, h5, 2, 1);
    int expected2[2][100] = {{10, 1, 1, 99}, {20, 2, 2, 100}};
    assert(checkResult(result, 2, 4, expected2));

    // Test 5: Duplicate keys in both tables
    int t6[2][100] = {{7, 1}, {7, 2}};
    int t7[2][100] = {{7, 3}, {7, 4}};
    HList* h6 = createTable(2, 2, t6);
    HList* h7 = createTable(2, 2, t7);
    result = listNaturalJoin(h6, h7, 1, 1);
    int expected3[4][100] = {{7,1,7,3}, {7,1,7,4}, {7,2,7,3}, {7,2,7,4}};
    assert(checkResult(result, 4, 4, expected3));

    // Clean up (memory not freed for brevity)
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
