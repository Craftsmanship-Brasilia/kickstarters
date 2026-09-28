using Kata.ModernCSharp;

namespace Kata.Tests.ModernCSharp;

public class RecordsTests
{
    [Fact]
    public void Records_have_value_equality()
    {
        Assert.Equal(new Money(10, "BRL"), new Money(10, "BRL"));
        Assert.True(new Point(1, 2) == new Point(1, 2));
    }

    [Fact]
    public void With_creates_a_modified_copy()
    {
        var original = new Money(10, "BRL");

        var copy = original with { Amount = 20 };

        Assert.Equal(10, original.Amount);
        Assert.Equal(20, copy.Amount);
    }

    [Fact]
    public void Records_deconstruct_and_print_nicely()
    {
        var (amount, currency) = new Money(5, "EUR");

        Assert.Equal((5m, "EUR"), (amount, currency));
        Assert.Equal("Money { Amount = 5, Currency = EUR }", new Money(5, "EUR").ToString());
    }

    [Fact]
    public void Required_members_are_set_in_the_initializer()
    {
        var customer = new Customer { Name = "Ada" }; // omitting Name is a compile error

        Assert.Null(customer.Email);
    }
}

public class SwitchExpressionsTests
{
    [Theory]
    [InlineData(-5, "freezing")]
    [InlineData(10, "cold")]
    [InlineData(20, "mild")]
    [InlineData(30, "hot")]
    public void Relational_patterns(int celsius, string expected) =>
        Assert.Equal(expected, SwitchExpressions.Temperature(celsius));

    [Fact]
    public void Property_patterns()
    {
        Assert.Equal(0m, SwitchExpressions.Shipping(new Order(150, "BR")));
        Assert.Equal(10m, SwitchExpressions.Shipping(new Order(50, "BR")));
        Assert.Equal(15m, SwitchExpressions.Shipping(new Order(50, "PT")));
        Assert.Equal(25m, SwitchExpressions.Shipping(new Order(50, "US")));
    }

    [Fact]
    public void Type_and_list_patterns()
    {
        Assert.Equal("null", SwitchExpressions.Describe(null));
        Assert.Equal("negative number", SwitchExpressions.Describe(-1));
        Assert.Equal("empty text", SwitchExpressions.Describe(""));
        Assert.Equal("list from 1 to 3", SwitchExpressions.Describe(new[] { 1, 2, 3 }));
        Assert.Equal("list with only 7", SwitchExpressions.Describe(new[] { 7 }));
    }
}

public class NullableReferenceTypesTests
{
    [Fact]
    public void Null_operators()
    {
        var directory = new UserDirectory();
        directory.Register("ada", "ada@example.com");
        directory.Register("bob", null);

        Assert.Equal("no-email", directory.EmailOrPlaceholder("bob"));
        Assert.Equal(0, directory.EmailLength("bob"));
        Assert.True(directory.TryGetDomain("ada", out var domain));
        Assert.Equal("example.com", domain); // no warning: [NotNullWhen(true)] told the compiler
    }
}

public class PrimaryConstructorsTests
{
    [Theory]
    [InlineData(9, "Good morning, Ada")]
    [InlineData(15, "Good afternoon, Ada")]
    [InlineData(21, "Good evening, Ada")]
    public void Dependencies_come_from_the_primary_constructor(int hour, string expected)
    {
        var clock = new FixedClock(new DateTimeOffset(2026, 1, 1, hour, 0, 0, TimeSpan.Zero));

        Assert.Equal(expected, new Greeter(clock, "Ada").Greet());
    }

    [Fact]
    public void Parameters_can_initialise_properties()
    {
        var account = new Account("Ada", 10);

        account.Deposit(5);

        Assert.Equal(15, account.Balance);
    }
}

public class NewInDotnet10Tests
{
    [Fact]
    public void Field_keyword_validates_in_the_setter()
    {
        var thermometer = new Thermometer { Celsius = 21 };

        Assert.Equal(21, thermometer.Celsius);
        Assert.Throws<ArgumentOutOfRangeException>(() => thermometer.Celsius = -300);
    }

    [Fact]
    public void Extension_properties()
    {
        Assert.True("   ".IsBlank);
        Assert.Equal("AL", "Ada Lovelace".Initials);
    }

    [Fact]
    public void Null_conditional_assignment()
    {
        var profile = new Profile();

        ProfileUpdater.Rename(profile, "ada");
        ProfileUpdater.Rename(null, "nobody"); // no NullReferenceException

        Assert.Equal("ada", profile.Nickname);
    }

    [Fact]
    public void Raw_strings_params_spans_and_spreads()
    {
        Assert.Equal("""{ "name": "Ada" }""", Newer.Json("Ada"));
        Assert.Equal(6, Newer.Sum(1, 2, 3));
        Assert.Equal([1, 2, 3], Newer.Concat([1], [2, 3]));
    }

    [Fact]
    public void File_scoped_namespace()
    {
        Assert.Equal("Kata.ModernCSharp", FileScopedNamespaceExample.Namespace);
    }
}
