# C kata kickstart (C17 + gcc + Unity + make + gdb)

Why each tool and flag was picked is in [DECISIONS.md](DECISIONS.md).

```bash
make                           # same as make test
make test                      # build + run every test file (UBSan always on)
make test TEST_NAME=roll       # only tests whose name contains "roll"
make debug                     # gdb, stopped at the failing assertion
make debug TEST_FILE=test_foo  # ...in another test file
make memcheck                  # tests under valgrind (leaks, invalid reads/writes, uninitialised values)
make asan                      # tests with AddressSanitizer instead (no valgrind needed)
make coverage                  # line coverage per src/*.c (gcov)
make watch                     # rerun tests on save (needs entr)
make clean
make setup KATA=bowling        # scaffold a new kata in an empty folder (only the Makefile needed)
```

Requirements: `gcc`, `make`, `gdb`. Optional: `valgrind`, `entr`. On Debian/Ubuntu run
`sudo apt install build-essential gdb valgrind entr`. On macOS, `gcc` is clang: everything except
`memcheck` and `debug` works (use `make asan` and `lldb`). On Windows, use WSL.

## Starting a new kata

```bash
mkdir ~/katas/bowling && cp Makefile ~/katas/bowling/ && cd ~/katas/bowling
make setup KATA=bowling
make test
```

`setup` downloads Unity (pinned by `UNITY_VERSION`) into `vendor/unity/` and writes:

```
.gitignore
src/bowling.h           # the public API of your kata
src/bowling.c           # the implementation
tests/runner.h          # RUN() macro (see below)
tests/test_bowling.c    # one passing test: delete it once your first real test is red
```

Every file is only created if missing, so running `setup` again never overwrites your work.

## How it works

### A test file is a program

C has no reflection, so no framework can "discover" your tests without a code generator. Here, each
`tests/test_*.c` has its own `main()` that lists the tests to run:

```c
#include "unity.h"
#include "runner.h"
#include "bowling.h"

void setUp(void) {}      /* Unity calls this before every test... */
void tearDown(void) {}   /* ...and this after it, even if the test failed */

static void test_gutter_game_scores_zero(void)
{
    Game game = game_new();
    for (int i = 0; i < 20; i++) {
        game_roll(&game, 0);
    }
    TEST_ASSERT_EQUAL_INT(0, game_score(&game));
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {   /* reads -f name / -x name */
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_gutter_game_scores_zero);          /* forget this line and the test never runs! */
    return UNITY_END();                         /* number of failures = exit code */
}
```

The TDD loop is: write the test function, **add it to `main`**, and see it fail. That last step is
what catches a forgotten `RUN(...)`.

`RUN(test)` (in `tests/runner.h`) is Unity's `RUN_TEST` plus a name check, which makes `make test TEST_NAME=...`
work. When a Unity assertion fails, it prints `file:line:test:FAIL: Expected X Was Y`, `longjmp`s out of the test,
runs `tearDown`, and carries on with the next test.

Useful assertions (full list: `vendor/unity/unity.h`):

| Assertion                                            | For                                   |
| ---------------------------------------------------- | ------------------------------------- |
| `TEST_ASSERT_TRUE(x)` / `TEST_ASSERT_FALSE(x)`       | booleans                              |
| `TEST_ASSERT_EQUAL_INT(expected, actual)`            | ints (also `_UINT`, `_INT64`, `_HEX`) |
| `TEST_ASSERT_EQUAL_STRING(expected, actual)`         | `const char *` contents, not pointers |
| `TEST_ASSERT_EQUAL_INT_ARRAY(expected, actual, n)`   | arrays                                |
| `TEST_ASSERT_EQUAL_MEMORY(expected, actual, size)`   | structs / raw bytes                   |
| `TEST_ASSERT_NULL(p)` / `TEST_ASSERT_NOT_NULL(p)`    | pointers                              |
| `TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual)` | floating point                        |
| `TEST_ASSERT_EQUAL_INT_MESSAGE(e, a, "context")`     | any assertion + `_MESSAGE`            |
| `TEST_FAIL_MESSAGE("todo")` / `TEST_IGNORE()`        | placeholders                          |

### What `make` does

```
src/bowling.c ─┐
               ├─> build/src/bowling.o ──┐
vendor/unity/unity.c ─> build/unity/unity.o ├─> build/test_bowling ─> run it
tests/test_bowling.c ─> build/tests/test_bowling.o ─┘
```

- Every `src/*.c` is linked into every test binary. Every `tests/test_*.c` becomes its own binary.
  To add a file, you just create it; the Makefile doesn't need editing.
- Don't put a `main()` in `src/`, because it would clash with the test's `main()`.
- gcc writes `.d` files that list the headers each file includes, so after editing a `.h` only the affected
  files are recompiled.
- `make asan` and `make coverage` build into `build/asan/` and `build/coverage/`, so the three builds
  never mix.

### Output of a failing run

```
tests/test_kata.c:11:test_setup_works:FAIL: Expected 'kata' Was 'oops'
-----------------------
1 Tests 1 Failures 0 Ignored
FAIL
```

When UBSan finds undefined behaviour, it stops the test binary like this:

```
src/kata.c:4:35: runtime error: signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'
```

## Debugging with gdb

`make debug` starts gdb with a breakpoint on `longjmp`, which is how Unity leaves a failing test. It runs
until the first failure and prints the backtrace:

```
#0  __libc_siglongjmp (...)
#1  UnityAssertEqualString (expected="kata", actual="oops", ...) at vendor/unity/unity.c:1766
#2  test_setup_works () at tests/test_kata.c:11          <- your test
#3  UnityDefaultTestRun (...)
#4  main (...)
```

Cheat sheet:

| Command                     | Does                                                         |
| --------------------------- | ------------------------------------------------------------ |
| `frame 2` / `up` / `down`   | move to your test's frame / one frame out / one frame in     |
| `list`                      | show source around the current line                          |
| `info locals` / `info args` | all local variables / the function's arguments               |
| `print x` / `p *game`       | print a value / a whole struct (`p arr[0]@5` prints 5 items) |
| `break bowling.c:42`        | stop at a line (`break game_roll` stops at a function)       |
| `run` / `run -f gutter`     | restart the program (optionally with a test filter)          |
| `next` / `step`             | next line, stepping over / into calls                        |
| `finish`                    | run until the current function returns, print return value   |
| `continue`                  | run to the next breakpoint                                   |
| `watch game->score`         | stop whenever this value changes                             |
| `display x`                 | print `x` after every step                                   |
| `tui enable`                | split screen with source (Ctrl-x a to toggle)                |
| `quit`                      | leave                                                        |

If you just want to step through a passing test, use `gdb build/test_kata` directly, then
`break test_setup_works` and `run`.

## Memory checking

| Tool                   | When                          | Catches                                                                  |
| ---------------------- | ----------------------------- | ------------------------------------------------------------------------ |
| UBSan                  | every build                   | signed overflow, bad shifts, NULL/misaligned access, out-of-bounds index |
| valgrind (`memcheck`)  | on demand, Linux              | leaks, heap overflows, use-after-free, **reading uninitialised memory**  |
| ASan (`asan`)          | on demand, where no valgrind  | leaks (Linux), heap/stack/global overflows, use-after-free, ~2× slower   |

A failing test `longjmp`s out, skipping any `free` after the assertion, so a red test can show a leak. Fix
the test first, then look at leaks.

## Troubleshooting

**`undefined reference to '__isoc23_strtoul@GLIBC_2.38'` (or similar) when linking.** A `ld` other than
the system one is first in your `PATH`, for example Homebrew's (`which ld`). It links the sanitizer runtimes against
the wrong libraries. Make gcc use the system linker on your machine only:

```bash
echo 'LDFLAGS += -B/usr/bin/' > local.mk   # untracked, read by the Makefile
```

**`0 Tests 0 Failures ... OK` with `TEST_NAME`.** Nothing matched the filter. Check the spelling.

**A test never runs.** It's missing from `main()`. Add `RUN(test_...)`.
