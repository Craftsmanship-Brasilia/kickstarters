namespace Kata.ModernCSharp;

// ── C# 14 (ships with .NET 10) ─────────────────────────────────────────────

// 1) The `field` keyword: add logic to an auto-property without declaring a backing field.
public sealed class Thermometer
{
    public double Celsius
    {
        get;
        set => field = value >= -273.15 ? value : throw new ArgumentOutOfRangeException(nameof(value));
    }
}

// 2) Extension members: extension *properties* (and static extensions), not just methods.
public static class StringExtensions
{
    extension(string text)
    {
        public bool IsBlank => string.IsNullOrWhiteSpace(text);
        public string Initials => string.Concat(text.Split(' ', StringSplitOptions.RemoveEmptyEntries).Select(w => w[0]));
    }
}

// 3) Null-conditional assignment: `target?.Property = value` only assigns if target is not null.
public sealed class Profile
{
    public string Nickname { get; set; } = "";
}

public static class ProfileUpdater
{
    public static void Rename(Profile? profile, string nickname) => profile?.Nickname = nickname;
}

// ── A few more from C# 11–13 you will see in modern code ───────────────────
public static class Newer
{
    // Raw string literals (C# 11): no escaping, indentation is trimmed to the closing quotes.
    public static string Json(string name) => $$"""
        { "name": "{{name}}" }
        """;

    // `params` collections (C# 13): params can be a span/list, not only an array.
    public static int Sum(params ReadOnlySpan<int> numbers)
    {
        var total = 0;
        foreach (var n in numbers)
            total += n;
        return total;
    }

    // Collection expressions (C# 12) with spread `..`.
    public static int[] Concat(int[] first, int[] second) => [.. first, .. second];
}
