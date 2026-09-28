# .NET kata kickstart (.NET 10 LTS + xUnit v3)

`global.json` in this folder pins SDK 10 (your home-level pin to 9 is untouched)
and opts `dotnet test` into Microsoft.Testing.Platform, which xUnit v3 needs.

```bash
make test                      # all tests
make test TEST_NAME=SetupWorks # filter by method name
make watch                     # dotnet watch test (TDD loop)
make build | format | clean | rebuild
make setup                     # recreate from scratch in an empty folder (only the Makefile needed)
```

## Examples

| File                                              | Shows                                                                                             |
| ------------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| `Kata/Katas/FizzBuzz.cs`                          | switch expression on a tuple, guard helpers, LINQ method groups                                   |
| `Kata/Katas/TicTacToe/`                           | records, record structs, closed record hierarchy, `with`, list patterns, C# 14 extension property |
| `Kata/ModernCSharp/Records.cs`                    | records vs the C# 7.3 class you had to write                                                      |
| `Kata/ModernCSharp/SwitchExpressions.cs`          | relational, logical, property, type and list patterns                                             |
| `Kata/ModernCSharp/NullableReferenceTypes.cs`     | `string?`, `?.`, `??`, `??=`, `!`, `[NotNullWhen]`                                                |
| `Kata/ModernCSharp/FileScopedNamespaces.cs`       | file-scoped namespaces and implicit usings                                                        |
| `Kata/ModernCSharp/PrimaryConstructors.cs`        | primary constructors for dependency injection                                                     |
| `Kata/ModernCSharp/NewInDotnet10.cs`              | C# 14 `field`, extension members, null-conditional assignment, raw strings, `params` spans        |
| `Kata.Tests/XunitV3/TheoryDataRowTests.cs`        | `object[]` vs `TheoryData` vs `TheoryDataRow` (display name, skip, traits, explicit)              |
| `Kata.Tests/XunitV3/MatrixTheoryDataTests.cs`     | cartesian-product test data                                                                       |
| `Kata.Tests/XunitV3/TestContextTests.cs`          | `TestContext.Current`: cancellation token, test info, output, attachments                         |
| `Kata.Tests/XunitV3/LifecycleAndSkippingTests.cs` | `IAsyncLifetime` (ValueTask), `Skip`, `SkipUnless`, `Assert.SkipWhen`, `Explicit`                 |

4 tests are skipped/explicit on purpose to demonstrate those features.
