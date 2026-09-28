// File-scoped namespace (C# 10): everything in this file belongs to Kata.Katas, no extra braces/indentation.
namespace Kata.Katas;

public static class FizzBuzz
{
    // Expression-bodied member (=>) + switch expression over a tuple (C# 8).
    // Each arm is `pattern => result`; `_` is the discard (matches anything).
    public static string Of(int number)
    {
        // .NET 8+ guard helpers replace the classic `if (number <= 0) throw new ...`.
        ArgumentOutOfRangeException.ThrowIfNegativeOrZero(number);

        return (number % 3, number % 5) switch
        {
            (0, 0) => "FizzBuzz",
            (0, _) => "Fizz",
            (_, 0) => "Buzz",
            _ => number.ToString(),
        };
    }

    // LINQ + method group (`Of` instead of `n => Of(n)`).
    public static IEnumerable<string> Sequence(int count) => Enumerable.Range(1, count).Select(Of);
}
