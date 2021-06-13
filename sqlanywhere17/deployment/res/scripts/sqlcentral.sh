# ***************************************************************************
# Copyright (c) 2015 SAP SE or an SAP affiliate company. All rights reserved.
# ***************************************************************************
set_install_sc_icon()
#####################
{
    INSTALL_SC_ICON=$1
}

get_install_sc_icon()
#####################
{
    [ "${INSTALL_SC_ICON:-FALSE}" = "TRUE" ]
}

create_sybase_central_icon()
############################
{
    INSTALLDIR=`get_install_dir SA`
    INSTALLDIR_UTF8=`convert_to_utf8 "$INSTALLDIR"`

    pre_install_icons

    for ICONBITNESS in 64 32; do
        BINsDIR="`get_install_dir BIN${ICONBITNESS}S`"
        BINsDIR_UTF8=`convert_to_utf8 "$BINsDIR"`
        writedesktopfile_scjview $XDG_DESKTOP_DIR
    done

    post_install_icons

    true
}


run_sybase_central_sample()
###########################
{
    if [ `plat_os` = "linux" ] ; then
        SETSID=setsid
    else
        SETSID=
    fi

    SAMPLE_BITNESS=64
    OPP_SAMPLE_BITNESS=32
    scjview="`get_install_dir BIN64S`/scjview"
    if [ ! -x "${scjview}" ] ; then
        scjview="`get_install_dir BIN32S`/scjview"
        SAMPLE_BITNESS=32
        OPP_SAMPLE_BITNESS=64
    fi

    if [ ! -x "${scjview}" ] ; then
        false
        return
    fi

    sample_config="`get_install_dir SAMPLES`/sample_config$SAMPLE_BITNESS.sh"
    if [ ! -f "${sample_config}" ] ; then
        sample_config="`get_install_dir SAMPLES`/sample_config$OPP_SAMPLE_BITNESS.sh"
    fi
        
    if [ -z "${DISPLAY}" ] && [ "${TERM_PROGRAM}" != "Apple_Terminal" ] ; then
        false
        return
    fi

    if [ ! -f "${sample_config}" ] ; then
        false
        return
    fi

    echo "#!/bin/sh" > "`get_install_dir SAMPLES`/sc.sh" ; echo "echo '' | . \"${sample_config}\" ;  "$SETSID" \"${scjview}\" -sqlanywhere`get_version`0:connect_to_demo &" >> "`get_install_dir SAMPLES`/sc.sh"
    /bin/sh "`get_install_dir SAMPLES`/sc.sh" > /dev/null
    rm "`get_install_dir SAMPLES`/sc.sh"

    true
}
