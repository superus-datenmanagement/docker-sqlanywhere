# ***************************************************************************
# Copyright (c) 2015 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************
CS_TARGET_CHARSET=""

set_target_charset()
####################
{
    CS_TARGET_CHARSET="${1:-}"
}

get_target_charset()
####################
{
    echo "${CS_TARGET_CHARSET:-}"
}
