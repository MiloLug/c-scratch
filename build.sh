#!/usr/bin/env sh

GCST_WERROR="${GCST_WERROR:-OFF}"
export GCST_WERROR

DIR="$(cd "$(dirname "$0")" && pwd)"
export PYTHONPATH="$DIR/.gcst:$PYTHONPATH"
python3 .gcst/scripts/build.py "$@"