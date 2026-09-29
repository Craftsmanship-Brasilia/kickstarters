#ifndef RUNNER_H
#define RUNNER_H

#include "unity.h"

/*
 * RUN(test): Unity's RUN_TEST, but honouring `-f name` / `-x name` on the command line
 * (what `make test TEST_NAME=...` passes). It does the same as the runners Unity's
 * Ruby generator writes: set the current test name, then ask UnityTestMatches().
 */
#define RUN(test) do { Unity.CurrentTestName = #test; if (UnityTestMatches()) { RUN_TEST(test); } } while (0)

#endif
