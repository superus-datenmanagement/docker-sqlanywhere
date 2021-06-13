#!/bin/sh
#

# the following lines set the SA location.
SQLANY17="/opt/sqlanywhere17"
export SQLANY17

[ -r "$HOME/.sqlanywhere17/sample_env64.sh" ] && . "$HOME/.sqlanywhere17/sample_env64.sh" 
[ -z "${SQLANYSAMP17:-}" ] && SQLANYSAMP17="/opt/sqlanywhere17/samples"
export SQLANYSAMP17

# the following lines add SA binaries to your path.
PATH="$SQLANY17/bin64:$SQLANY17/bin32:${PATH:-}"
export PATH
NODE_PATH="$SQLANY17/node:${NODE_PATH:-}"
export NODE_PATH
LD_LIBRARY_PATH="$SQLANY17/lib64:${LD_LIBRARY_PATH:-}"
export LD_LIBRARY_PATH
