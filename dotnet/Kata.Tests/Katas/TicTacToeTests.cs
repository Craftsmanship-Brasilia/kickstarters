using Kata.Katas.TicTacToe;

namespace Kata.Tests.Katas;

public class TicTacToeTests
{
    // Helper: play a list of (row, column) moves from a fresh game.
    // `params (int, int)[]` + tuple deconstruction in the lambda.
    private static Game PlayMoves(params (int Row, int Column)[] moves) =>
        moves.Aggregate(Game.Start(), (game, move) => game.Play(move.Row, move.Column));

    [Fact]
    public void X_plays_first()
    {
        var game = Game.Start();

        // Type pattern + property pattern in an assertion.
        Assert.IsType<GameStatus.InProgress>(game.Status);
        Assert.Equal(new GameStatus.InProgress(Player.X), game.Status); // record value equality
    }

    [Fact]
    public void Players_alternate()
    {
        var game = PlayMoves((0, 0));

        Assert.Equal(Player.X, game.Board[new Position(0, 0)]);
        Assert.Equal(Player.O, game.NextPlayer);
    }

    [Fact]
    public void Playing_returns_a_new_game_and_leaves_the_old_one_untouched()
    {
        var before = Game.Start();

        var after = before.Play(1, 1);

        Assert.Null(before.Board[new Position(1, 1)]);
        Assert.Equal(Player.X, after.Board[new Position(1, 1)]);
    }

    [Fact]
    public void Cannot_play_on_a_taken_position()
    {
        var game = PlayMoves((0, 0));

        var exception = Assert.Throws<PositionTakenException>(() => game.Play(0, 0));
        Assert.Equal(new Position(0, 0), exception.Position);
    }

    [Theory]
    [InlineData(-1, 0)]
    [InlineData(3, 0)]
    [InlineData(0, 3)]
    public void Cannot_play_outside_the_board(int row, int column)
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => Game.Start().Play(row, column));
    }

    [Fact]
    public void X_wins_with_a_full_row()
    {
        // X: (0,0) (0,1) (0,2) — O: (1,0) (1,1)
        var game = PlayMoves((0, 0), (1, 0), (0, 1), (1, 1), (0, 2));

        Assert.Equal(new GameStatus.Won(Player.X), game.Status);
    }

    [Fact]
    public void O_wins_with_a_diagonal()
    {
        var game = PlayMoves((0, 1), (0, 0), (0, 2), (1, 1), (1, 0), (2, 2));

        Assert.Equal(new GameStatus.Won(Player.O), game.Status);
    }

    [Fact]
    public void Full_board_without_winner_is_a_draw()
    {
        // X O X
        // X O O
        // O X X
        var game = PlayMoves((0, 0), (0, 1), (0, 2), (1, 1), (1, 0), (1, 2), (2, 1), (2, 0), (2, 2));

        Assert.IsType<GameStatus.Draw>(game.Status);
    }

    [Fact]
    public void Cannot_play_after_the_game_is_over()
    {
        var game = PlayMoves((0, 0), (1, 0), (0, 1), (1, 1), (0, 2));

        Assert.Throws<InvalidOperationException>(() => game.Play(2, 2));
    }
}
