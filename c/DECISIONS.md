# Decisions

Each decision lists what was chosen, the alternatives, and why. If one stops fitting, change it here and
in the `Makefile`.

## 1. Test framework: Unity, vendored

**Chosen:** [Unity](https://github.com/ThrowTheSwitch/Unity) `v2.7.0`. Three files are copied into `vendor/unity/`
and compiled with the tests.

| Option           | Install                  | Test registration         | Why not                                                  |
| ---------------- | ------------------------ | ------------------------- | -------------------------------------------------------- |
| **Unity**        | none (3 files in repo)   | by hand in `main()`       |                                                          |
| Check            | `libcheck` + pkg-config  | by hand (suites + tcases) | more boilerplate; everyone who clones must install it   |
| Criterion        | package, not everywhere  | automatic                 | install friction; linker magic hides how tests run      |
| CMocka           | package                  | by hand (array)           | similar to Check; strength is mocking, rarely needed     |
| our own macros   | none                     | by hand                   | great to learn from, poor failure messages              |

- **No install:** a clone plus `make` works anywhere gcc works, including when pairing on someone else's machine.
- **Plain C, nothing hidden:** `RUN(test)` is a function call inside a `main()` you can read and step
  through in gdb. When you're learning how C tests work, that transparency is worth more than auto-registration.
- **It's the framework used in James Grenning's _Test-Driven Development for Embedded C_**, the standard book
  on TDD in C.
- **Pinned version:** `UNITY_VERSION` in the Makefile. Upgrading means deleting `vendor/unity/` and running `make vendor`.

**Cost:** you must add every new test to `main()`. Unity offers a Ruby script that generates runners;
we didn't use it because it adds a Ruby dependency and a build step. Writing each test red first
catches a forgotten `RUN(...)` anyway.

## 2. `RUN()` instead of `RUN_TEST()`

Unity's `RUN_TEST` ignores the command-line filter; only the runners its Ruby script generates check it.
`tests/runner.h` does the same two steps as those generated runners, setting the test name and calling
`UnityTestMatches()`, so `make test TEST_NAME=...` works in hand-written runners too.

## 3. C standard: C17

**Chosen:** `-std=c17`, with GNU extensions off.

- C17 is C11 plus bug fixes, and every current compiler supports it fully.
- C23 has nice additions (`bool` without a header, `nullptr`, `constexpr`), but gcc 12 supports it only
  partly (`-std=c2x`). That could cause surprises mid-kata.
- C99 would give up C11 features like `_Static_assert`, anonymous structs and `_Generic` for portability
  a kata doesn't need.
- We don't use `gnu17` so code that compiles here also compiles with clang/MSVC.

## 4. Warnings: strict, as errors

**Chosen:** `-Wall -Wextra -Wpedantic -Werror -Wshadow`

- In C, many bugs are only warnings: a missing `return`, `=` where `==` was meant, an unused parameter
  that shows a wrong signature, or a variable shadowing another. `-Werror` makes those fail the build.
  In TDD a failed build counts as red, so you get the feedback straight away.
- `-Wpedantic` rejects compiler extensions, which keeps the code standard C17.
- `-Wshadow` catches `int count` inside a loop hiding the outer `count`, a classic kata bug.
- **Not (yet) `-Wconversion`.** It flags every implicit `int`→`char` or `size_t`→`int` conversion. That's
  valuable, but noisy while you're getting used to C. Turn it on once the kata works and see what it finds.
- Unity itself compiles cleanly with these flags, so no exceptions are needed.

## 5. Always `-g -O0`

Test builds are for debugging, so every build has debug info and no optimisation. `make debug` never needs
a rebuild, and gdb shows every variable and line in order, instead of `<optimized out>`.

## 6. Undefined behaviour: UBSan in every build

**Chosen:** `-fsanitize=undefined -fno-sanitize-recover=all` on by default.

- C lets signed overflow, bad shifts, NULL dereference and out-of-bounds indexing "work" silently. A test can
  pass on your machine and fail elsewhere. UBSan turns these into a runtime error with `file:line` that
  fails the test.
- `-fno-sanitize-recover=all` makes it stop and fail, instead of printing a warning and carrying on.
- It's cheap (a few % slower), works with gcc and clang on Linux and macOS, and, unlike ASan, it runs fine
  under valgrind. So it can stay on all the time.

## 7. Memory errors: valgrind by default, ASan as the alternative

**Chosen:** `make memcheck` (valgrind) and `make asan` (AddressSanitizer + UBSan, separate build).

| Tool     | Needs                         | Catches uninitialised reads | Speed          | Platforms                            |
| -------- | ----------------------------- | --------------------------- | -------------- | ------------------------------------ |
| valgrind | the tool, same binary         | **yes**                     | ~20-50× slower | Linux (and WSL); not macOS on ARM    |
| ASan     | a separate instrumented build | no                          | ~2× slower     | gcc/clang: Linux, macOS (no leaks on ARM) |
| MSan     | **clang only**                | yes                         | ~3× slower     | Linux + clang                        |

- valgrind is the default because it needs no rebuild and is the only option here that catches reading
  uninitialised memory with gcc.
- valgrind isn't available everywhere, so `make asan` covers the other platforms.
- **Why ASan isn't combined with valgrind:** an ASan binary reserves huge shadow-memory regions that
  valgrind can't handle. Hence the separate `build/asan/` directory.
- **Why no MSan:** gcc doesn't implement `-fsanitize=memory` (`unrecognized argument`), and this
  kickstarter targets gcc. With clang on Linux you could still run
  `make test CC=clang BUILD=build/msan SANITIZE="-fsanitize=memory -fno-sanitize-recover=all"`, but it's
  untested here.

## 8. Build: GNU make, one binary per test file

- **make, not CMake:** a single ~200-line Makefile you can read, compared with a CMake project generating a
  Makefile you can't. Katas have no dependencies to find and no platforms to configure, which is what CMake
  is for.
- **One executable per `tests/test_*.c`:** each file keeps its own `setUp`/`tearDown`. A crash in one
  file can't hide the results of another, and `make test` still runs them all and fails if any fail.
- **Wildcards and pattern rules:** a new `src/x.c` or `tests/test_x.c` is picked up automatically.
- **`.d` dependency files (`-MMD -MP`):** editing a header recompiles only what includes it.
- **`.SECONDARY:`:** otherwise make deletes the `.o` files it considers intermediate, and every run
  becomes a full rebuild.
- **`local.mk` (untracked):** per-machine tweaks, like a linker override, stay out of the shared Makefile.

## 9. Debugger: gdb, breaking on `longjmp`

A failing Unity assertion ends the test with `longjmp`. `make debug` sets one breakpoint there, so it stops at
*whichever* assertion fails first, with your test two frames up. There's no need to know the line in advance.
On macOS use `lldb` (gdb isn't supported on Apple Silicon); the same idea works with `b longjmp`.

## 10. Coverage: gcov

gcov ships with gcc, so nothing extra is needed. `make coverage` does a separate `--coverage` build and prints
the % of lines run per `src/*.c`, leaving annotated sources in `build/coverage/*.gcov`. In TDD, coverage
below 100% usually means code was written without a failing test first. lcov/gcovr would give HTML reports
but need installing.

## 11. `make setup`

The scaffold is written by the Makefile itself (the same idea as `dotnet/Makefile`), so a kata needs nothing
but this one file to start. The committed `c/` folder *is* the output of `make setup`, which means the
kickstarter and the scaffolder can't drift apart. Every generated file is a make file target, so it's only
created when missing.

## 12. Strings in tests (a C-specific TDD choice)

Katas often return strings (FizzBuzz, Roman numerals...). C gives you three ways to do that:

| Style                                          | Example                                  | Trade-off                                       |
| ---------------------------------------------- | ---------------------------------------- | ----------------------------------------------- |
| return a string literal / static buffer        | `const char *fizzbuzz(int n)`            | simplest; static buffer isn't reentrant        |
| **caller passes a buffer**                     | `void fizzbuzz(int n, char *out, size_t size)` | no allocation, no leaks; the C library's own style |
| return `malloc`ed memory                       | `char *fizzbuzz(int n)` + `free()`       | flexible; every test must `free`, and valgrind catches the ones that don't |

The examples use the caller-supplied buffer. Try the `malloc` version once with `make memcheck` to see
leak detection in action.
