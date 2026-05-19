#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define ROWS 50
#define COLS 100

// char *red = "\x1b[41m \0";
// char *red = "\033[41m🦠\033[0m";
char *red = "🟥";
char *dead = "⬜️";
// char *dead = "\033[41m \033[0m";

struct pos_t {
  int i;
  int j;
};

char *grid[ROWS][COLS];

void new_game() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      grid[i][j] = dead; // dead
    }
  }

  for (int r = 0; r < ROWS; r++) {
    int alive_max = rand() % 80;
    for (int i = 0; i < alive_max; i++) {
      grid[r][rand() % COLS] = red; // alive
    }
  }
}

void print() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%s", grid[i][j]);
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
      if (!strcmp(grid[neighbors[x].i][neighbors[x].j], red)) {
        alive_count++;
      }
    }
  }

  return alive_count;
}

void exit_game() {
  printf("\x1b[0m");
  exit(0);
}

void pause_screen() {
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 500000000L; // 500 million nanoseconds
  nanosleep(&ts, NULL);
}

int main() {

  signal(SIGINT, exit_game);

  srand(time(NULL));
  // init state
  new_game();

  print();
  pause_screen();
  // reset
  printf("\033[2J\033[H");

  while (1) {
    // update state
    char *new_grid[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {

        int alive_count = get_alive(i, j);
        if (alive_count < 2 || alive_count > 3) {
          new_grid[i][j] = dead;
        } else if (!strcmp(grid[i][j], red) &&
                   (alive_count == 2 || alive_count == 3)) {
          new_grid[i][j] = grid[i][j];
        } else if (!strcmp(grid[i][j], dead) && alive_count == 3) {
          new_grid[i][j] = red;
        } else {
          new_grid[i][j] = dead;
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
    pause_screen();
  }

  return 0;
}
