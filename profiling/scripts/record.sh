#!/usr/bin/env bash
set -euo pipefail

# shellcheck shell=bash

if [ "$(uname)" != "Linux" ]; then
    echo "error: profiling is only supported on Linux" >&2
    exit 1
fi

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <binary> [args...]" >&2
    echo "  Records a perf profile for <binary> and saves perf.data to profiling/results/" >&2
    exit 1
fi

BINARY="$1"
shift

if [ ! -x "$BINARY" ]; then
    echo "error: '$BINARY' is not executable" >&2
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
RESULTS_DIR="$SCRIPT_DIR/../results"
mkdir -p "$RESULTS_DIR"

OUTPUT="$RESULTS_DIR/perf.data"

echo "--- Recording: $BINARY $*"
echo "--- Output:    $OUTPUT"

perf record \
    -F 997 \
    -g \
    -o "$OUTPUT" \
    -- "$BINARY" "$@"

echo "--- Done. Run flamegraph.sh or report.sh to analyze."
