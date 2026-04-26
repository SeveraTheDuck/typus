#!/usr/bin/env bash
set -euo pipefail

# shellcheck shell=bash

if [ "$(uname)" != "Linux" ]; then
    echo "error: profiling is only supported on Linux" >&2
    exit 1
fi

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <binary> [args...]" >&2
    echo "  Runs record.sh then flamegraph.sh in one step." >&2
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

"$SCRIPT_DIR/record.sh" "$@"
"$SCRIPT_DIR/flamegraph.sh"
