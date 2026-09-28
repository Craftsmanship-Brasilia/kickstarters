namespace Kata.Katas.TicTacToe;

// `readonly record struct` (C# 10): a small immutable value type with value equality,
// ToString() and deconstruction generated for free. Position(0, 1) == Position(0, 1).
public readonly record struct Position(int Row, int Column)
{
    public const int Size = 3;

    // Relational + logical patterns (C# 9): `is >= 0 and < Size`.
    public bool IsOnBoard => Row is >= 0 and < Size && Column is >= 0 and < Size;

    public int Index => Row * Size + Column;
}
