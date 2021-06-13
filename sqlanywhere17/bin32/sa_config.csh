#!/bin/csh
#

# the following lines set the SA location.
setenv SQLANY17 "/opt/sqlanywhere17"

if ( -r "$HOME/.sqlanywhere17/sample_env32.csh" ) then
    source "$HOME/.sqlanywhere17/sample_env32.csh"
endif
if ( ! $?SQLANYSAMP17 ) then
    setenv SQLANYSAMP17 "/opt/sqlanywhere17/samples"
endif

# the following lines add SA binaries to your path.
if ( $?PATH ) then
    setenv PATH "$SQLANY17/bin32:$SQLANY17/bin64:$PATH"
else
    setenv PATH "$SQLANY17/bin32:$SQLANY17/bin64"
endif
if ( $?NODE_PATH ) then
    setenv NODE_PATH "$SQLANY17/node:$NODE_PATH"
else
    setenv NODE_PATH "$SQLANY17/node"
endif
if ( $?LD_LIBRARY_PATH ) then
    setenv LD_LIBRARY_PATH "$SQLANY17/lib64:$LD_LIBRARY_PATH"
else
    setenv LD_LIBRARY_PATH "$SQLANY17/lib64"
endif
