#!/bin/sh
SAROOT=`dirname "$0"`/..
. "$SAROOT/bin32/sa_config.sh" >/dev/null 2>&1
exec "$SAROOT/bin32/newdemo.sh" "$@"
