namespace Kata.Katas.TicTacToe;

// Positional record: the compiler generates the constructor, init-only properties,
// Equals/GetHashCode, ToString and Deconstruct. `with` creates modified copies.
public sealed record Game(Board Board, Player NextPlayer)
{
    public static Game Start() => new(Board.Empty, Player.X);

    // Switch expression on a tuple with property patterns.
    public GameStatus Status => (Board.Winner(), Board.IsFull) switch
    {
        ({ } winner, _) => new GameStatus.Won(winner),
        (null, true) => new GameStatus.Draw(),
        _ => new GameStatus.InProgress(NextPlayer),
    };

    // A `throw` expression is allowed inside a switch arm.
    public Game Play(int row, int column) => Status switch
    {
        GameStatus.InProgress => this with
        {
            Board = Board.Place(NextPlayer, new Position(row, column)),
            NextPlayer = NextPlayer.Opponent, // C# 14 extension property, see Player.cs
        },
        _ => throw new InvalidOperationException($"Game is over: {Status}"),
    };
}
