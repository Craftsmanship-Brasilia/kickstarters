using Kata.Katas;
using Kata.Katas.TicTacToe;

namespace Kata.Tests.XunitV3;

// MatrixTheoryData builds the cartesian product of its inputs:
// 4 multipliers x 1 factor = 4 test cases, without writing each combination by hand.
public class MatrixTheoryDataTests
{
    public static MatrixTheoryData<int, int> MultiplesOfFifteen => new([1, 2, 3, 4], [15]);

    [Theory]
    [MemberData(nameof(MultiplesOfFifteen))]
    public void Every_multiple_of_fifteen_is_FizzBuzz(int multiplier, int factor) =>
        Assert.Equal("FizzBuzz", FizzBuzz.Of(multiplier * factor));

    // 3 rows x 3 columns = 9 test cases, one per board position.
    public static MatrixTheoryData<int, int> AllPositions => new([0, 1, 2], [0, 1, 2]);

    [Theory]
    [MemberData(nameof(AllPositions))]
    public void The_first_move_can_go_anywhere(int row, int column)
    {
        var game = Game.Start().Play(row, column);

        Assert.Equal(Player.X, game.Board[new Position(row, column)]);
    }
}
