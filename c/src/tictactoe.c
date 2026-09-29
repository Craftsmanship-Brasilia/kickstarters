#include "tictactoe.h"

/* Every line that wins: 3 rows, 3 columns, 2 diagonals, as {row, col} triples */
static const int LINES[8][3][2] = {
    {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}},
    {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}},
    {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}},
};

Game game_new(void)
{
    /* Designated initializer: every cell not mentioned is zero, i.e. PLAYER_NONE */
    return (Game){.next = PLAYER_X};
}

static bool on_board(int row, int col)
{
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

static Player winner(const Game *game)
{
    for (int i = 0; i < 8; i++) {
        Player first = game->cells[LINES[i][0][0]][LINES[i][0][1]];
        if (first != PLAYER_NONE && first == game->cells[LINES[i][1][0]][LINES[i][1][1]] &&
            first == game->cells[LINES[i][2][0]][LINES[i][2][1]]) {
            return first;
        }
    }
    return PLAYER_NONE;
}

static bool board_full(const Game *game)
{
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (game->cells[row][col] == PLAYER_NONE) {
                return false;
            }
        }
    }
    return true;
}

GameStatus game_status(const Game *game)
{
    switch (winner(game)) {
    case PLAYER_X:
        return GAME_X_WINS;
    case PLAYER_O:
        return GAME_O_WINS;
    case PLAYER_NONE:
        break;
    }
    return board_full(game) ? GAME_DRAW : GAME_IN_PROGRESS;
}

MoveResult game_play(Game *game, int row, int col)
{
    if (game_status(game) != GAME_IN_PROGRESS) {
        return MOVE_GAME_OVER;
    }
    if (!on_board(row, col)) {
        return MOVE_OUT_OF_BOUNDS;
    }
    if (game->cells[row][col] != PLAYER_NONE) {
        return MOVE_SQUARE_TAKEN;
    }
    game->cells[row][col] = game->next;
    game->next = game->next == PLAYER_X ? PLAYER_O : PLAYER_X;
    return MOVE_OK;
}

Player game_at(const Game *game, int row, int col)
{
    return on_board(row, col) ? game->cells[row][col] : PLAYER_NONE;
}
