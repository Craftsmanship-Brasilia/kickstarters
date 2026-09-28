namespace Kata.ModernCSharp;

// ── Primary constructors for classes (C# 12) ───────────────────────────────
//
// The parameters are in scope for the whole class body. Great for dependency
// injection: no field declarations, no constructor body.
public interface IClock
{
    DateTimeOffset Now { get; }
}

public sealed class FixedClock(DateTimeOffset now) : IClock
{
    public DateTimeOffset Now => now;
}

public sealed class Greeter(IClock clock, string name = "friend")
{
    public string Greet() => clock.Now.Hour switch
    {
        < 12 => $"Good morning, {name}",
        < 18 => $"Good afternoon, {name}",
        _ => $"Good evening, {name}",
    };
}

// Note: primary-constructor parameters are captured, not `readonly` fields.
// If you want a real property, initialise one explicitly:
public sealed class Account(string owner, decimal balance)
{
    public string Owner { get; } = owner;
    public decimal Balance { get; private set; } = balance;

    public void Deposit(decimal amount) => Balance += amount;
}

// BEFORE (C# 7.3):
//
//   public sealed class Greeter
//   {
//       private readonly IClock _clock;
//       private readonly string _name;
//
//       public Greeter(IClock clock, string name = "friend")
//       {
//           _clock = clock ?? throw new ArgumentNullException(nameof(clock));
//           _name = name;
//       }
//
//       public string Greet()
//       {
//           var hour = _clock.Now.Hour;
//           if (hour < 12) return "Good morning, " + _name;
//           if (hour < 18) return "Good afternoon, " + _name;
//           return "Good evening, " + _name;
//       }
//   }
