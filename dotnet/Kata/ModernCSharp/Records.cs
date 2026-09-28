namespace Kata.ModernCSharp;

// ── Records (C# 9) ──────────────────────────────────────────────────────────
//
// One line gives you: constructor, init-only properties, value equality
// (Equals/GetHashCode/==), a readable ToString(), Deconstruct() and `with`.
public sealed record Money(decimal Amount, string Currency)
{
    // Records can have normal members too.
    public Money Add(Money other) =>
        other.Currency == Currency
            ? this with { Amount = Amount + other.Amount } // non-destructive mutation
            : throw new InvalidOperationException($"Cannot add {other.Currency} to {Currency}");
}

// `record struct` (C# 10): same idea, but a value type (no allocation).
public readonly record struct Point(int X, int Y);

// Nominal record with `required` (C# 11) and `init` (C# 9) properties:
// must be set in the object initializer, and cannot change afterwards.
public sealed record Customer
{
    public required string Name { get; init; }
    public string? Email { get; init; }
}

// BEFORE (C# 7.3) — the equivalent of `record Money(decimal Amount, string Currency)`:
//
//   public sealed class Money : IEquatable<Money>
//   {
//       public Money(decimal amount, string currency)
//       {
//           Amount = amount;
//           Currency = currency;
//       }
//
//       public decimal Amount { get; }
//       public string Currency { get; }
//
//       public Money WithAmount(decimal amount) => new Money(amount, Currency);
//
//       public bool Equals(Money other) =>
//           !ReferenceEquals(other, null) && Amount == other.Amount && Currency == other.Currency;
//       public override bool Equals(object obj) => Equals(obj as Money);
//       public override int GetHashCode()
//       {
//           unchecked { return (Amount.GetHashCode() * 397) ^ (Currency?.GetHashCode() ?? 0); }
//       }
//       public static bool operator ==(Money a, Money b) => Equals(a, b);
//       public static bool operator !=(Money a, Money b) => !Equals(a, b);
//       public override string ToString() => $"Money {{ Amount = {Amount}, Currency = {Currency} }}";
//       public void Deconstruct(out decimal amount, out string currency)
//       {
//           amount = Amount;
//           currency = Currency;
//       }
//   }
