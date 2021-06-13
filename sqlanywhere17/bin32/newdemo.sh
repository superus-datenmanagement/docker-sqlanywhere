#!/bin/sh	

BINDIR=`dirname "$0"`
SADIR="$BINDIR"/..
DBENG=`/bin/ls -1 "$BINDIR" | grep dbeng | head -n1`

. "$BINDIR/sa_config.sh" >/dev/null 2>&1

if [ "`command -v dbisql`" = "" ]; then
    dbisql=dbisqlc
else
    dbisql=dbisql
fi


if [ "$1" = "" ]; then
    __new=demo.db
fi

__new=demo.db
if [ "$1" != "" ]; then
    __new=$1.db

    __new=`echo $__new | sed -e s/\.db\.db$/.db/`
fi
dberase $__new
if [ ! -r $__new ]; then
    dbinit -dba DBA,sql -mpl 3 $__new
    dbspawn -q -f "$DBENG" -n newdemo $__new
    cd "$SADIR/scripts"
    $dbisql -c "UID=DBA;PWD=sql;SERVERNAME=newdemo" -q mkdemo.sql
    if [ -r "$SADIR/mobilink/setup/syncsa.sql" ]; then
	$dbisql -c "UID=DBA;PWD=sql;SERVERNAME=newdemo" -q "grant connect to ml_server identified by sql;grant resource to ml_server"
	$dbisql -c "UID=ml_server;PWD=sql;SERVERNAME=newdemo" -q read "\"$SADIR/mobilink/setup/syncsa.sql\""
    fi
    dbstop -c "UID=DBA;PWD=sql;SERVERNAME=newdemo" -q
fi

