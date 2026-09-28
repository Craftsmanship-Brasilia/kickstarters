namespace Kata.Tests.XunitV3;

// Setup/teardown: the constructor runs before EACH test, Dispose/DisposeAsync after each.
// v3 change: IAsyncLifetime now returns ValueTask and extends IAsyncDisposable.
public class LifecycleTests : IAsyncLifetime
{
    private readonly List<string> _log = [];

    public ValueTask InitializeAsync()
    {
        _log.Add("initialized");
        return ValueTask.CompletedTask;
    }

    public ValueTask DisposeAsync() => ValueTask.CompletedTask;

    [Fact]
    public void Runs_after_InitializeAsync() => Assert.Equal(["initialized"], _log);
}

public class SkippingTests
{
    public static bool IsLinux => OperatingSystem.IsLinux();

    // Static skip (v2 and v3).
    [Fact(Skip = "Example of a statically skipped test")]
    public void Always_skipped() => Assert.Fail("never runs");

    // v3: conditional skip decided by a static property.
    [Fact(SkipUnless = nameof(IsLinux), Skip = "Only runs on Linux")]
    public void Only_on_linux() => Assert.True(OperatingSystem.IsLinux());

    // v3: skip from inside the test at runtime.
    [Fact]
    public void Dynamic_skip()
    {
        Assert.SkipWhen(Environment.ProcessorCount < 1, "Needs at least one CPU");
        Assert.True(true);
    }

    // v3: explicit tests only run when requested (e.g. dotnet test -- --explicit only).
    [Fact(Explicit = true)]
    public void Slow_or_manual_test() => Assert.True(true);
}
