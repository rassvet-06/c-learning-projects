#include <stdlib.h>

/* Tel buren van (row,col) in een rows x cols raster */
static int countNeighbors(int **board, int rows, int cols, int row, int col) {
    int cnt = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            int r = row + dr, c = col + dc;
            if (r >= 0 && r < rows && c >= 0 && c < cols && board[r][c] == 1) {
                ++cnt;
            }
        }
    }
    return cnt;
}
void gameOfLife(int **board, int rows, int cols) {
    int *next = (int*)malloc((size_t)rows * (size_t)cols * sizeof(int));
    if (!next) return; /* out of memory */

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int cnt = countNeighbors(board, rows, cols, i, j);
            int val;
            if (cnt < 2 || cnt > 3) {
                val = 0;
            } else if (board[i][j] == 1 && (cnt == 2 || cnt == 3)) {
                val = 1;
            } else if (board[i][j] == 0 && cnt == 3) {
                val = 1;
            } else {
                val = board[i][j]; /* valt in praktijk samen met de regels hierboven */
            }
            next[i*cols + j] = val;
        }
    }

    /* kopieer terug */
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            board[i][j] = next[i*cols + j];

    free(next);
}