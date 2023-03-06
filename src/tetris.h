#ifndef TETRIS_H
#define TETRIS_H

#define ROWS 22
#define COLS 10

#define PIECES 7
#define BLOCKS 4

#define MAX_LEVEL 9

extern int level_speed[MAX_LEVEL + 1];

void init_board(void);
void display_board(void);

int collide(int piece, int rotation, int row, int col);
void place(int piece, int rotation, int row, int col);
void clear_lines(void);

int game_over(void);
int next_piece(void);
int drop_piece(int piece, int rotation, int row, int col);
void move_piece(int dx);
void rotate_piece(int dr);

int get_level(void);
void set_level(int level);

int get_score(void);

#endif /* TETRIS_H */
