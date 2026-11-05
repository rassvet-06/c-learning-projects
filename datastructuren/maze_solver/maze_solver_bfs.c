#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define NOT_FOUND -1
#define ERROR -2

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int bfs_solve(struct maze *m) {
    if (!m) {
        return ERROR;
    }

    int start_r, start_c, dest_r, dest_c;
    maze_start(m, &start_r, &start_c);
    maze_destination(m, &dest_r, &dest_c);

    struct queue *q = queue_init(4000);
    if (!q) {
        return ERROR;
    }

    // Track parent for backtracking shortest path
    size_t total_cells = (size_t)maze_size(m) * (size_t)maze_size(m);
    int *parent = calloc(total_cells, sizeof(int));
    if (!parent) {
        queue_cleanup(q);
        return ERROR;
    }

    for (size_t i = 0; i < total_cells; i++) {
        parent[i] = -1;
    }

    int start_idx = maze_index(m, start_r, start_c);
    if (queue_push(q, start_idx) != 0) {
        free(parent);
        queue_cleanup(q);
        return ERROR;
    }

    while (!queue_empty(q)) {
        int idx = queue_pop(q);
        int r = maze_row(m, idx);
        int c = maze_col(m, idx);

        if (maze_at_destination(m, r, c)) {
            // reconstruct path
            int steps = -1;

            while (idx != -1) {
                int rr = maze_row(m, idx);
                int cc = maze_col(m, idx);
                if (!maze_at_start(m, rr, cc) && !maze_at_destination(m, rr, cc))
                    maze_set(m, rr, cc, 'x');
                idx = parent[idx];
                steps++;
            }

            free(parent);
            queue_cleanup(q);

            return steps;
        }

        if (maze_get(m, r, c) == FLOOR)
            maze_set(m, r, c, '.'); // mark visited

        for (int i = 0; i < N_MOVES; i++) {
            int nr = r + m_offsets[i][0];
            int nc = c + m_offsets[i][1];

            if (maze_valid_move(m, nr, nc)) {
                int nidx = maze_index(m, nr, nc);
                if (parent[nidx] == -1 && maze_get(m, nr, nc) == FLOOR) {
                    parent[nidx] = idx;
                    maze_set(m, nr, nc, ',');
                    if (queue_push(q, nidx) != 0) {
                        free(parent);
                        queue_cleanup(q);
                        return ERROR;
                    }
                }
            }
        }
    }

    free(parent);
    queue_cleanup(q);
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
    int path_length = bfs_solve(m);
    if (path_length == ERROR) {
        printf("bfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("bfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
