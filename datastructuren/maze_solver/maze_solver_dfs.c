#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m) {
    if (!m) {
        return ERROR;
    }

    int start_r, start_c, dest_r, dest_c;
    maze_start(m, &start_r, &start_c);
    maze_destination(m, &dest_r, &dest_c);

    struct stack *s = stack_init(4000);
    if (!s) {
        return ERROR;
    }

    size_t total_cells = (size_t)maze_size(m) * (size_t)maze_size(m);
    int *parent = calloc(total_cells, sizeof(int));
    if (!parent) {
        stack_cleanup(s);
        return ERROR;
    }

    for (size_t i = 0; i < total_cells; i++)
        parent[i] = -1;

    int start_idx = maze_index(m, start_r, start_c);
    if (stack_push(s, start_idx) != 0) {
        free(parent);
        stack_cleanup(s);
        return ERROR;
    }

    while (!stack_empty(s)) {
        int idx = stack_pop(s);
        int r = maze_row(m, idx);
        int c = maze_col(m, idx);

        if (maze_at_destination(m, r, c)) {
            // reconstruct actual path
            int steps = 0;
            while (idx != -1) {
                int rr = maze_row(m, idx);
                int cc = maze_col(m, idx);
                if (!maze_at_start(m, rr, cc) && !maze_at_destination(m, rr, cc))
                    maze_set(m, rr, cc, 'x');
                idx = parent[idx];
                steps++;
            }

            free(parent);
            stack_cleanup(s);
            return steps - 1;  // subtract 1 to match expected length
        }

        if (maze_get(m, r, c) == FLOOR)
            maze_set(m, r, c, '.');

        for (int i = N_MOVES - 1; i >= 0; i--) { // reversed for DFS order
            int nr = r + m_offsets[i][0];
            int nc = c + m_offsets[i][1];

            if (maze_valid_move(m, nr, nc)) {
                int nidx = maze_index(m, nr, nc);
                if (parent[nidx] == -1 && maze_get(m, nr, nc) == FLOOR) {
                    parent[nidx] = idx;
                    if (stack_push(s, nidx) != 0) {
                        free(parent);
                        stack_cleanup(s);
                        return ERROR;
                    }
                }
            }
        }
    }

    free(parent);
    stack_cleanup(s);

    return NOT_FOUND;
}

int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = dfs_solve(m);
    if (path_length == ERROR) {
        printf("dfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("dfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
