#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <stdbool.h>

#define BOARD_SIZE 3

/* Enums prefixed with their type: C has one global namespace for enum constants */
typedef enum { PLAYER_NONE, PLAYER_X, PLAYER_O } Player;

typedef enum { GAME_IN_PROGRESS, GAME_X_WINS, GAME_O_WINS, GAME_DRAW } GameStatus;

/* Errors are return values: C has no exceptions */
typedef enum { MOVE_OK, MOVE_OUT_OF_BOUNDS, MOVE_SQUARE_TAKEN, MOVE_GAME_OVER } MoveResult;

typedef struct {
    Player cells[BOARD_SIZE][BOARD_SIZE];
    Player next;
} Game;

/* Returned by value: a Game is small, so no malloc/free and nothing to leak */
Game game_new(void);

/* Places the next player's mark; on anything but MOVE_OK the game is left unchanged */
MoveResult game_play(Game *game, int row, int col);

Player game_at(const Game *game, int row, int col);
GameStatus game_status(const Game *game);

#endif
