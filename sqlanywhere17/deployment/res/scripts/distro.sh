# ***************************************************************************
# Copyright (c) 2015 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************
is_redflag()
############
{
    if [ `plat_os` = "linux" ] ; then
	grep -q -s "Red Flag" /etc/issue >/dev/null
	if [ $? -ne 0 ]; then
	    false
	else
	    true
	fi
    else
	false
    fi
}

is_redhat()
###########
{
    if [ `plat_os` = "linux" ] ; then
	grep -q -s "Red Hat" /etc/issue >/dev/null
	if [ $? -ne 0 ]; then
	    false
	else
	    true
	fi
    else
	false
    fi
}

is_suse()
#########
{
    if [ `plat_os` = "linux" ] ; then
	grep -q -s -i "SuSE" /etc/issue >/dev/null
	if [ $? -ne 0 ]; then
	    false
	else
	    true
	fi
    else
	false
    fi
}

is_ubuntu()
###########
{
    if [ `plat_os` = "linux" ] ; then
	grep -q -s -i "Ubuntu" /etc/issue >/dev/null
	if [ $? -ne 0 ]; then
	    false
	else
	    true
	fi
    else
	false
    fi
}
