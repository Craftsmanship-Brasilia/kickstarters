# Variables: override any of them from the command line, e.g. `make setup SOLUTION_NAME=Bowling`
SOLUTION_NAME = Kata
LIB_PROJECT = Kata
TEST_PROJECT = Kata.Tests
FRAMEWORK = net10.0
SDK_VERSION = 10.0.401
TEST_NAME =

# .NET 10 SDK creates .slnx solutions by default
SOLUTION = $(SOLUTION_NAME).slnx
TEST_CSPROJ = $(TEST_PROJECT)/$(TEST_PROJECT).csproj

# Compile every project in the solution
build:
	dotnet build $(SOLUTION)

# make test                      -> all tests
# make test TEST_NAME=SetupWorks -> tests whose method name contains SetupWorks
# (Microsoft.Testing.Platform uses --filter-method/--filter-class instead of VSTest's --filter)
test:
	@if [ -z "$(TEST_NAME)" ]; then \
		dotnet test --solution $(SOLUTION); \
	else \
		dotnet test --solution $(SOLUTION) --filter-method "*$(TEST_NAME)*"; \
	fi

# TDD loop: rebuilds and reruns the tests on every file save
watch:
	dotnet watch test --project $(TEST_CSPROJ)

# Apply the standard C# code style (whitespace, usings, analyzers)
format:
	dotnet format $(SOLUTION)

# Remove build outputs (bin/obj contents)
clean:
	dotnet clean $(SOLUTION)

# Download NuGet packages
restore:
	dotnet restore $(SOLUTION)

rebuild: clean restore build

# Local global.json pins SDK 10 for this folder only and opts `dotnet test`
# into Microsoft.Testing.Platform (required by xUnit v3 on SDK 10).
# Only created when missing (it's a file target, not .PHONY).
global.json:
	printf '{\n  "sdk": {\n    "version": "$(SDK_VERSION)",\n    "rollForward": "latestFeature"\n  },\n  "test": {\n    "runner": "Microsoft.Testing.Platform"\n  }\n}\n' > global.json

# Scaffold a fresh solution in an empty folder (only this Makefile needed)
setup: global.json
	# solution file (Kata.slnx)
	dotnet new sln --name $(SOLUTION_NAME)
	# production code: class library
	dotnet new classlib --framework $(FRAMEWORK) --output $(LIB_PROJECT)
	rm -f $(LIB_PROJECT)/Class1.cs
	dotnet sln $(SOLUTION) add $(LIB_PROJECT)/$(LIB_PROJECT).csproj
	# test project from the xUnit template
	dotnet new xunit --framework $(FRAMEWORK) --output $(TEST_PROJECT)
	rm -f $(TEST_PROJECT)/UnitTest1.cs
	# the template still ships xUnit v2 + VSTest; swap to xUnit v3 on Microsoft.Testing.Platform
	dotnet remove $(TEST_CSPROJ) package xunit
	dotnet remove $(TEST_CSPROJ) package xunit.runner.visualstudio
	dotnet remove $(TEST_CSPROJ) package Microsoft.NET.Test.Sdk
	dotnet remove $(TEST_CSPROJ) package coverlet.collector
	dotnet add $(TEST_CSPROJ) package xunit.v3
	# xUnit v3 test projects must be executables
	sed -i 's|    <IsPackable>false</IsPackable>|&\n    <OutputType>Exe</OutputType>|' $(TEST_CSPROJ)
	dotnet sln $(SOLUTION) add $(TEST_CSPROJ)
	# tests can see the library's public types
	dotnet add $(TEST_CSPROJ) reference $(LIB_PROJECT)/$(LIB_PROJECT).csproj

.PHONY: build test watch format clean restore rebuild setup
