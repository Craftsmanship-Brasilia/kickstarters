using System.Diagnostics.CodeAnalysis;

namespace Kata.ModernCSharp;

// ── Nullable reference types (C# 8, on by default via <Nullable>enable</Nullable>) ──
//
// `string`  = never null (the compiler warns if you might assign null)
// `string?` = may be null (the compiler warns if you dereference it without checking)
public sealed class UserDirectory
{
    private readonly Dictionary<string, string> _emails = [];

    public void Register(string name, string? email)
    {
        if (email is not null) // `is not null` pattern (C# 9)
            _emails[name] = email; // compiler knows email is non-null here (flow analysis)
    }

    public string? FindEmail(string name) => _emails.GetValueOrDefault(name);

    public string EmailOrPlaceholder(string name) => FindEmail(name) ?? "no-email"; // null-coalescing

    public int EmailLength(string name) => FindEmail(name)?.Length ?? 0; // null-conditional

    // Attributes teach the compiler about your API: when this returns true, `domain` is not null.
    public bool TryGetDomain(string name, [NotNullWhen(true)] out string? domain)
    {
        domain = FindEmail(name)?.Split('@').LastOrDefault();
        return domain is not null;
    }

    private string? _cachedSummary;

    public string Summary() => _cachedSummary ??= $"{_emails.Count} users"; // null-coalescing assignment

    // `!` is the null-forgiving operator: "trust me, it's not null". Use sparingly.
    public string FirstEmail() => _emails.Values.FirstOrDefault()!;

    // BEFORE (C# 7.3): every reference type is implicitly nullable and the compiler
    // never warns you. `string FindEmail(string name)` could return null and the
    // caller finds out with a NullReferenceException at runtime.
}
