#!/usr/bin/env bash
# Coverage report. Run via `just coverage` (summary) or CI (lcov/cobertura).
#   usage: report.sh [summary|lcov]
# Configure+build of the coverage preset happens before this script; here we run
# the instrumented tests and emit a report in the requested format.
set -euo pipefail

mode="${1:-summary}"
BUILD=build/coverage
# --- GCC: arc-based (gcovr) -------------------------------------------------
ctest --preset coverage

case "$mode" in
  summary)
    gcovr --root . --filter "source/" --gcov-executable gcov --print-summary "$BUILD"
    ;;
  lcov)
    gcovr --root . --filter "source/" --gcov-executable gcov \
      --cobertura "$BUILD/coverage.xml" "$BUILD"
    ;;
  *)
    echo "report.sh: unknown mode '$mode' (use summary|lcov)" >&2
    exit 2
    ;;
esac
