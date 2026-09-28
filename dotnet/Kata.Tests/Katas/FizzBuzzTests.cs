using Kata.Katas;

namespace Kata.Tests.Katas;

public class FizzBuzzTests
{
    // [Fact] = a single test with no parameters.
    [Fact]
    public void Returns_the_number_as_text_when_not_a_multiple_of_three_or_five()
    {
        Assert.Equal("1", FizzBuzz.Of(1));
    }

    // [Theory] + [InlineData] = the same test run once per data row (constants only).
    [Theory]
    [InlineData(3, "Fizz")]
    [InlineData(6, "Fizz")]
    [InlineData(5, "Buzz")]
    [InlineData(10, "Buzz")]
    [InlineData(15, "FizzBuzz")]
    [InlineData(30, "FizzBuzz")]
    public void Replaces_multiples(int number, string expected)
    {
        Assert.Equal(expected, FizzBuzz.Of(number));
    }

    [Theory]
    [InlineData(0)]
    [InlineData(-3)]
    public void Rejects_non_positive_numbers(int number)
    {
        Assert.Throws<ArgumentOutOfRangeException>(() => FizzBuzz.Of(number));
    }

    [Fact]
    public void Produces_a_sequence()
    {
        // Collection expression as the expected value.
        string[] expected = ["1", "2", "Fizz", "4", "Buzz"];

        Assert.Equal(expected, FizzBuzz.Sequence(5));
    }
}
