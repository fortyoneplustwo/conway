#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define ROWS 30
#define COLS 100

struct pos_t {
  int i;
  int j;
};

char grid[ROWS][COLS];

void new_game() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      grid[i][j] = ' '; // dead
    }
  }

  for (int r = 0; r < ROWS; r++) {
    int alive_max = rand() % 20;
    for (int i = 0; i < alive_max; i++) {
      grid[r][rand() % COLS] = 'x'; // alive
    }
  }
}

void print() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%c", grid[i][j]);
    }
    printf("\n");
  }
}

int get_alive(int i, int j) {
  int alive_count = 0;
  // n, ne, e, se, s, sw, w, nw
  struct pos_t neighbors[8];
  neighbors[0].i = i - 1;
  neighbors[0].j = j;

  neighbors[1].i = i - 1;
  neighbors[1].j = j + 1;

  neighbors[2].i = i;
  neighbors[2].j = j + 1;

  neighbors[3].i = i + 1;
  neighbors[3].j = j + 1;

  neighbors[4].i = i + 1;
  neighbors[4].j = j;

  neighbors[5].i = i + 1;
  neighbors[5].j = j - 1;

  neighbors[6].i = i;
  neighbors[6].j = j - 1;

  neighbors[7].i = i - 1;
  neighbors[7].j = j - 1;

  for (int x = 0; x < 8; x++) {
    if (neighbors[x].i >= 0 && neighbors[x].i < ROWS && neighbors[x].j >= 0 &&
        neighbors[x].j < COLS) {
      if (grid[neighbors[x].i][neighbors[x].j] == 'x') {
        alive_count++;
      }
    }
  }

  return alive_count;
}

int main() {
  srand(time(NULL));
  // init state
  new_game();

  print();

  // reset

  while (1) {
    // update state
    char new_grid[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {

        int alive_count = get_alive(i, j);
        if (alive_count < 2 || alive_count > 3) {
          new_grid[i][j] = ' ';
        } else if (grid[i][j] == 'x' &&
                   (alive_count == 2 || alive_count == 3)) {
          new_grid[i][j] = grid[i][j];
        } else if (grid[i][j] == ' ' && alive_count == 3) {
          new_grid[i][j] = 'x';
        } else {
          new_grid[i][j] = ' ';
        }
      }
    }

    // copy to old grid
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        grid[i][j] = new_grid[i][j];
      }
    }

    // print
    print();
    // printf("\x1b0m")
    printf("\033[2J\033[H");
    sleep(1);
  }

  return 0;
}
