namespace Kata.Katas.TicTacToe;

// A closed hierarchy of records ("discriminated union" style).
// The private constructor means nobody outside can add new cases.
public abstract record GameStatus
{
    private GameStatus() { }

    public sealed record InProgress(Player NextPlayer) : GameStatus;

    public sealed record Won(Player Winner) : GameStatus;

    public sealed record Draw : GameStatus;
}
