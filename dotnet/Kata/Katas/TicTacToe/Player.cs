namespace Kata.Katas.TicTacToe;

public enum Player
{
    X,
    O,
}

// C# 14 (.NET 10) extension members: an `extension` block can declare extension
// *properties*, not only methods. Usage: `player.Opponent`.
// Before C# 14 you could only write: public static Player Opponent(this Player player) => ...
public static class PlayerExtensions
{
    extension(Player player)
    {
        public Player Opponent => player == Player.X ? Player.O : Player.X;
    }
}
