namespace Kata.Katas.TicTacToe;

// Immutable board: Place() returns a new Board instead of mutating this one.
public sealed class Board
{
    // Collection expressions (C# 12): `[...]` builds arrays, lists, spans, etc.
    private static readonly int[][] Lines =
    [
        [0, 1, 2], [3, 4, 5], [6, 7, 8], // rows
        [0, 3, 6], [1, 4, 7], [2, 5, 8], // columns
        [0, 4, 8], [2, 4, 6], // diagonals
    ];

    // `Player?` on an enum (value type) is Nullable<Player>: a cell is X, O or empty (null).
    private readonly Player?[] _cells;

    private Board(Player?[] cells) => _cells = cells;

    // Target-typed `new` (C# 9): the type is inferred from the declaration.
    public static Board Empty { get; } = new(new Player?[Position.Size * Position.Size]);

    // Indexer: board[position]
    public Player? this[Position position] => _cells[position.Index];

    public bool IsFull => _cells.All(cell => cell is not null);

    public Board Place(Player player, Position position)
    {
        if (!position.IsOnBoard)
            throw new ArgumentOutOfRangeException(nameof(position), position, "Outside the board");
        if (this[position] is not null)
            throw new PositionTakenException(position);

        var cells = (Player?[])_cells.Clone();
        cells[position.Index] = player;
        return new(cells);
    }

    public Player? Winner()
    {
        foreach (var line in Lines)
        {
            // List pattern (C# 11) deconstructs the array; property pattern `{ } winner`
            // matches "not null" and captures the value.
            if (line is [var a, var b, var c] && _cells[a] is { } winner && _cells[b] == winner && _cells[c] == winner)
                return winner;
        }

        return null;
    }
}

// Primary constructor (C# 12) on a class, forwarding to the base constructor.
public sealed class PositionTakenException(Position position)
    : InvalidOperationException($"Position {position} is already taken")
{
    public Position Position { get; } = position;
}
