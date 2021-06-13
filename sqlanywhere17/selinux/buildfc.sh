#!/bin/sh
# ***************************************************************************
# Copyright (c) 2015 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************

rm -f sqlanywhere.fc

while [ -n "$1" ] ; do
    sed "s|\${INSTALL_ROOT}|$1|g" sqlanywhere.fct >> sqlanywhere.fc
    shift
done
