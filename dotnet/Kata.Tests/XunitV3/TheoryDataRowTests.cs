using Kata.Katas;

namespace Kata.Tests.XunitV3;

// Three ways to feed a [Theory] from code, from oldest to newest.
public class TheoryDataRowTests
{
    // 1) v2 style: IEnumerable<object[]>. Works, but no type safety at all.
    public static IEnumerable<object[]> ObjectRows =>
    [
        [3, "Fizz"],
        [5, "Buzz"],
    ];

    [Theory]
    [MemberData(nameof(ObjectRows))]
    public void Object_arrays(int number, string expected) => Assert.Equal(expected, FizzBuzz.Of(number));

    // 2) TheoryData<T1, T2> (v2 and v3): strongly typed, but rows are just values.
    public static TheoryData<int, string> TypedRows => new() { { 6, "Fizz" }, { 10, "Buzz" } };

    [Theory]
    [MemberData(nameof(TypedRows))]
    public void Theory_data(int number, string expected) => Assert.Equal(expected, FizzBuzz.Of(number));

    // 3) v3: TheoryDataRow<T1, T2>. Strongly typed AND each row carries its own metadata:
    //    display name, skip reason, traits, timeout, explicit...
    public static IEnumerable<TheoryDataRow<int, string>> Rows =>
    [
        new(3, "Fizz") { TestDisplayName = "3 is a multiple of three" },
        new TheoryDataRow<int, string>(5, "Buzz").WithTrait("Category", "buzz"),
        (15, "FizzBuzz"), // implicit conversion from a tuple
        new(7, "7") { Skip = "Example of skipping a single row" },
        new(9, "Fizz") { Explicit = true }, // only runs when asked for explicitly
    ];

    [Theory]
    [MemberData(nameof(Rows))]
    public void Theory_data_rows(int number, string expected) => Assert.Equal(expected, FizzBuzz.Of(number));
}
