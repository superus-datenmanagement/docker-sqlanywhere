#! /bin/sh

# ***************************************************************************
# Copyright (c) 2020 SAP AG or an SAP affiliate company. All rights reserved.
# ***************************************************************************

get_default_install_product()
#############################
{
    echo "sqlanywhere"
}

get_default_license_key()
#########################
{
    echo ""
}

get_install_type()
##################
{
    echo "DEPLOY"
}

SQLANY_STATE=cd6e41d87a4b68337d73c7015943908f
export SQLANY_STATE
