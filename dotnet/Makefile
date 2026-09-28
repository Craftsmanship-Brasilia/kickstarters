SOLUTION_NAME = Kata
LIB_PROJECT = Kata
TEST_PROJECT = Kata.Tests
FRAMEWORK = net10.0
SDK_VERSION = 10.0.401
TEST_NAME =

# .NET 10 SDK creates .slnx solutions by default
SOLUTION = $(SOLUTION_NAME).slnx
TEST_CSPROJ = $(TEST_PROJECT)/$(TEST_PROJECT).csproj

build:
	dotnet build $(SOLUTION)

# make test                      -> all tests
# make test TEST_NAME=SetupWorks -> tests whose method name contains SetupWorks
test:
	@if [ -z "$(TEST_NAME)" ]; then \
		dotnet test --solution $(SOLUTION); \
	else \
		dotnet test --solution $(SOLUTION) --filter-method "*$(TEST_NAME)*"; \
	fi

watch:
	dotnet watch test --project $(TEST_CSPROJ)

format:
	dotnet format $(SOLUTION)

clean:
	dotnet clean $(SOLUTION)

restore:
	dotnet restore $(SOLUTION)

rebuild: clean restore build

# Local global.json pins SDK 10 for this folder only and opts `dotnet test`
# into Microsoft.Testing.Platform (required by xUnit v3 on SDK 10).
global.json:
	printf '{\n  "sdk": {\n    "version": "$(SDK_VERSION)",\n    "rollForward": "latestFeature"\n  },\n  "test": {\n    "runner": "Microsoft.Testing.Platform"\n  }\n}\n' > global.json

setup: global.json
	dotnet new sln --name $(SOLUTION_NAME)
	dotnet new classlib --framework $(FRAMEWORK) --output $(LIB_PROJECT)
	rm -f $(LIB_PROJECT)/Class1.cs
	dotnet sln $(SOLUTION) add $(LIB_PROJECT)/$(LIB_PROJECT).csproj
	dotnet new xunit --framework $(FRAMEWORK) --output $(TEST_PROJECT)
	rm -f $(TEST_PROJECT)/UnitTest1.cs
	# the template still ships xUnit v2 + VSTest; swap to xUnit v3 on Microsoft.Testing.Platform
	dotnet remove $(TEST_CSPROJ) package xunit
	dotnet remove $(TEST_CSPROJ) package xunit.runner.visualstudio
	dotnet remove $(TEST_CSPROJ) package Microsoft.NET.Test.Sdk
	dotnet remove $(TEST_CSPROJ) package coverlet.collector
	dotnet add $(TEST_CSPROJ) package xunit.v3
	sed -i 's|    <IsPackable>false</IsPackable>|&\n    <OutputType>Exe</OutputType>|' $(TEST_CSPROJ)
	dotnet sln $(SOLUTION) add $(TEST_CSPROJ)
	dotnet add $(TEST_CSPROJ) reference $(LIB_PROJECT)/$(LIB_PROJECT).csproj

.PHONY: build test watch format clean restore rebuild setup
