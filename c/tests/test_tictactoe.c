#include "unity.h"
#include "runner.h"
#include "tictactoe.h"

static Game game;

void setUp(void)
{
    game = game_new();
}

void tearDown(void) {}

/* Test helper: plays moves in order ({row, col} pairs), failing the test if any is rejected */
#define PLAY(...) play_all((const int[][2]){__VA_ARGS__}, sizeof((const int[][2]){__VA_ARGS__}) / sizeof(int[2]))

static void play_all(const int moves[][2], size_t count)
{
    for (size_t i = 0; i < count; i++) {
        TEST_ASSERT_EQUAL_INT_MESSAGE(MOVE_OK, game_play(&game, moves[i][0], moves[i][1]), "setup move rejected");
    }
}

static void test_new_game_is_empty_and_in_progress(void)
{
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            TEST_ASSERT_EQUAL_INT(PLAYER_NONE, game_at(&game, row, col));
        }
    }
    TEST_ASSERT_EQUAL_INT(GAME_IN_PROGRESS, game_status(&game));
}

static void test_x_plays_first(void)
{
    PLAY({1, 1});
    TEST_ASSERT_EQUAL_INT(PLAYER_X, game_at(&game, 1, 1));
}

static void test_players_alternate(void)
{
    PLAY({0, 0}, {2, 2});
    TEST_ASSERT_EQUAL_INT(PLAYER_O, game_at(&game, 2, 2));
}

static void test_taken_square_is_rejected_and_turn_is_kept(void)
{
    PLAY({0, 0});
    TEST_ASSERT_EQUAL_INT(MOVE_SQUARE_TAKEN, game_play(&game, 0, 0));
    PLAY({1, 1});
    TEST_ASSERT_EQUAL_INT(PLAYER_O, game_at(&game, 1, 1));
}

static void test_moves_off_the_board_are_rejected(void)
{
    TEST_ASSERT_EQUAL_INT(MOVE_OUT_OF_BOUNDS, game_play(&game, -1, 0));
    TEST_ASSERT_EQUAL_INT(MOVE_OUT_OF_BOUNDS, game_play(&game, 0, BOARD_SIZE));
}

static void test_three_in_a_row_wins(void)
{
    PLAY({0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2});
    TEST_ASSERT_EQUAL_INT(GAME_X_WINS, game_status(&game));
}

static void test_three_in_a_column_wins(void)
{
    PLAY({0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 2}, {2, 1});
    TEST_ASSERT_EQUAL_INT(GAME_O_WINS, game_status(&game));
}

static void test_diagonal_wins(void)
{
    PLAY({0, 2}, {0, 0}, {1, 1}, {0, 1}, {2, 0});
    TEST_ASSERT_EQUAL_INT(GAME_X_WINS, game_status(&game));
}

static void test_full_board_without_line_is_a_draw(void)
{
    /* X O X
       X O O
       O X X */
    PLAY({0, 0}, {0, 1}, {0, 2}, {1, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 0}, {2, 2});
    TEST_ASSERT_EQUAL_INT(GAME_DRAW, game_status(&game));
}

static void test_no_moves_after_game_over(void)
{
    PLAY({0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2});
    TEST_ASSERT_EQUAL_INT(MOVE_GAME_OVER, game_play(&game, 2, 2));
    TEST_ASSERT_EQUAL_INT(PLAYER_NONE, game_at(&game, 2, 2));
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_new_game_is_empty_and_in_progress);
    RUN(test_x_plays_first);
    RUN(test_players_alternate);
    RUN(test_taken_square_is_rejected_and_turn_is_kept);
    RUN(test_moves_off_the_board_are_rejected);
    RUN(test_three_in_a_row_wins);
    RUN(test_three_in_a_column_wins);
    RUN(test_diagonal_wins);
    RUN(test_full_board_without_line_is_a_draw);
    RUN(test_no_moves_after_game_over);
    return UNITY_END();
}
