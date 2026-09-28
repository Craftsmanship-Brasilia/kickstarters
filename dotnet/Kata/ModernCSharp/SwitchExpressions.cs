namespace Kata.ModernCSharp;

public sealed record Order(decimal Total, string Country);

// ── Switch expressions (C# 8) and patterns (C# 9–11) ────────────────────────
public static class SwitchExpressions
{
    // Relational (`< 0`) and logical (`and`, `or`, `not`) patterns — C# 9.
    public static string Temperature(int celsius) => celsius switch
    {
        < 0 => "freezing",
        >= 0 and < 15 => "cold",
        >= 15 and < 25 => "mild",
        _ => "hot",
    };

    // Property patterns: match on the shape of an object.
    public static decimal Shipping(Order order) => order switch
    {
        { Total: >= 100 } => 0m,
        { Country: "BR" } => 10m,
        { Country: "PT" or "ES" } => 15m,
        _ => 25m,
    };

    // Type patterns, `when` guards and list patterns (C# 11).
    public static string Describe(object? value) => value switch
    {
        null => "null",
        int n when n < 0 => "negative number",
        int => "number",
        string { Length: 0 } => "empty text",
        string text => $"text with {text.Length} chars",
        int[] and [] => "empty list",
        int[] and [var first, .., var last] => $"list from {first} to {last}",
        int[] and [var only] => $"list with only {only}",
        _ => "something else",
    };

    // BEFORE (C# 7.3) — a switch *statement* plus if/else chains:
    //
    //   public static string Temperature(int celsius)
    //   {
    //       if (celsius < 0) return "freezing";
    //       if (celsius < 15) return "cold";
    //       if (celsius < 25) return "mild";
    //       return "hot";
    //   }
    //
    //   public static string Describe(object value)
    //   {
    //       switch (value)
    //       {
    //           case null: return "null";
    //           case int n when n < 0: return "negative number";
    //           case int _: return "number";
    //           case string s when s.Length == 0: return "empty text";
    //           case string s: return "text with " + s.Length + " chars";
    //           default: return "something else";   // no list patterns at all
    //       }
    //   }
}
