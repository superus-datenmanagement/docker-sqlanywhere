# ***************************************************************************
# Copyright (c) 2015 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************
real_user()
##########
{
    echo "`who am i | cut -f1 -d' '`"
}

effective_user()
################
{
    if [ `plat_os` = "solaris" ] && [ -f "/usr/ucb/whoami" ] ; then
        echo "`/usr/ucb/whoami`"
    else
        echo "`whoami`"
    fi

}

user_homedir()
##############
# get any user's home directory (not necessarily yours)
{
    USR=~$1
    echo `eval echo $USR`
}

real_user_homedir()
###################
{
    USR=`real_user`
    user_homedir ${USR}
}

effective_user_homedir()
########################
{
    USR=`effective_user`
    user_homedir ${USR}
}

is_root_priv()
##############
{
    [ -w / ]
}

is_elevated()
#############
{
    if is_root_priv; then
	if [ "`real_user`" != "`effective_user`" ]; then
	    true
	else
	    false
	fi
    else
	false
    fi
}

