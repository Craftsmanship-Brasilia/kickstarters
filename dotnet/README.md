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
