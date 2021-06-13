# ***************************************************************************
# Copyright (c) 2015 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************
check_os_version()
##################
{
    case `plat_os` in
	aix)	
            OS_REL=`oslevel`
            OS_REL_COMP=`oslevel | sed 's?\.??' | sed 's?\.??' | sed 's?\.??'`
	    
            OS_MIN_REL="5.3.0.0"
            OS_MIN_COMP="5300"
            ;;

	macos) 
	    OS_REL=`uname -r`
	    OS_REL_1=`echo $OS_REL | cut -d. -f 1`
	    OS_REL_2=`echo $OS_REL | cut -d. -f 2`
	    OS_REL_COMP=`expr $OS_REL_1 \* 100 + $OS_REL_2`

	    OS_MIN_REL="8.0"
	    OS_MIN_COMP="800"
	    ;;

	hpux)	
            OS_REL=`uname -r`
	    OS_REL_1=`echo $OS_REL | cut -d. -f2`
	    OS_REL_2=`echo $OS_REL | cut -d. -f3`
            OS_REL_COMP=`expr $OS_REL_1 \* 100 + $OS_REL_2`

            OS_MIN_REL="1123"
            OS_MIN_COMP="1123"
            ;;

	solaris)	
            OS_REL=`uname -r`
            OS_REL_COMP=`echo $OS_REL |sed 's?\.??'`
	    
	    if [ `plat_hw` = "sparc" ]; then
		OS_MIN_REL="5.8"
		OS_MIN_COMP="58"
	    else
		OS_MIN_REL="5.10"
		OS_MIN_COMP="510"
	    fi
            ;;

       linux)  
            OS_REL=`uname -r`
            OS_REL_1=`echo $OS_REL | cut -d. -f1`
            OS_REL_2=`echo $OS_REL | cut -d. -f2`
            OS_REL_3=`echo $OS_REL | sed 's?\-.*??' | cut -d. -f3`
	
	    # remove text as in "17pre18"
	    OS_REL_3=`echo $OS_REL_3 | sed "s/[a-zA-Z+].*$//g"`
            OS_REL_COMP=`expr $OS_REL_1 \* 1000 + $OS_REL_2 \* 100 + $OS_REL_3`

            OS_MIN_REL="2.6.28"
            OS_MIN_COMP="2628"
            ;;

       *)   echo "${ERR_OS_UNSUPPORTED}"
            exit 1
            ;;
    esac			

    if [ "$OS_REL_COMP" -lt "$OS_MIN_COMP" ] ; then
        msg_error_os_version_too_low "`get_package_name`" "${OS_REL}" "${OS_MIN_REL}"
	exit 1
    fi

    OS="`get_intended_os`"
    HW="`get_intended_hw`"
    if [ "`plat_os_hw`" != "`plat_os_hw \"${OS:-}\" \"${HW:-}\"`" ]; then
	msg_error_os_mismatch "`plat_os_display \"${OS:-}\" \"${HW:-}\"`" "`plat_os_display`"
	exit 1
    fi
}

check_hp_system_requirements()
##############################
{
    result=0

    os_version_major=`uname -r | sed -e 's/B\.\(..\)\..*/\1/'`
    os_version_minor=`uname -r | sed -e 's/B\..*\.//'`
    
    if [ "$os_version_major" -gt 11 ] ; then
	result=1
    fi

    if [ "$result" = 0 ] && [ "$os_version_major" -eq 11 ] ; then

	if [ "$os_version_minor" -ge 11 ] && [ "$os_version_minor" -lt 23 ] ; then

	    # OS version between 11.11 and 11.22, need TOUR

	    tour_result=`swlist -l product -a revision TOUR_PRODUCT 2>&1`
	    tour_installed_cmd=`echo $tour_result | grep "ERROR:"`
	    tour_installed=$?

	    if [ "$tour_installed" -eq 1 ] ; then
		result=1
	    fi
	fi

	if [ "$os_version_minor" -ge 23 ] ; then
	    # 11.23 or greater don't need TOUR
	    result=1
	fi
    fi

    echo $result
}

check_macos_system_requirements()
#################################
{
    result=0

    os_version=`sw_vers | grep 'ProductVersion:' | grep -o '[0-9]*\.[0-9]*\(\.[0-9]*\)\?'`
    os_version_major=`echo $os_version | cut -d. -f1`
    os_version_minor=`echo $os_version | cut -d. -f2`
    os_version_patch=`echo $os_version | cut -d. -f3`
    
    if [ "$os_version_major" -ge 10 ] && [ "$os_version_minor" -ge 8 ] ; then
	result=1
    fi

    echo $result
}


check_system_requirements( )
############################
{
    if [ "${SQLANY_FORCE_INSTALL:-0}" = "1" ]; then
	return
    fi

    check_os_version

    case "`plat_os`" in
	hpux)
	    ok=`check_hp_system_requirements`
	;;
        macos)
	    ok=`check_macos_system_requirements`
        ;;
	*)
	    ok=1
	;;
    esac

    if [ $ok -eq 0 ] ; then 
	msg_system_requirements_too_low
	exit 0
    fi
}

get_java_major_version()
########################
{
    JRE_JAVA="${1:-java}"

    JAVA_VERSION="`"${JRE_JAVA}" -version 2>&1`"
    if [ $? -eq 0 ]; then
        JAVA_VERSION=`echo "${JAVA_VERSION}"  | sed '1!d' | tr -s ' ' | cut -d" " -f3 `
        JAVA_VERSION=`eval echo "${JAVA_VERSION}"`
        JAVA_VERSION=`echo "${JAVA_VERSION}" | cut -d"." -f1,2 `
        echo "${JAVA_VERSION}"
    else
        echo ""
    fi
}

check_macos_jre_requirements()
##############################
{
    if [ true ]; then  # This is for mainline only, REMOVE this for the branches
        false
        return
    fi

    MACOS_JRE_JAVA=/Library/Internet\ Plug-Ins/JavaAppletPlugin.plugin/Contents/Home/bin/java
    if [ ! -f "${MACOS_JRE_JAVA}" ] ; then
        false
        return
    fi

    JRE_VER=`get_java_major_version "${MACOS_JRE_JAVA}"`
    if [ "${JRE_VER}" = "1.8" ]; then
        true
    else
        false
    fi
}
