namespace Kata.Tests.XunitV3;

// TestContext.Current is available from anywhere during a test (v3 only).
public class TestContextTests
{
    [Fact]
    public async Task Cancellation_token_stops_async_work_when_the_run_is_cancelled()
    {
        // Cancelled when the test times out or the run is aborted (Ctrl+C).
        // The xUnit analyzer (xUnit1051) suggests passing it to every async call.
        var cancellationToken = TestContext.Current.CancellationToken;

        await Task.Delay(10, cancellationToken);

        Assert.False(cancellationToken.IsCancellationRequested);
    }

    // Timeout only works for async tests; it cancels TestContext.Current.CancellationToken.
    [Fact(Timeout = 5_000)]
    public async Task Timeout_uses_the_same_token()
    {
        await Task.Delay(10, TestContext.Current.CancellationToken);
    }

    [Fact]
    public void Information_about_the_running_test()
    {
        var context = TestContext.Current;

        Assert.Equal(nameof(Information_about_the_running_test), context.TestMethod?.MethodName);
        Assert.Contains(nameof(TestContextTests), context.TestClass?.TestClassName);
        Assert.Contains(nameof(Information_about_the_running_test), context.Test?.TestDisplayName);

        // Output without injecting ITestOutputHelper in the constructor.
        context.TestOutputHelper?.WriteLine("Hello from TestContext.Current");

        // Attachments and warnings show up in the test results.
        context.AddAttachment("board", "X|O|X");
    }
}

// The v2 way still works: ITestOutputHelper injected in the constructor.
// In v3 it lives in namespace Xunit (it used to be Xunit.Abstractions).
public class OutputHelperTests(ITestOutputHelper output)
{
    [Fact]
    public void Writes_output() => output.WriteLine("Hello from the constructor-injected helper");
}
