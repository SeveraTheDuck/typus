#!/usr/bin/env bash
set -euo pipefail

# shellcheck shell=bash

if [ "$(uname)" != "Linux" ]; then
    echo "error: profiling is only supported on Linux" >&2
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
FLAMEGRAPH_DIR="$SCRIPT_DIR/../flamegraph"
RESULTS_DIR="$SCRIPT_DIR/../results"

if [ ! -f "$FLAMEGRAPH_DIR/flamegraph.pl" ]; then
    echo "error: flamegraph submodule not found" >&2
    echo "  Run: git submodule update --init profiling/flamegraph" >&2
    exit 1
fi

INPUT="${1:-$RESULTS_DIR/perf.data}"

if [ ! -f "$INPUT" ]; then
    echo "error: '$INPUT' not found — run record.sh first" >&2
    exit 1
fi

BASENAME="$(basename "$INPUT" .data)"
OUTPUT="$RESULTS_DIR/${BASENAME}.svg"

echo "--- Generating flamegraph from: $INPUT"

perf script -i "$INPUT" \
    | "$FLAMEGRAPH_DIR/stackcollapse-perf.pl" \
    | "$FLAMEGRAPH_DIR/flamegraph.pl" \
    > "$OUTPUT"

echo "--- Flamegraph saved to: $OUTPUT"
