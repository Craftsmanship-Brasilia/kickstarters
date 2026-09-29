# Variables: override any of them from the command line, e.g. `make setup KATA=bowling`
KATA = kata
TEST_NAME =
TEST_FILE = $(basename $(notdir $(firstword $(wildcard tests/test_*.c))))
UNITY_VERSION = v2.7.0
BUILD = build

CC = gcc
# UBSan in every build: overflow, bad shifts, misaligned/NULL access... abort the test with file:line.
# It runs fine under valgrind; `make asan` swaps in AddressSanitizer on top (see DECISIONS.md)
SANITIZE = -fsanitize=undefined -fno-sanitize-recover=all
# C17 + every warning that is cheap to fix, as errors (see DECISIONS.md)
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror -Wshadow -g -O0 $(SANITIZE)
# -MMD -MP: gcc writes a .d file per object listing the headers it included,
# so editing a header rebuilds exactly the objects that use it
CPPFLAGS = -Isrc -Ivendor/unity -DUNITY_USE_COMMAND_LINE_ARGS -MMD -MP
LDFLAGS = $(SANITIZE)

# Machine-specific overrides live in an untracked local.mk, e.g. `LDFLAGS += -B/usr/bin/`
# (see Troubleshooting in README.md)
-include local.mk

# Every src/*.c is production code; every tests/test_*.c becomes its own test executable
SRCS = $(wildcard src/*.c)
TESTS = $(wildcard tests/test_*.c)
OBJS = $(SRCS:src/%.c=$(BUILD)/src/%.o) $(BUILD)/unity/unity.o
TEST_BINS = $(TESTS:tests/%.c=$(BUILD)/%)
FILTER = $(if $(TEST_NAME),-f $(TEST_NAME))

# make test                     -> build and run every test file
# make test TEST_NAME=roll      -> only tests whose name contains "roll"
# Keeps going after a failing file so you see everything, then exits non-zero
test: $(TEST_BINS)
	@status=0; for t in $(TEST_BINS); do ./$$t $(FILTER) || status=1; done; exit $$status

# Compile and link without running
build: $(TEST_BINS)

# make debug                    -> gdb on the first test file, stopping where an assertion fails
# make debug TEST_FILE=test_foo TEST_NAME=bar
# Unity aborts a failing test with longjmp, so a breakpoint there catches every failure;
# the backtrace shows your test's frame; `frame N` jumps there (the cheat sheet in README.md has the rest)
debug: $(BUILD)/$(TEST_FILE)
	gdb -q -ex 'break longjmp' -ex 'run $(FILTER)' -ex backtrace $(BUILD)/$(TEST_FILE)

# Run every test file under valgrind: invalid reads/writes, uninitialised values and leaks fail the run
memcheck: $(TEST_BINS)
	@command -v valgrind >/dev/null || { echo "make memcheck needs valgrind (Linux: sudo apt install valgrind). No valgrind? use: make asan"; exit 1; }
	@status=0; for t in $(TEST_BINS); do \
		valgrind -q --leak-check=full --errors-for-leak-kinds=definite,indirect \
			--error-exitcode=1 ./$$t $(FILTER) || status=1; \
	done; exit $$status

# Alternative to memcheck where valgrind is unavailable (macOS, Windows/WSL without it):
# separate build in build/asan with AddressSanitizer + UBSan. Out-of-bounds, use-after-free
# and (on Linux) leaks abort the test with a stack trace. Can't be combined with valgrind.
asan:
	@$(MAKE) --no-print-directory test BUILD=$(BUILD)/asan \
		SANITIZE="-fsanitize=address,undefined -fno-sanitize-recover=all"

# Separate instrumented build in build/coverage, then a line-coverage summary per source file.
# Annotated sources (##### marks lines never run) end up in build/coverage/*.gcov
coverage:
	@rm -f $(BUILD)/coverage/src/*.gcda
	@$(MAKE) --no-print-directory test BUILD=$(BUILD)/coverage \
		CFLAGS="$(CFLAGS) --coverage" LDFLAGS="$(LDFLAGS) --coverage" || true
	@gcov -o $(BUILD)/coverage/src $(SRCS) | grep -A1 "^File 'src"
	@mv *.gcov $(BUILD)/coverage/

# TDD loop: reruns the tests whenever a .c/.h file is saved (needs entr: sudo apt install entr).
# The outer loop restarts entr when a new file is added (-d)
watch:
	@command -v entr >/dev/null || { echo "make watch needs entr: sudo apt install entr"; exit 1; }
	@while true; do find src tests -name '*.[ch]' | entr -c -d $(MAKE) --no-print-directory test; done

clean:
	rm -rf $(BUILD)

# --- compile and link rules -------------------------------------------------

$(BUILD)/src/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/tests/%.o: tests/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/unity/unity.o: vendor/unity/unity.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# One executable per test file: its own main() + all production code + Unity
$(BUILD)/test_%: $(BUILD)/tests/test_%.o $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

-include $(wildcard $(BUILD)/*/*.d)

# --- scaffolding --------------------------------------------------------------

UNITY_URL = https://raw.githubusercontent.com/ThrowTheSwitch/Unity/$(UNITY_VERSION)
UNITY_FILES = vendor/unity/unity.c vendor/unity/unity.h vendor/unity/unity_internals.h

# Download the pinned Unity release (only the files that are missing)
vendor: $(UNITY_FILES) vendor/unity/LICENSE.txt

$(UNITY_FILES):
	@mkdir -p $(@D)
	curl -fsSL -o $@ $(UNITY_URL)/src/$(@F)

vendor/unity/LICENSE.txt:
	@mkdir -p $(@D)
	curl -fsSL -o $@ $(UNITY_URL)/LICENSE.txt

# Scaffold a fresh kata in an empty folder (only this Makefile needed):
#   make setup KATA=bowling  ->  src/bowling.{h,c}, tests/test_bowling.c, tests/runner.h, vendor/unity/
# Every file is a file target, so existing files are never overwritten.
KATA_GUARD = $(shell echo '$(KATA)' | tr 'a-z-' 'A-Z_')_H
KATA_ID = $(subst -,_,$(KATA))

setup: vendor .gitignore src/$(KATA).h src/$(KATA).c tests/runner.h tests/test_$(KATA).c
	@echo "Ready: run 'make test'"

define GITIGNORE
build/
*.gcov
local.mk
endef

define RUNNER_H
#ifndef RUNNER_H
#define RUNNER_H

#include "unity.h"

/*
 * RUN(test): Unity's RUN_TEST, but honouring `-f name` / `-x name` on the command line
 * (what `make test TEST_NAME=...` passes). It does the same as the runners Unity's
 * Ruby generator writes: set the current test name, then ask UnityTestMatches().
 */
#define RUN(test)                          \
    do {                                   \
        Unity.CurrentTestName = #test;     \
        if (UnityTestMatches()) {          \
            RUN_TEST(test);                \
        }                                  \
    } while (0)

#endif
endef

define KATA_H
#ifndef $(KATA_GUARD)
#define $(KATA_GUARD)

/* Replace with the kata's real API */
const char *$(KATA_ID)_name(void);

#endif
endef

define KATA_C
#include "$(KATA).h"

const char *$(KATA_ID)_name(void)
{
    return "$(KATA)";
}
endef

define TEST_KATA_C
#include "unity.h"
#include "runner.h"
#include "$(KATA).h"

/* Unity calls these around every test */
void setUp(void) {}
void tearDown(void) {}

static void test_setup_works(void)
{
    TEST_ASSERT_EQUAL_STRING("$(KATA)", $(KATA_ID)_name());
}

int main(int argc, char **argv)
{
    if (UnityParseOptions(argc, argv) != 0) {
        return 1;
    }
    UNITY_BEGIN();
    RUN(test_setup_works);
    return UNITY_END();
}
endef

export GITIGNORE RUNNER_H KATA_H KATA_C TEST_KATA_C

.gitignore:
	printf '%s\n' "$$GITIGNORE" > $@

tests/runner.h:
	@mkdir -p $(@D)
	printf '%s\n' "$$RUNNER_H" > $@

src/$(KATA).h:
	@mkdir -p $(@D)
	printf '%s\n' "$$KATA_H" > $@

src/$(KATA).c:
	@mkdir -p $(@D)
	printf '%s\n' "$$KATA_C" > $@

tests/test_$(KATA).c:
	@mkdir -p $(@D)
	printf '%s\n' "$$TEST_KATA_C" > $@

# Keep .o files between runs (make would otherwise delete them as "intermediate" files)
.SECONDARY:

.PHONY: test build debug memcheck asan coverage watch clean vendor setup
